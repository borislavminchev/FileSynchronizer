#include "FileComparator.h"
#include <stdexcept>
#include <fstream>

namespace fs = std::filesystem;

bool  SafeFileComparator::compareContent(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const
{
	std::ifstream stream1(d1 / f1, std::ios_base::binary);
	std::ifstream stream2(d2 / f2, std::ios_base::binary);
	char c1;
	char c2;
	while (!stream1.eof() && !stream2.eof())
	{
		stream1.read(&c1, sizeof(char));
		stream2.read(&c2, sizeof(char));
		if (c1 != c2)
		{
			return false;
		}
	}

	if (stream1.eof() && stream2.eof())
	{
		stream1.close();
		stream2.close();
		return true;
	}

	stream1.close();
	stream2.close();
	return false;
}


bool SafeFileComparator::compare(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const
{
	if (!fs::is_directory(d1) || !fs::is_directory(d2))
	{
		throw std::invalid_argument("Some of given directoy paths of comparator are not directory");
	}
	if (fs::is_directory(f1) || fs::is_directory(f2))
	{
		throw std::invalid_argument("Some of given file paths of comparator is directory");
	}
	bool b1 = f1 == f2;
	bool b2 = fs::last_write_time(fs::absolute(d1 / f1)) == fs::last_write_time(d2 / f1);
	bool b3 = fs::file_size(fs::absolute(d1 / f1)) == fs::file_size(fs::absolute(d2 / f2));
	if (!(b1 && b2 && b3))
	{
		return false;
	}
	
	return compareContent(d1, f1, d2, f2);
}