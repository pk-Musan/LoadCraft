#pragma once

class Loader {

public:
	enum Image {
		TITLE,
		PRESS_1,
		PRESS_2,
		ANY_KEY_1,
		ANY_KEY_2,

		RIGHT,
		RIGHT_ATTACK_1,
		RIGHT_ATTACK_2,
		LEFT,
		LEFT_ATTACK_1,
		LEFT_ATTACK_2,

		BLOCK_1,
		BLOCK_1_BROKEN,
		BLOCK_2,
		BLOCK_2_BROKEN,

		UNBREAKABLE_BLOCK,

		GOAL,

		MAX_INDEX
	};
	static int imageHandles[];

public:
	static void loadGraph();
	static void deleteGraph();
};