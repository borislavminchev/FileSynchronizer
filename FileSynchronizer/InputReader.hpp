#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "tclap/CmdLine.h"
#include "DirectoryAnalyzer.h"
#include "CmdFileInterpreter.hpp"

/// <summary>
/// Initial conversation and setup for further use
/// </summary>
class InputReader
{
private:
	/// <summary>
	/// analyzer that will be used
	/// </summary>
	DirectoryAnalyzer* analyzer;
	/// <summary>
	/// command interpreter
	/// </summary>
	CmdFileInterpreter interpreter;
	/// <summary>
	/// path to file with commands that will be executed
	/// </summary>
	std::string cmdFile;

	/// <summary>
	/// interprets setup command
	/// </summary>
	/// <param name="a">command arguments</param>
	void parseArgs(std::vector<std::string> a) {
		try {


			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

			TCLAP::ValueArg<fs::path> leftPath("l", "left", "left path to analyze", true, "", "string");
			cmd.add(leftPath);

			TCLAP::ValueArg<fs::path> rightPath("r", "right", "right path to analyze", true, "", "string");
			cmd.add(rightPath);

			TCLAP::ValueArg<std::string> cmdFile("f", "cmdFile", "file where commands will be stored", true, "file.txt", "string");
			cmd.add(cmdFile);

			TCLAP::OneOf analyzeMethod;
			TCLAP::SwitchArg mirror("m", "mirror", "Use mirror analyzer to analyze dirs", analyzeMethod, false);
			TCLAP::SwitchArg safe("s", "safe", "Use safe analyzer to analyze dirs", analyzeMethod, false);
			TCLAP::SwitchArg standart("d", "standart", "Use standart analyzer to analyze dirs", analyzeMethod, false);
			cmd.add(analyzeMethod);

			TCLAP::OneOf compareMethod;
			TCLAP::SwitchArg quick("Q", "quick", "Use quick metod to compare files", compareMethod, false);
			TCLAP::SwitchArg safeComp("S", "safeComp", "Use safe metod to compare files", compareMethod, false);
			cmd.add(compareMethod);

			cmd.parse(a);

			fs::path p1 = leftPath.getValue();
			fs::path p2 = rightPath.getValue();
			std::string file = cmdFile.getValue();
			this->cmdFile = file;
			
			if (mirror.isSet())
			{
				if (quick.isSet())
					this->analyzer = new MirrorDirectoryAnalyzer(p1, p2, file, QUICK);
				else if (safeComp.isSet())
					this->analyzer = new MirrorDirectoryAnalyzer(p1, p2, file, SAFE);
				else
					throw std::runtime_error("there was not provided file comparing method");
			}
			else if (safe.isSet())
			{
				if (quick.isSet())
					this->analyzer = new SafeDirectoryAnalyzer(p1, p2, file, QUICK);
				else if (safeComp.isSet())
					this->analyzer = new SafeDirectoryAnalyzer(p1, p2, file, SAFE);
				else
					throw std::runtime_error("there was not provided file comparing method");
			}
			else if (standart.isSet())
			{
				if (quick.isSet())
					this->analyzer = new StandartDirectoryAnalyzer(p1, p2, file, QUICK);
				else if (safeComp.isSet())
					this->analyzer = new StandartDirectoryAnalyzer(p1, p2, file, SAFE);
				else
					throw std::runtime_error("there was not provided file comparing method");
			}

		}
		catch (TCLAP::ArgException& e)
		{
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
	}

	/// <summary>
	/// Splits arguments from line
	/// </summary>
	/// <param name="line">line to be splitted</param>
	/// <returns>splited arguments</returns>
	std::vector<std::string> splitArgs(const std::string& line) const
	{
		std::istringstream iss(line);
		std::vector<std::string> v;
		std::string s;

		v.push_back("%");
		while (iss >> std::quoted(s, '\"', '\n')) {
			v.push_back(s);
		}

		return v;
	}

	/// <summary>
	/// reads setup configuration from user input
	/// </summary>
	void readInput()
	{
		std::string command;
		std::string s;
		std::cin >> s;
		std::getline(std::cin, command);
		command = s + " " + command;
		try
		{
			parseArgs(splitArgs(command));
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
	}

	std::string buildCommand(std::unordered_map<std::string, std::string>& map) const {
		std::string cmd;
		cmd += "-l " + map["left_folder"];
		cmd += " -r " + map["right_folder"];
		cmd += " -f " + map["output_file"];

		std::string analyzeMethod = map["analyze_method"];
		std::string compareMethod = map["compare_method"];

		if (analyzeMethod == "mirror") cmd += " -m ";
		else if(analyzeMethod == "safe") cmd += " -s ";
		else if(analyzeMethod == "standart") cmd += " -d ";

		if (compareMethod == "safe") cmd += " -S ";
		else if (compareMethod == "quick") cmd += " -Q ";

		return cmd;
	}

	/// <summary>
	/// reads setup configuration from file
	/// </summary>
	/// <param name="configFile">path to file</param>
	void readInput(const std::string& configFile)
	{
		try
		{
			std::unordered_map<std::string, std::string> args;
			args.insert({ "left_folder", "" });
			args.insert({ "right_folder", "" });
			args.insert({ "output_file", "" });
			args.insert({ "analyze_method", "" });
			args.insert({ "compare_method", "" });

			std::ifstream stream(configFile);
			std::string line;
			while (std::getline(stream, line)) {
				std::istringstream lineStream(line);
				std::string arg = "";
				if (std::getline(lineStream, arg, '=')) {
					std::string val = "";
					std::getline(lineStream, val);
					args[arg] = val;
				}
			}
			stream.close();
			std::string s = buildCommand(args);
			parseArgs(splitArgs(s));
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
	}

public:
	/// <summary>
	/// starts communication with user
	/// </summary>
	void startConversation()
	{
		std::cout << "How will you provide configuration (0)by input, (1)from file\n";
		int i;
		std::cin >> i;
		if (i == 0)
		{
			readInput();
		}
		else if (i == 1)
		{
			std::cout << "Please give config file path:\n";
			std::string s;
			std::cin >> s;
			readInput(s);
		}
		else
		{
			std::cout << "Invalid input\n";
		}

		this->analyzer->analyze();
		std::cout << "Do you want to edit cmd file: ";
		std::string confirmation;
		std::cin >> confirmation;
		if (confirmation == "yes" || confirmation == "y")
		{
			std::string some;
			do
			{
				std::cout << "now you can edit your file at path: " + fs::absolute(this->cmdFile).string() << std::endl;
				std::cout << "when you are redy type someting here: ";
				
				std::cin >> some;
				some.clear();
				std::cout << "Your file will be executed are you sure: ";
				std::cin >> some;
			} while (some != "yes");
			
		}
		this->interpreter.exectuteFileCommands(this->cmdFile);
	}

	~InputReader()
	{
		delete analyzer;
	}
};