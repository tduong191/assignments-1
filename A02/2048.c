#include <stdio.h>

int max(int num1, int num2) {
    return (num1 > num2 ) ? num1 : num2;
}

int main() {
  int board[4][4];
  int max_val = 0;
  // read text file into board
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      scanf("%d", &board[i][j]);
      max_val = max(max_val, board[i][j]);
    }
  }

  // loop horizontally
  int row = 0;
  int temp = 0;
  int max_val_shift = 0;
  while (row < 4) {
    int col = 0;
    while (col < 4) {
      if (board[row][col] != 0) { 
        temp = board[row][col]; // if temp is not 0 then find the check == temp
        int check_col = col + 1;
        while (check_col < 4) {
          if (board[row][check_col] == temp) {
            max_val_shift = max(max_val_shift, temp * 2);
            check_col = 4;
          } else if (board[row][check_col] != 0) {
            check_col = 4;
          } else {
            check_col = check_col + 1;
          }
        }
      }
      col ++;
    }
    row ++;
  }

  // loop vertically
  int col = 0;
  row = 0;
  while (col < 4) {
    int row = 0;
    while (row < 4) {
      if (board[row][col] != 0) { 
        temp = board[row][col]; // if temp is not 0 then find the check == temp
        int check_row = row + 1;
        while (check_row < 4) {
          if (board[check_row][col] == temp) {
            max_val_shift = max(max_val_shift, temp * 2);
            check_row = 4;
          } else if (board[check_row][col] != 0) {
            check_row = 4;
          } else {
            check_row = check_row + 1;
          }
        }
      }
      row ++;
    }
    col ++;
  }

  // if there is no shift, return the max value in th board
  if (max_val_shift != 0) {
    printf("The largest value is %d.\n", max_val_shift);
  } else {
    printf("The largest value is %d.\n", max_val);
  }
  return 0;
}

