#include "game.h"

int main() {

	Game g = Game();

	g.setEvaluators(Game::userInput, Game::standardEvaluator);

	g.run();

	return 0;
}
