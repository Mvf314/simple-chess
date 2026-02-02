#ifndef __CHESS_ROOK_H__
#define __CHESS_ROOK_H__

#include "piece.h"

class Rook : public Piece {
public:
	Rook(Color col, Position position) : Piece(col, position) {};

	char getChar() override;

	int getScore() override;
	Rook* clone() override;
private:
	std::vector<Position::MoveResult> moveset() override;


	
};

#endif
