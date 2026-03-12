#include "king.h"

#include "rook.h"

std::vector<Position> King::validMoves(const std::vector<std::shared_ptr<Piece>>& pieces) {
	// First use the base implementation.
	std::vector<Position> standardMoves = Piece::validMoves(pieces);
	std::vector<Position> moves = std::vector<Position>();

	for (const auto& move : standardMoves) {
		if (abs(move.file - pos.file) == 2) {
			// Castling move.
			bool legalCastleMove = true;

			// Do checks here

			// King must not have moved
			if (moved) {
				legalCastleMove = false;
			}
			

			// Associated rook must not have moved.
			int file = 0;
			if (move.file - pos.file > 0) {
				// This is a move from left to right
				// So the rook should be in file H.
				file = 8;
			} else {
				// Move from R2L, so the rook should be in file A.
				file = 1;
			}

			// Seach thhrough pieces
			bool foundRook = false;
			bool queensideRookOccupied = false;
			for (auto piece_ptr : pieces) {
				Piece* piece = piece_ptr.get();
				if (piece->pos == Position(file, pos.rank)) {
					if (piece->c == c) {
						if (Rook* rook = dynamic_cast<Rook*>(piece)) {
							foundRook = true;
							// There is a friendly rook on (file, pos.rank).
							if (rook->hasMoved()) {
								// Rook must not have moved.
								legalCastleMove = false;
							}
						}
					}
				} else if (piece->pos == Position(2, pos.rank)) {
					queensideRookOccupied = true;
				} else if (piece->pos == pos) {
					// Found ourselves
					// We cannot castle through a check.
					// This is taking one step. If the simulated board with this move is check, illegal.
					Position intermediateMove = Position((move.file + pos.file) / 2, pos.rank);
					if (Piece::inCheck(simulateMove(pieces, piece_ptr, intermediateMove), c)) {
						legalCastleMove = false;
					}
				}
			}

			// We can't castle ourselves out of check.
			if (Piece::inCheck(pieces, c)) {
				legalCastleMove = false;
			}

			// The rook must actually be there.
			if (!foundRook) {
				legalCastleMove = false;
			}

			// If we castle queenside, the rook can't be blocked.
			if (queensideRookOccupied && file == 1) {
				legalCastleMove = false;
			}

			
			



			if (legalCastleMove) {
				moves.push_back(move);
			}
		} else {
			moves.push_back(move);
		}
	}

	
	return moves;
}

char King::getChar() {
	if (c == Color::WHITE) {
		return 'K';
	} else {
		return 'k';
	}
}

int King::getScore() {
	return 100;
}

King* King::clone() {
	return dynamic_cast<King*>(new King(*this));
}

void King::setMoved() {
	moved = true;
}

std::vector<Position::MoveResult> King::moveset() {
	std::vector<Position::MoveResult> results;

	results.push_back(pos.moveTruncate(1, -1, true));
	results.push_back(pos.moveTruncate(1, 0, true));
	results.push_back(pos.moveTruncate(1, 1, true));
	results.push_back(pos.moveTruncate(-1, -1, true));
	results.push_back(pos.moveTruncate(-1, 0, true));
	results.push_back(pos.moveTruncate(-1, 1, true));
	results.push_back(pos.moveTruncate(0, -1, true));
	results.push_back(pos.moveTruncate(0, 1, true));

	// These are used for castling. King needs line of sight, so it makes sense to add it here.
	results.push_back(pos.moveTruncate(0, 2, true));
	results.push_back(pos.moveTruncate(0, -2, true));

	return results;
}
