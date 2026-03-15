#ifndef __CHESS_WINDOW_H__
#define __CHESS_WINDOW_H__

#include "game.h"

class Window {
public:
	Window(Game* g);

	void updateComponents();

	void startApp(ftxui::App app);
private:
	Game* game;

	size_t screenWidth;
	size_t screenHeight;

	size_t boardWidth;
	size_t boardHeight;

	// menu selectors
	int pieceSelected;
	std::vector<int> moveSelected;

	// Components
	ftxui::Component boardComponent;
	ftxui::Component titleComponent;
	ftxui::Component piecesComponent;
	ftxui::Component movesComponent;
	ftxui::Component infoComponent;

	ftxui::Component layout;
	ftxui::Component content;
};

#endif
