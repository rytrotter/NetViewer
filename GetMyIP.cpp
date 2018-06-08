#include "stdafx.h"
#include <iostream>
#include "GetMyIP.h"
#include "DisplayGraphics.h"

#include "ConvertStringToLPCWSTR.h"

using namespace std;

void getMyIP(HWND hwndProto) {

	WSADATA wsaData;
	char thisComputerName[80];
	string ipOutputString = { "" };
	string hostnameOutputString = { "" };


	// MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//outputString = "WSAStartup failed.";
		exit(EXIT_FAILURE);
	}

	if (gethostname(thisComputerName, sizeof(thisComputerName)) == SOCKET_ERROR) {
		auto errorData = std::to_string(WSAGetLastError());
		hostnameOutputString = "Error: ";
		hostnameOutputString.append(errorData);
		exit(EXIT_FAILURE);
	}
	else {
		hostnameOutputString = "Hostname: ";
		hostnameOutputString.append(thisComputerName);
	}


	struct hostent *pointerToHostInfo = gethostbyname(thisComputerName);

	if (pointerToHostInfo == 0) {
		hostnameOutputString = "Bad Host Lookup.";
	}

	for (int i = 0; pointerToHostInfo->h_addr_list[i] != 0; ++i) {
		struct in_addr thisComputerIPv4;
		auto rawIPAddrStruct = memcpy(&thisComputerIPv4, pointerToHostInfo->h_addr_list[i], sizeof(struct in_addr));
		std::string rawIPAddrString(reinterpret_cast<char*>(inet_ntoa(thisComputerIPv4)));
		ipOutputString = "\nIP Address: ";
		ipOutputString.append(rawIPAddrString);
	}

	std::wstring temporaryString = convertStringToLPCWSTR(ipOutputString);
	LPCWSTR ipAddrStringInLPCWSTR = temporaryString.c_str();

	std::wstring hostnameTempString = convertStringToLPCWSTR(hostnameOutputString);
	LPCWSTR hostnameStringInLPCWSTR = hostnameTempString.c_str();

	HWND displayMyIP = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", ipAddrStringInLPCWSTR, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
		575, 15, 250, 65, hwndProto, NULL, hInstanceMainWinProc, NULL);
	HWND displayMyHostname = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", hostnameStringInLPCWSTR, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
		575, 96, 250, 65, hwndProto, NULL, hInstanceMainWinProc, NULL);

	WSACleanup();
}