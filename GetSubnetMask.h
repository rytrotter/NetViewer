#ifndef GETSUBNETMASK_H
#define GETSUBNETMASK_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iphlpapi.h>
#include <WinSock2.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

void getSubnetMask(HWND hwndProto);

#endif // !GETSUBNETMASK_H
