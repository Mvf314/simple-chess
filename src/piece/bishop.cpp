#include "bishop.h"

char Bishop::getChar() {
	if (c == Color::WHITE) {
		return 'B';
	} else {
		return 'b';
	}
}

int Bishop::getScore() {
	return 3;
}

Bishop* Bishop::clone() {
	return dynamic_cast<Bishop*>(new Bishop(*this));
}

std::vector<Position::MoveResult> Bishop::moveset() {
	std::vector<Position::MoveResult> results;

	for (int i = 1; i < 8; i++) {
		results.push_back(pos.moveTruncate(i, i, true));
		results.push_back(pos.moveTruncate(-i, i, true));
		results.push_back(pos.moveTruncate(-i, -i, true));
		results.push_back(pos.moveTruncate(i, -i, true));
	}
	return results;
}
