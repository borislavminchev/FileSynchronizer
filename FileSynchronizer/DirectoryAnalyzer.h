
#pragma once
#include <filesystem>
#include <iostream>
#include <queue>
#include "FileComparator.h"
#include "CommandGenerator.h"

namespace fs = std::filesystem;

/// <summary>
/// Analyzer that generates commands in cmd file which will be executed lately
/// </summary>
class DirectoryAnalyzer
{
private:
	/// <summary>
	/// helper method that inserts element in already sorted list and keeps list sorted after that
	/// </summary>
	/// <param name="list">list where element will be inserted</param>
	/// <param name="path">new element value</param>
	void insert_sorted(std::list<fs::path>& list, const fs::path& path) const;
protected:
	/// <summary>
	/// Comparator for files
	/// </summary>
	FileComparator* comparator;
	/// <summary>
	/// Generates commands that will be written to cmd file
	/// </summary>
	CommandGenerator generator;
	/// <summary>
	/// path to left dir
	/// </summary>
	fs::path left;
	/// <summary>
	/// path to right dir
	/// </summary>
	fs::path right;
	/// <summary>
	/// path to cmd file
	/// </summary>
	std::string cmdFile;

	/// <summary>
	/// intersects elements of two lists( both must be sorted)
	/// </summary>
	/// <param name="l1">first list</param>
	/// <param name="l2">second list</param>
	/// <returns>intersectio of 2 list</returns>
	std::list<fs::path> intersect(const std::list<fs::path>& l1, const std::list<fs::path>& l2) const;

	/// <summary>
	/// gets all elements from first list that are not in second list(both list must be sorted)
	/// </summary>
	/// <param name="l1">first list</param>
	/// <param name="l2">second list</param>
	/// <returns>all different elements</returns>
	std::list<fs::path> difference(const std::list<fs::path>& l1, const std::list<fs::path>& l2) const;

	/// <summary>
	/// writes command in given file
	/// </summary>
	/// <param name="str">command to be written</param>
	void writeToFile(const std::string& str) const;

	/// <summary>
	/// gets all subentries from given direcory
	/// </summary>
	/// <param name="root">path to root dir</param>
	/// <param name="entries">entries from which will get sub entries</param>
	/// <returns>all subentries</returns>
	std::list<fs::path> getSubEntries(const fs::path& root, std::list<fs::path>& entries) const;

	/// <summary>
	/// compares directory entries and writes commads in cmd file for synchronization on these entries
	/// </summary>
	/// <param name="leftEntries">left entries to be compared</param>
	/// <param name="rightEntries">right entries to be compared</param>
	virtual void compareEntries(const std::list<fs::path>& leftEntries, const std::list<fs::path>& rightEntries) const = 0;
public:
	DirectoryAnalyzer() = delete;
	/// <summary>
	/// constructs the analyzer
	/// </summary>
	/// <param name="l">left dir path</param>
	/// <param name="r">right dir path</param>
	/// <param name="file">cmd file path</param>
	/// <param name="method">file comparing method</param>
	DirectoryAnalyzer(const fs::path& l, const fs::path& r,const std::string& file, const ComparingMethod& method);

	/// <summary>
	/// starts analyzing process
	/// </summary>
	virtual void analyze() const;

	
	~DirectoryAnalyzer();
};

/// <summary>
/// analyzes 2 given direcories using mirror policy
/// </summary>
class MirrorDirectoryAnalyzer : public DirectoryAnalyzer
{
private:
	/// <summary>
	/// base class method implementation
	/// </summary>
	/// <param name="leftEntries">left entries to analyze</param>
	/// <param name="rightEntries">right entries to analyze</param>
	virtual void compareEntries(const std::list<fs::path>& leftEntries, const std::list<fs::path>& rightEntries) const;
public:
	using DirectoryAnalyzer::DirectoryAnalyzer;
};

/// <summary>
/// analyzes 2 given direcories using safe policy
/// </summary>
class SafeDirectoryAnalyzer : public DirectoryAnalyzer
{
private:
	/// <summary>
	/// base class method implementation
	/// </summary>
	/// <param name="leftEntries">left entries to analyze</param>
	/// <param name="rightEntries">right entries to analyze</param>
	virtual void compareEntries(const std::list<fs::path>& leftEntries, const std::list<fs::path>& rightEntries) const;
public:
	using DirectoryAnalyzer::DirectoryAnalyzer;
};

/// <summary>
/// analyzes 2 given direcories using standart policy
/// </summary>
class StandartDirectoryAnalyzer : public DirectoryAnalyzer
{
private:
	/// <summary>
	/// base class method implementation
	/// </summary>
	/// <param name="leftEntries">left entries to analyze</param>
	/// <param name="rightEntries">right entries to analyze</param>
	virtual void compareEntries(const std::list<fs::path>& leftEntries, const std::list<fs::path>& rightEntries) const;
public:
	using DirectoryAnalyzer::DirectoryAnalyzer;
};