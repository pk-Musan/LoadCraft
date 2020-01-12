#pragma once

class Loader {

public:
	enum Image {
		RIGHT,
		RIGHT_ATTACK_1,
		RIGHT_ATTACK_2,
		LEFT,
		LEFT_ATTACK_1,
		LEFT_ATTACK_2,

		BLOCK,
		BLOCK_BROKEN,

		UNBREAKABLE_BLOCK,

		MAX_INDEX
	};
	static int imageHandles[];

public:
	static void loadGraph();
	static void deleteGraph();
};