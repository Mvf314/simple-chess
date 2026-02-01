#ifndef __CHESS_PIECE_H__
#define __CHESS_PIECE_H__

#include <vector>
#include <array>

#include "position.h"

enum class Color {
	WHITE,
	BLACK,
};

enum class Type {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	KING,
	QUEEN,
};


class Piece {
public:
	Piece(Color color, Type type, Position position) : c(color), t(type), pos(position) {};

	Color c;
	Type t;
	Position pos;

	char getChar();

	std::vector<Position> validMoves(const std::vector<Piece>& pieces);

	int getScore();

	bool operator==(const Piece& other);

private:
	// Finds moves based on the moveset of our piece
	std::vector<Position> possibleMoves(const std::vector<Piece>& pieces);
	// Checks if a move is not blocked, either by a piece or line of sight.
	bool isValidMove(const std::vector<Piece>& pieces, const Position move);
};

#endif
