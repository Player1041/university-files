#pragma once
class ItemManager
{
	bool ITEM_SPAWNED;
	int TOTAL_POSSIBLE; // how many items available at once
	int TOTAL_TO_SPAWN; // how many items should spawn
	
	enum ITEM_TYPE {
		NUMBER,
		SIGN,
		LETTER,
		FOOD
	};

	void spawnFood();
	void removeFood();
};

