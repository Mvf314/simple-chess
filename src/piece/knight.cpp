#include "knight.h"

char Knight::getChar() {
	if (c == Color::WHITE) {
		return 'N';
	} else {
		return 'n';
	}
}

int Knight::getScore() {
	return 3;
}

Knight* Knight::clone() {
	return dynamic_cast<Knight*>(new Knight(*this));
}

std::vector<Position> Knight::lineOfSight(const std::vector<std::shared_ptr<Piece>>& pieces) {
	std::vector<Position::MoveResult> results = moveset();
	std::vector<Position> moves = std::vector<Position>();

	std::vector<Position> allowedMoves = std::vector<Position>();

	// boilerplate
	for (Position::MoveResult result : results) {
		if (!result.first) {
			moves.push_back(result.second);
		}
	}

	bool includeMove = true;

	for (Position move : moves) {
		for (auto piece_ptr : pieces) {
			Piece* piece = piece_ptr.get();
			if (piece->pos == move && piece->c == c) {
				includeMove = false;
			}
		}
		if (includeMove) {
			allowedMoves.push_back(move);
		}
	}
	return allowedMoves;
}

std::vector<Position::MoveResult> Knight::moveset() {
	std::vector<Position::MoveResult> results;
	results.push_back(pos.moveTruncate( 2,  1, true));
	results.push_back(pos.moveTruncate( 2, -1, true));
	results.push_back(pos.moveTruncate(-2,  1, true));
	results.push_back(pos.moveTruncate(-2, -1, true));
	results.push_back(pos.moveTruncate(-1,  2, true));
	results.push_back(pos.moveTruncate( 1,  2, true));
	results.push_back(pos.moveTruncate(-1, -2, true));
	results.push_back(pos.moveTruncate( 1, -2, true));
	return results;
}
