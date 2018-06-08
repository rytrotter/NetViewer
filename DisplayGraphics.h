#ifndef DISPLAYGRAPHICS_H
#define DISPLAYGRAPHICS_H

#define Main WinMain

#include "resource.h"

int WINAPI Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

static HINSTANCE hInstanceMainWinProc = GetModuleHandle(NULL);


#endif // !DISPLAYGRAPHICS_H