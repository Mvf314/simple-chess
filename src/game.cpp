#include "game.h"

#include <sstream>
#include <iostream>
#include <map>

Game::Game() {
	pieces = std::vector<Piece>();
	
	// Place pawns
	for (int i = 0; i < 8; i++) {
		// White pawns are rank 2, black pawns on rank 7.
		Piece p1(Color::WHITE, Type::PAWN, Position(i + 1, 2));
		Piece p2(Color::BLACK, Type::PAWN, Position(i + 1, 7));
		pieces.push_back(p1);
		pieces.push_back(p2);
	}

	// Place other pieces
	for (int j = 0; j < 2; j++) {
		Piece wRook(Color::WHITE, Type::ROOK, Position(1 + (7 * j), 1));
		Piece bRook(Color::BLACK, Type::ROOK, Position(1 + (7 * j), 8));
		Piece wKnight(Color::WHITE, Type::KNIGHT, Position(2 + (5 * j), 1));
		Piece bKnight(Color::BLACK, Type::KNIGHT, Position(2 + (5 * j), 8));
		Piece wBishop(Color::WHITE, Type::BISHOP, Position(3 + (3 * j), 1));
		Piece bBishop(Color::BLACK, Type::BISHOP, Position(3 + (3 * j), 8));

		pieces.push_back(wRook);
		pieces.push_back(bRook);
		pieces.push_back(wKnight);
		pieces.push_back(bKnight);
		pieces.push_back(wBishop);
		pieces.push_back(bBishop);
	}

	// Place kings and queens
	Piece wKing(Color::WHITE, Type::KING, Position(5, 1));
	Piece bKing(Color::BLACK, Type::KING, Position(5, 8));
	Piece wQueen(Color::WHITE, Type::QUEEN, Position(4, 1));
	Piece bQueen(Color::BLACK, Type::QUEEN, Position(4, 8));

	pieces.push_back(wKing);
	pieces.push_back(bKing);
	pieces.push_back(wQueen);
	pieces.push_back(bQueen);


	killed = std::vector<Piece>();

	evaluator = &Game::standardEvaluator;

	state = State::GAME;

	gen = std::mt19937(rd());
}

void Game::run() {


	// White starts :)
	
	while ((state != State::BLACK_WIN) && (state != State::WHITE_WIN)) {

		printBoard();

		// get input
		auto result = getPiece();
		Piece piece = result.first;
		std::vector<Position> moves = result.second;
		// Found valid piece
		
		printBoard(moves);
		
		Position move = getMove(moves);

		// Found valid move.
		
		executeMove(piece, move);
		
		if (checkBoard()) {
			break;
		}


		auto turn = doTurn(Color::BLACK);
		executeMove(turn.first, turn.second);

		if (checkBoard()) {
			break;
		}
	}

	// Find out who won.
	if (state == State::WHITE_WIN) {
		std::cout << "White won!\n";
	}
	if (state == State::BLACK_WIN) {
		std::cout << "Black won!\n";
	}
	return;
}

bool Game::checkBoard() {

	// check mate first;

	if (isCheck(pieces, Color::BLACK)) {
		std::cout << "Black in check!\n";
		state = State::BLACK_CHECK;
	}
	if (isCheck(pieces, Color::WHITE)) {
		std::cout << "White in check!\n";
		state = State::WHITE_CHECK;
	}
}


