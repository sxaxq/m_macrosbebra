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

#define CREATE 255
#define SHOW 9898
#define BUTTON_START 8888
#define BUTTON_STOP 7233
#define KEY_LINE 999
#define PLINE 111
#define SEND_STRINGS 222
#define SEND_DATA 666
#define EDIT_LINE 333
#define EDIT_LINES 555
#define KEY_CODE int
#define BIND_TEXT vector<string>

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
		CW_USEDEFAULT, 250, 170, NULL, NULL, GetModuleHandle(NULL), NULL);

	HWND hWndText2 = CreateWindowExW(
		0, L"STATIC", L"”кажите количество строк", WS_CHILD | WS_VISIBLE | SS_LEFT,
		10, 0, 200, 50, hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);

	HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER,
		10, 20, 200, 20, hWnd, (HMENU)EDIT_LINE, GetModuleHandle(NULL), NULL);

	HWND hButton = CreateWindowEx(0, L"BUTTON", L"OK", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		10, 40, 50, 20,
		hWnd, (HMENU)SEND_DATA, GetModuleHandle(NULL), nullptr);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ButtonWndProc);
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)ButtonWndProc);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

int added_count = 0;

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
		cout << "wm_create 1" << endl;
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
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		return (LRESULT)hBrush;
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case SEND_DATA:
			cout << "ok key" << endl;
			// —читываем число из пол€ ввода
			WCHAR buffer[256];
			GetDlgItemText(hwnd, EDIT_LINE, buffer, 256);
			added_count = static_cast<int>(wcstol(buffer, nullptr, 10));
			cout << "added_count: " << added_count << endl;
			Sleep(1000);
			DestroyWindow(hwnd);
			init();
			break;
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

#pragma endregion

vector<bind_t> binders;
size_t dcount = 0;

void init()
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
		CW_USEDEFAULT, 400, added_count * 70 + 20, NULL, NULL, GetModuleHandle(NULL), NULL);

	HWND hButton = CreateWindowEx(0, L"BUTTON", L"Send", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		300, 10, 55, 40,
		hWnd, (HMENU)SEND_STRINGS, GetModuleHandle(NULL), nullptr);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)LinesWndProc);
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)LinesWndProc);

	if (dcount % 2 == 1)
	{
		cout << "destroyed window" << endl;
		DestroyWindow(hWnd);
	}

	ShowWindow(hWnd, SW_SHOW);
	dcount++;

	cout << "bebra" << endl;

	vector<HWND> edit_lines(added_count, {});
	int top_pos = 10;

	cout << "bebra" << endl;
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

	HWND pauseLine = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE,
		125, top_pos, 80, 20, hWnd, (HMENU)PLINE, GetModuleHandle(NULL), NULL);

	HWND hWndText3 = CreateWindowExW(
		0, L"STATIC", L"KEY: ", WS_CHILD | WS_VISIBLE | SS_LEFT,
		70, top_pos + 20, 200, 50, hWnd, nullptr, GetModuleHandle(NULL), nullptr);

	HWND keyLine = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE,
		125, top_pos + 20, 80, 20, hWnd, (HMENU)KEY_LINE, GetModuleHandle(NULL), NULL);

	top_pos += 30;
	UpdateWindow(hWnd);

	top_pos = 10;

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
				int length = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
				char* buf = new char[length];
				WideCharToMultiByte(CP_UTF8, 0, buffer, -1, buf, length, NULL, NULL);
				std::string str(buf);
				cout << "added line: " << str << endl;
				v.push_back(str);
			}
			binders[binders.size() - 1].push_text(v);
			cout << "added " << endl;
			binders[binders.size() - 1].show_info();
			dcount++;

			WCHAR buffer[256];
			GetDlgItemText(hwnd, PLINE, buffer, 256);
			int length = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
			char* buf = new char[length];
			WideCharToMultiByte(CP_UTF8, 0, buffer, -1, buf, length, NULL, NULL);
			std::string str(buf);
			int pause = atoi(str.c_str());
			binders[binders.size() - 1].set_pause(pause);

			WCHAR buffer2[256];
			GetDlgItemText(hwnd, KEY_LINE, buffer2, 256);
			int length2 = WideCharToMultiByte(CP_UTF8, 0, buffer2, -1, NULL, 0, NULL, NULL);
			char* buf2 = new char[length];
			WideCharToMultiByte(CP_UTF8, 0, buffer2, -1, buf2, length2, NULL, NULL);
			std::string str2(buf2);
			int key = to_virtual_key(str2);
			binders[binders.size() - 1].set_key(key);
			add_column(binders[binders.size() - 1].get_id(), key, true, pause);
			
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
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		return (LRESULT)hBrush;
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}