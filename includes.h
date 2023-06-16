#pragma once
#include <windows.h>
#include <string>
#include <prsht.h>
#include <commctrl.h>
#include <map>

LPWSTR to_lpwstr(std::string str);
LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LinesWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init();

static bool binder_worked = true;
static int last_key = 0;
static bool get_key = false;
static HWND mainHwnd;
static HWND hListView;
static size_t row = 0;
static size_t col = 0;

enum class NameGame
{
	SAMP,
	CSGO,
	DOTA2,
	NONE
};

NameGame currentGame;

struct GameInfo
{
	int id;
	NameGame ngame;
	LPCWSTR game_str;
};

void detected_game()
{
	vector<GameInfo> games;
	games.push_back(GameInfo{ 0, NameGame::SAMP, L"GTA:SA:MP" });
	games.push_back(GameInfo{ 1, NameGame::CSGO, L"Counter-Strike: Global Offensive - Direct3D 9" });
	games.push_back(GameInfo{ 2, NameGame::DOTA2, L"dota2.exe" });
	
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
	currentGame = NameGame::NONE;
}

void add_column(size_t id, char key, bool haveText, int pause)
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