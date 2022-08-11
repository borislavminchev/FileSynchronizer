#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "catch.hpp"
#include "helper.hpp"
#include "../FileSynchronizer/DirectoryAnalyzer.h"

std::vector<std::string> getLines(const std::string& file) {
	std::vector<std::string> res;
	std::ifstream stream(file);
	std::string buffer;
	while (std::getline(stream, buffer)) {
		res.push_back(buffer);
	}

	return res;
}

//mirror, safe, standart
TEST_CASE("Testing mirror directory analyzer") {
	DirGenerator g;
	g.createTestDirs();
	
	SECTION("Check if right and left paths are defined") {
		MirrorDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", SAFE);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");
		REQUIRE(lines[0] == "LEFT is \".\\test\"");
		REQUIRE(lines[1] == "RIGHT is \".\\example\"");
	}
	SECTION("Check if it works with files and dirs") {
		fs::create_directory("./test/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << " ";
		f.close();
		std::ofstream f2("./test/f2.txt");
		f2 << " ";
		f2.close();

		MirrorDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", SAFE);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");
		
		REQUIRE(lines[2] == "CREATE-DIR \"R\\dir1\"");
		REQUIRE(lines[3] == "COPY \"L\\dir1\\f.txt\" \"R\\dir1\\f.txt\"");
		REQUIRE(lines[4] == "COPY \"L\\f2.txt\" \"R\\f2.txt\"");

	}
}

TEST_CASE("Testing safe directory analyzer") {
	

	SECTION("Check if right and left paths are defined") {
		DirGenerator g;
		g.createTestDirs();
		SafeDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", SAFE);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");
		REQUIRE(lines[0] == "LEFT is \".\\test\"");
		REQUIRE(lines[1] == "RIGHT is \".\\example\"");
	}
	
	SECTION("Check if it works with dirs") {
		DirGenerator g;
		g.createTestDirs();
		fs::create_directory("./test/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << " ";
		f.close();
		

		SafeDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", SAFE);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");

		REQUIRE(lines[2] == "CREATE-DIR \"R\\dir1\"");
		REQUIRE(lines[3] == "COPY \"L\\dir1\\f.txt\" \"R\\dir1\\f.txt\"");

	}

	SECTION("Check if it works with existing but different files") {
		DirGenerator g;
		g.createTestDirs();
		fs::create_directory("./test/dir1");
		fs::create_directory("./example/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << "a";
		f.close();
		std::ofstream f1("./example/dir1/f.txt");
		f1 << "abc";
		f1.close();

		fs::last_write_time("./test/dir1/f.txt", fs::last_write_time("./example/dir1/f.txt") + std::chrono::hours(1));
		SafeDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", QUICK);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");

		REQUIRE(lines.size() == 2);//no change in cmdfile

	}

	SECTION("Check if it works with existing but different files") {
		DirGenerator g;
		g.createTestDirs();
		fs::create_directory("./test/dir1");
		fs::create_directory("./example/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << "a";
		f.close();
		

		SafeDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", QUICK);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");

		REQUIRE(lines[2] == "COPY \"L\\dir1\\f.txt\" \"R\\dir1\\f.txt\"");

	}

}

TEST_CASE("Testing standart directory analyzer") {


	SECTION("Check if right and left paths are defined") {
		DirGenerator g;
		g.createTestDirs();
		StandartDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", SAFE);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");
		REQUIRE(lines[0] == "LEFT is \".\\test\"");
		REQUIRE(lines[1] == "RIGHT is \".\\example\"");
	}

	SECTION("Check if it works with dirs") {
		DirGenerator g;
		g.createTestDirs();
		fs::create_directory("./test/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << " ";
		f.close();


		StandartDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", SAFE);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");

		REQUIRE(lines[2] == "CREATE-DIR \"R\\dir1\"");
		REQUIRE(lines[3] == "COPY \"L\\dir1\\f.txt\" \"R\\dir1\\f.txt\"");

	}

	SECTION("Check if it works with existing but different files") {
		DirGenerator g;
		g.createTestDirs();
		fs::create_directory("./test/dir1");
		fs::create_directory("./example/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << "a";
		f.close();
		std::ofstream f1("./example/dir1/f.txt");
		f1 << "abc";
		f1.close();

		fs::last_write_time("./example/dir1/f.txt", fs::last_write_time("./test/dir1/f.txt") + std::chrono::hours(1));
		StandartDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", QUICK);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");

		REQUIRE(lines[2] == "COPY \"R\\dir1\\f.txt\" \"L\\dir1\\f.txt\"");

	}

	SECTION("Check if it works with existing but different files") {
		DirGenerator g;
		g.createTestDirs();
		fs::create_directory("./test/dir1");
		fs::create_directory("./example/dir1");
		std::ofstream f("./test/dir1/f.txt");
		f << "a";
		f.close();


		StandartDirectoryAnalyzer analyzer("./test", "./example", "./lib/cmdfile.txt", QUICK);
		analyzer.analyze();
		std::vector<std::string> lines = getLines("./lib/cmdfile.txt");

		REQUIRE(lines[2] == "COPY \"L\\dir1\\f.txt\" \"R\\dir1\\f.txt\"");

	}

}