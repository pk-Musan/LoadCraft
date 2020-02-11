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
		RIGHT_WALK_1,
		RIGHT_WALK_2,
		RIGHT_ATTACK_1,
		RIGHT_ATTACK_2,

		LEFT,
		LEFT_WALK_1,
		LEFT_WALK_2,
		LEFT_ATTACK_1,
		LEFT_ATTACK_2,

		TARGET,

		BLOCK_1,
		BLOCK_1_CRACKED,
		BLOCK_1_BROKEN,
		BLOCK_2,
		BLOCK_2_CRACKED,
		BLOCK_2_BROKEN,

		SPRING_BLOCK,
		SPRING_BLOCK_BROKEN,

		DISAPPEARING_BLOCK,

		UNBREAKABLE_BLOCK,

		WARP_1,
		WARP_2,
		GOAL,

		ARROW,

		MAX_INDEX
	};
	static int imageHandles[];

public:
	static void loadGraph();
	static void deleteGraph();
};