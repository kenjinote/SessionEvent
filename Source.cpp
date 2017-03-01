#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"wtsapi32")
#include <windows.h>
#include <wtsapi32.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	static HWND hEdit;
	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("クリア"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_ALL_SESSIONS);
		break;
	case WM_SIZE:
		MoveWindow(hButton, 10, 10, 256, 32, TRUE);
		MoveWindow(hEdit, 10, 50, LOWORD(lParam) - 20, HIWORD(lParam) - 60, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			SetWindowText(hEdit, 0);
		}
		break;
	case WM_WTSSESSION_CHANGE:
		{
			const DWORD_PTR dwLength = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
			SendMessage(hEdit, EM_SETSEL, (WPARAM)dwLength, (LPARAM)dwLength);
			{
				TCHAR szText[256];
				SYSTEMTIME st;
				GetLocalTime(&st);
				wsprintf(szText, TEXT("[%04d/%02d/%02d %02d:%02d:%02d] "), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)szText);
			}
			switch (wParam)
			{
			case WTS_CONSOLE_CONNECT:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_CONSOLE_CONNECT\r\n"));
				break;
			case WTS_CONSOLE_DISCONNECT:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_CONSOLE_DISCONNECT\r\n"));
				break;
			case WTS_REMOTE_CONNECT:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_REMOTE_CONNECT\r\n"));
				break;
			case WTS_REMOTE_DISCONNECT:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_REMOTE_DISCONNECT\r\n"));
				break;
			case WTS_SESSION_LOGON:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_SESSION_LOGON\r\n"));
				break;
			case WTS_SESSION_LOGOFF:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_SESSION_LOGOFF\r\n"));
				break;
			case WTS_SESSION_LOCK:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_SESSION_LOCK\r\n"));
				break;
			case WTS_SESSION_UNLOCK:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_SESSION_UNLOCK\r\n"));
				break;
			case WTS_SESSION_REMOTE_CONTROL:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("WTS_SESSION_REMOTE_CONTROL\r\n"));
				break;
			default:
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("不明\r\n"));
				break;
			}
		}
		break;
	case WM_DESTROY:
		WTSUnRegisterSessionNotification(hWnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("セッションイベントを監視する"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
