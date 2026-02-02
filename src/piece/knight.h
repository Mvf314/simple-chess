#ifndef __CHESS_KNIGHT_H__
#define __CHESS_KNIGHT_H__

#include "piece.h"

class Knight : public Piece {
public:
	Knight(Color col, Position position) : Piece(col, position) {};

	char getChar() override;

	// Knight has no line of sight check.
	std::vector<Position> lineOfSight(const std::vector<std::shared_ptr<Piece>>& pieces);

	int getScore() override;
	Knight* clone() override;

private:
	std::vector<Position::MoveResult> moveset() override;
};

#endif
