#include <iostream>
#include <string>
#include <cstdlib>
#include <direct.h>
#include <fstream>

struct buildCommandRet
{
	std::string command;
	std::string outFile;
	std::string fileName;
};

struct outputResults
{
	double missRate;
	double memCPI;
	double accTime;
};

// Builds the string to run the command
buildCommandRet buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
{
	// EXAMPLE: .\CacheSim.exe -a 2 -s 32 -l 64 -mp 30 < art.trace > art_2_32_64_30.stats
	// -a: associativitty
	// -s: size of cache in KB
	// -l: the size of a cache line/block in bytes
	// -mp: the miss penalty in cycles

	buildCommandRet ret;

	std::string cacheSimulatorPath = R"(.\CacheSim.exe)";
	std::string associativityStr = std::to_string(associativity);
	std::string cacheSizeStr = std::to_string(cacheSize);
	std::string lineSizeStr = std::to_string(lineSize);
	std::string missPenaltyStr = std::to_string(missPenalty);

	// Find name of outFile (everything before file extension)
	std::string name = inFile.substr(0, inFile.find_last_of("."));
	std::string outFile = directoryName + "/" + name +"_"+associativityStr+"_"+cacheSizeStr+"_"+lineSizeStr+"_"+missPenaltyStr+".stats";


	ret.command = cacheSimulatorPath + " -a " + associativityStr + " -s " + cacheSizeStr + " -l " + lineSizeStr + " -mp " + missPenaltyStr + " < " + inFile + " > " + outFile;
	ret.outFile = outFile;
	ret.fileName = name;
	std::cout << ret.command << std::endl;

	return ret;
}

// Creates an output directory file that will be used to store the contents of each files simulation
void outputDirectory(const std::string& directoryName)
{
	int status = _mkdir(directoryName.c_str());
	if (status == 0) 
	{
		std::cout << "Directory created successfully!" << std::endl;
	}
	else 
	{
		std::cerr << "Error creating directory: " << std::endl;
	}
}

outputResults findResults(std::string outFile)
{
	outputResults results = {0.0, 0.0, 0.0};
	std::string tempStr;
	std::ifstream out(outFile);

	// Read file line by line
	std::string line;
	while (getline(out, line))
	{
		if (line.find("average memory access time ") != std::string::npos)
		{
			// Takes just the number from the string
			tempStr = line.substr(28);
			tempStr = tempStr.substr(0, tempStr.find(" "));
			results.accTime = std::stod(tempStr);
			std::cout << results.accTime << std::endl;
		}
		if (line.find("memory CPI ") != std::string::npos)
		{
			// Takes just the number from the string
			tempStr = line.substr(12);
			tempStr = tempStr.substr(0, tempStr.find(" "));
			results.memCPI = std::stod(tempStr);
			std::cout << results.memCPI << std::endl;
		}
		if (line.find("overall miss rate ") != std::string::npos)
		{
			// Takes just the number from the string
			tempStr = line.substr(19);
			tempStr = tempStr.substr(0, tempStr.find(" "));
			results.missRate = std::stod(tempStr);
			std::cout << results.missRate << std::endl;
		}
	}
	out.close();
	return results;
}

void makeOutput(std::string name, outputResults fileResults, bool first)
{
	std::ofstream outFile;
	if (first) // check if the file is empty
	{
		outFile.open(name + ".txt", std::ios::trunc);
		outFile << "Overall Miss Rate, Memory CPI, Average Memory Access Time\n";
	}
	else
	{
		std::ofstream appendFile(name + ".txt", std::ios::app);
		outFile = std::move(appendFile);
	}
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file " << name << ".txt for writing" << std::endl;
		return;
	}
	outFile << fileResults.missRate << "," << fileResults.memCPI << "," << fileResults.accTime << "\n";
	outFile.close();
}

