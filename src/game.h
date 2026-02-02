#include <vector>		// vector
#include <functional>	// function
#include <random>		// random_device, mt19937
#include <memory>		// shared_ptr

#include "piece/piece.h"

typedef std::vector<std::shared_ptr<Piece>> Board;
typedef std::array<std::array<char, 8>, 8> BoardText;
typedef std::pair<std::shared_ptr<Piece>, Position> Move;

enum class State {
	WHITE_TURN,
	BLACK_TURN,
	WHITE_CHECK,
	BLACK_CHECK,
	WHITE_WIN,
	BLACK_WIN,
	DRAW
};

class Game {
public:
	Game();

	void run();

	void printBoard(const std::vector<Position>& moves);
	void printBoard();

	void updateState();

	void setEvaluators(std::function<Move (const Board&)> white, std::function<Move (const Board&)> black);

	static bool isCheck(std::vector<Piece>& pcs, Color c);
	
	static Move standardEvaluator(const Board& pcs);
	static Move userInput(const Board& pcs);

private:
	Board pieces;
	Board killed;

	BoardText getBoard();

	std::function<Move (const Board&)> whiteEvaluator;
	std::function<Move (const Board&)> blackEvaluator;

	
	State state;


	// Random 
	std::random_device rd;
	std::mt19937 gen;
};
