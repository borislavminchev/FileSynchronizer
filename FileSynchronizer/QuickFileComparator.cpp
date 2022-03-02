#include "FileComparator.h"
#include <stdexcept>
namespace fs = std::filesystem;

bool QuickFileComparator::compare(const fs::path& d1, const fs::path& f1, const fs::path& d2, const fs::path& f2) const
{
	if (!fs::is_directory(d1) || !fs::is_directory(d2))
	{
		throw std::invalid_argument("Some of given directoy paths of comparator are not directory");
	}
	if (fs::is_directory(d1 / f1) || fs::is_directory(d2 / f2))
	{
		throw std::invalid_argument("Some of given file paths of comparator is directory");
	}

	bool b1 = f1 == f2;
	bool b2 = fs::last_write_time(fs::absolute(d1 / f1)) == fs::last_write_time(d2 / f1);
	bool b3 = fs::file_size(fs::absolute(d1 / f1)) == fs::file_size(fs::absolute(d2 / f2));
	return  b1 && b2 && b3;

}