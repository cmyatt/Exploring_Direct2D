#include "Log.h"
using namespace std;

Log::Log(void)
	: output(NULL)
{}

Log::~Log(void)
{
	if (*output)
		output->close();
	if (output)
		delete output;
}

bool Log::init(string filename)
{
	output = new ofstream(filename, ios::trunc);
	if (!(*output)) {
		delete output;
		return false;
	}
	return true;
}

void Log::note(string title, string msg)
{
	*output << "[Note] " << title << ": " << msg << "\n";
}

void Log::note(string title, string msg, HWND window)
{
	*output << "[Note] " << title << ": " << msg << "\n";
	MessageBox(window, (title+": "+msg).c_str(), "Note", MB_OK);
}

void Log::error(string file, string func, string msg)
{
	*output << "[Error] " << file << ", " << func << ": " << msg << "\n";
}

void Log::error(string file, string func, string msg, HWND window)
{
	*output << "[Error] " << file << ", " << func << ": " << msg << "\n";
	MessageBox(window, (file+", "+func+": "+msg).c_str(), "Error", MB_OK);
}