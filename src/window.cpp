#include <iostream>

#include <ftxui/component/app.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>

#include "window.h"

Window::Window(Game* g) {
	game = g;
	auto app = ftxui::App::Fullscreen();

	screenWidth = ftxui::Terminal::Size().dimx;
	screenHeight = ftxui::Terminal::Size().dimy;

	// TODO maybe pass as arguments? const somewhere.
	boardWidth = 64;
	boardHeight = 36;

	pieceSelected = 0;
	moveSelected = std::vector<int>(16);
}

// Now we rebuild this every move. Could be more efficient but does not really matter.
void Window::updateMenus(Color col) {
	Board* piecesTemp = game->getPieces();
	pieces = std::vector<std::string>();
	moves = std::vector<std::vector<std::string>>();
	actualMoves = std::vector<std::vector<Move>>(); // track possible moves, to return when done.
	pieceMap = std::vector<size_t>();

	// For every move, we will create a board.
	boards = std::vector<std::vector<ftxui::Component>>();

	std::stringstream ss = std::stringstream();
	std::vector<Position> tempMoves;
	int i = 0;
	for (size_t j = 0; j < piecesTemp->size(); j++) {
		if (piecesTemp->at(j)->c == col) {
			ss = std::stringstream();
			ss << piecesTemp->at(j)->getChar();
			ss << " @ ";
			ss << piecesTemp->at(j)->pos.toString();
			
			tempMoves = piecesTemp->at(j)->validMoves(*piecesTemp);

			if (tempMoves.size() > 0) {
				pieces.push_back(ss.str());
				pieceMap.push_back(i);
				i++;

				std::vector<std::string> tempMoveList = std::vector<std::string>();
				std::vector<ftxui::Component> tempBoardList = std::vector<ftxui::Component>();
				std::vector<Move> tempMoveVector = std::vector<Move>();
				tempMoveList.push_back("Back");

				tempBoardList.push_back(getBoard(*piecesTemp, tempMoves));

				for (auto move : tempMoves) {
					tempMoveList.push_back(move.toString());
					tempBoardList.push_back(getBoard(*piecesTemp, tempMoves));
					tempMoveVector.push_back(std::make_pair(piecesTemp->at(j), move));
				}
				moves.push_back(tempMoveList);
				boards.push_back(tempBoardList);
				// Remember: The "first" move is not in here (back).
				// So we need to select actualMoves[pieceSel][moveSel[pieceSel] - 1].
				actualMoves.push_back(tempMoveVector);
			}
		}
	}
	pieceSelected = 0;
	moveSelected = std::vector<int>(pieces.size());
}

ftxui::Component Window::getBoard(Board& b, std::vector<Position> moveList) {
	const std::string board_header =	"  A   B   C   D   E   F   G   H  ";
	const std::string board_row =   	"+---+---+---+---+---+---+---+---+";

	// Board textual representation
	auto board = Game::getBoard(b);

	for (const auto& move : moveList) {
		board[move.rank - 1][move.file - 1] = '.';
	}


	return ftxui::Renderer([board_row, board_header, board, this]{
		std::vector<std::string> boardRows = std::vector<std::string>();
		std::stringstream ss;
		for (int rank = 0; rank < 8; rank++) {
			ss = std::stringstream();
			ss << 8 - rank;
			ss << " | ";
			for (int file = 0; file < 8; file++) {
				ss << board[7 - rank][file] << " | ";
			}
			ss << "  ";
			boardRows.push_back(ss.str());
		}
		return ftxui::vbox({
			ftxui::text(board_header)	| ftxui::center,
			ftxui::text(board_row)		| ftxui::center,
			ftxui::text(boardRows[0])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[1])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[2])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[3])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[4])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[5])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[6])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(boardRows[7])	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
		}) | ftxui::center;
	});

}

