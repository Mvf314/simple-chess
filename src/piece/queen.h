#ifndef __CHESS_QUEEN_H__
#define __CHESS_QUEEN_H__

#include "piece.h"

class Queen : public Piece {
public:
	Queen(Color col, Position position) : Piece(col, position) {};

	char getChar() override;
	int getScore() override;
	Queen* clone() override;

private:
	std::vector<Position::MoveResult> moveset() override;
};

#endif
