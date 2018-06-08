#ifndef CHECKFORNETWORK_H
#define CHECKFORNETWORK_H

#include <Windows.h>
#include <WinInet.h>
#include <SensAPI.h>					// Used to test if local machine is currently connected to the network

#pragma comment(lib, "Sensapi.lib")		// Tells linker to add the library "Sensapi.h" to the list of library dependencies

void checkForNetwork(HWND hwndProto);

#endif // !CHECKFORNETWORK_H
