#include "rook.h"

char Rook::getChar() {
	if (c == Color::WHITE) {
		return 'R';
	} else {
		return 'r';
	}
}

int Rook::getScore() {
	return 5;
}

Rook* Rook::clone() {
	return dynamic_cast<Rook*>(new Rook(*this));
}

std::vector<Position::MoveResult> Rook::moveset() {

	std::vector<Position::MoveResult> results;

	for (int i = 1; i < 8; i++) {
		results.push_back(pos.moveTruncate(i, 0, true));
		results.push_back(pos.moveTruncate(-i, 0, true));
		results.push_back(pos.moveTruncate(0, i, true));
		results.push_back(pos.moveTruncate(0, -i, true));
	}

	return results;
}
