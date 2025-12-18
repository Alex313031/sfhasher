#include "sfhasher.h"

#include "framework.h"
#include "hash.h"

HINSTANCE hInst;
HWND hMainWnd;

HWND hInputEdit;
HWND hOutputEdit;

HWND hHashButton;
HWND hAboutButton;

// The main entry point, equivalent to int main()
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow) {
  // Contains HINSTANCE from previous running instance, unused.
  UNREFERENCED_PARAMETER(hPrevInstance);

  // Import ComCtl32.dll
  InitCommonControls();

  // Allow and allocate conhost for cmd.exe logging window
  if (!AllocConsole()) {
    return 1;
  }

  // File handler pointer to a dummy file, possibly an actual logfile
  FILE* fNonExistFile = fDummyFile;
#ifndef __MINGW32__
  freopen_s(&fNonExistFile, "CONOUT$", "w", stdout); // Standard error
  freopen_s(&fNonExistFile, "CONOUT$", "w", stderr); // Standard out
#else
  // freopen_s doesn't exist in MinGW...
  fNonExistFile = freopen("CONOUT$", "w", stdout); // Standard error
  fNonExistFile = freopen("CONOUT$", "w", stderr); // Standard out
#endif // __MINGW32__
  if (!fNonExistFile) {
    return 1;
  }

  // Initialize global strings.
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_MAINMENU, szWindowClass, MAX_LOADSTRING);

  // Create and register the window class.
  RegisterWndClass(hInstance);

  // Perform application initialization.
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  } else {
    std::wcout << L"Welcome to SFHasher ver. " << VERSION_STRING << std::endl;
  }

  // Load keyboard accelerators.
  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAINMENU));

  MSG msg;
  // Main window message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return static_cast<int>(msg.wParam);
}

// Register the WNDCLASSEXW structure
ATOM RegisterWndClass(HINSTANCE hInstance) {
  // Declare and set size of this window class struct.
  WNDCLASSEXW wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);

  // Set styles, icons, and window message handling function.  
  wcex.style          = CS_HREDRAW | CS_VREDRAW; // Drawing style
  wcex.lpfnWndProc    = WndProc; // Window Procedure function
  wcex.cbClsExtra     = 0; // Extra bytes to add to end of this window class
  wcex.cbWndExtra     = 0; // Extra bytes to add to end hInstance
  wcex.hInstance      = hInstance; // This instance
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUTWIN)); // Load our main app icon
  wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW); // Choose default cursor style to show
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW); // Choose window client area background color
  wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAINMENU); // Attach menu to window
  wcex.lpszClassName  = szWindowClass; // Use our unique window class name
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // Load titlebar icon

  // Returns a "class atom", a win32 specific data type.
  return RegisterClassExW(&wcex);
}

// Saves global instance handle and creates the main window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  bool success = false; // 
  hInst = hInstance; // Store instance handle in our global variable

  // The all important Win32 function that every GUI app must have to create
  // the Window.
  hMainWnd = CreateWindowW(szWindowClass,
                            szTitle,
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            DEFAULT_WIDTH,
                            DEFAULT_HEIGHT,
                            nullptr,
                            nullptr,
                            hInstance,
                            nullptr);

  // Early fail if we can't create the window.
  if (!hMainWnd) {
    success = false;
  } else {
    // Actually show the window (or hide it).
    ShowWindow(hMainWnd, nCmdShow);
    // Initialize controls
    InitControls(hMainWnd);
    // If we showed the window, now the status will be set 
    // Sucessfully created the window
    success = UpdateWindow(hMainWnd); // Start painting by sending the WM_PAINT message
  }
  
  return success;
}

//  Processes window messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//  WM_RESIZE   - Handle window resizing
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    // Handle commands sent to window via wParam
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId) {
        case IDM_ABOUT:
        case IDC_ABOUT_BUTTON:
          // Show "About" dialog box
          HandleAbout(hWnd);
          break;
        case IDM_REFRESH:
          RefreshControls(hWnd);
          break;
        case IDM_EXIT:
          // Send WM_DESTROY message to close window 
          DestroyWindow(hWnd);
          break;
        case IDC_HASH_BUTTON:
          HandleHash(hWnd);
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;
    // Begin painting
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      // TODO: Add any drawing code that uses hdc here...
      if (hdc) {
      }
      EndPaint(hWnd, &ps);
    } break;
    // Get/Set min/max window size
    case WM_GETMINMAXINFO: {
      // Set the minimum size for the window
      LPMINMAXINFO pMinMaxInfo = (LPMINMAXINFO)lParam;
      pMinMaxInfo->ptMinTrackSize.x = 300;
      pMinMaxInfo->ptMinTrackSize.y = 150;
    } break;
    // Handle resize events
    case WM_SIZE: {
    } break;
    // When close button is pressed
    case WM_CLOSE:
      PostQuitMessage(0); // Exit the application
      break;
    // Handle destroy message
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    // Fall through to DefWindowProc, any unhandled messages will be sent here
    // and Windows will decide what to do with it.
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

