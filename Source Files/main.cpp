#include "Game.h"

int main(int, char**) {

	Game* game{ new Game{ 1280, 720 } };

	game->run();

	delete game;

}