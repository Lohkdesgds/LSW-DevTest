#include "connection.h"

namespace LSW {
	namespace v5 {
		namespace Interface {


			bool ConnectionCore::initialize(const char* ip_str, const int port, const int isthis_ipv6)
			{
				if (init) return true;

				if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				{
					failure = true;
					return false;
				}

				struct addrinfo hints;

				char port_str[16];
				sprintf_s(port_str, "%d", port);

				SecureZeroMemory(&hints, sizeof(hints));
				if (isthis_ipv6 >= 0) hints.ai_family = isthis_ipv6 ? AF_INET6 : AF_INET;
				else hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;
				if (isthis_ipv6 >= 0) hints.ai_flags = AI_PASSIVE;

				// Resolve the server address and port
				if (getaddrinfo(ip_str, port_str, &hints, &result) != 0) return [&] {failure = true; return false; }();

				init = true;
				return true;
			}

			bool ConnectionCore::as_client(SOCKET& ConnectSocket)
			{
				if (failure || !init) return false;

				struct addrinfo* ptr = NULL;

				// Attempt to connect to an address until one succeeds
				for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

					// Create a SOCKET for connecting to server
					ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
						ptr->ai_protocol);
					if (ConnectSocket == INVALID_SOCKET) return false;

					// Connect to server.
					if (connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
						closesocket(ConnectSocket);
						ConnectSocket = INVALID_SOCKET;
						continue;
					}
					break;
				}

				freeaddrinfo(result);

				if (ConnectSocket == INVALID_SOCKET) return false;
				// connected
				return true;
			}

			bool ConnectionCore::as_host(SOCKET& ListenSocket)
			{
				if (failure || !init) return false;

				ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				if (ListenSocket == INVALID_SOCKET) return false;

				// Setup the TCP listening socket
				if (bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
					freeaddrinfo(result);
					closesocket(ListenSocket);
					return false;
				}

				freeaddrinfo(result);
				// ready to listen
				return true;
			}

			void Connection::handle_send()
			{
				std::stringstream ss;
				ss << std::this_thread::get_id();
				const std::string common = std::string("[HANDLE_SEND] T#") + ss.str() + std::string(": ");

				logg.debug(common + "Started.");

				while (keep_connection) {

					std::string pack;

					if (alt_generate_auto) {
						pack = alt_generate_auto();
						if (pack.empty()) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
							continue;
						}
						//logg.debug(common + "(auto) tasked.");
					}
					else {
						std::lock_guard<std::mutex> luck(packs_sending_m);
						if (packs_sending.size() == 0) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
							continue;
						}
						else {
							pack = packs_sending.front();
							packs_sending.erase(packs_sending.begin());
							//logg.debug(common + "(default) send tasked.");
						}
					}

					const unsigned len = (static_cast<unsigned>(pack.length()) - 1) / connection::package_size; // if len == package_size, it is only one pack, so (len-1)...

