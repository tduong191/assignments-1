# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

void update_bomb_count(int* i, int* j, int* m, int* n, char** board) {
  // update the bomb count for the cell at (i, j)
  // if the cell is a bomb, do nothing
  if (board[*i][*j] == 'X') {
    return;
  }

  // count the number of bombs in the 8 neighboring cells
  int count = 0;
  for (int x = *i - 1; x <= *i + 1; x++) {
    for (int y = *j - 1; y <= *j + 1; y++) {
      // check that the cell is in bounds
      if (x < 0 || x >= *m || y < 0 || y >= *n) {
        continue;
      }
      // check that the cell is not the current cell
      if (x == *i && y == *j) {
        continue;
      }
      // check that the cell is a bomb
      if (board[x][y] == 'X') {
        count++;
      }
    }
  }
  // update the cell with the number of bombs
  board[*i][*j] = count + '0';
}

float rand_float() {
  return (float)rand() / (float)RAND_MAX;
}

int main(int argc, char** argv) {
  srand(time(0)); 
  if (argc != 4) {
    printf("usage: %s m n p\n", argv[0]);
  }
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  float p = atof(argv[3]);

  // create board
  char** board = malloc(m * sizeof(char*));
  for (int i = 0; i < m; i++) {
    board[i] = malloc(n * sizeof(char));
  }

  // fill board
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      // get a random number between 0 and 1
      float r = rand_float();
      if (r < p) {
        board[i][j] = 'X';
      } else {
        board[i][j] = '.';
      }
    }
  }

  // print board
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }

  printf("\n");

  // count neighboring bombs
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
        update_bomb_count(&i, &j, &m, &n, board);
    }
  }

  // print board
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }

  // free board
  for (int i = 0; i < m; i++) {
    free(board[i]);
  }
  free(board);
}
