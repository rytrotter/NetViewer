#ifndef GETWIRELESSSSID_H
#define GETWIRELESSSSID_H

#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

void getWirelessSSID(HWND hwndProto);

#endif // !GETWIRELESSSSID_H