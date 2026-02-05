#include "game.h"

#include "piece/king.h"
#include "piece/queen.h"
#include "piece/rook.h"
#include "piece/bishop.h"
#include "piece/knight.h"
#include "piece/pawn.h"

#include <sstream>
#include <iostream>
#include <map>

Game::Game() {
	
	pieces = Board();

	for (int i = 0; i < 8; i++) {
		pieces.push_back(std::make_shared<Pawn>(Color::WHITE, Position(1 + i, 2)));
		pieces.push_back(std::make_shared<Pawn>(Color::BLACK, Position(1 + i, 7)));
	}

	for (int i = 0; i < 2; i++) {
		pieces.push_back(std::make_shared<Rook>(Color::WHITE, Position(1 + (7 * i), 1)));
		pieces.push_back(std::make_shared<Rook>(Color::BLACK, Position(1 + (7 * i), 8)));
		pieces.push_back(std::make_shared<Knight>(Color::WHITE, Position(2 + (5 * i), 1)));
		pieces.push_back(std::make_shared<Knight>(Color::BLACK, Position(2 + (5 * i), 8)));
		pieces.push_back(std::make_shared<Bishop>(Color::WHITE, Position(3 + (3 * i), 1)));
		pieces.push_back(std::make_shared<Bishop>(Color::BLACK, Position(3 + (3 * i), 8)));
	}

	pieces.push_back(std::make_shared<King>(Color::WHITE, Position(5, 1)));
	pieces.push_back(std::make_shared<King>(Color::BLACK, Position(5, 8)));
	pieces.push_back(std::make_shared<Queen>(Color::WHITE, Position(4, 1)));
	pieces.push_back(std::make_shared<Queen>(Color::BLACK, Position(4, 8)));


	killed = Board();

	state = State::WHITE_TURN;
	gen = std::mt19937(rd());
}


void Game::run() {
	while (state != State::WHITE_WIN && state != State::BLACK_WIN && state != State::DRAW) {
		// WHITE TURN

		Move whiteMove = whiteEvaluator(pieces);

		printMove(whiteMove);
		executeMove(whiteMove);

		updateState();
		if (state == State::WHITE_WIN || state == State::BLACK_WIN || state == State::DRAW) {
			break;
		}

		Move blackMove = blackEvaluator(pieces);
		printMove(blackMove);
		executeMove(blackMove);
		
		updateState();
	}

}


void Game::printBoard(const Board& pcs, const std::vector<Position>& moves) {

	auto board = getBoard(pcs);

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

void Game::printBoard(const Board& pcs) {
	std::vector<Position> empty = std::vector<Position>();
	printBoard(pcs, empty);
}

void Game::printBoard(const std::vector<Position>& moves) {
	printBoard(pieces, moves);
}

void Game::printBoard() {
	std::vector<Position> empty = std::vector<Position>();

	printBoard(empty);
}

void Game::setEvaluators(std::function<Move (const Board&)> white, std::function<Move (const Board&)> black) {
	whiteEvaluator = white;
	blackEvaluator = black;
}

BoardText Game::getBoard(const Board& pcs) {
	BoardText b = BoardText();

	for (auto ptr : pcs) {
		Piece* p = ptr.get();
		b[p->pos.rank - 1][p->pos.file - 1] = p->getChar();
	}

	return b;
}

BoardText Game::getBoard() {
	return getBoard(pieces);
}

void Game::updateState() {

	// Check mate here.

	if (Piece::inCheck(pieces, Color::WHITE)) {
		state = State::WHITE_CHECK;
	}
	if (Piece::inCheck(pieces, Color::BLACK)) {
		state = State::BLACK_CHECK;
	}
}

// TODO check sensible smart pointer use.
void Game::executeMove(Move move) {
	int remove = -1;
	for (size_t i = 0; i < pieces.size(); i++) {
		Piece* piece = pieces[i].get();

		if (piece->pos == move.second) {
			// We capture a piece.
			killed.push_back(std::shared_ptr<Piece>(piece->clone()));
			remove = i;
		}
	}

	// Find piece to move
	for (auto piece_ptr : pieces) {
		Piece* piece = piece_ptr.get();
		if (piece->pos == move.first.get()->pos) {
			piece->pos = move.second;
		}
	}

	if (remove != -1) {
		// Should happen always.
		pieces.erase(pieces.begin() + remove);
	}
}


// TODO implement evaluators
Move Game::standardEvaluator(const Board& pcs) {
	for (auto piece_ptr : pcs) {
		Piece* piece = piece_ptr.get();
		std::vector<Position> moves = piece->validMoves(pcs);
		if (moves.size() > 0) {
			// bad
			return std::make_pair(std::shared_ptr<Piece>(piece->clone()), moves[0]);
		}
	}
	return std::make_pair(pcs[0], Position(1,1));
}

// User input (for white)
Move Game::userInput(const Board& pcs) {
	bool foundPiece = false;
	bool foundMove = false;

	std::string input;
	std::vector<Position> moves;

	while (!foundPiece) {

		printBoard(pcs);

		int piece_id;

		std::cout << "move piece at: ";
		std::cin >> input;
		if (input.length() != 2) {
			std::cout << "Need input of length 2!\n";
		} else {
			try {
				Position p = Position(input.at(0), input.at(1));
				piece_id = -1;
				for (size_t i = 0; i < pcs.size(); i++) {
					Piece* piece = pcs[i].get();
					if (piece->pos == p && piece->c == Color::WHITE) {
						piece_id = i;
						moves = piece->validMoves(pcs);
						if (moves.size() > 0) {
							foundPiece = true;
						} else {
							std::cout << "Piece has no possible moves!\n";
						}
					}
				}
				if (piece_id == -1) {
					std::cout << "Not a valid piece!\n";
				}
			} catch (std::out_of_range &e) {
				std::cout << "Game::userInput out_of_range : " << e.what() << "\n";
				foundPiece = false;
			} catch (std::invalid_argument& e) {
				std::cout << "Game::userInput invalid_argument : " << e.what() << "\n";
				foundPiece = false;
			}
		}

		while (foundPiece && !foundMove) {

			printBoard(pcs, moves);

			std::cout << "move piece at " << pcs[piece_id].get()->pos.toString() << " to: ";
			std::cin >> input;
			if (input.length() == 1) {
				if (input.at(0) == 'q') {
					// Discard piece
					foundMove = false;
					foundPiece = false;
				}
			} else if (input.length() != 2) {
				std::cout << "Need input of length 2!\n";
			} else {
				try {
					Position p = Position(input.at(0), input.at(1));
					for (auto move : moves) {
						if (move == p) {
							// Return
							return std::make_pair(pcs[piece_id], move);
						}
					}
					std::cout << "Not a valid move!\n";
				} catch (std::out_of_range& e) {
					std::cout << "Game::userInput out_of_range : " <<  e.what() << "\n";
				} catch (std::invalid_argument& e) {
					std::cout << "Game::userInput invalid_argument : " << e.what() << "\n";
				}
			}
		}
	}
	

	// TODO to please the compiler
	std::cout << "Inaccessible location reached!\n";
	return std::make_pair(pcs[0], Position(1, 1));


}

void Game::printMove(Move move) {
	std::cout << "Moved " << move.first.get()->getChar() << " to " << move.second.toString() << ".\n";
}
