#include "queen.h"

char Queen::getChar() {
	if (c == Color::WHITE) {
		return 'Q';
	} else {
		return 'q';
	}
}

int Queen::getScore() {
	return 9;
}

Queen* Queen::clone() {
	return dynamic_cast<Queen*>(new Queen(*this));
}

std::vector<Position::MoveResult> Queen::moveset() {
	std::vector<Position::MoveResult> results;
	for (int i = 1; i < 8; i++) {
		results.push_back(pos.moveTruncate(i, 0, true));
		results.push_back(pos.moveTruncate(-i, 0, true));
		results.push_back(pos.moveTruncate(0, i, true));
		results.push_back(pos.moveTruncate(0, -i, true));
		results.push_back(pos.moveTruncate(i, i, true));
		results.push_back(pos.moveTruncate(i, -i, true));
		results.push_back(pos.moveTruncate(-i, i, true));
		results.push_back(pos.moveTruncate(-i, -i, true));
	}
	return results;
}
