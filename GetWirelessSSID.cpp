#include "stdafx.h"
#include <iostream>
#include "GetWirelessSSID.h"
#include "DisplayGraphics.h"

#include "ConvertStringToLPCWSTR.h"



void getWirelessSSID(HWND hwndProto) {

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;

	std::string ssidOutputString = { "" };
	std::string sigStrengthOutputString = { "" };

	// Variables used for WlanEnumInterfaces
	PWLAN_INTERFACE_INFO_LIST pointerInterfaceList = NULL;
	PWLAN_INTERFACE_INFO pointerInterfaceInfo = NULL;

	PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
	PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		auto dwResultString = std::to_string(dwResult);
		ssidOutputString = "WlanOpenHandle error:\n ";
		ssidOutputString.append(dwResultString);
		WlanCloseHandle(hClient, NULL);
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pointerInterfaceList);
	if (dwResult != ERROR_SUCCESS) {
		if (pointerInterfaceList == NULL) {
			ssidOutputString = "pointerInterfaceList is NULL.";
		}
		else if (hClient == NULL) {
			ssidOutputString = "hClient is NULL.";
		}
		else {
			auto dwResultString = std::to_string(dwResult);
			ssidOutputString = "WlanEnumInterfaces error:\n ";
			ssidOutputString.append(dwResultString);
		}
		WlanCloseHandle(hClient, NULL);
	}

		pointerInterfaceInfo = (WLAN_INTERFACE_INFO *)&pointerInterfaceList->InterfaceInfo;
		dwResult = WlanGetAvailableNetworkList(hClient, &pointerInterfaceInfo->InterfaceGuid, 0, NULL, &pBssList);

		pBssEntry = (WLAN_AVAILABLE_NETWORK *)&pBssList->Network;
		if (dwResult != ERROR_SUCCESS) {
			if (hClient == NULL) {
				ssidOutputString = "hClient is NULL.";
			}
			else if (&pointerInterfaceInfo->InterfaceGuid == NULL) {
				ssidOutputString = "pointerInterfaceInfo->InterfaceGuid is NULL;";
			}
			else if (&pBssList == NULL) {
				ssidOutputString = "pBssList is NULL.";
			}
			else {
				auto dwResultString = std::to_string(dwResult);
				ssidOutputString = "WlanGetAvailableNetworkList error:\n ";
				ssidOutputString.append(dwResultString);
			}
			WlanCloseHandle(hClient, NULL);
		}


		if (pointerInterfaceInfo->isState == 1) {
		
			if (pBssEntry->dot11Ssid.uSSIDLength == 0) {
			
				ssidOutputString = { " " };
				sigStrengthOutputString = { " " };
			}
			else {

				ssidOutputString = "Connected to: ";
				sigStrengthOutputString = "Signal Strength:  ";

				std::string ssidNameRAW(reinterpret_cast<char*>(pBssEntry->dot11Ssid.ucSSID));
				ssidOutputString.append(ssidNameRAW);

				auto sigStrengthRAW = std::to_string(pBssEntry->wlanSignalQuality);
				sigStrengthOutputString.append(sigStrengthRAW);
				sigStrengthOutputString.append("%");
			}
		}

		std::wstring ssidTemporaryString = convertStringToLPCWSTR(ssidOutputString);
		LPCWSTR ssidStringInLPCWSTR = ssidTemporaryString.c_str();

		std::wstring sigStrengthTemporaryString = convertStringToLPCWSTR(sigStrengthOutputString);
		LPCWSTR sigStrengthStringInLPCWSTR = sigStrengthTemporaryString.c_str();

		HWND displaySSID = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", ssidStringInLPCWSTR, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			300, 15, 250, 65, hwndProto, NULL, hInstanceMainWinProc, NULL);
		HWND displayMyHostname = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", sigStrengthStringInLPCWSTR, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
			300, 95, 250, 65, hwndProto, NULL, hInstanceMainWinProc, NULL);

		WlanCloseHandle(hClient, NULL);
}