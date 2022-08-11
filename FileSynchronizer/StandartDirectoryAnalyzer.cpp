#include "DirectoryAnalyzer.h"

void StandartDirectoryAnalyzer::compareEntries(const std::list<fs::path>& leftEntries, const std::list<fs::path>& rightEntries) const
{
	if (leftEntries.empty() && rightEntries.empty())
	{
		return;
	}
	std::list<fs::path> entriesToAnalyzeLeft;
	std::list<fs::path> entriesToAnalyzeRight;

	std::list<fs::path> common = this->intersect(leftEntries, rightEntries);
	std::list<fs::path> cpyLeft = this->difference(leftEntries, rightEntries);
	std::list<fs::path> cpyRight = this->difference(rightEntries, leftEntries);

	//create all different elements from left to right
	for (std::list<fs::path>::iterator i = cpyLeft.begin(); i != cpyLeft.end(); i++)
	{
		if (fs::is_directory(this->left / *i))
		{
			this->writeToFile(this->generator.createDir(this->right, *i));
			this->writeToFile(this->generator.deepCpyDir(this->left, *i, this->right));
		}
		else
		{
			this->writeToFile(this->generator.copy(this->left, *i, this->right, *i));
		}
	}
	//create all different elements from right to left
	for (std::list<fs::path>::iterator i = cpyRight.begin(); i != cpyRight.end(); i++)
	{
		if (fs::is_directory(this->right / *i))
		{
			this->writeToFile(this->generator.createDir(this->left, *i));
			this->writeToFile(this->generator.deepCpyDir(this->right, *i, this->left));
		}
		else
		{
			this->writeToFile(this->generator.copy(this->right, *i, this->left, *i));
		}

	}


	for (std::list<fs::path>::iterator i = common.begin(); i != common.end(); i++)
	{
		if (!fs::is_directory(this->left / *i) && !fs::is_directory(this->right / *i))
		{
			if (!this->comparator->compare(this->left, *i, this->right, *i))
			{
				if (fs::last_write_time(this->left / *i) < fs::last_write_time(this->right / *i))
				{
					this->writeToFile(this->generator.copy(this->right, *i, this->left, *i));
				}
				else
				{
					this->writeToFile(this->generator.copy(this->left, *i, this->right, *i));
				}
			}
		}
		else
		{
			entriesToAnalyzeLeft.push_back(*i);
			entriesToAnalyzeRight.push_back(*i);
		}
	}

	compareEntries(this->getSubEntries(this->left, entriesToAnalyzeLeft), this->getSubEntries(this->right, entriesToAnalyzeRight));
}