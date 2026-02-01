#include "piece.h"

#include <stdexcept>

char Piece::getChar() {
	char tempChar = ' ';
	switch (t) {
		case Type::PAWN:
			tempChar = 'P';
			break;
		case Type::ROOK:
			tempChar = 'R';
			break;
		case Type::KNIGHT:
			tempChar = 'N';
			break;
		case Type::BISHOP:
			tempChar = 'B';
			break;
		case Type::QUEEN:
			tempChar = 'Q';
			break;
		case Type::KING:
			tempChar = 'K';
			break;
		default:
			tempChar = '?';
			break;
	}
	if (c == Color::WHITE) {
		return toupper(tempChar);
	} else {
		return tolower(tempChar);
	}
	return tempChar;
}

std::vector<Position> Piece::possibleMoves(const std::vector<Piece>& pieces) {
	// First check if this piece is actually on the board.
	bool foundPiece = false;
	for (Piece piece : pieces) {
		if (piece.c == c && piece.t == t && piece.pos == pos) {
			foundPiece = true;
			break;
		}
	}
	if (!foundPiece) {
		std::string err = "Piece::validMoves() : Board does not contain piece ";
		err += getChar();
		err += " on position ";
		err += pos.toString();
		err += "!";
		throw std::invalid_argument(err);
	}

	std::vector<Position> moves = std::vector<Position>();

	// Define what way white moves
	const bool up = true; // true means white moves "up" in rank (white starts at rank 1 & 2)

	bool dir = (c == Color::WHITE) ? up : !up;

	// The workflow is the same: First determine all moves which are allowed by the moveset rules. If this results in a truncated move (move off the board), do not include it. So we collect all the data in results, and filter at the end.
	std::vector<std::pair<bool, Position>> results = std::vector<std::pair<bool, Position>>();
	switch (t) {
		case Type::PAWN: 
			{
			results.push_back(pos.moveTruncate(1, 0, dir));
			// Add capture moves. These are checked in isValidMove()
			results.push_back(pos.moveTruncate(1, 1, dir));
			results.push_back(pos.moveTruncate(1, -1, dir));
			// First move can be double move.
			switch (c) {
				case Color::WHITE:
					if (pos.rank == 2) {
						results.push_back(pos.moveTruncate(2, 0, dir));
					}
					break;
				case Color::BLACK:
					if (pos.rank == 7) {
						results.push_back(pos.moveTruncate(2, 0, dir));
					}
					break;
			}
			break;
			}
		case Type::ROOK:
			{
			// We can at most move 7 spaces in any direction
			for (int i = 1; i < 8; i++) {
				results.push_back(pos.moveTruncate(i,  0, dir));
				results.push_back(pos.moveTruncate(-i, 0, dir));
				results.push_back(pos.moveTruncate(0,  i, dir));
				results.push_back(pos.moveTruncate(0, -i, dir));
			}
			break;
			}
		case Type::KNIGHT:
			{
			results.push_back(pos.moveTruncate(2,  -1, dir));
			results.push_back(pos.moveTruncate(2,   1, dir));
			results.push_back(pos.moveTruncate(1,   2, dir));
			results.push_back(pos.moveTruncate(-1,  2, dir));
			results.push_back(pos.moveTruncate(-2,  1, dir));
			results.push_back(pos.moveTruncate(-2, -1, dir));
			results.push_back(pos.moveTruncate(-1, -2, dir));
			results.push_back(pos.moveTruncate(1,  -2, dir));
			break;
			}
		case Type::BISHOP:
			{
			// Similarly to the rook, we can move at maximum 7 spaces.
			for (int i = 1; i < 8; i++) {
				results.push_back(pos.moveTruncate(i,   i, dir));
				results.push_back(pos.moveTruncate(i,  -i, dir));
				results.push_back(pos.moveTruncate(-i,  i, dir));
				results.push_back(pos.moveTruncate(-i, -i, dir));
			}
			break;
			}
		case Type::KING:
			{
			results.push_back(pos.moveTruncate(1,   0, dir));
			results.push_back(pos.moveTruncate(1,   1, dir));
			results.push_back(pos.moveTruncate(0,   1, dir));
			results.push_back(pos.moveTruncate(-1,  1, dir));
			results.push_back(pos.moveTruncate(-1,  0, dir));
			results.push_back(pos.moveTruncate(-1, -1, dir));
			results.push_back(pos.moveTruncate(0,  -1, dir));
			results.push_back(pos.moveTruncate(1,  -1, dir));
			break;
			}
		case Type::QUEEN:
			{
				// Rook + Bishop 
			for (int i = 1; i < 8; i++) {
				// Cardinal 
				results.push_back(pos.moveTruncate(i,  0, dir));
				results.push_back(pos.moveTruncate(-i, 0, dir));
				results.push_back(pos.moveTruncate(0,  i, dir));
				results.push_back(pos.moveTruncate(0, -i, dir));
				// Diagonal
				results.push_back(pos.moveTruncate(i,   i, dir));
				results.push_back(pos.moveTruncate(i,  -i, dir));
				results.push_back(pos.moveTruncate(-i,  i, dir));
				results.push_back(pos.moveTruncate(-i, -i, dir));
			}
			break;
			}
		default:
			break;
	}
	// TODO: Implement castling.

	for (auto result : results) {
		if (!result.first) {
			// Move was not truncated
			moves.push_back(result.second);
		}
	}


	return moves;
}

