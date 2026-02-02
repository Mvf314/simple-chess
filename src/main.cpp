#include "game.h"

int main() {

	Game g = Game();

	g.printBoard();

	g.setEvaluators(Game::userInput, Game::standardEvaluator);

	return 0;
}
