#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <cstdlib>
#include "bind.h"
#include "includes.h"

using namespace std;

#define DEBUG          666
#define CHANGE_THEME   999
#define GAME_LIST      1111
#define CREATE         2222
#define SHOW           3333
#define BUTTON_START   4444
#define BUTTON_STOP    5555
#define KEY_LINE       6666
#define PLINE          7777
#define SEND_STRINGS   8888
#define SEND_DATA      9999
#define EDIT_LINE      1010
#define EDIT_LINES     1011
#define KEY_CODE int
#define BIND_TEXT vector<string>

int added_count;
vector<bind_t> binders;

#pragma region add_bind

void add_bind()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"class";

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(0, L"class", L"Input", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 250, 200, NULL, NULL, GetModuleHandle(NULL), NULL);

	SetLayeredWindowAttributes(hWnd, RGB(80, 80, 80), 160, LWA_ALPHA);

	HWND hWndText2 = CreateWindowExW(
		0, L"STATIC", L"Укажите количество строк", WS_CHILD | WS_VISIBLE | SS_LEFT,
		10, 15, 200, 50, hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);

	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Consolas");

	HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER,
		10, 35, 200, 20, hWnd, (HMENU)EDIT_LINE, GetModuleHandle(NULL), NULL);

	HWND hButton = CreateWindowEx(0, L"BUTTON", L"OK", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		10, 70, 50, 20,
		hWnd, (HMENU)SEND_DATA, GetModuleHandle(NULL), nullptr);

	SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hWndText2, WM_SETFONT, (WPARAM)hFont, TRUE);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ButtonWndProc);
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)ButtonWndProc);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;

	switch (msg)
	{
	case WM_DESTROY:
		DestroyWindow(hwnd);
		break;
	case WM_CREATE:
	{
#ifdef DEBUG
		cout << "wm_create 1" << endl;
#endif // DEBUG
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER,
			10, 10, 200, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
	}
	case WM_ERASEBKGND:
	{
		HDC hdc = HDC(wParam);
		RECT rect;
		GetClientRect(hwnd, &rect);
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
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH brush = CreateSolidBrush(RGB(67, 65, 112));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case SEND_DATA:
#ifdef DEBUG
			cout << "ok key" << endl;
#endif // DEBUG
			
			WCHAR buffer[256];
			GetDlgItemText(hwnd, EDIT_LINE, buffer, 256);
			added_count = static_cast<int>(wcstol(buffer, nullptr, 10));
			cout << "added_count: " << added_count << endl;
			Sleep(1000);
			DestroyWindow(hwnd);
			added_handler();
			break;
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

#pragma endregion

void added_handler()
{
	if (binders.empty())
	{
		binders.push_back(bind_t(1));
	}
	else
	{
		binders.push_back(bind_t(binders[binders.size() - 1].get_id() + 1));
	}

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"class2";

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(0, L"class2", L"strings", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 400, added_count * 70 + 25, NULL, NULL, GetModuleHandle(NULL), NULL);

	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Consolas");

	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 200, LWA_ALPHA);

	HWND hButton = CreateWindowEx(0, L"BUTTON", L"Send", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		300, 20, 55, 30,
		hWnd, (HMENU)SEND_STRINGS, GetModuleHandle(NULL), nullptr);

	SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)LinesWndProc);
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)LinesWndProc);

	vector<HWND> edit_lines(added_count, {});

	int top_pos = 10;

	for (size_t i = 0; i < added_count; i++)
	{
		cout << "created edit line" << endl;
		edit_lines[i] = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE,
			70, top_pos, 200, 20, hWnd, (HMENU)(EDIT_LINES + i), GetModuleHandle(NULL), NULL);
		top_pos += 30;
	}
	HWND hWndText2 = CreateWindowExW(
		0, L"STATIC", L"PAUSE: ", WS_CHILD | WS_VISIBLE | SS_LEFT,
		70, top_pos, 200, 50, hWnd, nullptr, GetModuleHandle(NULL), nullptr);

	HWND pauseLine = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER,
		125, top_pos, 80, 20, hWnd, (HMENU)PLINE, GetModuleHandle(NULL), NULL);

	HWND hWndText3 = CreateWindowExW(
		0, L"STATIC", L"KEY: ", WS_CHILD | WS_VISIBLE | SS_LEFT,
		70, top_pos + 25, 200, 50, hWnd, nullptr, GetModuleHandle(NULL), nullptr);


	HWND keyLine = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE,
		125, top_pos + 25, 80, 20, hWnd, (HMENU)KEY_LINE, GetModuleHandle(NULL), NULL);

	HWND hWndText4 = CreateWindowExW(
		0, L"STATIC", L"GAME: ", WS_CHILD | WS_VISIBLE | SS_LEFT,
		70, top_pos + 50, 200, 50, hWnd, nullptr, GetModuleHandle(NULL), nullptr);

	SendMessage(hWndText2, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hWndText3, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hWndText4, WM_SETFONT, (WPARAM)hFont, TRUE);

	HWND hComboBox = CreateWindowEx( 0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		125, top_pos + 50, 100, 200, hWnd, (HMENU)GAME_LIST, GetModuleHandle(NULL), NULL
	);

	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"GTA:SA:MP");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"GTA V");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Counter-Strike: Global Offensive - Direct3D 9");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Dota 2");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Desktop");

	top_pos += 30;
	ShowWindow(hWnd, SW_SHOW);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK LinesWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		DestroyWindow(hwnd);
	    break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case SEND_STRINGS:
			
			vector<string> v;
			for (size_t i = 0; i < added_count; i++)
			{
				WCHAR buffer[256];
				GetDlgItemText(hwnd, (EDIT_LINES + i), buffer, 256);
				std::string str = test(buffer);
#ifdef DEBUG
				cout << "added line: " << str << endl;
#endif // DEBUG
				v.push_back(str);
			}

			// GameList read
			HWND hGameList = GetDlgItem(hwnd, GAME_LIST);
			int index = SendMessage(hGameList, CB_GETCURSEL, 0, 0);
			int len = SendMessage(hGameList, CB_GETLBTEXTLEN, index, 0);
			WCHAR* gbuffer = new WCHAR[len + 1];
			SendMessage(hGameList, CB_GETLBTEXT, index, (LPARAM)gbuffer);
			wcout << gbuffer << endl;
			auto result = is_game(gbuffer);
			binders[binders.size() - 1].set_bind_game(result);

			binders[binders.size() - 1].push_text(v);
