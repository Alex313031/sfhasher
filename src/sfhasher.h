#ifndef SFHASHER_H_
#define SFHASHER_H_

// This file is for separating out declarations from the main application about_win.cc file
// Use it for forward declaring the main window processing functions, extern keywords for
// handles, and system DLL pointer types.

#include "framework.h"

#include "resource.h"

// Maximum size of string that can be loaded from resource table with LoadString().
#define MAX_LOADSTRING 255

/* Global Variables */

constexpr unsigned int DEFAULT_WIDTH = 300;
constexpr unsigned int DEFAULT_HEIGHT = 200;
constexpr unsigned int STATIC_LEFT = 12;
constexpr unsigned int STATIC_TOP = 12;
constexpr unsigned int INTRA_PADDING = 6;

constexpr unsigned int EDIT_WIDTH = 200;
constexpr unsigned int EDIT_HEIGHT = 24;
constexpr unsigned int BUTTON_WIDTH = 50;
constexpr unsigned int BUTTON_HEIGHT = EDIT_HEIGHT;

const LPCWSTR kBlank = L"";

// current instance
extern HINSTANCE hInst;

// The title bar text
static WCHAR szTitle[MAX_LOADSTRING];

// The main window class name
static WCHAR szWindowClass[MAX_LOADSTRING];

// Dummy file output for conhost
static FILE* fDummyFile;

/* End of global variables */

/* Forward declarations of functions included in this translation unit. */

// Registers the window class.
ATOM RegisterWndClass(HINSTANCE hInstance);

// Creates the main window with CreateWindowW()
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

// Window procedure function https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Initializes input boxes, static labels, buttons, etc.
void InitControls(HWND hWnd);

// Refreshes controls as needed
void RefreshControls(HWND hWnd);

void HandleHash(HWND hWnd);

void HandleAbout(HWND hWnd);

// Message handler for the "About" dialog box.
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Returns the HINSTANCE of a given Window's HWND
HINSTANCE GetHinstanceFromHwnd(HWND hWnd);

#endif // SFHASHER_H_
