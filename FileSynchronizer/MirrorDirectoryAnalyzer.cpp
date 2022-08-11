#include "DirectoryAnalyzer.h"

void MirrorDirectoryAnalyzer::compareEntries(const std::list<fs::path>& leftEntries, const std::list<fs::path>& rightEntries) const
{
	if (leftEntries.empty() && rightEntries.empty())
	{
		return;
	}

	std::list<fs::path> entriesToAnalyzeLeft;
	std::list<fs::path> entriesToAnalyzeRight;


	std::list<fs::path> common = this->intersect(leftEntries, rightEntries);
	std::list<fs::path> diffLeft = this->difference(leftEntries, rightEntries);
	std::list<fs::path> diffRight = this->difference(rightEntries, leftEntries);

	//delete all different elements from right
	for (std::list<fs::path>::iterator i = diffRight.begin(); i != diffRight.end(); i++)
	{
		this->writeToFile(this->generator.deletePath(this->right, *i));
	}

	//create all different elements from left to right
	for (std::list<fs::path>::iterator i = diffLeft.begin(); i != diffLeft.end(); i++)
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


	for (std::list<fs::path>::iterator i = common.begin(); i != common.end(); i++)
	{
		if (!fs::is_directory(this->left / *i) && !fs::is_directory(this->right / *i))
		{
			if (!this->comparator->compare(this->left, *i, this->right, *i))
			{
				this->writeToFile(this->generator.copy(this->left, *i, this->right, *i));
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