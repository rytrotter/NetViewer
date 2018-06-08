#include "stdafx.h"
#include "DisplayGraphics.h"
#include "ConvertStringToLPCWSTR.h"

using namespace std;

std::wstring convertStringToLPCWSTR(std::string &stringToChange) {

	int length;
	int stringLength = (int)stringToChange.length() + 1;
	length = MultiByteToWideChar(CP_ACP, 0, stringToChange.c_str(), stringLength, 0, 0);
	wchar_t *buffer = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, stringToChange.c_str(), stringLength, buffer, length);
	std::wstring r(buffer);
	delete[] buffer;
	return r;
}
