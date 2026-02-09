#include "game.h"

int main() {

	Game g = Game();

	g.setEvaluators(Game::userInput, Game::standardEvaluatorBlack);

	g.run();

	return 0;
}