void part2_Cache()
{
	// Part 2 Varies the Cache Size (4 to 128) doubling
	buildCommandRet comRet;
	std::string outFolder;
	outputResults temp;
	bool firstOpen = true;

	outFolder = "artOutput_Cache";
	outputDirectory(outFolder);

	// For art.trace
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, i, 32, 50, "art.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "CacheOut", temp, firstOpen);
		firstOpen = false;
	}
	// For swim.trace
	firstOpen = true;
	outFolder = "swimOutput_Cache";
	outputDirectory(outFolder);
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, i, 32, 50, "swim.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "CacheOut", temp, firstOpen);
		firstOpen = false;
	}
	// For trace.out
	firstOpen = true;
	outFolder = "traceOutput_Cache";
	outputDirectory(outFolder);
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, i, 32, 50, "trace.out", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "CacheOut", temp, firstOpen);
		firstOpen = false;
	}
	// For crafty_mem.trace
	firstOpen = true;
	outFolder = "crafty_memOutput_Cache";
	outputDirectory(outFolder);
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, i, 32, 50, "crafty_mem.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "CacheOut", temp, firstOpen);
		firstOpen = false;
	}
}

void part3_Block()
{
	// Part 3 Varies the Block Size (4 to 128) doubling
	buildCommandRet comRet;
	std::string outFolder;
	outputResults temp;
	bool firstOpen = true;

	outFolder = "artOutput_Block";
	outputDirectory(outFolder);

	// For art.trace
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, 128, i, 50, "art.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "BlockOut", temp, firstOpen);
		firstOpen = false;
	}

	// For swim.trace
	firstOpen = true;
	outFolder = "swimOutput_Block";
	outputDirectory(outFolder);
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, 128, i, 50, "swim.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "BlockOut", temp, firstOpen);
		firstOpen = false;
	}

	// For trace.out
	firstOpen = true;
	outFolder = "traceOutput_Block";
	outputDirectory(outFolder);
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, 128, i, 50, "trace.out", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "BlockOut", temp, firstOpen);
		firstOpen = false;
	}

	// For crafty_mem.trace
	firstOpen = true;
	outFolder = "crafty_memOutput_Block";
	outputDirectory(outFolder);
	for (int i = 4; i <= 128; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(4, 128, i, 50, "crafty_mem.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "BlockOut", temp, firstOpen);
		firstOpen = false;
	}
}

void part4_Associativity()
{
	// Part 4 Varies the Associativity Size (1 to 32) doubling
	buildCommandRet comRet;
	std::string outFolder;
	outputResults temp;
	bool firstOpen = true;

	outFolder = "artOutput_Associativity";
	outputDirectory(outFolder);

	// For art.trace
	for (int i = 1; i <= 32; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(i, 128, 32, 50, "art.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "AssociativityOut", temp, firstOpen);
		firstOpen = false;
	}

	// For swim.trace
	firstOpen = true;
	outFolder = "swimOutput_Associativity";
	outputDirectory(outFolder);
	for (int i = 1; i <= 32; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(i, 128, 32, 50, "swim.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "AssociativityOut", temp, firstOpen);
		firstOpen = false;
	}

	// For trace.out
	firstOpen = true;
	outFolder = "traceOutput_Associativity";
	outputDirectory(outFolder);
	for (int i = 1; i <= 32; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(i, 128, 32, 50, "trace.out", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "AssociativityOut", temp, firstOpen);
		firstOpen = false;
	}

	// For crafty_mem.trace
	firstOpen = true;
	outFolder = "crafty_memOutput_Associativity";
	outputDirectory(outFolder);
	for (int i = 1; i <= 32; i = i * 2)
	{
		// Parameters: buildCommand(int associativity, int cacheSize, int lineSize, int missPenalty, std::string inFile, std::string directoryName)
		comRet = buildCommand(i, 128, 32, 50, "crafty_mem.trace", outFolder);
		// Run the cacheSim
		system(comRet.command.c_str());
		// Pickout results from output file
		temp = findResults(comRet.outFile);
		// Create a file compiling the results
		makeOutput(comRet.fileName + "AssociativityOut", temp, firstOpen);
		firstOpen = false;
	}
}


int main()
{
	part2_Cache();
	part3_Block();
	part4_Associativity();
}
