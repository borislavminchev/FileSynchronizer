#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "FileManager.h"
#include "tclap/CmdLine.h"

namespace fs = std::filesystem;

/// <summary>
/// Interpreter class that reads commands from cmd file and executes them.
/// If there is some problem with execution of certain command, then no chandes are made to original directories.
/// </summary>
class CmdFileInterpreter
{
private:

	/// <summary>
	/// Manager that executes given command
	/// </summary>
	FileManager manager;

	/// <summary>
	/// path to original left dir
	/// </summary>
	fs::path originalLeft;

	/// <summary>
	/// path to original right dir
	/// </summary>
	fs::path originalRight;

	/// <summary>
	/// path to temp left dir where all chandes are made and finally all content of that dir is coppied back to originalLeft
	/// </summary>
	fs::path tempLeft;

	/// <summary>
	/// path to temp right dir where all chandes are made and finally all content of that dir is coppied back to originalRight
	/// </summary>
	fs::path tempRight;
	
	/// <summary>
	/// Interprets command for defining left path
	/// </summary>
	/// <param name="a">command arguments</param>
	void doLeft(std::vector<std::string>& a)
	{
		try {
			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

			TCLAP::UnlabeledValueArg<std::string> nolabel("is", "confirmation", true, "default", "nameString");
			cmd.add(nolabel);

			TCLAP::UnlabeledValueArg<fs::path> targetPath("path", "targetPath", true, "C:\\Users\\Desktop", "nameString");
			cmd.add(targetPath);

			cmd.parse(a);

			std::string confirm = nolabel.getValue();
			fs::path p = targetPath.getValue();

			if (confirm == "is")
			{
				this->originalLeft = p;
				this->tempLeft =fs::path(".\\Temp") / p.filename();
				fs::create_directories(tempLeft);
				fs::copy(this->originalLeft, this->tempLeft, fs::copy_options::recursive);
				manager.defineLeft(tempLeft);
			}
			else
			{
				throw std::runtime_error("Invalid commad");
			}

		}
		catch (TCLAP::ArgException& e)
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
	}

	/// <summary>
	/// Interprets command for defining right path
	/// </summary>
	/// <param name="a">command arguments</param>
	void doRight(std::vector<std::string>& a)
	{
		try {
			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

			TCLAP::UnlabeledValueArg<std::string> nolabel("is", "confirmation", true, "default", "nameString");
			cmd.add(nolabel);

			TCLAP::UnlabeledValueArg<fs::path> targetPath("path", "targetPath", true, "C:\\Users\\Desktop", "nameString");
			cmd.add(targetPath);

			cmd.parse(a);

			std::string confirm = nolabel.getValue();
			fs::path p = targetPath.getValue();

			if (confirm == "is")
			{
				this->originalRight = p;
				this->tempRight =fs::path(".\\Temp") / p.filename();
				fs::create_directories(tempRight);
				fs::copy(this->originalRight, this->tempRight, fs::copy_options::recursive);

				manager.defineRight(tempRight);
			}
			else
			{
				throw std::runtime_error("Invalid commad");
			}
		}
		catch (TCLAP::ArgException& e)
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
	}

	/// <summary>
	/// Interprets command for copping one file to other
	/// </summary>
	/// <param name="a">command arguments</param>
	void doCopy(std::vector<std::string>& a) const
	{
		try {
			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

			
			TCLAP::UnlabeledValueArg<fs::path> srcPath("srcPath", "srcPath", true, "C:\\Desktop", "nameString");
			cmd.add(srcPath);

			TCLAP::UnlabeledValueArg<fs::path> targetPath("targetPath", "targetPath", true, "C:\\Desktop", "nameString");
			cmd.add(targetPath);

			cmd.parse(a);

			fs::path p1 = srcPath.getValue();
			fs::path p2 = targetPath.getValue();

			manager.copyFile(p1, p2);

		}
		catch (TCLAP::ArgException& e)
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
	}

	/// <summary>
	/// Interprets command for creating new dir at given path 
	/// </summary>
	/// <param name="a">command arguments</param>
	void doCreateDir(std::vector<std::string>& a) const
	{
		try {
			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

			TCLAP::UnlabeledValueArg<fs::path> targetPath("targetPath", "targetPath", true, "C:\\Desktop", "nameString");
			cmd.add(targetPath);

			cmd.parse(a);

			fs::path p2 = targetPath.getValue();

			manager.createDirectory(p2);
		}
		catch (TCLAP::ArgException& e)
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
	}

	/// <summary>
	/// Interprets command for deleting element
	/// </summary>
	/// <param name="a">command arguments</param>
	void doDelete(std::vector<std::string>& a) const
	{
		try {
			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

			TCLAP::UnlabeledValueArg<fs::path> targetPath("targetPath", "targetPath", true, "C:\\Desktop", "nameString");
			cmd.add(targetPath);

			cmd.parse(a);

			fs::path p2 = targetPath.getValue();
			manager.deleteAt(p2);
		}
		catch (TCLAP::ArgException& e)
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
	}

	/// <summary>
	/// Splits given line
	/// </summary>
	/// <param name="line">command as one line</param>
	/// <returns>All arguments of given line</returns>
	std::vector<std::string> readArgs(const std::string& line) const
	{
		std::istringstream iss(line);
		std::vector<std::string> v;
		std::string s;

		while (iss >> std::quoted(s, '\"', '\n')) {
			v.push_back(s);
		}

		return v;
	}

	/// <summary>
	/// Executes generic command. By first argument function passes all arguments to corresponding function.
	/// </summary>
	/// <param name="line">command to be interpreted</param>
	void executeCommand(const std::string& line)
	{
		std::vector<std::string> args = readArgs(line);
		std::string command = args[0];
		if (command == "LEFT")
		{
			doLeft(args);
		}
		else if (command == "RIGHT")
		{
			doRight(args);
		}
		else if (command == "COPY")
		{
			doCopy(args);
		}
		else if (command == "CREATE-DIR")
		{
			doCreateDir(args);
		}
		else if (command == "DELETE")
		{
			doDelete(args);
		}
		else
		{
			std::cout << "Could't recognize command: " << line << std::endl;
		}
	}

public:
	/// <summary>
	/// Executes commands from given file path;
	/// </summary>
	/// <param name="filePath">path to cmd file</param>
	void exectuteFileCommands(const std::string& filePath)
	{
		fs::create_directories(".\\Temp");
		try
		{
			std::ifstream stream(filePath);
			std::string buffer = "";
			while (std::getline(stream, buffer))
			{
				if (!buffer.empty())
				{
					executeCommand(buffer);
				}
				buffer.clear();
			}
			stream.close();
		}
		catch (const std::exception& e)
		{
			std::cout << "Error occured: " << e.what() << std::endl;
			std::cout << "\n\n NO CHANGES WERE MAADE TO GIVEN FOLDERS!" << std::endl;
			fs::remove_all(".\\Temp");
			return;
		}
		
		
		fs::remove_all(this->originalLeft);
		fs::remove_all(this->originalRight);

		fs::copy(this->tempLeft, this->originalLeft, fs::copy_options::recursive);
		fs::copy(this->tempRight, this->originalRight, fs::copy_options::recursive);

		fs::remove_all(".\\Temp");

		std::cout << "Cmd file was executed successfully\n";
	}
};
