#ifndef __CHESS_PAWN_H__
#define __CHESS_PAWN_H__

#include "piece.h"

class Pawn : public Piece {
public:
	Pawn(Color col, Position position) : Piece(col, position), vulnerable(false) {};

	char getChar() override;

	// Diagonal capturing & en-passant rules. Standard line of sight rules apply, so override validMoves.
	std::vector<Position> validMoves(const std::vector<std::shared_ptr<Piece>>& pieces) override;

	int getScore() override;
	Pawn* clone() override;
	// Vulnerable to en passant capture
	void setVulnerable();
	void setNotVulnerable();
	bool isVulnerable();
private:
	bool vulnerable;
	std::vector<Position::MoveResult> moveset() override;
};

#endif
