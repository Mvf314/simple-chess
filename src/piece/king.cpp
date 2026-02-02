#include "king.h"

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

	return results;
}