void InitControls(HWND hWnd) {
  if (hWnd != nullptr) {
    hInputEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE, WC_EDIT, L"input",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP,
        STATIC_LEFT,
        STATIC_TOP,
        EDIT_WIDTH - STATIC_LEFT,
        EDIT_HEIGHT,
        hWnd, (HMENU)IDC_INPUT, hInst, nullptr
    );
    hOutputEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE, WC_EDIT, L"output",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP,
        STATIC_LEFT,
        STATIC_TOP + EDIT_HEIGHT + INTRA_PADDING,
        EDIT_WIDTH - STATIC_LEFT,
        EDIT_HEIGHT,
        hWnd, (HMENU)IDC_INPUT, hInst, nullptr
    );
    hHashButton = CreateWindowExW(
        0, WC_BUTTON, L"Hash!",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        EDIT_WIDTH + STATIC_LEFT + INTRA_PADDING,
        STATIC_TOP + (EDIT_HEIGHT / 2),
        BUTTON_WIDTH,
        BUTTON_HEIGHT,
        hWnd, (HMENU)IDC_HASH_BUTTON, hInst, nullptr
    );
    hAboutButton = CreateWindowExW(
        0, WC_BUTTON, L"About",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        EDIT_WIDTH + STATIC_LEFT + INTRA_PADDING,
        STATIC_TOP + BUTTON_HEIGHT + INTRA_PADDING + EDIT_HEIGHT + INTRA_PADDING,
        BUTTON_WIDTH,
        BUTTON_HEIGHT,
        hWnd, (HMENU)IDC_ABOUT_BUTTON, hInst, nullptr
    );
  } else {
    return;
  }
}

void HandleHash(HWND hWnd) {
  DWORD dwInputSize = GetWindowTextLength(hInputEdit);
  const bool is_empty = (BOOL)(dwInputSize == 0);
  if (is_empty) {
    MessageBoxW(hWnd, L"No input!    ", L"Empty Input", MB_OK | MB_ICONWARNING);
  } else {
    static std::wstring kHashInput;
    std::wostringstream wostr;
    wchar_t* input_buff = new wchar_t[dwInputSize + 1];
    GetWindowTextW(hInputEdit, input_buff, dwInputSize + 1);
    wostr << std::fixed << std::setprecision(MAX_LOADSTRING) << input_buff;
    kHashInput = wostr.str();
    const std::wstring kHashOutput = SimpleW(kHashInput);
    std::wcout << "kHashInput = " << kHashInput << std::endl;
    std::wcout << "kHashOutput = " << kHashOutput << std::endl;
    SetWindowTextW(hOutputEdit, kHashOutput.c_str());
  }
}

void RefreshControls(HWND hWnd) {
  if (hWnd != nullptr) {
    SetWindowTextW(hInputEdit, kBlank);
    SetWindowTextW(hOutputEdit, kBlank);
  } else {
    return;
  }
}

void HandleAbout(HWND hWnd) {
  DialogBoxW(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
}

// Processes window messages for the "About" dialog box window procedure.
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);

  bool AboutHandled = false; // Stores status of whether dialog has been handled user-wise.
  const HICON kSmallIcon = LoadIcon(GetHinstanceFromHwnd(hDlg), MAKEINTRESOURCE(IDI_SMALL));
  switch (message) {
    case WM_INITDIALOG: {
      // Set icon in titlebar of about dialog
      SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)kSmallIcon);
      SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)kSmallIcon);
      AboutHandled = true;
    } break;
    case WM_COMMAND:
      // Exit the dialog
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        if (EndDialog(hDlg, LOWORD(wParam))) {
          AboutHandled = true;
          return (INT_PTR)AboutHandled;
        } else {
          AboutHandled = false;
          break;
        }
      } break;
    default:
      break;
  }

  // About dialog failed
  return (INT_PTR)AboutHandled;
}

// Grabs the HINSTANCE of a given Window's HWND
HINSTANCE GetHinstanceFromHwnd(HWND hWnd) {
  // GetWindowLongPtr is the recommended function for 64-bit compatibility
  LONG_PTR hInstancePtr = GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
  // Cast the result to HINSTANCE
  HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(hInstancePtr);

  return hInstance;
}
