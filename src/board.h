#ifndef __CHESS_BOARD_H__
#define __CHESS_BOARD_H__

#include <vector>

#include "piece.h"

class Board {
private:
	std::vector<Piece> pieces;
	std::array<std::array<char, 8>, 8> board;

public:
	Board();

	void test();

	void print();

	char getPiece(int file, int rank);
	bool isEmpty(int file, int rank);

	bool isWhite(int file, int rank);
	bool isBlack(int file, int rank);

	// Returns true if the square is occupied by a black piece, or free.
	bool isNotWhite(int file, int rank);
	bool isNotBlack(int file, int rank);
};

#endif
