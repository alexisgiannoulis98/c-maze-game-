#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

const int ROWS = 10;
const int MAZE_COLS = 13;

struct Player {
    int row;
    int col;
};

struct Stone {
    int row;
    int col;
};

bool checkWin(const Player& player1, const Player& player2, const Stone& stone) {
    if (player1.row == stone.row && player1.col == stone.col) {
        return true;
    }
    if (player2.row == stone.row && player2.col == stone.col) {
        return true;
    }
    return false;
}

bool isValidMove(int row, int col, const char maze[ROWS][MAZE_COLS]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= MAZE_COLS || maze[row][col] == '*') {
        return false;
    }
    return true;
}

Player createPlayer(const char maze[ROWS][MAZE_COLS]) {
    Player player;
    do {
        player.row = rand() % ROWS;
        player.col = rand() % MAZE_COLS;
    } while (!isValidMove(player.row, player.col, maze));
    return player;
}

int main() {
    srand(time(nullptr));

    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    std::ifstream mazeFile("maze.txt");
    if (!mazeFile) {
        endwin();
        std::cerr << "Failed to open maze.txt" << std::endl;
        return 1;
    }

    char maze[ROWS][MAZE_COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < MAZE_COLS; j++) {
            mazeFile >> maze[i][j];
        }
    }

    Player playerL = createPlayer(maze);
    Player playerM = createPlayer(maze);

    Stone stone;
    do {
        stone.row = rand() % ROWS;
        stone.col = rand() % MAZE_COLS;
    } while (maze[stone.row][stone.col] == '*');

    bool gameOver = false;
    while (!gameOver) {
        clear();
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < MAZE_COLS; j++) {
                if (i == playerL.row && j == playerL.col) {
                    if (maze[i][j] != '*') {
                        attron(COLOR_PAIR(2));
                        addch('L');
                        attroff(COLOR_PAIR(2));
                    } else {
                        addch(maze[i][j]);
                    }
                } else if (i == playerM.row && j == playerM.col) {
                    if (maze[i][j] != '*') {
                        attron(COLOR_PAIR(1));
                        addch('M');
                        attroff(COLOR_PAIR(1));
                    } else {
                        addch(maze[i][j]);
                    }
                } else if (i == stone.row && j == stone.col) {
                    if (maze[i][j] != '*') {
                        attron(COLOR_PAIR(3));
                        addch('O');
                        attroff(COLOR_PAIR(3));
                    } else {
                        addch(maze[i][j]);
                    }
                } else {
                    addch(maze[i][j]);
                }
            }
            addch('\n');
        }
        if (playerL.row == stone.row && playerL.col == stone.col) {
            gameOver = true;
            mvprintw(ROWS + 1, 0, "Player L wins!");
        } else if (playerM.row == stone.row && playerM.col == stone.col) {
            gameOver = true;
            mvprintw(ROWS + 1, 0, "Player M wins!");
        }



        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                if (isValidMove(playerL.row - 1, playerL.col, maze)) {
                    playerL.row--;
                }
                break;
            case KEY_DOWN:
                if (isValidMove(playerL.row + 1, playerL.col, maze)) {
                    playerL.row++;
                }
                break;
            case KEY_LEFT:
                if (isValidMove(playerL.row, playerL.col - 1, maze)) {
                    playerL.col--;
                }
                break;
            case KEY_RIGHT:
                if (isValidMove(playerL.row, playerL.col + 1, maze)) {
                    playerL.col++;
                }
                break;
            case 27: // Escape key
                gameOver = true;
                break;
        }

        // Move player M randomly
        int randomDirection = rand() % 4;
        int mRow = playerM.row;
        int mCol = playerM.col;
        switch (randomDirection) {
            case 0:
                mRow--;
                break;
            case 1:
                mRow++;
                break;
            case 2:
                mCol--;
                break;
            case 3:
                mCol++;
                break;
        }
        if (isValidMove(mRow, mCol, maze)) {
            playerM.row = mRow;
            playerM.col = mCol;
        }

    }

    endwin();

    return 0;
}
