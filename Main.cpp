//--------------------------------------------------------------------------------------
// Entry point for the application
// Window creation code
//--------------------------------------------------------------------------------------

#include "Scene.h"
#include "Direct3DSetup.h"
#include "Input.h"
#include "Timer.h"
#include "Common.h"
#include <windows.h>
#include <string>


// Forward declarations of functions in this file
BOOL             InitWindow(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
// Used to keep code simpler, but try to architect your own code in a better way

// Identifiers for the window that will show our app
HINSTANCE gHInst;
HWND      gHWnd;

// Viewport size (which determines the size of the window used for our app)
int gViewportWidth = 1280;
int gViewportHeight = 960;

const float ROTATION_SPEED = 2.0f;  // 2 radians per second for rotation
const float MOVEMENT_SPEED = 50.0f; // 50 units per second for movement (what a unit of length is depends on 3D model - i.e. an artist decision usually)


// A timer class to provide frame timing
Timer gTimer;

// A global error message to help track down fatal errors - set it to a useful message
// when a serious error occurs. See how it is used belwo
std::string gLastError;


//--------------------------------------------------------------------------------------
// The entry function for a Windows application is called wWinMain
//--------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance); // Stop warnings when not using some parameters
    UNREFERENCED_PARAMETER(lpCmdLine);
    DirectXManager dm;
    Scene scene(&dm);

    // Create a window to display the scene
    if (!InitWindow(hInstance, nCmdShow))  { return 0; }

    // Prepare TL-Engine style input functions
    InitInput(); 

    // Initialise Direct3D
    if (!dm.InitDirect3D())
    {
        MessageBoxA(gHWnd, gLastError.c_str(), NULL, MB_OK);
        return 0;
    }
    scene.SetDirectXManager(&dm);
    // Initialise scene
    if (!scene.InitGeometry() || !scene.InitScene())
    {   
        MessageBoxA(gHWnd, gLastError.c_str(), NULL, MB_OK);
        scene.ReleaseResources();
        dm.ShutdownDirect3D();
        return 0;
    }

    // Will use a timer class to help in this tutorial (not part of DirectX). It's like a stopwatch - start it counting now
    gTimer.Start();


    // Main message loop - this is a Windows equivalent of the loop in a TL-Engine application
    MSG msg = {};
    while (msg.message != WM_QUIT) // As long as window is open
    {
        // Check for and deal with any window messages (input, window resizing, minimizing, etc.).
        // The actual message processing happens in the function WndProc below
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // Deal with messages
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else // When no windows messages left to process then render & update our scene
        {
            // Update the scene by the amount of time since the last frame
            float frameTime = gTimer.GetLapTime();
            scene.UpdateScene(frameTime);

            // Draw the scene
            scene.RenderScene();

            if (KeyHit(Key_Escape))
            {
                DestroyWindow(gHWnd); // This will close the window and ultimately exit this loop
            }
        }
    }


    // Release everything before quitting
    scene.ReleaseResources();
    dm.ShutdownDirect3D();

    return (int)msg.wParam;
}



//--------------------------------------------------------------------------------------
// Create a window to display our scene, returns false on failure.
//--------------------------------------------------------------------------------------
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Get a stock icon to show on the taskbar for this program.
    SHSTOCKICONINFO stockIcon;
    stockIcon.cbSize = sizeof(stockIcon);
    if (SHGetStockIconInfo(SIID_APPLICATION, SHGSI_ICON, &stockIcon) != S_OK) // Returns false on failure
    {
        return false;
    }

    // Register window class. Defines various UI features of the window for our application.
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;    // Which function deals with windows messages
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0; SIID_APPLICATION;
    wcex.hInstance = hInstance;
    wcex.hIcon = stockIcon.hIcon; // Which icon to use for the window
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); // What cursor appears over the window
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"CO2409WindowClass";
    wcex.hIconSm = stockIcon.hIcon;
    if (!RegisterClassEx(&wcex)) // Returns false on failure
    {
        return false;
    }


    // Select the type of window to show our application in
    DWORD windowStyle = WS_OVERLAPPEDWINDOW; // Standard window
    //DWORD windowStyle = WS_POPUP;          // Alternative: borderless. If you also set the viewport size to the monitor resolution, you 
                                             // get a "fullscreen borderless" window, which works better with alt-tab than DirectX fullscreen,
                                             // which is an option in Direct3DSetup.cpp. DirectX fullscreen has slight better performance though.

    // Calculate overall dimensions for the window. We will render to the *inside* of the window. But the
    // overall winder will be larger because it includes the borders, title bar etc. This code calculates
    // the overall size of the window given our choice of viewport size.
    RECT rc = { 0, 0, gViewportWidth, gViewportHeight };
    AdjustWindowRect(&rc, windowStyle, FALSE);

    // Create window, the second parameter is the text that appears in the title bar
    gHInst = hInstance;
    gHWnd = CreateWindow(L"CO2409WindowClass", L"Direct3D 11", windowStyle,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
    if (!gHWnd)
    {
        return false;
    }

    ShowWindow(gHWnd, nCmdShow);
    UpdateWindow(gHWnd);

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Deal with a message from Windows. There are very many possible messages, such as keyboard/mouse input, resizing
// or minimizing windows, the system shutting down etc. We only deal with messages that we are interested in
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT: // A necessary message to ensure the window content is displayed
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY: // Another necessary message to deal with the window being closed
        PostQuitMessage(0);
        break;


    // The WM_KEYXXXX messages report keyboard input to our window.
    // This application has added some simple functions (not DirectX) to process these messages (all in Input.cpp/h)
    // so you don't need to change this code. Instead simply use KeyHit, KeyHeld etc.
    case WM_KEYDOWN:
        KeyDownEvent(static_cast<KeyCode>(wParam));
        break;

    case WM_KEYUP:
        KeyUpEvent(static_cast<KeyCode>(wParam));
        break;


    // The following WM_XXXX messages report mouse movement and button presses
    // Use KeyHit to get mouse buttons, GetMouseX, GetMouseY for its position
    case WM_MOUSEMOVE:
    {
        MouseMoveEvent(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_LBUTTONDOWN:
    {
        KeyDownEvent(Mouse_LButton);
        break;
    }
    case WM_LBUTTONUP:
    {
        KeyUpEvent(Mouse_LButton);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        KeyDownEvent(Mouse_RButton);
        break;
    }
    case WM_RBUTTONUP:
    {
        KeyUpEvent(Mouse_RButton);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        KeyDownEvent(Mouse_MButton);
        break;
    }
    case WM_MBUTTONUP:
    {
        KeyUpEvent(Mouse_MButton);
        break;
    }


    // Any messages we don't handle are passed back to Windows default handling
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