bool Game::isValid() {
	return true;
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


std::array<std::array<char, 8>, 8> Game::getBoard() {
	auto board = std::array<std::array<char, 8>, 8>();

	for (size_t i = 0; i < board.size(); i++) {
		board[i].fill(' ');
	}

	for (Piece piece : pieces) {
		board[piece.pos.rank - 1][piece.pos.file - 1] = piece.getChar();
	}
	return board;
}

std::pair<Piece, std::vector<Position>> Game::getPiece() {
	std::string input;
	while (true) {
		std::cout << "Move piece at: ";
		std::cin >> input;
		if (input.length() != 2) {
			std::cout << "Need input of length 2!\n";
		} else {
			try {
				Position p = Position(input.at(0), input.at(1));
				for (Piece piece : pieces) {
					// TODO hardcoded play as white
					if (piece.pos == p && piece.c == Color::WHITE) {
						// Piece needs to be able to move.
						std::vector<Position> moves = piece.validMoves(pieces);
						if (moves.size() > 0) {
							return std::make_pair(piece, moves);
						} else {
							std::cout << "Piece has no possible moves!\n";
						}
					}
				}
				std::cout << "Not a valid piece!\n";
			} catch (std::out_of_range &e) {
				std::cout << e.what() << "\n";
			} catch (std::invalid_argument& e) {
				std::cout << e.what() << "\n";
			}
		}
	}
}

Position Game::getMove(std::vector<Position>& allowedMoves) {
	std::string input;
	while (true) {
		std::cout << "Move piece to: ";
		std::cin >> input;
		if (input.length() != 2) {
			std::cout << "Need input of length 2!\n";
		} else {
			try {
				Position p = Position(input.at(0), input.at(1));
				for (Position move : allowedMoves) {
					if (move == p)
						return p;
				}
				std::cout << "Not a valid move!\n";
			} catch (std::out_of_range &e) {
				std::cout << e.what() << "\n";
			} catch (std::invalid_argument& e) {
				std::cout << e.what() << "\n";
			}
		}

	}
}

void Game::executeMove(Piece p, Position move) {
	// We assume that this has been checked and is a valid move. 
	int killedIdx = 0;
	bool kill = false;
	for (size_t i = 0; i < pieces.size(); i++) {
		if (pieces[i].pos == move) {
			// By assumption that this move is valid, this is an enemy. So capture.
			// Queue kill.
			killedIdx = i;
			kill = true;
		}
		if (pieces[i].pos == p.pos) {
			// Move our piece. Because we don't have board state but only pieces it does not matter if the enemy is not killed first.
			pieces[i].pos = move;
		}
	}

	if (kill) {
		std::cout << p.getChar() << " kills " << pieces[killedIdx].getChar() << " at " << move.toString() << "!\n";
		killed.push_back(pieces[killedIdx]);
		pieces.erase(pieces.begin() + killedIdx);
	} else {
		std::cout << p.getChar() << " moves to " << move.toString() << "!\n";
	}


	// Check if we lost (very naive now)
	bool whiteKing = false;
	bool blackKing = false;
	for (Piece p : pieces) {
		if (p.t == Type::KING) {
			if (p.c == Color::WHITE)
				whiteKing = true;
			if (p.c == Color::BLACK) 
				blackKing = true;
		}
	}

	if (!whiteKing) 
		state = State::BLACK_WIN;
	if (!blackKing)
		state = State::WHITE_WIN;
}

std::vector<Piece> Game::simulateMove(Piece p, Position move) {
	// Basically the same as above, except do not actually do it.
	std::vector<Piece> tempPieces = pieces;
	int killedIdx = 0;
	bool kill = false;
	for (size_t i = 0; i < tempPieces.size(); i++) {
		if (tempPieces[i].pos == move) {
			killedIdx = i;
			kill = true;
		}
		if (tempPieces[i].pos == p.pos) {
			tempPieces[i].pos = move;
		}
	}

	if (kill) {
		tempPieces.erase(tempPieces.begin() + killedIdx);
	}

	return tempPieces;
}

// Determine if c is in check
bool Game::isCheck(std::vector<Piece>& pcs, Color c) {
	// Find king
	Position king = Position(1, 1);
	for (Piece p : pcs) {
		if (p.t == Type::KING) {
			if (p.c == c) {
				king = p.pos;
			}
		}
	}

	// To avoid circular function calling, do without p.validMoves(). So just analyze the board directly and look at character positions.
	for (Piece p : pcs) {
		if (p.c != c) {
			std::vector<Position> validMoves = p.validMoves(pcs);
			for (auto move : validMoves) {
				if (move == king) {
					return true;
				}
			}
		}
	}
	return false;
}


std::pair<Piece, Position> Game::doTurn(Color c) {
	std::multimap<float, std::pair<Piece, Position>> futureScores = std::multimap<float, std::pair<Piece, Position>>();
	for (Piece p : pieces) {
		if (p.c == c) {
			std::vector<Position> moves = p.validMoves(pieces);
			if (moves.size() > 0) {
				for (auto move : moves) {
					// Here we found a possible move. 
					std::vector<Piece> future = simulateMove(p, move);
					float score = evaluator(future);
					futureScores.emplace(score, std::make_pair(p, move));
				}
			}
		}
	}
	
	if (futureScores.size() > 0) {
		float bestScore = futureScores.begin()->first;
		size_t count = futureScores.count(bestScore);
		int idx = 0;
		if (count > 1) {
			// Pick random move
			std::uniform_int_distribution<> dist(0, count - 1);
			idx = dist(gen);
		}
		int counter = 0;
		for (auto kv : futureScores) {
			if (counter == idx) {
				return kv.second;
			}
			counter++;
		}
	}


	throw std::runtime_error("No possible move!");
}

void Game::setEvaluator(std::function<float (const std::vector<Piece>&)> fun) {
	evaluator = fun;
}

float Game::standardEvaluator(const std::vector<Piece>& pcs) {
	float scoreWhite = 0;
	float scoreBlack = 0;
	for (auto p : pcs) {
		switch (p.c) {
			case Color::WHITE:
				scoreWhite += p.getScore();
				break;
			case Color::BLACK:
				scoreBlack += p.getScore();
				break;
		}
	}
	return scoreWhite - scoreBlack;
}
