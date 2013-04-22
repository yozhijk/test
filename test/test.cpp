#include <windows.h>

#include <memory>

#include "WinOS.h"
#include "App.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::unique_ptr<WinOS> g_OS;
std::unique_ptr<App>   g_App;


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    g_OS.reset(new WinOS());
    g_App.reset(new App(*g_OS.get()));

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    g_OS->SetWindowParams(g_hWnd, core::ui_rect(0,0,WINDOW_WIDTH,WINDOW_HEIGHT));

    LARGE_INTEGER prevTime;
    LARGE_INTEGER freq;
    QueryPerformanceCounter(&prevTime);
    QueryPerformanceFrequency(&freq);


    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {

            LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);

            core::real timeDelta = static_cast<core::real>(currentTime.QuadPart - prevTime.QuadPart)/(freq.QuadPart);

            g_OS->Loop(timeDelta);

            prevTime = currentTime;

#ifdef _DEBUG
            char message[255];
            sprintf_s(message, "Timestamp: %f\n", timeDelta);
            OutputDebugStringA(message);
#endif
        }
    }

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )0 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Test";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )0 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"Test", L"Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
        NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}





//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
    case WM_PAINT:
        hdc = BeginPaint( hWnd, &ps );
        EndPaint( hWnd, &ps );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

    case WM_SIZE:
        // TODO: fix object lifetimes, now the following line causes a crash
        // when firing dead listener's callback
        // g_OS->ResizeWindow(core::ui_size(LOWORD(lParam), HIWORD(lParam)));
        break;

    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

