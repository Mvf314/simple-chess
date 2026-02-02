#ifndef __CHESS_KING_H__
#define __CHESS_KING_H__

#include "piece.h"

class King : public Piece {
public:
	King(Color col, Position position) : Piece(col, position) {};

	char getChar() override;
	int getScore() override;
	King* clone() override;
private:
	std::vector<Position::MoveResult> moveset() override;
};

#endif
