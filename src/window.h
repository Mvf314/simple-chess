#ifndef __CHESS_WINDOW_H__
#define __CHESS_WINDOW_H__

#include "game.h"

class Window {
public:
	Window(Game* g);

	void updateComponents();

	void startApp(ftxui::App);

	std::string result;
private:
	// Update pieces, moves and pieceMap. col is the color of the player.
	void updateMenus(Color col);
	void updateBoard(int pieceIdx, int moveIdx);
	
	ftxui::Component getBoard(Board& b, std::vector<Position> moveList);

	Game* game;

	size_t screenWidth;
	size_t screenHeight;

	size_t boardWidth;
	size_t boardHeight;

	// Board values;
	int test1, test2;

	// Menu values, so that they can be accessed through reference
	std::vector<std::string> pieces;
	std::vector<std::vector<std::string>> moves;
	
	// moves[i] is the allowed moves of pieces[i] = Game::pieces[j].
	// This is the translation map (pieceMap[i] = j);
	std::vector<size_t> pieceMap;


	// This will contain all possible boards. TODO this should be done differently maybe.
	std::vector<std::vector<ftxui::Component>> boards;


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
