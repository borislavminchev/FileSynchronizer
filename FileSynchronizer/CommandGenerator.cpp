#include "CommandGenerator.h"

CommandGenerator::CommandGenerator(const fs::path& l, const fs::path& r)
{
	if (!fs::is_directory(l))
	{
		throw std::invalid_argument("Left directoy paths of generator are not directory");
	}


	this->lPath = l;
	lPath.make_preferred();
	if (!fs::is_directory(r))
	{
		throw std::invalid_argument("Right directoy paths of generator are not directory");
	}

	this->rPath = r;
	rPath.make_preferred();
}
std::string CommandGenerator::defineLeft() const
{
	return "LEFT is \"" + this->lPath.string() + "\"";
}

std::string CommandGenerator::defineRight() const
{
	return "RIGHT is \"" + this->rPath.string() + "\"";
}

std::string CommandGenerator::copy(const fs::path& srcDir, const fs::path& src, const fs::path& targetDir, const fs::path& target) const
{
	if (srcDir == lPath && targetDir == rPath)
	{
		
		return "COPY \"L\\" + src.string() + "\" \"R\\" + target.string() + "\"";
	}
	else if (srcDir == rPath && targetDir == lPath)
	{
		return "COPY \"R\\" + src.string() + "\" \"L\\" + target.string() + "\"";
	}

	throw std::runtime_error("Some error occured while generating copy command (maybe wrong paths)");
}

std::string CommandGenerator::createDir(const fs::path& srcDir, const fs::path& dir) const
{
	if (srcDir == lPath)
	{
		return "CREATE-DIR \"L\\" + dir.string() + "\"";
	}
	else if (srcDir == rPath)
	{
		return "CREATE-DIR \"R\\" + dir.string() + "\"";
	}

	throw std::runtime_error("Some error occured while generating create-dir command (maybe wrong paths)");
}

std::string CommandGenerator::deepCpyDir(const fs::path& srcDir,const fs::path& dir, const fs::path& targetDir) const
{
	std::string res;
	for (const fs::directory_entry& entry : fs::recursive_directory_iterator(srcDir / dir))
	{
		if (fs::is_directory(entry.path()))
		{
			res += createDir(targetDir, fs::relative(entry.path(), srcDir)) + "\n";
		}
		else
		{
			std::string r;
			if (srcDir == lPath && targetDir == rPath)
			{
				res += "COPY \"L\\" + fs::relative(entry.path(), srcDir).string() + "\" \"R\\" + fs::relative(entry.path(), srcDir).string() + "\"" + "\n";
			}
			else if (srcDir == rPath && targetDir == lPath)
			{
				res += "COPY \"R\\" + fs::relative(entry.path(), srcDir).string() + "\" \"L\\" + fs::relative(entry.path(), srcDir).string() + "\"" + "\n";
			}
		}
	}
	res.pop_back();
	return res;
}

std::string CommandGenerator::deletePath(const fs::path& srcDir, const fs::path& path) const
{
	if (srcDir == lPath)
	{
		return "DELETE \"L\\" + path.string() + "\"";
	}
	else if (srcDir == rPath)
	{
		return "DELETE \"R\\" + path.string() + "\"";
	}

	throw std::runtime_error("Some error occured while generating delete command (maybe wrong paths)");
}

