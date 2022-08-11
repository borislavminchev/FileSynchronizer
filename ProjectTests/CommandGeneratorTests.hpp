
#include"catch.hpp"
#include"helper.hpp"
#include"../FileSynchronizer/CommandGenerator.h"

void createNonExisting() {
	CommandGenerator generator("./left", "./right");
}

void createExisting() {
	CommandGenerator generator("./test", "./example");
}

TEST_CASE("Testing creating Command generator") {
	DirGenerator g;
	g.createTestDirs();
	SECTION("Testing with non existing dirs") {
		REQUIRE_THROWS(createNonExisting());
	}
	SECTION("Testing with existing dirs") {
		REQUIRE_NOTHROW(createExisting());
	}
}

TEST_CASE("Testing defineLeft() and defineRight") {
	DirGenerator g;
	g.createTestDirs();
	CommandGenerator generator("./test", "./example");
	SECTION("Check if define left is working") {
		REQUIRE(generator.defineLeft() == std::string("LEFT is \".\\test\""));
	}
	SECTION("Check if define right is working") {
		REQUIRE(generator.defineRight() == std::string("RIGHT is \".\\example\""));
	}
	
}

TEST_CASE("Testing copy()") {
	DirGenerator g;
	g.createTestDirs();
	CommandGenerator generator("./test", "./example");

	SECTION("when source and target dirs are non-existing") {
		REQUIRE_THROWS(generator.copy("./notexisting", "./notexisting/f.txt", "./example", "./other/file1.txt"));
		REQUIRE_THROWS(generator.copy("./test", "./test/file1.txt", "./other", "./other/f.txt"));
	}

	SECTION("when source is left") {
		REQUIRE(generator.copy("./test", "file1.txt", "./example", "file1.txt") == 
									"COPY \"L\\file1.txt\" \"R\\file1.txt\"");
	}
	SECTION("when source is right") {
		REQUIRE(generator.copy("./example", "file1.txt", "./test", "file1.txt") ==
			"COPY \"R\\file1.txt\" \"L\\file1.txt\"");
	}
	
}

TEST_CASE("Testing createDir()") {
	DirGenerator g;
	g.createTestDirs();
	CommandGenerator generator("./test", "./example");

	SECTION("when source is left") {
		REQUIRE(generator.createDir("./test", "folder") ==
			"CREATE-DIR \"L\\folder\"");
	}
	SECTION("when source is right") {
		REQUIRE(generator.createDir("./example", "folder") ==
			"CREATE-DIR \"R\\folder\"");
	}

}

TEST_CASE("Testing deletePath()") {
	DirGenerator g;
	g.createTestDirs();
	CommandGenerator generator("./test", "./example");

	SECTION("when source is left") {
		REQUIRE(generator.deletePath("./test", "folder") ==
			"DELETE \"L\\folder\"");
	}
	SECTION("when source is right") {
		REQUIRE(generator.deletePath("./example", "folder") ==
			"DELETE \"R\\folder\"");
	}
}

TEST_CASE("Testing deepCpyDir()") {
	DirGenerator g;
	g.createTestDirs();
	fs::create_directory("./test/folder");
	std::ofstream stream("./test/folder/f.txt");
	stream << "something";
	stream.close();

	CommandGenerator generator("./test", "./example");

	SECTION("when source is left") {
		REQUIRE(generator.deepCpyDir("./test", "", "./example") ==
			"COPY \"L\\file1.txt\" \"R\\file1.txt\"\n" + 
			std::string("CREATE-DIR \"R\\folder\"\n") +
			"COPY \"L\\folder\\f.txt\" \"R\\folder\\f.txt\"");
	}
}