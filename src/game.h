#include <vector>
#include <functional>
#include <random>

#include "piece.h"

enum class State {
	GAME,
	WHITE_CHECK,
	BLACK_CHECK,
	WHITE_WIN,
	BLACK_WIN
};

class Game {
public:
	Game();

	void run();

	// Check board state
	bool isValid();

	void printBoard(const std::vector<Position>& moves);
	void printBoard();

	void setEvaluator(std::function<float (const std::vector<Piece>&)> fun);

	static bool isCheck(std::vector<Piece>& pcs, Color c);

private:
	// Because we will remove elements of this vector, use unique_ptr
	std::vector<Piece> pieces;
	std::vector<Piece> killed;

	std::array<std::array<char, 8>, 8> getBoard();

	// Get piece with allowed moves
	std::pair<Piece, std::vector<Position>> getPiece();
	Position getMove(std::vector<Position>& allowedMoves);

	void executeMove(Piece p, Position move);
	std::vector<Piece> simulateMove(Piece p, Position move);

	// return true to break out of loop.
	bool checkBoard();

	std::pair<Piece, Position> doTurn(Color c);

	std::function<float (const std::vector<Piece>&)> evaluator;

	static float standardEvaluator(const std::vector<Piece>& pcs);
	State state;


	// Random 
	std::random_device rd;
	std::mt19937 gen;
};
