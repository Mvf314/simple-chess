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
}


void Game::run() {

	Random random = Random();

	while (state != State::WHITE_WIN && state != State::BLACK_WIN && state != State::DRAW) {
		// WHITE TURN

		Move whiteMove = whiteEvaluator(pieces, killed, random);

		executeMove(whiteMove);


		updateState();
		if (state == State::WHITE_WIN || state == State::BLACK_WIN || state == State::DRAW) {
			break;
		}

		Move blackMove = blackEvaluator(pieces, killed, random);
		executeMove(blackMove);
		
		updateState();
	}

	printBoard();

	if (state == State::WHITE_WIN) {
		std::cout << "White wins!\n";
	} else if (state == State::BLACK_WIN) {
		std::cout << "Black wins!\n";
	}

}


void Game::printBoard(const Board& pcs, const Board& killed, const std::vector<Position>& moves) {

	auto board = getBoard(pcs);

	for (auto move : moves) {
		board[move.rank - 1][move.file - 1] = '.';
	}

	// Track killed pieces per color
	Board killedBlack = Board();
	Board killedWhite = Board();

	for (auto piece_ptr : killed) {
		Piece* piece = piece_ptr.get();
		if (piece->c == Color::WHITE) {
			killedWhite.push_back(std::shared_ptr<Piece>(piece->clone()));
		} else if (piece->c == Color::BLACK) {
			killedBlack.push_back(std::shared_ptr<Piece>(piece->clone()));
		}
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
		ss << "\u2551";
		
		if (killedBlack.size() > 0) {
			if (rank == 3) {
				ss << "\tPieces captured:";
			}
			if (rank == 2) {
				ss << "\t";
				for (auto piece_ptr : killedBlack) {
					Piece* piece = piece_ptr.get();
					ss << piece->getChar();
				}
			}
		}
		if (killedWhite.size() > 0) {
			if (rank == 6) {
				ss << "\tPieces captured:";
			}
			if (rank == 5) {
				ss << "\t";
				for (auto piece_ptr : killedWhite) {
					Piece* piece = piece_ptr.get();
					ss << piece->getChar();
				}
			}
		}
		ss << "\n \u255F\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u253C\u2500\u2562\n";
	}
	ss.seekp(-51, ss.cur); // Erase last line, kinda cursed
	ss << "\u255A\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u2567\u2550\u255D\n";
	

	std::cout << ss.str();
}

void Game::printBoard(const Board& pcs, const Board& killed) {
	std::vector<Position> empty = std::vector<Position>();
	printBoard(pcs, killed, empty);
}

void Game::printBoard(const std::vector<Position>& moves) {
	printBoard(pieces, killed, moves);
}

void Game::printBoard() {
	std::vector<Position> empty = std::vector<Position>();

	printBoard(empty);
}

void Game::setEvaluators(std::function<Move (const Board&, const Board&, Random&)> white, std::function<Move (const Board&, const Board&, Random&)> black) {
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

	if (Piece::inCheck(pieces, Color::WHITE)) {
		state = State::WHITE_CHECK;
		// Check mate
		for (auto piece_ptr : pieces) {
			Piece* piece = piece_ptr.get();
			if (piece->c == Color::WHITE) {
				// TODO
			}
		}
	}
	if (Piece::inCheck(pieces, Color::BLACK)) {
		state = State::BLACK_CHECK;
		int movesLeft = 0;
		for (auto piece_ptr : pieces) {
			Piece* piece = piece_ptr.get();
			if (piece->c == Color::BLACK) {
				movesLeft += piece->validMoves(pieces).size();
			}
		}
		if (movesLeft == 0) {
			state = State::WHITE_WIN;
		}
	}
}

