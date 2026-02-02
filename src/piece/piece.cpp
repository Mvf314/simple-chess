#include "piece.h"

#include "king.h"

#include <iostream>

std::vector<Position> Piece::validMoves(const std::vector<std::shared_ptr<Piece>>& pieces) {

	std::vector<Position> moves = std::vector<Position>();
	// Check for check :)
	// It suffices to use lineOfSight for all other pieces, as pinned pieces can attack the king.
	
	std::vector<Position> lineOfSightMoves = lineOfSight(pieces);
	for (Position move : lineOfSightMoves) {
		// Find current piece
		std::shared_ptr<Piece> ptr;
		bool foundPiece = false;
		for (const auto& piece_ptr : pieces) {
			if (piece_ptr.get()->pos == pos) {
				ptr = piece_ptr; // Will this work?
				foundPiece = true;
			}
		}
		if (foundPiece) {
			auto simulated = simulateMove(pieces, ptr, move);

			if (!inCheck(simulated, c)) {
				// This move does not put ourselves in check, and is therefore legal.
				moves.push_back(move);
			}
		} else {
			// Handle error...
		}

	}

	return moves;
}


std::vector<Position> Piece::lineOfSight(const std::vector<std::shared_ptr<Piece>>& pieces) {
	std::vector<Position::MoveResult> results = moveset();
	std::vector<Position> moves = std::vector<Position>();

	// Construct possible moves. We need to do this always, so best to not repeat it in every override. Maybe we need another layer for knight!
	for (Position::MoveResult result : results) {
		if (!result.first) {
			// Move was not truncated
			moves.push_back(result.second);
		}
	}

	// Standard cardinal + diagonal Line of sight check here.
	// Override for horse!
	
	std::vector<Position> allowedMoves = std::vector<Position>();
	// TODO maybe think of something smarter. Here we compute line of sight for every possible move. But performance is not a big deal here
	for (Position move : moves) {
		
		bool includeMove = true;

		for (auto piece_ptr : pieces) {
			Piece* piece = piece_ptr.get();
			if (piece->pos == move && piece->c == c) {
				// Cannot move to piece occupied by same color
				includeMove = false;
			}
		}

		int distance = std::max(std::abs(move.file - pos.file), std::abs(move.rank - pos.rank));
		int stepFile = (move.file - pos.file) / distance;
		int stepRank = (move.rank - pos.rank) / distance;
		
		// Check squares *between* piece and target.
		for (int i = 1; i < distance; i++) {
			for (auto piece_ptr : pieces) {
				Piece* piece = piece_ptr.get();
				if (piece->pos == pos.move(stepRank * i, stepFile * i, true)) {
					includeMove = false;
				}
			}
		}


		// Checks passed
		if (includeMove) {
			allowedMoves.push_back(move);
		}
	}
	return allowedMoves;
}

// TODO reevaluate const.
std::vector<std::shared_ptr<Piece>> Piece::simulateMove(const std::vector<std::shared_ptr<Piece>>& pieces, std::shared_ptr<Piece>& piece, Position move) {
	// Don't do any checking here. Simply just move piece to move.
	std::vector<std::shared_ptr<Piece>> newPieces = std::vector<std::shared_ptr<Piece>>();
	for (const auto& piece_ptr : pieces) {
		if (piece_ptr.get() == piece.get()) {
			// The piece we move
			std::shared_ptr temp = std::shared_ptr<Piece>(piece_ptr.get()->clone());
			temp.get()->pos = move;
			newPieces.push_back(temp);
		} else if (piece_ptr.get()->pos == move) {
			// Do not add if we get caaptured.
		} else {
			// All other pieces which do not get captured. Just push these back as normal
			newPieces.push_back(std::shared_ptr<Piece>(piece_ptr.get()->clone()));
		}
	}
	return newPieces;
}

bool Piece::inCheck(const std::vector<std::shared_ptr<Piece>>& pieces, Color col) {

	Position kingPos = Position(1, 1);
	bool kingFound = false;

	// Find king
	for (const auto& piece_ptr : pieces) {
		if (piece_ptr.get()->c == col) {
			if (King* king = dynamic_cast<King*>(piece_ptr.get())) {
				// Found our king
				kingPos = king->pos;
				kingFound = true;
				break;
			}
		}
	}
	if (!kingFound) {
		// Not in check, but no king found.
		return false;
	}

	// Check possible moves.
	for (const auto& piece_ptr : pieces) {
		if (piece_ptr.get()->c != col) {
			// Enemy piece.
			std::vector<Position> moves = piece_ptr.get()->lineOfSight(pieces);
			for (auto move : moves) {
				if (move == kingPos) {
					return true;
				}
			}
		}
	}

	return false;


}
