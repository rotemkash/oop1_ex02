#include "GameRunner.h"

#include <conio.h>

#include <cstdlib>  // for std::system()
#include <iostream>

/* Constructor of GameRunner*/
GameRunner::GameRunner() : _hp(INIT_HP), _score(INIT_SCORE), _level(INIT_LVL) {
    // ifstream game_file;
    open_file(_game_file);
}

/* This fucntion runs the game when called*/
void GameRunner::Run() {
    int move_score = INIT_SCORE;  // hold the score for the current move made

    // loop while pacman hasn't died or the last level
    while (_hp > 0 && _level <= LVL_COUNT) {
        Board board(_game_file);
        printGame(board);

        // loop for the current level, until all cookies have been eaten
        //  or pacman died
        while (board.cookieCount() > 0 && _hp > 0) {
            // get user input and return the score for that move
            move_score = handleSpecialKey(board);
            switch (move_score) {
            case EATEN:  // eaten by ghost
                _hp--;
                board.resetBoard();
                continue;
                break;
            case ILLEGAL_MV:  // illegal move, try again!
                continue;
                break;
            case NORMAL_MV:  // legal move with no points
                break;
            case ATE_COOKIE:  // ate cookie
            case ATE_GIFT:    // ate gift
            case ATE_KEY:     // took a key
                // the the score of this move to the total score
                _score += move_score;
                break;
            }

            // move the ghosts and check if they ate the pacman
            if (board.moveDemons()) {
                _hp--;
                board.resetBoard();
            }

            printGame(board);
        }
        // add the bonus at the end of the level
        _score += LVL_BONUS + board.demonCount() * DEMON_BONUS;
        printGame(board);

        _level++;
    }
    _game_file.close();
}

/* Print the board and the game info*/
void GameRunner::printGame(Board board) {
    system("cls");
    board.printBoard();
    cout << "HP: " << _hp << " score: " << _score << " level: " << _level << endl;
}

/* handle the input from the user*/
int GameRunner::handleSpecialKey(Board& board) {
    auto c = _getch();
    switch (c) {
    case KB_Up:
        return board.movePacUp();
    case KB_Down:
        return board.movePacDown();
    case KB_Left:
        return board.movePacLeft();
    case KB_Right:
        return board.movePacRight();
    case KB_Escape:
        return EXIT;  // exit game
    default:
        return ILLEGAL_KB;  // do nothing
    }
}

/* open the file for reading*/
void GameRunner::open_file(ifstream& file) {
    file.open("board.txt");
    if (!file.is_open()) {
        cerr << "cannot open file";
        exit(EXIT_FAILURE);
    }
}