#include <vector>
#include <functional>
#include <random>
#include <memory>

#include "piece.h"

typedef std::vector<std::shared_ptr<Piece>> Board;
typedef std::array<std::array<char, 8>, 8> BoardText;
typedef std::pair<std::shared_ptr<Piece>, Position> Move;

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

	void printBoard(const std::vector<Position>& moves);
	void printBoard();

	void setEvaluators(std::function<Move (const Board&)> white, std::function<Move (const Board&)> black);

	static bool isCheck(std::vector<Piece>& pcs, Color c);

private:
	Board pieces;
	Board killed;

	BoardText getBoard();

	std::function<Move (const Board&)> whiteEvaluator;
	std::function<Move (const Board&)> blackEvaluator;

	static Move standardEvaluator(const Board& pcs);
	State state;


	// Random 
	std::random_device rd;
	std::mt19937 gen;
};
