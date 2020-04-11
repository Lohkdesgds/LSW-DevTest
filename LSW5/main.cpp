#include "Library/LSW5/logger/logger.h"

using namespace LSW::v5;

int main() {
	Logger logg;
	logg.init("%appdata%/Lohk's Studios/Testing/log.log");

	logg << L::SLF << fsr(__FUNCSIG__) << "&6Hello world&5!" << L::ELF;



}