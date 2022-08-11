#include"catch.hpp"
#include"helper.hpp"
#include"../FileSynchronizer/FileManager.h"
#include"../FileSynchronizer/FileComparator.h"


TEST_CASE("Testing createDirectory()") {
	SECTION("When left or right are not defined") {
		FileManager manager;
		REQUIRE_THROWS(manager.createDirectory("L\\newDir"));
	}

	SECTION("When left is defined") {
		DirGenerator g;
		g.createTestDirs();
		FileManager manager;
		manager.defineLeft("./test");
		REQUIRE_THROWS(manager.createDirectory("R\\newDir"));
		REQUIRE(!fs::exists("./test/newDir"));
		REQUIRE_NOTHROW(manager.createDirectory("L\\newDir"));
		REQUIRE(fs::exists("./test/newDir"));
	}
	SECTION("When right is defined") {
		DirGenerator g;
		g.createTestDirs();
		FileManager manager;
		manager.defineRight("./test");
		REQUIRE_THROWS(manager.createDirectory("L\\newDir"));
		REQUIRE(!fs::exists("./test/newDir"));
		REQUIRE_NOTHROW(manager.createDirectory("R\\newDir"));
		REQUIRE(fs::exists("./test/newDir"));
	}
}

TEST_CASE("Testing copyFile()") {
	SECTION("When left or right are not defined") {
		FileManager manager;
		REQUIRE_THROWS(manager.copyFile("L\\file1.txt", "R\\file1.txt"));
	}

	SECTION("When left or right is defined (only one)") {
		DirGenerator g;
		g.createTestDirs();
		FileManager manager;
		manager.defineLeft("./test");
		REQUIRE_THROWS(manager.copyFile("L\\file1.txt", "R\\file1.txt"));
	}
	SECTION("When right is defined") {
		DirGenerator g;
		g.createTestDirs();
		FileManager manager;
		manager.defineLeft("./test");
		manager.defineRight("./example");		
		REQUIRE_NOTHROW(manager.copyFile("L\\file1.txt", "R\\file1.txt"));
		SafeFileComparator comparator;
		REQUIRE(comparator.compare("./test", "file1.txt", "./example", "file1.txt"));
	}
}

TEST_CASE("Testing deleteAt()") {
	SECTION("When left or right are not defined") {
		FileManager manager;
		REQUIRE_THROWS(manager.deleteAt("L\\file1.txt"));
	}

	SECTION("Trying to delete file") {
		DirGenerator g;
		g.createTestDirs();
		FileManager manager;
		manager.defineLeft("./test");
		REQUIRE_NOTHROW(manager.deleteAt("L\\file1.txt"));
		REQUIRE(fs::exists("./test"));
		REQUIRE(!fs::exists("./test/file1.txt"));
	}
	SECTION("Trying to delete file") {
		DirGenerator g;
		g.createTestDirs();
		FileManager manager;
		manager.defineLeft("./test");
		REQUIRE_NOTHROW(manager.deleteAt("L\\"));
		REQUIRE(!fs::exists("./test"));
		REQUIRE(!fs::exists("./test/file1.txt"));
	}
}


//bool deleteAt(const fs::path& toDelete) const;