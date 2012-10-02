#include "util.h"
#include <ObjBase.h>
using namespace std;

wstring convertToWideString(const string& s)
{
    int len=0, slength = s.size() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
