#ifndef SUDOKU_H

void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);
bool is_complete(char board[9][9]);
bool make_move(const char* position, char digit, char board[9][9]);
bool save_board(const char* name, const char board[9][9]);
bool solve_board(char board[9][9]);
bool check_copy_is_valid(char board[9][9], char board2[9][9]);
#define SUDOKU_H
#endif

