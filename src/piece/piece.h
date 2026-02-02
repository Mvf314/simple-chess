#ifndef __CHESS_PIECE_H__
#define __CHESS_PIECE_H__

#include <vector>
#include <array>
#include <memory>

#include "../position.h"

enum class Color {
	WHITE,
	BLACK,
};


class Piece {
public:
	Piece(Color col, Position position) : c(col), pos(position) {};

	Color c;
	Position pos;

	virtual char getChar() = 0;

	virtual std::vector<Position> validMoves(const std::vector<std::shared_ptr<Piece>>& pieces);
	virtual std::vector<Position> lineOfSight(const std::vector<std::shared_ptr<Piece>>& pieces);

	virtual int getScore() = 0;
	// TODO implementations have new. Maybe add virtual destructor?
	virtual Piece* clone() = 0;
	virtual std::vector<std::shared_ptr<Piece>> simulateMove(const std::vector<std::shared_ptr<Piece>>& pieces, std::shared_ptr<Piece>& piece, Position move);
	static bool inCheck(const std::vector<std::shared_ptr<Piece>>& pieces, Color col);
private:
	virtual std::vector<Position::MoveResult> moveset() = 0;


};

#endif