					for (unsigned count = 0; count <= len; count++) {
						_pack one;
						one.sum_with_n_more = len - count;
						std::string res = pack.substr(0, connection::package_size);
						one.data_len = static_cast<unsigned>(res.length());
						size_t _count = 0;
						for (auto& i : res) one.data[_count++] = i;
						if (pack.length() > connection::package_size) pack = pack.substr(connection::package_size);
						else pack.clear();

						auto fi = ::send(connected, (char*)&one, sizeof(_pack), 0);
						packages_sent++;

						logg.debug(common + "SEND once.");

						if (fi < 0) {
							keep_connection = false;
							break;
						}
					}
				}
			}

			void Connection::handle_recv()
			{
				std::stringstream ss;
				ss << std::this_thread::get_id();
				const std::string common = std::string("[HANDLE_RECV] T#") + ss.str() + std::string(": ");

				logg.debug(common + "Started.");

				while (keep_connection) {
					_pack pack;

					auto fi = ::recv(connected, (char*)&pack, sizeof(_pack), 0);
					packages_recv++;

					logg.debug(common + "RECV once.");

					if (fi > 0) {
						std::string data;
						std::lock_guard<std::mutex> luck(packs_received_m);
						for (unsigned p = 0; p < pack.data_len; p++) data += pack.data[p];


						while (pack.sum_with_n_more > 0) {
							auto fi2 = ::recv(connected, (char*)&pack, sizeof(_pack), 0);
							packages_recv++;

							logg.debug(common + "RECV once.");

							if (fi2 < 0) {
								keep_connection = false;
								break;
							}
							else if (fi2 == 0) continue;

							for (unsigned p = 0; p < pack.data_len; p++) data += pack.data[p];
							//printf_s("\nReceived one package.");
						}

						if (alt_receive_autodiscard) {
							alt_receive_autodiscard(data);
							logg.debug(common + "(auto) tasked.");
						}
						else {
							packs_received.emplace_back(std::move(data));
							logg.debug(common + "(default) tasked.");
						}
					}
					else if (fi < 0) {
						keep_connection = false;
						break;
					}
				}
			}

			void Connection::init()
			{
				keep_connection = true;
				send_thread = std::thread([&] { handle_send(); });
				recv_thread = std::thread([&] { handle_recv(); });
			}

			Connection::Connection(SOCKET socket)
			{
				if (socket != INVALID_SOCKET) {
					connected = socket;
					init();
				}
			}

			Connection::~Connection()
			{
				close();
			}

			bool Connection::connect(const char* a, const int b)
			{
				if (!core.initialize(a, b, -1)) return false;
				if (!core.as_client(connected)) return false;
				init();
				//printf_s("\nConnected");
				return true;
			}

			void Connection::close()
			{
				keep_connection = false;
				if (connected != INVALID_SOCKET) {
					::closesocket(connected);
					connected = INVALID_SOCKET;
					send_thread.join();
					recv_thread.join();
					std::lock_guard<std::mutex> lol1(packs_sending_m);
					std::lock_guard<std::mutex> lol2(packs_received_m);
					packs_sending.clear();
					packs_received.clear();
				}
			}

			bool Connection::is_connected()
			{
				return connected != INVALID_SOCKET && keep_connection;
			}

			bool Connection::has_package()
			{
				std::lock_guard<std::mutex> luck(packs_received_m);
				return packs_received.size();
			}

			std::string Connection::get_next()
			{
				if (alt_generate_auto) throw Handling::Abort(__FUNCSIG__, "Recvs are overrwriten by a function! You cannot recv package when function is set!");
				std::lock_guard<std::mutex> luck(packs_received_m);
				if (packs_received.size() == 0) return "";
				std::string cpy = std::move(packs_received.front());
				packs_received.erase(packs_received.begin());
				return std::move(cpy);
			}

			void Connection::send_package(std::string pack)
			{
				if (alt_receive_autodiscard) throw Handling::Abort(__FUNCSIG__, "Sends are overrwriten by a function! You cannot send package when function is set!");
				std::lock_guard<std::mutex> luck(packs_sending_m);

				packs_sending.push_back(std::move(pack));
			}

			size_t Connection::get_packages_sent()
			{
				return packages_sent;
			}

			size_t Connection::get_packages_recv()
			{
				return packages_recv;
			}

			void Connection::overwrite_reads_to(std::function<void(const std::string&)> ow)
			{
				alt_receive_autodiscard = ow;
			}

			void Connection::overwrite_sends_to(std::function<std::string(void)> ow)
			{
				alt_generate_auto = ow;
			}

			void Connection::reset_overwrite_reads()
			{
				alt_receive_autodiscard = std::function<void(const std::string&)>();
			}

			void Connection::reset_overwrite_sends()
			{
				alt_generate_auto = std::function<std::string(void)>();
			}

			void Hosting::handle_queue()
			{
				while (keep_connection) {
					if (listen(Listening, SOMAXCONN) == SOCKET_ERROR) continue;

					// Accept a client socket
					SOCKET ClientSocket = ::accept(Listening, NULL, NULL);
					if (ClientSocket == INVALID_SOCKET) continue;

					std::shared_ptr<Connection> dis = std::make_shared<Connection>(ClientSocket);

					if (max_connections_allowed > 0 && connections.size() >= max_connections_allowed) {
						dis->close();
						continue;
					}

					//dis->start_internally_as_host(); // cause sended last time, so should receive so there's no error

					std::lock_guard<std::mutex> safe(connections_m);
					connections.emplace_back(std::move(dis));

					//printf_s("\nSomeone has connected!");

					for (size_t p = 0; p < connections.size(); p++) {
						if (!connections[p]->is_connected()) {
							//printf_s("\nSomeone has disconnected!");
							connections.erase(connections.begin() + p--);
						}
					}

					connection_event.signal_one();
				}
				closesocket(Listening);
				Listening = INVALID_SOCKET;
			}

			void Hosting::init()
			{
				keep_connection = true;
				handle_thread = std::thread([&] { handle_queue(); });
			}

			Hosting::Hosting(const int port, const bool ipv6)
			{
				core.initialize(nullptr, port, ipv6);
				core.as_host(Listening);
				init();
			}

			Hosting::Hosting(const bool ipv6)
			{
				core.initialize(nullptr, connection::default_port, ipv6);
				core.as_host(Listening);
				init();
			}

			Hosting::~Hosting()
			{
				close();
			}

			size_t Hosting::size()
			{
				std::lock_guard<std::mutex> safe(connections_m);
				return connections.size();
			}

			void Hosting::close()
			{
				keep_connection = false;
				if (Listening != INVALID_SOCKET) {
					::closesocket(Listening);
					Listening = INVALID_SOCKET;
					handle_thread.join();
					std::unique_lock<std::mutex> luck(connections_m);
					connections.clear();
				}
			}

			bool Hosting::is_connected()
			{
				return keep_connection && Listening != INVALID_SOCKET;
			}

			void Hosting::set_connections_limit(const size_t nl)
			{
				max_connections_allowed = nl;
			}

			std::shared_ptr<Connection> Hosting::get_connection(const size_t p)
			{
				std::lock_guard<std::mutex> luck(connections_m);
				if (connections.size() <= p) return std::shared_ptr<Connection>();
				return connections[p];
			}

			std::shared_ptr<Connection> Hosting::get_latest_connection()
			{
				std::unique_lock<std::mutex> luck(connections_m);
				while (connections.size() == 0) {
					luck.unlock();
					connection_event.wait_signal(1000);
					luck.lock();
				}
				return connections.back();
			}

		}
	}
}