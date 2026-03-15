#include <ftxui/component/app.hpp>

#include <iostream>

#include "game.h"
#include "window.h"

int main() {
	Game g = Game();

	g.setEvaluators(Game::userInput, Game::standardEvaluatorBlack);

	//g.run();
	Window win = Window(&g);
	win.updateComponents();

	win.startApp(ftxui::App::Fullscreen());


	return 0;
}
