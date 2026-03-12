#ifndef __CHESS_GAME_H__
#define __CHESS_GAME_H__

#include <vector>		// vector
#include <functional>	// function
#include <memory>		// shared_ptr

#include "piece/piece.h"
#include "random.h"

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

	static void printBoard(const Board& pcs, const Board& killed, const std::vector<Position>& moves);
	static void printBoard(const Board& pcs, const Board& killed);

	void updateState();

	void executeMove(Move move);

	void setEvaluators(std::function<Move (const Board&, const Board&, Random&)> white, std::function<Move (const Board&, const Board&, Random&)> black);

	static bool isCheck(std::vector<Piece>& pcs, Color c);
	
	static Move standardEvaluator(const Board& pcs, const Board& killed, Random& rand, Color c);
	static Move standardEvaluatorBlack(const Board& pcs, const Board& killed, Random& rand);
	static Move userInput(const Board& pcs, const Board& killed, Random& rand);

	static int getScore(const Board& pcs, Color c);

private:
	Board pieces;
	Board killed;

	BoardText getBoard();
	static BoardText getBoard(const Board& pcs);

	std::function<Move (const Board&, const Board&, Random&)> whiteEvaluator;
	std::function<Move (const Board&, const Board&, Random&)> blackEvaluator;

	
	State state;
};

#endif