std::vector<Position> Piece::validMoves(const std::vector<Piece>& pieces) {
	auto moves = possibleMoves(pieces);
	std::vector<Position> validMoves = std::vector<Position>();
	for (auto move : moves) {
		if (isValidMove(pieces, move)) {
			validMoves.push_back(move);
		}
	}
	return validMoves;
}

// Standard score
int Piece::getScore() {
	switch (t) {
		case Type::PAWN:
			return 1;
		case Type::ROOK:
			return 5;
		case Type::KNIGHT:
			return 3;
		case Type::BISHOP:
			return 3;
		case Type::KING:
			return 100;
		case Type::QUEEN:
			return 9;
	}
	return 0;
}

bool Piece::operator==(const Piece& other) {
	return (c == other.c) && (t == other.t) && (pos == other.pos);
}

bool Piece::isValidMove(const std::vector<Piece>& pieces, const Position move) {
	// This is a fall-through function, keep checking and if it gets to the end it must be valid.
	for (Piece p : pieces) {
		if ((p.pos == move) && (p.c == c)) {
			// We can't move onto on of our own pieces
			return false;
		}
	}

	// We have to have line of sight, except when we are a horse
	if (t != Type::KNIGHT) {
		// Now the move is either cardinal or diagonal. Find distance first.
		int distance = std::max(std::abs(move.file - pos.file), std::abs(move.rank - pos.rank));
		int stepFile = (move.file - pos.file) / distance;
		int stepRank = (move.rank - pos.rank) / distance;

		// Check squares *between* piece and target, these should be all empty for there to be line of sight.
		for (int i = 1; i < distance; i++) {
			for (Piece p : pieces) {
				// TODO this if statement is bad.
				if (p.pos == Position(pos.move(stepRank * i, stepFile * i, true))) {
					// Found a piece, so move is not valid.
					return false;
				}
			}
		}
	}

	if (t == Type::PAWN) {
		// Capture logic for pawn is different.
		// TODO en passant capture. For this we need to know the previous move...
		bool encounter = false;
		for (Piece p : pieces) {
			if ((p.pos == move) && (p.c != c)) {
				encounter = true;
				// Enemy encountered (NB: The second check is superfluous)
				// In this case the move is illegal if it is a cardinal move.
				if (move.file == pos.file) {
					return false;
				}
			}
		}
		if (!encounter) {
			// No enemy encountered, in which case the move is illegal if it is diagonal.
			if (move.file != pos.file) {
				return false;
			}
		}
	}

	// TODO we can't put ourselves in check
	return true;
}
