#include "catch.hpp"
#include "helper.hpp"
#include "../FileSynchronizer/FileComparator.h"

TEST_CASE("Testing Quick File Comparator") {
	DirGenerator g;
	g.createTestDirs();
	QuickFileComparator comp;

	SECTION("same date, size and path, SAME content") {
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt"));
		REQUIRE(comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}

	SECTION("same date, size and path, DIFFERENT content") {
		std::ofstream stream("./example/file1.txt");
		stream << "World";
		stream.close();
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt"));
		REQUIRE(comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}

	SECTION("Different size") {
		std::ofstream stream("./example/file1.txt");
		stream << "Worldd";
		stream.close();
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt"));
		REQUIRE(!comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}

	SECTION("Different date") {
		std::ofstream stream("./example/file1.txt");
		stream << "World";
		stream.close();
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt") + std::chrono::hours(1));
		REQUIRE(!comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}
}

TEST_CASE("Testing Safe File Comparator") {
	DirGenerator g;
	g.createTestDirs();
	SafeFileComparator comp;

	SECTION("same date, size and path, SAME content") {
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt"));
		REQUIRE(comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}

	SECTION("same date, size and path, DIFFERENT content") {
		std::ofstream stream("./example/file1.txt");
		stream << "World";
		stream.close();
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt"));
		REQUIRE(!comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}

	SECTION("Different size") {
		std::ofstream stream("./example/file1.txt");
		stream << "Worldd";
		stream.close();
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt"));
		REQUIRE(!comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}

	SECTION("Different date") {
		std::ofstream stream("./example/file1.txt");
		stream << "Hello";
		stream.close();
		fs::last_write_time("./test/file1.txt", fs::last_write_time("./example/file1.txt") + std::chrono::hours(1));
		REQUIRE(!comp.compare("./test", "file1.txt", "./example", "file1.txt"));
	}
}
