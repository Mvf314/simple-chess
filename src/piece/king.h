#ifndef __CHESS_KING_H__
#define __CHESS_KING_H__

#include "piece.h"

class King : public Piece {
public:
	King(Color col, Position position) : Piece(col, position), moved(false) {};

	std::vector<Position> validMoves(const std::vector<std::shared_ptr<Piece>>& pieces) override;
	char getChar() override;
	int getScore() override;
	King* clone() override;
	void setMoved();
private:
	bool moved;
	std::vector<Position::MoveResult> moveset() override;
};

#endif