#ifdef DEBUG
			cout << "added " << endl;
			binders[binders.size() - 1].show_info();
#endif // DEBUG


			WCHAR buffer[256];
			GetDlgItemText(hwnd, PLINE, buffer, 256);
			std::string str = test(buffer);
			int pause = atoi(str.c_str());
			binders[binders.size() - 1].set_pause(pause);

			WCHAR buffer2[256];
			GetDlgItemText(hwnd, KEY_LINE, buffer2, 256);
			int length2 = WideCharToMultiByte(CP_UTF8, 0, buffer2, -1, NULL, 0, NULL, NULL);
			char* buf2 = new char[length2];
			WideCharToMultiByte(CP_UTF8, 0, buffer2, -1, buf2, length2, NULL, NULL);
			std::string str2(buf2);
			int key = to_virtual_key(str2);
			binders[binders.size() - 1].set_key(key);
			add_column(binders[binders.size() - 1].get_id(), key, true, pause, result.second, v.size());
			
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_ERASEBKGND:
	{
		HDC hdc = HDC(wParam);
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);
		return (LRESULT)TRUE;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH brush = CreateSolidBrush(RGB(67, 65, 112));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		HBRUSH hBrush = CreateSolidBrush(RGB(67, 65, 112));
		return (LRESULT)hBrush;
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

std::string test(WCHAR buffer[256])
{
	int length = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
	char* buf = new char[length];
	WideCharToMultiByte(CP_UTF8, 0, buffer, -1, buf, length, NULL, NULL);
	std::string str(buf);

	return str;
}