#pragma once
#include<filesystem>
#include<fstream>

namespace fs = std::filesystem;
struct DirGenerator
{
	void createTestDirs() {
		fs::create_directory("./test");
		std::ofstream file("./test/file1.txt");
		file << "Hello";
		

		fs::create_directory("./example");
		std::ofstream file1("./example/file1.txt");
		file1 << "Hello";

		fs::create_directory("./lib");
		std::ofstream cmdfile("./lib/cmdfile.txt");
		cmdfile << "";

		cmdfile.close();
		file.close();
		file1.close();
		fs::last_write_time("./example/file1.txt", fs::last_write_time("./test/file1.txt"));
	}

	~DirGenerator() {
		fs::remove_all("./test");
		fs::remove_all("./example");
		fs::remove_all("./lib");
	}

};
