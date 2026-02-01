#include "board.h"

#include <array>
#include <iostream>
#include <sstream>

Board::Board() {
	pieces = std::vector<Piece>();

	// Place pawns
	for (int i = 0; i < 8; i++) {
		// White pawns are rank 2, black pawns on rank 7.
		Piece p1(Color::WHITE, Type::PAWN, Position(i + 'A', '2'));
		Piece p2(Color::BLACK, Type::PAWN, Position(i + 'A', '7'));
		pieces.push_back(p1);
		pieces.push_back(p2);
	}

	// Place other pieces
	for (int j = 0; j < 2; j++) {
		Piece wRook(Color::WHITE, Type::ROOK, Position('A' + (7 * j), '1'));
		Piece bRook(Color::BLACK, Type::ROOK, Position('A' + (7 * j), '8'));
		Piece wKnight(Color::WHITE, Type::KNIGHT, Position('B' + (5 * j), '1'));
		Piece bKnight(Color::BLACK, Type::KNIGHT, Position('B' + (5 * j), '8'));
		Piece wBishop(Color::WHITE, Type::BISHOP, Position('C' + (3 * j), '1'));
		Piece bBishop(Color::BLACK, Type::BISHOP, Position('C' + (3 * j), '8'));

		pieces.push_back(wRook);
		pieces.push_back(bRook);
		pieces.push_back(wKnight);
		pieces.push_back(bKnight);
		pieces.push_back(wBishop);
		pieces.push_back(bBishop);
	}

	// Place kings and queens
	Piece wKing(Color::WHITE, Type::KING, Position('E', '1'));
	Piece bKing(Color::BLACK, Type::KING, Position('E', '8'));
	Piece wQueen(Color::WHITE, Type::QUEEN, Position('D', '1'));
	Piece bQueen(Color::BLACK, Type::QUEEN, Position('D', '8'));

	pieces.push_back(wKing);
	pieces.push_back(bKing);
	pieces.push_back(wQueen);
	pieces.push_back(bQueen);

	board = std::array<std::array<char, 8>, 8>();

	for (size_t i = 0; i < board.size(); i++) {
		board[i].fill(' ');
	}

	for (Piece piece : pieces) {
		// Decrement by 1 because we store rank-file as a digit 1-8.
		board[piece.pos.rank - 1][piece.pos.file - 1] = piece.getChar();
	}

}

void Board::test() {
	for (Piece piece : pieces) {
		std::vector<Position> moves = piece.validMoves(board);
		std::cout << "Piece " << piece.getChar() << " at " << piece.pos.toString() << " has " << moves.size() << " valid moves.\n";
	}	
}

void Board::print() {

	std::stringstream ss = std::stringstream();
	ss << "  ABCDEFGH\n";
	// Loop in reverse, because rank 1 is the rank closes to us and therefore printed last.
	for (size_t rank = board.size() - 1; rank < 8; rank--) {
		ss << rank + 1 << " ";
		for (char square : board[rank]) {
			ss << square;
		}
		ss << '\n';
	}

	std::cout << ss.str();
}

char Board::getPiece(int file, int rank) {
	if (file < 1 || file > 8) {
		std::string err = "Board::getPiece() : File needs to be in [1,8], got ";
		err += file;
		err += "!";
		throw std::invalid_argument(err);
	}
	if (rank < 1 || rank > 8) {
		std::string err = "Board::getPiece() : Rank needs to be in [1,8], got ";
		err += file;
		err += "!";
		throw std::invalid_argument(err);
	}
	return board[rank - 1][file - 1];
}

bool Board::isEmpty(int file, int rank) {
	return getPiece(file, rank) == ' ';
}

bool Board::isWhite(int file, int rank) {
	return std::isupper(getPiece(file, rank));
}

bool Board::isBlack(int file, int rank) {
	return std::islower(getPiece(file, rank));
}

bool Board::isNotWhite(int file, int rank) {
	char c = getPiece(file, rank);
	return std::islower(c) || (c == ' ');
}

bool Board::isNotBlack(int file, int rank) {
	char c = getPiece(file, rank);
	return std::isupper(c) || (c == ' ');
}
