#pragma once

#include <fstream>
#include <string>

inline void logMsg(std::string msg)
{
	std::ofstream myfile("log.txt", std::ios::app);
	if (myfile.is_open())
	{
		myfile << msg + "\n";
		myfile.close();
	}
}