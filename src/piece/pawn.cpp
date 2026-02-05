#include "pawn.h"

char Pawn::getChar() {
	if (c == Color::WHITE) {
		return 'P';
	} else {
		return 'p';
	}
}

int Pawn::getScore() {
	return 1;
}

Pawn* Pawn::clone() {
	return dynamic_cast<Pawn*>(new Pawn(*this));
}

std::vector<Position> Pawn::validMoves(const std::vector<std::shared_ptr<Piece>>& pieces) {
	std::vector<Position> lineOfSightMoves = lineOfSight(pieces);
	std::vector<Position> validMoves = std::vector<Position>();
	std::vector<Position> moves = std::vector<Position>();


	for (Position move : lineOfSightMoves) {
		bool isCapture = false;
		for (const auto& piece_ptr : pieces) {
			if (piece_ptr.get()->pos == move) {
				isCapture = true;
				// If move is a capture
				// NB: We don't need to check chat piece_ptr points toward an enemy. This is ensured in lineOfSight.
				if (move.file != pos.file) {
					// We can only capture diagonally
					validMoves.push_back(move);
				}
			}
		}
		if (!isCapture) {
			// If the move is not a capture,
			if (move.file == pos.file) {
				// We can move only cardinally
				validMoves.push_back(move);
			}
		}

	}




	// Here do the standard check, anti-self-check-check as done in piece.h.
	for (Position move : validMoves) {
		std::shared_ptr<Piece> ptr;
		bool foundPiece = false;
		for (const auto& piece_ptr : pieces) {
			if (piece_ptr.get()->pos == pos) {
				ptr = piece_ptr;
				foundPiece = true;
			}
		}
		if (foundPiece) {
			auto simulated = simulateMove(pieces, ptr, move);

			if (!Piece::inCheck(simulated, c)) {
				moves.push_back(move);
			}
		} else {
			// Handle error...
		}

	}
	return moves;
}

std::vector<Position::MoveResult> Pawn::moveset() {
	std::vector<Position::MoveResult> results;

	// Movement is direction-dependent.
	bool dir = (c == Color::WHITE ? true : false);

	// Double move (for first move)
	if (c == Color::WHITE && pos.rank == 2) {
		results.push_back(pos.moveTruncate(2, 0, dir));
	} else if (c == Color::BLACK && pos.rank == 7) {
		results.push_back(pos.moveTruncate(2, 0, dir));
	}
	// Single move
	results.push_back(pos.moveTruncate(1, 0, dir));
	// Diagonal moves (for capture)
	results.push_back(pos.moveTruncate(1, 1, dir));
	results.push_back(pos.moveTruncate(1, -1, dir));

	return results;
}
