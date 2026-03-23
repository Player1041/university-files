#pragma once
class Game
{
	bool INGAME;

	enum GAMEMODE {
		MATHS,
		ENGLISH,
		PUZZLE,
		RANDOM
	};

	enum MATHS_GAME_TYPES {
		TIMES_TABLE,
		ARITHMETIC,
		DIVISION
	};

	enum ENGLISH_GAME_TYPES {
		ALPHABET,
		SPELLING
	};

	enum PUZZLE_GAME_TYPES {
		MIRRORS,
		CLASSIC,
		BOXED,
		MAZE
	};

	void startGame();
	void stopGame();

	void loadLevel();
	void unloadLevel();

	void pauseGame();
	void startGame();
};

