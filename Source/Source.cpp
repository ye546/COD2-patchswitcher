// cod 2 patch switcher.cpp : Defines the entry point for the application.
//
#pragma once
#include "resource.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <shlobj_core.h>
#include <filesystem>

#define MAX_LOADSTRING 100

namespace fs = std::filesystem;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//windows inside main window
HWND textwindow, textfield, patch1button, patch2button, patch3button, browsebutton, version;

//define application size
constexpr int width = 400;
constexpr int height = 400;

static fs::path path_from_patch1[5] =
{
  ("struct/game_v_1/CoD2MP_s.exe"),
  ("struct/game_v_1/CoD2SP_s.exe"),
  ("struct/game_v_1/gfx_d3d_mp_x86_s.dll"),
  ("struct/game_v_1/gfx_d3d_x86_s.dll"),
  ("struct/game_v_1/mss32.dll")
};

static fs::path path_from_patch2[6] =
{
  ("struct/game_v_2/CoD2MP_s.exe"),
  ("struct/game_v_2/CoD2SP_s.exe"),
  ("struct/game_v_2/gfx_d3d_mp_x86_s.dll"),
  ("struct/game_v_2/gfx_d3d_x86_s.dll"),
  ("struct/game_v_2/mss32.dll"),
  ("struct/game_v_2/main/iw_15.iwd"),
};

