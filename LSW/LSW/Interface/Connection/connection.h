#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
// C++
#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include <functional>
#include <memory>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "..\..\Handling\Abort\abort.h"
#include "..\..\Tools\SuperMutex\supermutex.h"
#include "..\Logger\logger.h"

namespace LSW {
	namespace v5 {
		namespace Interface {

			namespace connection {
				constexpr int default_port = 42069;
				constexpr unsigned package_size = 1 << 8;
			}
			struct _pack {
				char data[connection::package_size] = { 0 };
				unsigned data_len = 0;
				unsigned sum_with_n_more = 0;
			};

			class ConnectionCore {
				WSADATA wsaData = WSADATA();
				addrinfo* result = nullptr;
				bool failure = false, init = false;
			public:
				// ip (can be null for server), port, ipv6? -1 for client
				bool initialize(const char*, const int = connection::default_port, const int = 0);
				bool as_client(SOCKET&);
				bool as_host(SOCKET&);
			};


			class Connection {
				ConnectionCore core;
				Logger logg;

				mutable std::mutex packs_received_m;
				std::vector<std::string> packs_received;
				std::mutex packs_sending_m;
				std::vector<std::string> packs_sending;

				SOCKET connected = INVALID_SOCKET;
				bool keep_connection = false;

				std::thread send_thread;
				std::thread recv_thread;

				// debugging
				size_t packages_sent = 0;
				size_t packages_recv = 0;

				std::function<void(const std::string&)> alt_receive_autodiscard; // your handle, no saving. It calls this instead.
				std::function<std::string(void)>		alt_generate_auto;		 // it will read only from this if set.

				void handle_send();
				void handle_recv();

				// starts handle's
				void init();
			public:
				Connection(const Connection&) = delete;
				Connection(Connection&&) = delete;
				Connection(SOCKET = INVALID_SOCKET);
				~Connection();

				// normally client call this one
				bool connect(const char* = "127.0.0.1", const int = connection::default_port);
				// close the communication and threads.
				void close();

				bool is_connected() const;

				bool has_package() const;
				std::string get_next();

				void send_package(std::string);

				size_t get_packages_sent() const;
				size_t get_packages_recv() const;

				// ALL reads (recv) goes to this instead of internal management
				// DO NOT SET A FUNCTION THAT LOCKS FOR TOO LONG! IT MIGHT BREAK THE APP!
				void overwrite_reads_to(std::function<void(const std::string&)>);

				// ALL sends HAS TO COME from this function
				// DO NOT SET A FUNCTION THAT LOCKS FOR TOO LONG! IT MIGHT BREAK THE APP!
				void overwrite_sends_to(std::function<std::string(void)>);

				void reset_overwrite_reads();
				void reset_overwrite_sends();
			};

			class Hosting {
				ConnectionCore core;
				Logger logg;

				SOCKET Listening = INVALID_SOCKET;
				bool keep_connection = false;

				size_t max_connections_allowed = 1;

				std::thread handle_thread;

				Tools::Waiter connection_event;
				std::vector<std::shared_ptr<Connection>> connections;
				mutable std::mutex connections_m;

				void handle_queue();

				void init();
			public:
				Hosting(const Hosting&) = delete;
				Hosting(Hosting&&) = delete;
				Hosting(const int, const bool = false);
				Hosting(const bool = false);
				~Hosting();

				size_t size() const;

				void close();

				bool is_connected() const;

				void set_connections_limit(const size_t);

				std::shared_ptr<Connection> get_connection(const size_t);
				std::shared_ptr<Connection> get_latest_connection();
			};

		}
	}
}