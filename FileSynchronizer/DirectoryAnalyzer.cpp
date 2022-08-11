#include "DirectoryAnalyzer.h"
#include <stdexcept>
#include <fstream>

std::list<fs::path> DirectoryAnalyzer::intersect(const std::list<fs::path>& l1, const std::list<fs::path>& l2) const
{
	std::list<fs::path>::const_iterator it1 = l1.cbegin();
	std::list<fs::path>::const_iterator it2 = l2.cbegin();
	std::list<fs::path> res;
	while (it1 != l1.cend() && it2 != l2.cend()) {
		if (*it1 < *it2) {
			it1++;
		}
		else {
			if (*it1 == *it1) {
				res.push_back(*it1);
				it1++;
			}

			++it2;
		}
	}
	return res;
}

std::list<fs::path> DirectoryAnalyzer::difference(const std::list<fs::path>& l1, const std::list<fs::path>& l2) const
{
	if (l2.empty())
	{
		return l1;
	}
	std::list<fs::path>::const_iterator it1 = l1.cbegin();
	std::list<fs::path>::const_iterator it2 = l2.cbegin();
	std::list<fs::path> res;
	while (it1 != l1.cend()) {
		if (it2 == l2.cend())
		{
			res.insert(res.cend(), it1, l1.cend());
			return res;
		}

		if (*it1 < *it2) {
			res.push_back(*it1);
			it1++;
		}
		else {
			if (*it1 == *it2) {
				it1++;
			}
			it2++;
		}
	}
	return res;
}

void DirectoryAnalyzer::insert_sorted(std::list<fs::path>& list, const fs::path& path) const
{
	for (std::list<fs::path>::iterator i = list.begin(); i != list.end(); i++)
	{
		if (i->string() > path.string())
		{
			list.insert(i, path);
			return;
		}
	}
	list.push_back(path);
}

void DirectoryAnalyzer::writeToFile(const std::string& str) const
{
	std::ofstream stream(this->cmdFile, std::ios_base::app);
	stream << str << std::endl;
	stream.close();
}

std::list<fs::path> DirectoryAnalyzer::getSubEntries(const fs::path& root, std::list<fs::path>& entries) const
{
	std::list<fs::path> res;
	for (std::list<fs::path>::iterator entry = entries.begin(); entry != entries.end(); ++entry)
	{
		fs::path current = root / (*entry);
		fs::directory_iterator end_iter;
		for (fs::directory_iterator i(current); i != end_iter; ++i)
		{
			insert_sorted(res, fs::relative(i->path(), root));
		}
	}

	
	return res;
}

DirectoryAnalyzer::DirectoryAnalyzer(const fs::path& l, const fs::path& r,const std::string& file, const ComparingMethod& method):generator(l, r)
{
	if (!fs::is_directory(l) || !fs::is_directory(r))
	{
		throw std::invalid_argument("Given directoy paths are not directories");
	}
	this->left = l;
	this->right = r;
	this->cmdFile = file;
	this->generator = CommandGenerator(this->left, this->right);
	switch (method)
	{
	case QUICK: this->comparator = new QuickFileComparator(); break;
	case SAFE: this->comparator = new SafeFileComparator(); break;
	default:
		throw std::runtime_error("Comparing method not recognized");
	}

	std::ofstream stream(this->cmdFile);
	stream << "";
	stream.close();
}

void DirectoryAnalyzer::analyze() const
{
	this->writeToFile(this->generator.defineLeft());
	this->writeToFile(this->generator.defineRight());
	std::list<fs::path> l;
	l.push_back("");
	std::list<fs::path> r;
	r.push_back("");
	compareEntries(this->getSubEntries(this->left, l), this->getSubEntries(this->right, r));
}

DirectoryAnalyzer::~DirectoryAnalyzer()
{
	delete this->comparator;
}