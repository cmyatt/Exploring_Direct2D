#pragma once
#ifndef LOG_H
#define LOG_H

#include <WTypes.h>
#include <string>
#include <fstream>

class Log
{
public:
	Log(void);
	~Log(void);

	bool init(std::string filename);

	void note(std::string title, std::string msg);
	void note(std::string title, std::string msg, HWND window);

	void error(std::string file, std::string func, std::string msg);
	void error(std::string file, std::string func, std::string msg, HWND window);

private:
	std::ofstream *output;
};

#endif