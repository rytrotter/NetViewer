#ifndef GETMYIP_H
#define GETMYIP_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Winsock2.h>

#pragma comment(lib, "ws2_32.lib")		// Tells linker to link "ws2_32.lib" to library dependencies

void getMyIP(HWND hwndProto);

#endif