void Window::updateBoard() {
	const std::string board_header =	"  A   B   C   D   E   F   G   H  ";
	const std::string board_row =   	"+---+---+---+---+---+---+---+---+";
	const std::string board_other = 	"|   |   |   |   |   |   |   |   |";

	std::vector<Position> p = std::vector<Position>();

	boardComponent = getBoard(*(game->getPieces()), p);
}

void Window::updateComponents() {

	updateMenus(Color::WHITE);
	updateBoard();

	titleComponent = ftxui::Renderer([this] {
		return ftxui::vbox({
			ftxui::text("hello there i am text") 	| ftxui::borderRounded | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, screenHeight - boardHeight),
			// this notation sucks
			boards[pieceSelected][moveSelected[pieceSelected]]->Render()			| ftxui::border | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, boardHeight),
		}) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, boardWidth);
	});

	piecesComponent = ftxui::Menu(&pieces, &pieceSelected) | ftxui::borderLight | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 16);

	std::vector<std::string> moves0 = {
		"Move 0-0",
	};
	std::vector<std::string> moves1 = {
		"Move 1-0",
		"Move 1-1",
		"Move 1-2",
		"Move 1-3",
		"Move 1-5",
	};
	std::vector<std::string> moves2 = {
		"Move 2-0",
		"Move 2-1",
		"Move 2-2",
	};

	ftxui::Components movesList = ftxui::Components();
	for (size_t i = 0; i < pieces.size(); i++) {
		movesList.push_back(ftxui::Menu(&moves[i], &moveSelected[i]));
	}

	movesComponent = ftxui::Container::Tab(movesList, &pieceSelected) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 16);

	std::vector<std::string> history_strs = {
		"Moved p to d4",
		"Moved p to e5",
		"p captured p at e5",
	};
	std::vector<std::string> history_alg = {
		"1.\td4",
		"  \te5",
		"2.\txe5",
	};

	// We capture the vectors by value:
	// The vectors go out of scope at the end of this function,
	// at this moments the references point to unallocated memory.
	historyComponent = ftxui::Renderer([history_strs, history_alg, this] {
		ftxui::Elements history_strs_els = ftxui::Elements();
		ftxui::Elements history_alg_not_els = ftxui::Elements();
		for (const std::string& str : history_strs) {
			history_strs_els.push_back(ftxui::text(str));
		}
		for (const std::string& alg_not : history_alg) {
			history_alg_not_els.push_back(ftxui::text(alg_not));
		}
		return ftxui::vbox({
			ftxui::hbox({
				ftxui::vbox(history_strs_els)		| ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 16),
				ftxui::separator(),
				ftxui::vbox(history_alg_not_els) 	| ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 8),
				ftxui::separator()
			}) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 48),
		});
	});

	controlsComponent = ftxui::Button("quit", [&] {
		shouldQuit = true;
		ftxui::App::Active()->Exit();
		return true;
	}) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, screenWidth - boardWidth - 80);

	layout = ftxui::Container::Horizontal({
		titleComponent,
		piecesComponent,
		movesComponent,
		historyComponent,
		controlsComponent,
	});
	
	content = ftxui::Renderer(layout, [&]{
		return ftxui::hbox({
			titleComponent->Render(),
			piecesComponent->Render(),
			movesComponent->Render(),
			historyComponent->Render(),
			controlsComponent->Render(),
		});
	});

	content |= ftxui::CatchEvent([&](ftxui::Event event) {
		// m for move
		if (event == ftxui::Event::m) {
			// If we have not selected a move
			if (moveSelected[pieceSelected] == 0) {
				return false;
			}
			// Else 
			// Set the result
			std::stringstream ss;
			int moveIdx = moveSelected[pieceSelected];
			result = actualMoves[pieceSelected][moveIdx - 1];
			// exit application
			ftxui::App::Active()->Exit();
			return true;
		}	
		return false;
	});
}

void Window::startApp(ftxui::App app) {
	app.Loop(content);
}