// TODO check sensible smart pointer use.
void Game::executeMove(Move move) {
	int remove = -1;
	char killedPiece = '?'; //TODO bad design?
	char movedPiece = '?';
	// If we capture another piece normally
	for (size_t i = 0; i < pieces.size(); i++) {
		Piece* piece = pieces[i].get();

		if (piece->pos == move.second) {
			// We capture a piece.
			killed.push_back(std::shared_ptr<Piece>(piece->clone()));
			killedPiece = piece->getChar();
			remove = i;
		}
	}

	for (auto piece_ptr : pieces) {
		Piece* piece = piece_ptr.get();
		if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
			// Ensure that pawns are only vulnerable to en-passant for one turn.
			if (move.first.get()->c != pawn->c) {
				if (pawn->isVulnerable()) {
					pawn->setNotVulnerable();
				}
			}
		}
	}

	// Detect en-passant.
	bool enPassant = false;
	if (Pawn* pawn = dynamic_cast<Pawn*>(move.first.get())) {
		if (pawn->pos.file != move.second.file) {
			// Capture move. Now figure out e.p.
			bool pieceOnTarget = false;
			for (auto piece_ptr : pieces) {
				Piece* piece = piece_ptr.get();
				if (piece->pos == move.second) {
					pieceOnTarget = true;
				}
			}

			if (!pieceOnTarget) {
				// E.P.
				enPassant = true;
				Position attacked = Position(move.second.file, pawn->pos.rank);
				// Remove a piece, same as earlier.
				for (size_t i = 0; i < pieces.size(); i++) {

					Piece* piece = pieces[i].get();
					if (piece->pos == attacked) {
						killed.push_back(std::shared_ptr<Piece>(piece->clone()));
						killedPiece = piece->getChar();
						// We can use the same remove variable, as we can never capture a piece normally and en-passant in the same turn.
						remove = i;
					}
				}
			}
		}
	}


	// Detect if we castled.
	bool castle = false;
	int castleDir = 0;
	if (King* king = dynamic_cast<King*>(move.first.get())) {
		if (abs(king->pos.file - move.second.file) == 2) {
			castle = true;
			castleDir = move.second.file - king->pos.file;
		}
	}

	int rookFile = 0;
	int rookRank = 0;
	if (castle) {
		// If we castled, find corresponding rook.
		// Hardcoded for normal chess. Later maybe do something more adaptable.
		if (castleDir < 0) {
			rookFile = 1;
		} else {
			rookFile = 8;
		}
		rookRank = move.second.rank;
	}

	// Find piece to move
	for (auto piece_ptr : pieces) {
		Piece* piece = piece_ptr.get();
		if (piece->pos == move.first.get()->pos) {

			movedPiece = piece->getChar();

			// For kings and rooks, keep track if they moved. 
			// Needed for check rules.
			if (King* king = dynamic_cast<King*>(piece)) {
				king->setMoved();
			} else if (Rook* rook = dynamic_cast<Rook*>(piece)) {
				rook->setMoved();
			} else if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
				if (abs(pawn->pos.rank - move.second.rank) == 2) {
					// Initial move
					pawn->setVulnerable();
				}
			}


			// Actuallu move the piece (need to do this after checking pawn's first move)
			piece->pos = move.second;
		} 
		if (castle) {
			if (piece->pos == Position(rookFile, rookRank)) {
				if (Rook* rook = dynamic_cast<Rook*>(piece)) {
					// Should always get here.
					// Harcoded for normal chess.
					if (castleDir < 0) {
						rook->pos = Position(4, rookRank);
					} else {
						rook->pos = Position(6, rookRank);
					}
				}
			}
		}
	}

	if (enPassant) {
		std::cout << movedPiece << " captured " << killedPiece << " en passant at " << move.second.toString() << ".\n";
	} else if (castle) {
		if (castleDir < 0) {
			std::cout << movedPiece << " castled queenside.\n";
		} else {
			std::cout << movedPiece << " castled kingside.\n";
		}
	} else if (remove == -1) {
		// Did not capture, did not castle.
		std::cout << movedPiece << " moved to " << move.second.toString() << ".\n";
	} else {
		// Captured a piece
		std::cout << movedPiece << " captured " << killedPiece << " at " << move.second.toString() << ".\n";
	}

	if (remove != -1) {
		// Should happen always.
		pieces.erase(pieces.begin() + remove);
	}
}


// TODO implement evaluators
Move Game::standardEvaluator(const Board& pcs, const Board& killed, Random& rand, Color c) {
	(void) killed; //Unused
	// Keep track of possible moves.
	std::multimap<float, Move> futureScores = std::multimap<float, Move>(); 
	for (auto piece_ptr : pcs) {
		Piece* piece = piece_ptr.get();
		if (piece->c == c) {
		std::vector<Position> moves = piece->validMoves(pcs);
			for (Position move : moves) {
				// Iterate through every possible move.
				

				// The future of this move is:
				auto nextBoard = piece->simulateMove(pcs, piece_ptr, move);

				int score = getScore(nextBoard, c);
			
				// Score for this move will be the minimum of the possible futures
				// If the opponent has even one good response, we dont want the move.
				// We want to minimize the possible responses.

				// Look one level deeper
				// Ensure we don't select a non-existent move.
				bool foundMove = false;
				int minScore = score;
				for (auto piece_ptr2 : nextBoard) {
					Piece* piece2 = piece_ptr2.get();

					// Look at piece of enemy
					if (piece2->c != c) {
						std::vector<Position> moves2 = piece2->validMoves(nextBoard);

						for (Position move2 : moves2) {
							auto nextNextBoard = piece2->simulateMove(nextBoard, piece_ptr2, move2);

							// Calculate score of this board.
							int futureScore = getScore(nextNextBoard, c);
							
							if (!foundMove) {
								minScore = futureScore;
								foundMove = true;
							} else {
								if (futureScore < minScore) {
									minScore = futureScore;
								}
							}
						}
					}
				}

				if (foundMove) {
					// Another copy?
					futureScores.emplace(minScore, std::make_pair(std::shared_ptr<Piece>(piece->clone()), move));
				}
			}
		}
	}

	// Pick the move. Can be done more elegantly.
	if (futureScores.size() > 0) {

		// Should always happen.
		float bestScore = futureScores.begin()->first;
		size_t count = futureScores.count(bestScore);
		int idx = futureScores.size() - 1; // Select the highest key.
		if (count > 1) {
			// If there is more than 1 possible move, pick a random best move
			idx = rand.getRandomInt(0, count - 1);
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

Move Game::standardEvaluatorBlack(const Board& pcs, const Board& killed, Random& rand) {
	return standardEvaluator(pcs, killed, rand, Color::BLACK);
}

// User input (for white)
Move Game::userInput(const Board& pcs, const Board& killed, Random& rand) {
	(void) rand; // unused
	bool foundPiece = false;
	bool foundMove = false;

	std::string input;
	std::vector<Position> moves;

	while (!foundPiece) {

		printBoard(pcs, killed);

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

			printBoard(pcs, killed, moves);

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

int Game::getScore(const Board& pcs, Color c) {
	int score = 0;
	for (auto piece_ptr : pcs) {
		Piece* piece = piece_ptr.get();
		if (piece->c == c) {
			score += piece->getScore();
		} else {
			score -= piece->getScore();
		}
	}
	return score;
}
