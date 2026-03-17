#include <ftxui/component/app.hpp>

#include <iostream>

#include "game.h"
#include "window.h"

int main() {
	Game g = Game();

	g.setEvaluators(Game::userInput, Game::standardEvaluatorBlack);

	Window win = Window(&g);

	Random random = Random();


	
	State state = g.getState();
	while (!win.shouldQuit && state != State::WHITE_WIN && state != State::BLACK_WIN && state != State::DRAW) {
		win.updateComponents();
		win.startApp(ftxui::App::Fullscreen()); 


		Move r = win.result;
		g.executeMove(r);

		g.updateState();
		state = g.getState();
		if (state == State::WHITE_WIN || state == State::BLACK_WIN || state == State::DRAW) {
			std::cout << "Game end\n";
			break;
		}
		Move blackMove = g.getBlackMove(random);
		g.executeMove(blackMove);
		g.updateState();
		state = g.getState();
	}


	return 0;
}
