#include "FileManager.h"
#include <stdexcept>

std::string FileManager::extractRoot(const fs::path& path) const
{
	std::string s = "";
	std::string asString = path.string();
	int i = 0;
	while (asString[i] != '\\')
	{
		s += asString[i];
		i++;
	}

	return s;
}

void FileManager::defineLeft(const fs::path& l)
{
	if (!fs::is_directory(l))
	{
		throw std::invalid_argument("Given a file when was expected a directory");
	}

	this->left = l;
	
}
void FileManager::defineRight(const fs::path& r)
{
	if (!fs::is_directory(r))
	{
		throw std::invalid_argument("Given a file when was expected a directory");
	}

	this->right = r;
}

bool FileManager::createDirectory(const fs::path& directoryToCreate) const
{
	
	
	std::string root = extractRoot(directoryToCreate);
	fs::path rest = fs::relative(directoryToCreate, root);
	if (root == "L")
	{
		if (this->left.string().empty())
		{
			throw std::runtime_error("Left path was not defined");
		}
		return fs::create_directories((this->left / rest));
	}
	else if (root == "R")
	{
		if (this->right.string().empty())
		{
			throw std::runtime_error("Right path was not defined");
		}
		return fs::create_directories((this->right / rest));
	}

	throw std::runtime_error("Given path was not in valid format");
}


bool FileManager::copyFile(const fs::path& srcFile, const fs::path& targetFile) const
{
	if (fs::is_directory(srcFile) || fs::is_directory(targetFile))
	{
		throw std::invalid_argument("Given a directory when was expected a file");
	}
	std::string rootSrc = extractRoot(srcFile);
	fs::path restSrc = fs::relative(srcFile, rootSrc);
	std::string rootTarget = extractRoot(targetFile);
	fs::path restTarget = fs::relative(targetFile, rootTarget);

	if (rootSrc == "L" && rootTarget == "R")
	{
		if (this->left.string().empty() || this->right.string().empty())
		{
			throw std::runtime_error("Left or right path was not defined");
		}
		return fs::copy_file(this->left / restSrc, this->right / restTarget, fs::copy_options::overwrite_existing);
	} 
	else if (rootSrc == "R" && rootTarget == "L")
	{
		if (this->left.string().empty() || this->right.string().empty())
		{
			throw std::runtime_error("Left or right path was not defined");
		}
		return fs::copy_file(this->right / restSrc, this->left / restTarget, fs::copy_options::overwrite_existing);
	}

	throw std::runtime_error("Given path was not in valid format");
}

bool FileManager::deleteAt(const fs::path& toDelete) const
{
	std::string root = extractRoot(toDelete);
	fs::path rest = fs::relative(toDelete, root);
	if (root == "L")
	{
		if (this->left.string().empty())
		{
			throw std::runtime_error("Left path was not defined");
		}
		return fs::remove_all((this->left / rest));
	}
	else if (root == "R")
	{
		if (this->right.string().empty())
		{
			throw std::runtime_error("Right path was not defined");
		}
		return fs::remove_all((this->right / rest));
	}

	throw std::runtime_error("Given path was not in valid format");
}