static fs::path path_from_patch3[6] =
{
  ("struct/game_v_3/CoD2MP_s.exe"),
  ("struct/game_v_3/CoD2SP_s.exe"),
  ("struct/game_v_3/gfx_d3d_mp_x86_s.dll"),
  ("struct/game_v_3/gfx_d3d_x86_s.dll"),
  ("struct/game_v_3/mss32.dll"),
  ("struct/game_v_3/main/iw_15.iwd"),
};

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void RemovePreviousPatchFiles(fs::path* p, const int size, char* string_path) {
    //std::vector<bool> flags(size, 0);

    //for patch 1.2 and 1.3 we need to remove an iwd file
    if (size >= 6)
    {
        fs::path main = ("/main/");
        fs::path string(string_path);
        string += main;
        string += path_from_patch2[5].filename();
       try
       {
           fs::remove(string);
       }
       catch (fs::filesystem_error & e)
       {
           MessageBoxA(NULL, e.what(), "Error", NULL);
       }
        
        //skip the iwd_15 which we just delted above anywas
        for (int i = 0; i < 5; i++) {
            //flags[i] = 
            try
            {
                fs::remove(string_path / p[i].filename());
            }
            catch (fs::filesystem_error & e)
            {
                MessageBoxA(NULL, e.what(), "Error", NULL);
            }
        }
    }
    else
    {
        //remove the unwanted files
        for (int i = 0; i < size; i++) {
            //flags[i] = 
            try
            {
                fs::remove(string_path / p[i].filename());
            }
            catch (fs::filesystem_error & e)
            {
                MessageBoxA(NULL, e.what(), "Error", NULL);
            }
        }
    }

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COD2PATCHSWITCHER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COD2PATCHSWITCHER));


    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(COD_ICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COD2PATCHSWITCHER);
    wcex.lpszClassName  = szWindowClass;
    //wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hIconSm        = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(COD_ICON), IMAGE_ICON, 16, 16, 0);

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OPENFILENAMEA ofn;
    BROWSEINFOA bi = { 0 };

    //Font
    HFONT hf;
    HDC hdc;

    long lfHeight;

    hdc = GetDC(NULL);
    lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);

    switch (message)
    {
    case WM_CREATE:
        textfield = CreateWindowExA(WS_EX_CLIENTEDGE, "Edit", "", WS_CHILD | WS_VISIBLE, (width / 2 - 165), 10, 300, 25, hWnd, NULL, hInst, (VOID*)lParam);
        
        browsebutton = CreateWindowExA(NULL, "Button", "Browse Folder", WS_CHILD | WS_VISIBLE, (width / 2 - 90), 50, 150, 25, hWnd, NULL, hInst, (VOID*)lParam);
        
        patch1button = CreateWindowExA(NULL, "Button", "Install Patch 1.0", WS_CHILD|WS_VISIBLE, (width / 2 - 90), 90, 150, 50, hWnd, NULL, hInst, (VOID*)lParam);
        patch2button = CreateWindowExA(NULL, "Button", "Install Patch 1.2", WS_CHILD|WS_VISIBLE, (width / 2 - 90), 150, 150, 50, hWnd, NULL, hInst, (VOID*)lParam);
        patch3button = CreateWindowExA(NULL, "Button", "Install Patch 1.3", WS_CHILD|WS_VISIBLE, (width / 2 - 90), 210, 150, 50, hWnd, NULL, hInst, (VOID*)lParam);

        //version = CreateWindowExA(WS_EX_LEFT, "static", "Version 1.0", WS_CHILD | WS_VISIBLE, 5, 250, 150, 50, hWnd, NULL, hInst, (VOID*)lParam);
        
        hf = CreateFontA(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Ebrima");

        SendMessage(patch1button, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(patch2button, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(patch3button, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(browsebutton, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(textfield, WM_SETFONT, (WPARAM)hf, TRUE);
        //textwindow = CreateWindowExA(NULL, "Static", "You can either chose to paste the path yourself in the field, or browse for it. After browsing the correct path just click on the patch you want to install.", WS_CHILD | WS_VISIBLE, (width / 2 - 90), 280, , 150, hWnd, NULL, hInst, (VOID*)lParam);

        break;
    case WM_COMMAND:
        {
            if (message == WM_COMMAND && (HWND)lParam == browsebutton) 
            {
                char chName[8096] = {0};
                std::string filepath;

                bi.hwndOwner = hWnd;
                bi.pszDisplayName = chName;
                bi.pidlRoot = NULL;
                bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_VALIDATE | BIF_NONEWFOLDERBUTTON | BIF_BROWSEINCLUDEFILES;
                bi.lpszTitle = "Select the folder which contains your Call of Duty 2 installation";
                
                if (SHGetPathFromIDListA(SHBrowseForFolderA(&bi), chName))
                {
                    SetWindowTextA(textfield, chName);
                    MessageBoxA(NULL, "Successfully read the directory", "Info", MB_ICONINFORMATION);
                }
                else
                    MessageBoxA(NULL, "Failed to read the directory", "Info", MB_ICONINFORMATION);
            }

            //patch 1.0
            if (message == WM_COMMAND && (HWND)lParam == patch1button) 
            {
                char string_path[8096] = { 0 }; //path shouldn't need more than 8096 chars(bytes)
                
                GetWindowTextA(textfield, string_path, 8096);
                if (strlen(string_path) <= 0)
                    MessageBoxA(NULL, "You need to select a directory first", "Info", MB_ICONINFORMATION);

                bool yes[5] = { 0 };

                RemovePreviousPatchFiles(path_from_patch2, 6, string_path);
                RemovePreviousPatchFiles(path_from_patch3, 6, string_path);
                // only perform 4 loops because we want to perform the move for
                //iwd_15 into main the last thing we do
                for (int i = 0; i < 5; i++) 
                {
                   try
                   {
                       yes[i] = fs::copy_file(path_from_patch1[i], string_path / path_from_patch1[i].filename(), fs::copy_options::overwrite_existing);
                   }
                   catch(fs::filesystem_error& e)
                   {
                       MessageBoxA(NULL, e.what(), "Error", NULL);
                   }
                }

                if (yes[0] && yes[1] && yes[2] && yes[3] && yes[4])
                    MessageBoxA(NULL, "Successfully installed patch 1.0", "Success", MB_ICONINFORMATION);
                else
                    MessageBoxA(NULL, "Failed to install patch 1.0. It could be because RemovePreviousPatchFiles failed to return true, and therefore copying the files didn't initiate. Or because moving the iwd_15.iwd file failed.", "Fail", MB_ICONERROR);
            }

            //patch 1.2
            if (message == WM_COMMAND && (HWND)lParam == patch2button) 
            {
                char string_path[8096] = { 0 }; //path shouldn't need more than 8096 chars(bytes)

                GetWindowTextA(textfield, string_path, 8096);
                if (strlen(string_path) <= 0)
                    MessageBoxA(NULL, "You need to select a directory first", "Info", MB_ICONINFORMATION);

                bool yes[6] = { 0 };

                RemovePreviousPatchFiles(path_from_patch1, 5, string_path);
                RemovePreviousPatchFiles(path_from_patch3, 6, string_path);

                for (int i = 0; i < 5; i++) 
                {
                    try 
                    {
                        //copy the files from the struct folder
                        yes[i] = fs::copy_file(path_from_patch2[i], string_path / path_from_patch2[i].filename(), fs::copy_options::overwrite_existing);
                    }
                    catch (fs::filesystem_error & e) 
                    {
                        MessageBoxA(NULL, e.what(), "Error", NULL);
                    }
                }

                //move the iwd_15 file to the main folder
                fs::path main = ("/main/");
                fs::path string(string_path);
                string += main; 
                string+= path_from_patch2[5].filename();
                try 
                {
                    yes[5] = fs::copy_file(path_from_patch2[5], string, fs::copy_options::overwrite_existing);
                }
                catch (fs::filesystem_error & e) 
                {
                    MessageBoxA(NULL, e.what(), "Error moving iwd_15.iwd file into main for patch 1.2", NULL);
                }

                if (yes[0] && yes[1] && yes[2] && yes[3] && yes[4] && yes[5])
                    MessageBoxA(NULL, "Successfully installed patch 1.2", "Success", MB_ICONINFORMATION);
                else
                    MessageBoxA(NULL, "Failed to install patch 1.2. It could be because RemovePreviousPatchFiles failed to return true, and therefore copying the files didn't initiate.", "Fail", MB_ICONERROR);
            }

            //patch 1.3
            if (message == WM_COMMAND && (HWND)lParam == patch3button) 
            {
                char string_path[8096] = { 0 }; //path shouldn't need more than 8096 chars(bytes)

                GetWindowTextA(textfield, string_path, 8096);
                if (strlen(string_path) <= 0)
                    MessageBoxA(NULL, "You need to select a directory first", "Info", MB_ICONINFORMATION);

                bool yes[6] = { 0 };

                RemovePreviousPatchFiles(path_from_patch1, 5, string_path);
                RemovePreviousPatchFiles(path_from_patch2, 6, string_path);

                for (int i = 0; i < 5; i++)
                {
                    try
                    {
                        //copy the files from the struct folder
                        yes[i] = fs::copy_file(path_from_patch3[i], string_path / path_from_patch3[i].filename(), fs::copy_options::overwrite_existing);
                    }
                    catch (fs::filesystem_error & e)
                    {
                        MessageBoxA(NULL, e.what(), "Error", NULL);
                    }
                }

                //move the iwd_15 file to the main folder
                fs::path main = ("/main/");
                fs::path string(string_path);
                string += main;
                string += path_from_patch3[5].filename();
                try
                {
                    yes[5] = fs::copy_file(path_from_patch3[5], string, fs::copy_options::overwrite_existing);
                }
                catch (fs::filesystem_error & e)
                {
                    MessageBoxA(NULL, e.what(), "Error moving iwd_15.iwd file into main for patch 1.2", NULL);
                }

                if (yes[0] && yes[1] && yes[2] && yes[3] && yes[4] && yes[5])
                    MessageBoxA(NULL, "Successfully installed patch 1.3", "Success", MB_ICONINFORMATION);
                else
                    MessageBoxA(NULL, "Failed to install patch 1.3. It could be because RemovePreviousPatchFiles failed to return true, and therefore copying the files didn't initiate.", "Fail", MB_ICONERROR);
            }
            
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
