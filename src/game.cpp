#include "game.h"

#include "rook.h"
#include "king.h"
#include "bishop.h"

#include <sstream>
#include <iostream>
#include <map>

Game::Game() {
	
	pieces = Board();

	pieces.push_back(std::make_shared<Rook>(Color::WHITE, Position('D', '4')));

	pieces.push_back(std::make_shared<Rook>(Color::BLACK, Position('B', '4')));

	pieces.push_back(std::make_shared<King>(Color::WHITE, Position('F', '4')));
	pieces.push_back(std::make_shared<King>(Color::BLACK, Position('A', '8')));


	pieces.push_back(std::make_shared<Bishop>(Color::WHITE, Position('D', '6')));
	
	
	printBoard();

	for (auto piece : pieces) {
		Piece* p = piece.get();
		std::cout << "Found piece " << p->getChar() << '\n';
		auto moves = p->validMoves(pieces);

		printBoard(moves);

	}


	state = State::GAME;

	gen = std::mt19937(rd());
}

void Game::printBoard(const std::vector<Position>& moves) {

	auto board = getBoard();

	for (auto move : moves) {
		board[move.rank - 1][move.file - 1] = '.';
	}

	std::stringstream ss = std::stringstream();
	ss << "  A B C D E F G H\n";
	ss << " \u2554\u2550\u2564\u2550\u2564\u2550\u2564\u2550\u2564\u2550\u2564\u2550\u2564\u2550\u2564\u2550\u2557\n";
	// Loop in reverse, because rank 1 is the rank closes to us and therefore printed last.
	for (size_t rank = board.size() - 1; rank < 8; rank--) {
		ss << rank + 1 << "\u2551";
		for (char square : board[rank]) {
			ss << square << "\u2502";
		}
		ss.seekp(-1, ss.cur); // Remove one character
		ss << "\u2551\n";
		ss << " \u255F\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u2562\n";
	}
	ss.seekp(-51, ss.cur); // Erase last line, kinda cursed
	ss << "\u255A\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u255D\n";
	

	std::cout << ss.str();
}

void Game::printBoard() {
	std::vector<Position> empty = std::vector<Position>();

	printBoard(empty);
}

void Game::setEvaluators(std::function<Move (const Board&)> white, std::function<Move (const Board&)> black) {
	whiteEvaluator = white;
	blackEvaluator = black;
}

BoardText Game::getBoard() {
	BoardText b = BoardText();

	for (auto ptr : pieces) {
		Piece* p = ptr.get();
		b[p->pos.rank - 1][p->pos.file - 1] = p->getChar();
	}

	return b;
}

Move Game::standardEvaluator(const Board& pcs) {
	// TODO
	/*
	for (auto p : pcs) {
		switch (p.c) {
			case Color::WHITE:
				scoreWhite += p.getScore();
				break;
			case Color::BLACK:
				scoreBlack += p.getScore();
				break;
		}
	}*/
	return std::make_pair(pcs[0], Position(1,1));
}
