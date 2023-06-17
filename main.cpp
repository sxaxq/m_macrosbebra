#include <Windows.h>
#include "config.h"
#include "includes.h"

using namespace std;

void pressed_handler(int keyCode);
LRESULT CALLBACK mainWindowProc(HWND mhwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

const LPCWSTR onStatus = L" [ON]";
const LPCWSTR offStatus = L" [OFF]";
HHOOK hook;
HWND statusText;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	detected_game();
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"mainWindow";

	RegisterClassEx(&wc);

	mainHwnd = CreateWindowEx(WS_EX_LAYERED, L"mainWindow", L"ÁÈÍÄÅÐ ÎÒ ÊÂÀÑÀ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 500, 460, NULL, NULL, GetModuleHandle(NULL), NULL);

	SetLayeredWindowAttributes(mainHwnd, RGB(0, 0, 0), 200, LWA_ALPHA);

	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Consolas");

	HFONT bigText = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Consolas");

	HWND buttonCreate = CreateWindowEx(0, L"BUTTON", L"CREATE", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		415, 20, 60, 30,
		mainHwnd, (HMENU)CREATE, GetModuleHandle(NULL), nullptr);

	HWND buttonStart = CreateWindowEx(0, L"BUTTON", L"START", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		415, 60, 60, 30,
		mainHwnd, (HMENU)BUTTON_START, GetModuleHandle(NULL), nullptr);

	HWND buttonStop = CreateWindowEx(0, L"BUTTON", L"STOP", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		415, 100, 60, 30,
		mainHwnd, (HMENU)BUTTON_STOP, GetModuleHandle(NULL), nullptr);

	HWND buttonShow = CreateWindowEx(0, L"BUTTON", L"SHOW", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		415, 140, 60, 30,
		mainHwnd, (HMENU)BUTTON_STOP, GetModuleHandle(NULL), nullptr);

	SendMessage(buttonCreate, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(buttonStart, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(buttonStop, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(buttonShow, WM_SETFONT, (WPARAM)hFont, TRUE);

	statusText = CreateWindowExW(
		0, L"STATIC", onStatus, WS_CHILD | WS_VISIBLE | SS_LEFT,
		415, 175, 50, 50, mainHwnd, nullptr, GetModuleHandle(NULL), nullptr
	);

	SendMessage(statusText, WM_SETFONT, (WPARAM)hFont, TRUE);

	hListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD |
		LVS_REPORT, 10, 10, 400, 400, mainHwnd, NULL, GetModuleHandle(NULL), NULL);
	SendMessage(hListView, WM_SETFONT, NULL, MAKELPARAM(0, 0));

	SetWindowLongPtr(mainHwnd, GWLP_USERDATA, (LONG_PTR)mainWindowProc);
	SetWindowLongPtr(mainHwnd, GWLP_WNDPROC, (LONG_PTR)mainWindowProc);

	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 40;
	lvColumn.pszText = to_lpwstr("ID");
	ListView_InsertColumn(hListView, 0, &lvColumn);

	lvColumn.cx = 50;
	lvColumn.pszText = to_lpwstr("KEY");
	ListView_InsertColumn(hListView, 1, &lvColumn);

	lvColumn.cx = 60;
	lvColumn.pszText = to_lpwstr("TEXT");
	ListView_InsertColumn(hListView, 2, &lvColumn);

	lvColumn.cx = 60;
	lvColumn.pszText = to_lpwstr("PAUSE");
	ListView_InsertColumn(hListView, 3, &lvColumn);
	ShowWindow(mainHwnd, SW_SHOW);

	lvColumn.cx = 80;
	lvColumn.pszText = to_lpwstr("GAME");
	ListView_InsertColumn(hListView, 4, &lvColumn);
	ShowWindow(mainHwnd, SW_SHOW);

	lvColumn.cx = 90;
	lvColumn.pszText = to_lpwstr("LINE COUNT");
	ListView_InsertColumn(hListView, 5, &lvColumn);
	ShowWindow(mainHwnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK mainWindowProc(HWND mhwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
	{
		HDC hdc = HDC(wParam);
		RECT rect;
		GetClientRect(mhwnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);
		return (LRESULT)TRUE;
	}
	break;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		HBRUSH hBrush = CreateSolidBrush(RGB(67, 65, 112));
		return (LRESULT)hBrush;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(mhwnd, &ps);
		RECT rect;
		GetClientRect(mhwnd, &rect);
		HBRUSH brush = CreateSolidBrush(RGB(67, 65, 112));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
		EndPaint(mhwnd, &ps);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CREATE:
			add_bind();
			break;
		case BUTTON_START:
			SetWindowText(statusText, onStatus);
			binder_worked = true;
			break;
		case BUTTON_STOP:
		{
			SetWindowText(statusText, offStatus);
			binder_worked = false;
			break;
		}
		case SHOW:
			ShowWindow(mhwnd, SW_MINIMIZE);
			break;
		case CHANGE_THEME:

			break;
		}
		break;

	default:
		return DefWindowProc(mhwnd, msg, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
		pressed_handler(p->vkCode);
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

void pressed_handler(int keyCode)
{
	if (binder_worked == false) return;

	if (isContains(binders, keyCode))
	{
		auto lp = get_game_text(binders, keyCode);
		HWND hwnd = GetForegroundWindow(); // Get the handle of the foreground window
		WCHAR buffer[256]; // Create a buffer to hold the window text
		GetWindowText(hwnd, buffer, 256); // Get the window text

		cout << "=====================" << endl;
		wcout << lp << endl;
		wcout << buffer << endl;
		cout << "=====================" << endl;

		if (wcscmp(lp, buffer) != 0 && wcscmp(lp, L"No-Game") != 0)
		{
			cout << "true" << endl;
			return;
		}

		int pause = getPause(binders, keyCode);
		cout << keyCode << endl;
		for (string s : getBind(binders, keyCode))
		{
			if (wcscmp(lp, L"GTA:SA:SAMP"))
			{
				keybd_event(0x54, 0, 0, 0);
				keybd_event(0x54, 0, KEYEVENTF_KEYUP, 0);
			}
			else
			{
				keybd_event(VK_BACK, 0, 0, 0);
				keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
			}

			for (char c : s)
			{
				int vk = VkKeyScan(c);
				if (vk != -1)
				{
					int scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
					keybd_event(vk, scan, 0, 0);
					keybd_event(vk, scan, KEYEVENTF_KEYUP, 0);
				}
			}
			keybd_event(VK_RETURN, 0, 0, 0);
			keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
			Sleep(pause);
		}
	}
}