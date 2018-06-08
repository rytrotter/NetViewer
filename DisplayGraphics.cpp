#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>
#include <mmsystem.h>
#include <Windows.h>
#include <WinInet.h>

using namespace std;


// My header files
#include "DisplayGraphics.h"
#include "CheckForNetwork.h"
#include "GetWirelessSSID.h"
#include "GetMyIP.h"
//#include "GatherHosts.h"

using namespace std;

const char g_szClassName[] = { "GraphicalNetViewer_MainView" };

								// -Handle of Dialog-  -handles messages- -wParam and lParam gather and process information based on window handle(hwndDialogProc)
BOOL CALLBACK AboutDialogProcedure(HWND hwndDialogProc, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
	case WM_INITDIALOG:

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hwndDialogProc, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwndDialogProc, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// The window procedure - This is the main procedure for the application (user input / output)
LRESULT CALLBACK WindowsProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

	case WM_RBUTTONDOWN:													// Shows the program filepath when user clicks RMB
	{
		char szFileName[MAX_PATH] = { "" };

		GetModuleFileName(hInstanceMainWinProc, (LPWSTR)szFileName, MAX_PATH);
		MessageBox(hwnd, (LPCWSTR)szFileName, L"This Program is: ", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_CREATE:															// Outputs all my data in a neat GUI
	{
		checkForNetwork(hwnd);			// Checks to see if computer application is run on is connected to network - Output in top left box

		HWND displayLoadingData = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", L"This (in theory) will display information about loading.",
			WS_VISIBLE | WS_CHILD | SS_CENTER, 25, 55, 250, 105, hwnd, NULL, hInstanceMainWinProc, NULL);
		
		getWirelessSSID(hwnd);			// Gathers the wireless SSID of the computer the app is running on - Output in top middle box
		getMyIP(hwnd);					// Gathers ip address and hostname for the computer the app is running on - Output in top right box
		
		HWND displayRefreshButton = CreateWindow(L"BUTTON", L"Refresh", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_TABSTOP | BS_DEFPUSHBUTTON,
			850, 15, 85, 65, hwnd, (HMENU)ID_REFRESH_BUTTON, hInstanceMainWinProc, NULL);

		HWND displayLegend = CreateWindow(L"BUTTON", L"Legend", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_TABSTOP,
			850, 95, 85, 65, hwnd, NULL, hInstanceMainWinProc, NULL);
		HWND displayStartButton = CreateWindow(L"BUTTON", L"Start Button", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_TABSTOP,
			960, 15, 200, 65, hwnd, NULL, hInstanceMainWinProc, NULL);
		HWND displayStopButton = CreateWindow(L"BUTTON", L"Stop Button", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_TABSTOP,
			960, 95, 200, 65, hwnd, NULL, hInstanceMainWinProc, NULL);
		HWND displayGraphicalLayout = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", L"This will be the graphical part of the application, viewing the hosts"
			" on the network by icons of their type", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 25, 180, 910, 700, hwnd, NULL, hInstanceMainWinProc, NULL);
		HWND displayIPLayout = CreateWindowEx(WS_EX_STATICEDGE, L"STATIC", L"This will be the list-view part where each IP that is used\n on the network shows up",
			WS_VISIBLE | WS_CHILD | SS_CENTER, 960, 180, 200, 700, hwnd, NULL, hInstanceMainWinProc, NULL);
		break;
	}

	case WM_CLOSE:															// Pressing 'X' in the top right corner - Closes window
		MessageBeep(MB_ICONWARNING);
		if (MessageBox(hwnd, L"Do you want to quit?", L"Graphical Network Viewer", MB_YESNO | MB_ICONQUESTION) == IDYES) {			// Ask for quit confirmation
			DestroyWindow(hwnd);
			return 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:													// WM_COMMAND takes care of top bar menu / buttons
		switch (LOWORD(wParam)) {

		case ID_FILE_EXIT: {											// Exits when user goes to File > Exit
			MessageBeep(MB_ICONWARNING);
			if (MessageBox(hwnd, L"Do you want to quit?", L"Graphical Network Viewer", MB_YESNO | MB_ICONQUESTION) == IDYES) {	// Ask for quit confirmation
				DestroyWindow(hwnd);
				return 0;
			}
			break;
		}
			case ID_ABOUT_HELP:
			{										// Displays "About" box when user goest to Help > About			
				int aboutReturnValue = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDialogProcedure);
				if (aboutReturnValue == -1) {
					MessageBox(hwnd, L"Dialog Failed to Open.", L"Error", MB_OK | MB_ICONINFORMATION);
				}
				break;
			}

			case ID_REFRESH_BUTTON:
			case ID_FILE_REFRESH:
			{
				checkForNetwork(hwnd);
				getWirelessSSID(hwnd);
				getMyIP(hwnd);
				break;
			}
		}

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

				//  -Instance of App-	-Previous instance-		-Command line args- -How window should be displayed-
int WINAPI Main(HINSTANCE hInstanceMain, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX mainWindowClass;											// Window Class
	HWND hwnd;															// Window Handle
	MSG message;

	// Step 1: Register the Window Class
	mainWindowClass.cbSize = sizeof(WNDCLASSEX);						// Size of the structure
	mainWindowClass.style = 0;											// Class Styles (cs_*), can usually be set to 0
	mainWindowClass.lpfnWndProc = WindowsProcedure;						// Pointer to the window procedure for this class
	mainWindowClass.cbClsExtra = 0;										// Amount of extra data allocated for this class in memory
	mainWindowClass.cbWndExtra = 0;										// Amount of extra data allocated in memory per window of this type
	mainWindowClass.hInstance = hInstanceMain;							// Handle to application instance
	mainWindowClass.hIcon = LoadIcon(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDR_TOPBAR));									// Large (Usually 32x32) icon shown when user presses Alt+Tab
	mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);				// Cursor that will be displayed about the window
	mainWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			// Backround brush to set the color of the window
	mainWindowClass.lpszMenuName = MAKEINTRESOURCE(IDR_TOPBAR);			// Name of a menu resource to use for the windows with this class
	mainWindowClass.lpszClassName = (LPCWSTR)g_szClassName;				// Name to identify the class with
	mainWindowClass.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDI_MAINICON), IMAGE_ICON,
		16, 16, 0);														// Small (usually 16x16) icon to show in the taskbar and in the top left corner

	if (!RegisterClassEx(&mainWindowClass)) {
		MessageBox(NULL, L"Window Registration Failure.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Creating the window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,									// Extended Windows Style
		(LPCWSTR)g_szClassName,								// Tells the system what type of window to create (based on our class)
		L"Graphical Network Viewer",						// Title of current window
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,								
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 960,			// X, Y, Width, Heigh
		NULL, NULL, hInstanceMain, NULL);					// Parent window handle, menu handle, application instance handle, pointer to window creation data

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// The Message Loop - processes and queues all information to do with the window
	while (GetMessage(&message, hwnd, 0, 0) > 0) {
		TranslateMessage(&message);
		
		DispatchMessage(&message);
	}
	return message.wParam;
}