#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>

#include "GetSubnetMask.h"
#include "DisplayGraphics.h"
#include "ConvertStringToLPCWSTR.h"

void getSubnetMask(HWND hwndProto) {

	PIP_ADAPTER_INFO pointerAdapterInfo = NULL;
	ULONG ulongOutBufferLength = NULL;
	DWORD dwResult = 0;

	std::string subnetString = { " " };
	std::string subnetErrorString = { " " };

	ulongOutBufferLength = sizeof(IP_ADAPTER_INFO);
	pointerAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));

	if (GetAdaptersInfo(pointerAdapterInfo, &ulongOutBufferLength) == ERROR_BUFFER_OVERFLOW) {

		free(pointerAdapterInfo);
		pointerAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulongOutBufferLength + sizeof(IP_ADAPTER_INFO));
		dwResult = GetAdaptersInfo(pointerAdapterInfo, &ulongOutBufferLength);

		if (GetAdaptersInfo(pointerAdapterInfo, &ulongOutBufferLength) != ERROR_SUCCESS) {

			if (pointerAdapterInfo == NULL) {

				subnetErrorString = "pointerAdapterInfo is NULL.";
			}
			else if (ulongOutBufferLength == NULL) {

				subnetErrorString = "ulongOutBufferLength is NULL.";
			}
			else if (dwResult != ERROR_SUCCESS) {

				auto dwResultString = std::to_string(dwResult);
				subnetErrorString = "GetAdaptersInfo error: ";
				subnetErrorString.append(dwResultString);
			}

			std::wstring subnetErrorTemporaryString = convertStringToLPCWSTR(subnetErrorString);
			LPCWSTR subnetErrorStringInLPCWSTR = subnetErrorTemporaryString.c_str();

			HWND displaySubnetMask = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", subnetErrorStringInLPCWSTR, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
				575, 95, 250, 65, hwndProto, NULL, hInstanceMainWinProc, NULL);
		}
		else {

			subnetString = "Subnet Mask: ";

			std::string targetMask(reinterpret_cast<char *>(inet_addr(pointerAdapterInfo->IpAddressList.IpMask.String)));
			subnetString.append(targetMask);

			std::wstring subnetTemporaryString = convertStringToLPCWSTR(subnetString);
			LPCWSTR subnetStringInLPCWSTR = subnetTemporaryString.c_str();

			HWND displaySubnetMask = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", subnetStringInLPCWSTR, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
				575, 95, 250, 65, hwndProto, NULL, hInstanceMainWinProc, NULL);
		}
	}
}