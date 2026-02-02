#ifndef __CHESS_POSITION_H
#define __CHESS_POSITION_H

#include <string>
#include <utility>

class Position {
public:

	typedef std::pair<bool, Position> MoveResult;

	// These are in the range [1,8].
	int file, rank;
	
	Position(const char f, const char r);
	Position(const int f, const int r);

	std::string toString();

	Position move(const int up, const int right, const bool dir);
	// Returns true if position was truncated.
	Position::MoveResult moveTruncate(const int up, const int right, const bool dir) noexcept;

	bool operator==(const Position& other);
};

#endif
