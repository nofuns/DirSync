#include <iostream>
#include <thread>


#include "DirectorySync.h"
#include "Logger.h"

int main(int argc, char* argv[])
{
	setlocale(0, "");

	if (argc > 1) {
		auto target = argv[1];
		auto backup = argv[2];
		unsigned timer	= atoi(argv[3]);

		DirectorySync sync(target, backup);

		while (true) {
			sync.StartSync();
			std::this_thread::sleep_for(std::chrono::seconds(timer));
		}
	}
	else {
		std::cout << "No arguments. Please, set [target] and [backup] directories and sync [timer]." << std::endl;
	}
	return 0;
}