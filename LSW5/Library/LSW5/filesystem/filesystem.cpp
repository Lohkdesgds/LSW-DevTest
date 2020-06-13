#include "filesystem.h"

namespace LSW {
	namespace v5 {

		bool PhysFS::physfs_initialized_once = false;

		std::string SmartFILE::convert(const file_modes m)
		{
			switch (m) {
			case file_modes::READ:
				return "rb";
			case file_modes::WRITE:
				return "wb";
			case file_modes::READ_WRITE_KEEP:
				return "rb+";
			case file_modes::READ_WRITE_OVERWRITE:
				return "wb+";
			case file_modes::APPEND_WRITE:
				return "ab";
			case file_modes::APPEND_READ_WRITE:
				return "ab+";
			}
			return "";
		}
		SmartFILE::~SmartFILE()
		{
			close();
		}
		bool SmartFILE::open(std::string path, const file_modes m)
		{
			eoff = false;
			close();
			Tools::interpretPath(path);
			size_of_this = Tools::getFileSize(path);
			return (fopen_s(&fp, path.c_str(), convert(m).c_str()) == 0);
		}

		long long SmartFILE::totalSize()
		{
			return size_of_this;
		}

		void SmartFILE::close()
		{
			if (fp) {
				fclose(fp);
				eoff = true;
				fp = nullptr;
			}
		}

		bool SmartFILE::eof()
		{
			return eoff;
		}

		size_t SmartFILE::read(std::string& buf, const size_t siz)
		{
			if (!fp) { return 0; }
			if (feof(fp)) { eoff = true; return 0;}

			buf.resize(siz); // set maximum size

			auto readd = fread_s(buf.data(), siz, sizeof(char), siz, fp);
			if (readd <= 0) {
				eoff = true;
				buf.clear();
				return 0;
			}
			buf.resize(readd); // set what is there
			buf.shrink_to_fit(); // clean up the rest of it (won't clear after it)
			return readd;
		}

		size_t SmartFILE::write(std::string& buf, const size_t s)
		{
			if (!fp) return 0;
			return fwrite(buf.data(), sizeof(char), s ? s : buf.size(), fp);
		}

		std::string PhysFS::readDir(const char* path)
		{
			std::string files;
			char** rc = PHYSFS_enumerateFiles(path ? path : "");
			for (auto j = rc; *j != NULL; j++) {
				PHYSFS_Stat fprop;
				std::string cpath = (path ? (std::string(path) + "/") : "") + *j;
				PHYSFS_stat(cpath.c_str(), &fprop);

				switch (fprop.filetype) {
				case PHYSFS_FILETYPE_REGULAR: // file
					files += cpath + " [" + Tools::byteAutoString(fprop.filesize, 1, true) + "B], ";
					break;
				case PHYSFS_FILETYPE_DIRECTORY: // path
					files += readDir(cpath.c_str()) + ", ";
					break;
				}
			}
			PHYSFS_freeList(rc);
			if (files.length() > 1) for (short u = 0; u < 2; u++) files.pop_back();

			return files;
		}

		PhysFS::PhysFS()
		{
			lsw_al_init();
			
			if (!physfs_initialized_once) {
				char myself[1024];
				GetModuleFileNameA(NULL, myself, 1024);

				if (!PHYSFS_init(myself)) throw Abort::Abort(__FUNCSIG__, "Failed to setup physfs!");
				al_set_physfs_file_interface();

				physfs_initialized_once = true;
			}
		}
		void PhysFS::disableALL()
		{
			PHYSFS_deinit();
			physfs_initialized_once = false;
		}

		void PhysFS::addPath(std::string s)
		{
			Tools::interpretPath(s);
			if (PHYSFS_mount(s.c_str(), nullptr, 1) == 0) throw Abort::Abort(__FUNCSIG__, "Couldn't add " + s + " to search paths.", Abort::abort_level::GIVEUP);
			if (prunt) prunt(std::string("Added path '") + s.c_str() + "' successfully.");
		}

		void PhysFS::delPath(const std::string s)
		{
			if (PHYSFS_unmount(s.c_str()) == 0) throw Abort::Abort(__FUNCSIG__, "Couldn't remove " + s + " from search paths.", Abort::abort_level::GIVEUP);
			if (prunt) prunt("Removed path '" + s + "' successfully.");
		}

		void PhysFS::hookPrint(const std::function<void(std::string)> f)
		{
			prunt = f;
		}

		void PhysFS::apply()
		{
			al_set_physfs_file_interface();
			if (prunt) prunt("Applied PhysFS on current thread #" + std::to_string(Tools::getThreadID()));
		}

		void PhysFS::cancel()
		{
			al_set_standard_file_interface();
			if (prunt) prunt("Removed PhysFS on current thread #" + std::to_string(Tools::getThreadID()));
		}

		void PhysFS::dir()
		{
			std::string raw_paths;
			std::string files;

			auto listpath = PHYSFS_getSearchPath();
			for (auto i = listpath; *i != NULL; i++) {
				raw_paths += std::string(*i) + ", ";
			}
			if (raw_paths.length() > 1) for (short u = 0; u < 2; u++) raw_paths.pop_back();
			PHYSFS_freeList(listpath);


			files = readDir();

			if (files.length() > 1) for (short u = 0; u < 2; u++) { files.pop_back(); } // last ", "
			//Tools::_clearPath(files);

			if (prunt) {
				prunt("Paths added: " + raw_paths);
				prunt("Files found: " + files);
			}
		}

	}
}