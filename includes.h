#pragma once
#include <windows.h>
#include <string>
#include <prsht.h>
#include <commctrl.h>
#include <utility>
#include <map>
#include "bind.h"

LPWSTR to_lpwstr(std::string str);
LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LinesWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void added_handler();
std::string test(WCHAR buffer[256]);

static bool binder_worked = true;
static int last_key = 0;
static bool get_key = false;
static HWND mainHwnd;
static HWND hListView;
static size_t row = 0;
static size_t col = 0;
vector<GameInfo> games;

std::pair<game_t, LPWSTR> is_game(WCHAR* str)
{
	for (size_t i = 0; i < games.size(); i++)
	{
		if (wcscmp(games[i].game_str, str) == 0)
		{
			wcout << "Try: " << games[i].game_str << " : " << str << endl;
			return make_pair(games[i].ngame, games[i].game_str);
		}
	}

	return make_pair(game_t::NONE, to_lpwstr("No-Game"));
}

void detected_game()
{
	games.push_back(GameInfo{ 0, game_t::SAMP, to_lpwstr("GTA:SA:MP")});
	games.push_back(GameInfo{ 1, game_t::CSGO, to_lpwstr("Counter-Strike: Global Offensive - Direct3D 9")});
	games.push_back(GameInfo{ 2, game_t::DOTA2, to_lpwstr("Dota 2") });
	
	for (size_t i = 0; i < games.size(); i++)
	{
		HWND tempHwnd = FindWindow(NULL, games[i].game_str);
		if (tempHwnd != NULL)
		{
			currentGame = games[i].ngame;
			wcout << "finded game: " << games[i].game_str << endl;
			return;
		}
	}
	cout << "not found game active process" << endl;
	currentGame = game_t::NONE;
}

void add_column(size_t id, char key, bool haveText, int pause, LPWSTR game, size_t line_count)
{
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = row;
	lvItem.iSubItem = 0;
	lvItem.pszText = to_lpwstr(std::to_string(id));
	ListView_InsertItem(hListView, &lvItem);

	// Set the text of each item

	ListView_SetItemText(hListView, row, 1, to_lpwstr(std::string(1, key)));
	string hText = haveText == true ? "yes" : "no";
	ListView_SetItemText(hListView, row, 2, to_lpwstr(hText));
	ListView_SetItemText(hListView, row, 3, to_lpwstr(std::to_string(pause)));
	ListView_SetItemText(hListView, row, 4, game);
	ListView_SetItemText(hListView, row, 5, to_lpwstr(std::to_string(line_count)));
	UpdateWindow(mainHwnd);

	++row;
}

LPWSTR to_lpwstr(std::string str)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	LPWSTR wideString = new WCHAR[length];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideString, length);
	return wideString;
}

int return_key(int key_code)
{
	return key_code;
}

int to_virtual_key(std::string str)
{
	int key = int(str[0]);
	cout << key << " <- key" << endl;
	return VkKeyScan(key);
}

