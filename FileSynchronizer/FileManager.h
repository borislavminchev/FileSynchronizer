#pragma once
#include <filesystem>

namespace fs = std::filesystem;

/// <summary>
/// Manager that executes commands in file system
/// </summary>
class FileManager
{
private:
	/// <summary>
	/// path to left dir
	/// </summary>
	fs::path left;

	/// <summary>
	/// path to right dir
	/// </summary>
	fs::path right;

	/// <summary>
	/// extracts root from given path(left and right path must be set)
	/// </summary>
	/// <param name="path">path from where root will be extracted</param>
	/// <returns></returns>
	std::string extractRoot(const fs::path& path) const;
public:

	/// <summary>
	/// defines left path
	/// </summary>
	/// <param name="l">value to be set</param>
	void defineLeft(const fs::path& l);

	/// <summary>
	/// defines right path
	/// </summary>
	/// <param name="r">value to be set</param>
	void defineRight(const fs::path& r);

	/// <summary>
	/// creates directory at given path
	/// </summary>
	/// <param name="directoryToCreate">path of ditectory that will be ctreated</param>
	/// <returns></returns>
	bool createDirectory(const fs::path& directoryToCreate) const;

	/// <summary>
	/// copies file to other file
	/// </summary>
	/// <param name="srcFile">source file</param>
	/// <param name="targetFile">target file</param>
	/// <returns></returns>
	bool copyFile(const fs::path& srcFile, const fs::path& targetFile) const;

	/// <summary>
	/// deletes flie at given path
	/// </summary>
	/// <param name="toDelete">path to element fo be deleted</param>
	/// <returns></returns>
	bool deleteAt(const fs::path& toDelete) const;
};
