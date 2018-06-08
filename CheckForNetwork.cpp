#include "stdafx.h"
#include "CheckForNetwork.h"
#include "DisplayGraphics.h"


void checkForNetwork(HWND hwndProto) {

	DWORD dwState = 0;
	BOOL connectionState = IsNetworkAlive(&dwState);

	if (!connectionState) {
		MessageBeep(MB_ICONSTOP);
		if (MessageBox(hwndProto, L"You must be connected to the network before running this application. Connect and try again.",
			L"Graphical Network Viewer", MB_OK | MB_ICONERROR) == IDOK) {
			DestroyWindow(hwndProto);
		}
		exit(EXIT_FAILURE);
	}
	else {
		HWND displayCheckNetwork = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", L"Congratulations, you're connected!",
			WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 25, 15, 250, 30, hwndProto, NULL, hInstanceMainWinProc, NULL);
	}
}