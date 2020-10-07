#include "SmartFile.h"


namespace LSW {
	namespace v5 {
		namespace Interface {

			std::string SmartFile::convert(const smartfile::file_modes m)
			{
				switch (m) {
				case smartfile::file_modes::READ:
					return "rb";
				case smartfile::file_modes::WRITE:
					return "wb";
				case smartfile::file_modes::READ_WRITE_KEEP:
					return "rb+";
				case smartfile::file_modes::READ_WRITE_OVERWRITE:
					return "wb+";
				case smartfile::file_modes::APPEND_WRITE:
					return "ab";
				case smartfile::file_modes::APPEND_READ_WRITE:
					return "ab+";
				}
				return "rb+";
			}
			SmartFile::SmartFile()
			{
				Handling::init_basic();
			}
			SmartFile::~SmartFile()
			{
				close();
			}
			bool SmartFile::open(std::string path, const smartfile::file_modes m)
			{
				eoff = false;
				close();
				Handling::interpret_path(path);
				size_of_this = Tools::get_file_size(path);

				return (fp = al_fopen(path.c_str(), convert(m).c_str()));
			}

			bool SmartFile::is_open() const
			{
				return fp;
			}

			long long SmartFile::total_size() const
			{
				return size_of_this;
			}

			void SmartFile::close()
			{
				if (fp) {
					al_fclose(fp);
					eoff = true;
					fp = nullptr;
				}
			}

			bool SmartFile::eof() const
			{
				return eoff;
			}

			void SmartFile::seek(const int off, const smartfile::file_seek point)
			{
				al_fseek(fp, off, static_cast<ALLEGRO_SEEK>(point));
			}

			size_t SmartFile::read(std::string& buf, const size_t siz)
			{
				if (!fp) { return 0; }
				if (al_feof(fp)) { eoff = true; return 0; }

				buf.resize(siz); // set maximum size

				auto readd = al_fread(fp, buf.data(), siz);
				if (readd <= 0) {
					eoff = true;
					buf.clear();
					return 0;
				}
				buf.resize(readd); // set what is there
				buf.shrink_to_fit(); // clean up the rest of it (won't clear after it)
				return readd;
			}

			size_t SmartFile::write(const std::string& buf, const size_t s)
			{
				if (!fp) return 0;
				return al_fwrite(fp, buf.data(), s ? s : buf.size());
			}


		}
	}
}
