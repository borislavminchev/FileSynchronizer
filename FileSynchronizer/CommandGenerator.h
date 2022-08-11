#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

/// <summary>
/// Class that generates commands for cmd file
/// </summary>
class CommandGenerator
{
private:
	/// <summary>
	/// path to left dir
	/// </summary>
	fs::path lPath;

	/// <summary>
	/// path to right dir
	/// </summary>
	fs::path rPath;
public:
	CommandGenerator() = delete;

	/// <summary>
	/// Constructs file object and sets paths to left arg right dir
	/// </summary>
	/// <param name="l">left dir path</param>
	/// <param name="r">right dir path</param>
	CommandGenerator(const fs::path& l, const fs::path& r);

	/// <summary>
	/// Returns message for defining left dir path
	/// </summary>
	/// <returns>message for defining left dir path as string</returns>
	std::string defineLeft() const;

	/// <summary>
	/// Returns message for defining right dir path
	/// </summary>
	/// <returns>message for defining right dir path as string</returns>
	std::string defineRight() const;

	/// <summary>
	/// Generates message for coping src file to target file
	/// </summary>
	/// <param name="srcDir">root dir of src from which command will be generated (could be either lPath or rPath )</param>
	/// <param name="src">path to file that will be coppied</param>
	/// <param name="targetDir">root dir of target from which command will be generated (could be either lPath or rPath )</param>
	/// <param name="target">path to file where new content will be written</param>
	/// <returns>string for coping src file to target file</returns>
	std::string copy(const fs::path& srcDir, const fs::path& src, const fs::path& targetDir,const fs::path& target) const;

	/// <summary>
	/// Generates command for creating new dir
	/// </summary>
	/// <param name="srcDir">root dir from which command will be generated (could be either lPath or rPath)</param>
	/// <param name="dir">path to dir that will be created</param>
	/// <returns>string for creating new dir</returns>
	std::string createDir(const fs::path& srcDir, const fs::path& dir) const;

	/// <summary>
	/// Generates command for deleting certain element in given path
	/// </summary>
	/// <param name="srcDir">root dir from which command will be generated (could be either lPath or rPath)</param>
	/// <param name="path">path to element that wil be deleted</param>
	/// <returns>string for deleting certain element in given path</returns>
	std::string deletePath(const fs::path& srcDir, const fs::path& path) const;

	/// <summary>
	/// Generates message for coping src dir to target dir where all sub elements of src dir are also copied
	/// </summary>
	/// <param name="srcDir">root dir from which command will be generated (could be either lPath or rPath)</param>
	/// <param name="dir">path to dir taht will be copied</param>
	/// <param name="targetDir">root to dir where elements will be coppied</param>
	/// <returns>string for coping src dir to target dir where all sub elements of src dir are also copied</returns>
	std::string deepCpyDir(const fs::path& srcDir,const fs::path& dir, const fs::path& targetDir) const;
};