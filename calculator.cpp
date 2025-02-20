#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <commctrl.h>

#define ID_NUM1 1
#define ID_NUM2 2
#define ID_ADD 3
#define ID_SUBTRACT 4
#define ID_MULTIPLY 5
#define ID_DIVIDE 6

/* Declare Windows procedure */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/* Make the class name into a global variable */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

HWND hWndResult;
HWND hwndNum1, hwndNum2, hwndAdd, hwndSubtract, hwndMultiply, hwndDivide;

bool isDragging = false;
POINT dragStart;

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilities for variation */
        szClassName,         /* Classname */
        _T("My Calculator"), /* Title Text */
        WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        250,                 /* The program's width */
        200,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void ShowResult(int result) {
    TCHAR buffer[50];
    wsprintf(buffer, _T("Result: %d"), result);

    if (hWndResult != NULL) {
        // If the result window already exists, update the text
        SetWindowText(hWndResult, buffer);
        ShowWindow(hWndResult, SW_SHOW);
    } else {
        // Create a new result window
        hWndResult = CreateWindow(
            _T("STATIC"), buffer,
            WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU,
            100, 100, 200, 100,
            NULL, NULL, GetModuleHandle(NULL), NULL
        );
    }
}

/* This function is called by the Windows function DispatchMessage() */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        hwndNum1 = CreateWindowEx(0, _T("EDIT"), _T(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 20, 100, 25, hwnd, (HMENU)ID_NUM1, NULL, NULL);

        hwndNum2 = CreateWindowEx(0, _T("EDIT"), _T(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 60, 100, 25, hwnd, (HMENU)ID_NUM2, NULL, NULL);

        hwndAdd = CreateWindowEx(0, _T("BUTTON"), _T("+"),
            WS_CHILD | WS_VISIBLE,
            140, 20, 50, 25, hwnd, (HMENU)ID_ADD, NULL, NULL);

        hwndSubtract = CreateWindowEx(0, _T("BUTTON"), _T("-"),
            WS_CHILD | WS_VISIBLE,
            140, 60, 50, 25, hwnd, (HMENU)ID_SUBTRACT, NULL, NULL);

        hwndMultiply = CreateWindowEx(0, _T("BUTTON"), _T("*"),
            WS_CHILD | WS_VISIBLE,
            140, 100, 50, 25, hwnd, (HMENU)ID_MULTIPLY, NULL, NULL);

        hwndDivide = CreateWindowEx(0, _T("BUTTON"), _T("/"),
            WS_CHILD | WS_VISIBLE,
            140, 140, 50, 25, hwnd, (HMENU)ID_DIVIDE, NULL, NULL);
        break;

    case WM_COMMAND:
        {
            if (LOWORD(wParam) == ID_ADD || LOWORD(wParam) == ID_SUBTRACT ||
                LOWORD(wParam) == ID_MULTIPLY || LOWORD(wParam) == ID_DIVIDE) {

                TCHAR num1[20], num2[20];
                GetWindowText(hwndNum1, num1, 20);
                GetWindowText(hwndNum2, num2, 20);

                int n1 = _ttoi(num1);
                int n2 = _ttoi(num2);
                int result = 0;

                switch (LOWORD(wParam)) {
                case ID_ADD: result = n1 + n2; break;
                case ID_SUBTRACT: result = n1 - n2; break;
                case ID_MULTIPLY: result = n1 * n2; break;
                case ID_DIVIDE:
                    if (n2 != 0) {
                        result = n1 / n2;
                    } else {
                        MessageBox(NULL, _T("Cannot divide by zero!"), _T("Error"), MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    break;
                }

                ShowResult(result);
            }
        }
        break;

    case WM_PAINT: {
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);

        // Optionally, you can fill the background with a color
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        isDragging = true;
        dragStart.x = LOWORD(lParam);
        dragStart.y = HIWORD(lParam);
        SetCapture(hwnd);
        break;
    }

    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);
            MoveWindow(hWndResult, p.x - dragStart.x, p.y - dragStart.y, 300, 100, TRUE);
        }
        break;
    }

    case WM_LBUTTONUP: {
        isDragging = false;
        ReleaseCapture();
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;

    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
