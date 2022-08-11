#pragma once

#include <filesystem>

namespace fs = std::filesystem;

/// <summary>
/// Method for comparing file paths
/// </summary>
enum ComparingMethod
{
	QUICK,
	SAFE
};

/// <summary>
/// Base class for comparing files.
/// </summary>
class FileComparator
{
public:
	/// <summary>
	/// method that compares given files at given path
	/// </summary>
	/// <param name="d1">root path to first file</param>
	/// <param name="f1">path to first file from root path</param>
	/// <param name="d2">root path to second file</param>
	/// <param name="f2">path to second file from root path</param>
	virtual bool compare(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const = 0;
};

/// <summary>
/// Compares files with quick comparison policy
/// </summary>
class QuickFileComparator : public FileComparator
{
public:
	/// <summary>
	/// method that compares given files at given path
	/// </summary>
	/// <param name="d1">root path to first file</param>
	/// <param name="f1">path to first file from root path</param>
	/// <param name="d2">root path to second file</param>
	/// <param name="f2">path to second file from root path</param>
	virtual bool compare(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const;
};

/// <summary>
/// Compares files with safe comparison policy
/// </summary>
class SafeFileComparator : public FileComparator
{
private:
	/// <summary>
	/// method implementationo of base class method
	/// </summary>
	/// <param name="d1">root path to first file</param>
	/// <param name="f1">path to first file from root path</param>
	/// <param name="d2">root path to second file</param>
	/// <param name="f2">path to second file from root path</param>
	bool compareContent(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const;
public:
	/// <summary>
	/// method implementationo of base class method
	/// </summary>
	/// <param name="d1">root path to first file</param>
	/// <param name="f1">path to first file from root path</param>
	/// <param name="d2">root path to second file</param>
	/// <param name="f2">path to second file from root path</param>
	virtual bool compare(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const;
};