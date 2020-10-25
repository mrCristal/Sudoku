#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sudoku.h"

using namespace std;


void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << '\n';
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
}


void print_frame(int row) {
  if (!(row % 3))
    cout << "  +===========+===========+===========+" << '\n';
  else
    cout << "  +---+---+---+---+---+---+---+---+---+" << '\n';
}


void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|" << '\n';
}


void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r) << "   ";
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}


/* function to check if all cells of the board are filled with a valid
 * integer */
bool is_complete(char board[9][9])
{
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			if ((static_cast<int>(board[r][c]) < 49) || (static_cast<int>(board[r][c]) > 57))
			{
				return false;
			}
		}
	}
	return true;
}

/* function to map given position into 0-8*/
/* can take either characters A to I or 1 to 9
 * and returns the corresponding int in the 0-8 space */
int map_index(char character)
{
	if (48 < static_cast<int>(character) && static_cast<int>(character) < 57)
	{
		return static_cast<int>(character) - 49;
	}
	else if (64 < static_cast<int>(character) && static_cast<int>(character) < 74)
	{
		return static_cast<int>(character) - 65;
	}
	else
	{
		cout << "Entered coordinate is invalid.\n";
		cout << "Please enter a letter between A and I for rows";
		cout << " or a number between 1 and 9 for columns";
		exit(1);
	}
}

/* function to check if proposed move is valid on the row */
bool is_valid_on_row(int row, char digit, char board[9][9])
{
	for (int col = 0; col < 9; ++col)
	{
		if (board[row][col] == digit)
			return false;
	}

	return true;
}

/* function to check if proposed move is valid on the column */
bool is_valid_on_col(int col, char digit, char board[9][9])
{
	for (int row = 0; row < 9; ++row)
	{
		if (board[row][col] == digit)
			return false;
	}
	return true;
}

/* function to set the indexes of the local 3x3 square */
/* the indexes to be set are determined by the result of the division by 3 
 * of the index of the cell in question */
void set_local_square_index(int index_set[3], int current_index)
{
	if (static_cast<float>(current_index) / 3 < 1)
	{
		for (int index = 0; index < 3; index++)
		{
			index_set[index] = index;
		}
	}
	else if (static_cast<float>(current_index) / 3 >= 2)
	{
		for (int index = 0; index < 3; index++)
		{
			index_set[index] = index + 6;
		}
	}
	else
	{
		for (int index = 0; index < 3; index++)
		{
			index_set[index] = index + 3;
		}
	}
}


/* function to check if proposed move is valid on the local 3x3 square*/
bool is_valid_on_local_square(int row, int col, char digit, char board[9][9])
{
	int row_set[3], col_set[3];
	int r, c;
	set_local_square_index(row_set, row);
	set_local_square_index(col_set, col);
	
	for (r = row_set[0]; r <= row_set[2]; ++r)
	{
		for (c = col_set[0]; c <= col_set[2]; ++c)
		{
			if (board[r][c] == digit)
			{
				return false;
			}
		}
	}

	return true;
}

/* function to check if proposed digit can be inserted at proposed location*/
/* this function involves three others to check if digit is valid on row, col and 3x3 square respectively */
bool is_valid_at_location(int row, int col, char digit, char board[9][9])
{
	if (is_valid_on_row(row, digit, board) && is_valid_on_col(col, digit, board) && is_valid_on_local_square(row, col, digit, board))
		return true;
	return false;
}

/* functioon to place digits onto the board*/
bool make_move(const char* position, char digit, char board[9][9])
{
	const int row = map_index(position[0]);
	const int col = map_index(position[1]);

	if (is_valid_at_location(row, col, digit, board))
	{
		board[row][col] = digit;
		return true;
	}
	return false;

}

/* function to save board state to a file in the local directory */
bool save_board(const char* name, const char board[9][9])
{
	ofstream outStream;

	outStream.open(name);

	while (!outStream.fail())
	{
		for (int row = 0; row < 9; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				outStream.put(board[row][col]);
			}
			outStream.put('\n'); 
		}

		outStream.close();

		return true;
	}

	return false;
}

/* function to solve the board by recursively trying all legal numbers in each cell */
/* until either a solution is foung or all possible combinations are exhausted */
bool solve_board(char board[9][9])
{
	static int count;
	if (not is_complete(board))
	{
		for (int row = 0; row < 9; row++) 
		{
			for (int col = 0; col < 9; col++) 
			{
				
				if (board[row][col] != '.')
					continue; 

				char possible_nums[9] = { '1','2','3','4','5','6','7','8','9' };

				int index = 0;

				while (index<9) 
				{
					if (is_valid_at_location(row, col, possible_nums[index], board)) // check if valid to put
					{	
						board[row][col] = possible_nums[index]; // set a number at the location
						
						if (!solve_board(board)) // if that does not solve the board
						{
							board[row][col] = '.'; // reset to empty and try the next number
						}
						else return true;
					}
					index++;
					count++;
				}
				return false;
			}
		}
	}
	cout << count<<endl;
	return true;
}
/* function to check that the saved copy and the original are the same */
/* and that no mistake was made */
bool check_copy_is_valid(char board[9][9], char board2[9][9])
{
	
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (board[row][col] != board2[row][col])
			{
				cout << row <<" "<< col<<endl; // prints the 1st case where there is mismatch
				return false;
			}
		}
	}
	return true;
}
