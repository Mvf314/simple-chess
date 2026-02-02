#ifndef __CHESS_BISHOP_H__
#define __CHESS_BISHOP_H__

#include "piece.h"

class Bishop : public Piece {
public:
	Bishop(Color col, Position position) : Piece(col, position) {};

	char getChar() override;
	int getScore() override;
	Bishop* clone() override;

private:
	std::vector<Position::MoveResult> moveset() override;
};

#endif
