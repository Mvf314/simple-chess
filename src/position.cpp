#include "position.h"

#include <iostream>
#include <stdexcept>

Position::Position(const char f, const char r) {
	if (f >= 'A' && f <= 'H') {
		file = f - 'A' + 1;
	} else if (f >= 'a' && f <= 'h') {
		file = f - 'a' + 1;
	} else if (f >= '1' && f <= '8') {
		file = f - '0';
	} else {
		std::string err = "Position::Position() : File needs to be either in [A, H], [a,h] or [1, 8], got ";
		err += f;
		err += "!";
		// Maybe add logic to distinguish range errors and malformed input.
		throw std::invalid_argument(err);
	}

	if (r >= '1' && r <= '8') {
		rank = r - '0';
	} else {
		std::string err = "Position::Position() : Rank needs to be in [1,8], got ";
		err += r;
		err += "!";
		throw std::invalid_argument(err);
	}
}

Position::Position(const int f, const int r) {
	if (f >= 1 && f <= 8) {
		file = f;
	} else {
		std::string err = "Position::Position(int, int) : File needs to be in [1, 8], got ";
		err += std::to_string(f);
		err += "!";
		throw std::out_of_range(err);
	}

	if (r >= 1 && r <= 8) {
		rank = r;
	} else {
		std::string err = "Position::Position(int, int) : Rank needs to be in [1,8], got ";
		err += std::to_string(r);
		err += "!";
		throw std::out_of_range(err);
	}
}

std::string Position::toString() {
	char fileChar = file + 'A' - 1;
	char rankChar = rank + '0';
	std::string s {fileChar, rankChar};
	return s;
}

Position Position::move(const int up, const int right, const bool dir) {
	if (dir) {
		return Position(file + right, rank + up);
	} else {
		return Position(file - right, rank - up);
	}
}

std::pair<bool, Position> Position::moveTruncate(const int up, const int right, const bool dir) noexcept {
	bool truncated = false;
	int newFile = dir ? file + right : file - right;
	int newRank = dir ? rank + up : rank - up;
	if (newFile < 1) {
		newFile = 1;
		truncated = true;
	} else if (newFile > 8) {
		newFile = 8;
		truncated = true;
	}

	if (newRank < 1) {
		newRank = 1;
		truncated = true;
	} else if (newRank > 8) {
		newRank = 8;
		truncated = true;
	}
	return std::make_pair(truncated, Position(newFile, newRank));
}

bool Position::operator==(const Position& other) {
	return (file == other.file) && (rank == other.rank);
}
