#include <ftxui/component/app.hpp>

#include <iostream>

#include "game.h"
#include "window.h"

int main() {
	Game g = Game();

	g.setEvaluators(Game::userInput, Game::standardEvaluatorBlack);

	Window win = Window(&g);

	win.updateComponents();

	win.startApp(ftxui::App::Fullscreen());
	std::cout << "Selected move is " << win.result << "\n";


	return 0;
}
