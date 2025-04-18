#include "pixul.h"
#include <unistd.h>

#define w 50
#define h 50

int** init_board() {
  int** board;

  board = malloc(w * sizeof(int*));
  for (int i = 0; i < w; i++) {
    board[i] = calloc(h, sizeof(int));
  }
  
  return board;
}

void destroy_board(int** board) {
  for (int i = 0; i < w; i++) {
    free(board[i]);
  }
  free(board);
}

void setup_board(int** board) {
  board[1][2] = 1;
  board[2][1] = 1;
  board[2][2] = 1;
  board[2][3] = 1;
  board[3][3] = 1;
}

int** iterate_gol(int** board) {
  int** new_board = init_board();
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      int neighbor_count = 0;
      new_board[i][j] = board[i][j];

      for (int di = -1; di < 2; di++) {
        for (int dj = -1; dj < 2; dj++) {
          if (di == 0 && dj == 0) continue;
          int ni = (i + di + w) % w;
          int nj = (j + dj + h) % h;
          if (board[ni][nj]) neighbor_count++;
        }
      }

      if (new_board[i][j] && (neighbor_count < 2 || neighbor_count > 3)) {
        new_board[i][j] = 0;
      } else if (!new_board[i][j] && (neighbor_count == 3)) {
        new_board[i][j] = 1;
      }
    }
  }

  return new_board;
}

void render_board(int** board) {
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      if (board[i][j]) {
        P_Set(i, j, (P_Color){255, 255, 255, 255});
      }
    }
  }
}

int main()
{
  P_Create("Conway's Game of Life", 500, 500, w, h);

  int** board = init_board();
  setup_board(board);

  int play = 1;
  
  while (!P_Done()) {
    P_Clear((P_Color){0, 0, 0, 255});

    render_board(board);

    if (play) {
      int** tmp_board = iterate_gol(board);
      destroy_board(board);
      board = tmp_board;
    } else {
      if (P_KeyPress(SDL_SCANCODE_N)) {
        // ITERATE
        int** tmp_board = iterate_gol(board);
        destroy_board(board);
        board = tmp_board;
      }
    }
      
    // PAUSE
    if (P_KeyPress(SDL_SCANCODE_SPACE)) {
      play = !play;
    }

    // QUIT
    if (P_KeyPress(SDL_SCANCODE_Q)) {
      break;
    }

    P_Update();
    usleep(10000);
  }

  destroy_board(board);
  P_Destroy();

  return 0;
}
