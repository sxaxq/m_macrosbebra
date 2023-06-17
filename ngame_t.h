#pragma once
enum class game_t
{
	SAMP,
	CSGO,
	DOTA2,
	NONE
};

game_t currentGame;

struct GameInfo
{
	int id;
	game_t ngame;
	LPWSTR game_str;
};