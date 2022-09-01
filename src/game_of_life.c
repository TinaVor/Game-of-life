#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#define SIZE_X 80
#define SIZE_Y 25
void loop(int** grid, int cycle);
void print(int** grid);
void create(int** grid, int* ptr, char filename[]);
int count(int **grid, int i, int j);
void check(int** grid, int cycle);
void gamaiobera(int** grid, int grid_compare[SIZE_Y][SIZE_X],
int grid_prev[SIZE_Y][SIZE_X], int cycle);
int key();
int main(int argc, char *argv[]) {
  int cycle = 0;
  argc += 1;
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  int** grid = malloc(SIZE_Y * sizeof(int*));
  int* ptr = (int*) (grid + SIZE_Y);
  create(grid, ptr, argv[1]);
  loop(grid, cycle);
  }
void create(int** grid, int* ptr, char filename[]) {
  for (int i = 0; i < SIZE_Y ; i++) {
      grid[i] = ptr + SIZE_X * i;
  }
  int var;
  FILE* fp;
  if (fopen(filename, "r+") != NULL) {
    fp = fopen(filename, "r+");
    rewind(fp);
    for (int i = 0; i < 25; i++) {
      for (int j = 0; j < 80; j++) {
        fscanf(fp, "%d", &var);
        grid[i][j] = var;
      }
    }
  } else {
    printf("File not found");
    free(grid);
    exit(0);
  }
  fclose(fp);
}
void delay(int milli_seconds) {
  clock_t start_time = clock();
  while (clock() < start_time + milli_seconds) {}
}
void loop(int** grid, int cycle) {
  int msec = 0, trigger = 10;
  int time = 150000;
  printw("Conway's Game of Life \n");
  refresh();
  delay(1000000);
  while (msec < trigger) {
    print(grid);
    printw("\n");
    check(grid, cycle);
    switch (getch()) {
    case KEY_UP:
      time += 10000;
      if (time > 1000000) {
        time = 1000000;
      }
      break;
    case KEY_DOWN:
      time -= 10000;
      if (time < 70000) {
        time = 70000;
      }
      break;
    }
      refresh();
      cycle++;
      delay(time);
    }
}
int key() {
  int keyblade = getch();
  if (keyblade != ERR) {
    ungetch(keyblade);
    return 1;
  } else {
    return 0;
  }
}
void check(int** grid, int cycle) {
  int grid_compare[SIZE_Y][SIZE_X];
  int grid_temp[SIZE_Y][SIZE_X];
  int grid_prev[SIZE_Y][SIZE_X];
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      grid_prev[i][j] = grid_temp[i][j];
      grid_temp[i][j] = grid[i][j];
    }
  }
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      grid_compare[i][j] = count(grid, i, j);
    }
  }
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      if (grid_compare[i][j] == 0 || grid_compare[i][j] == 1 || grid_compare[i][j] >= 4)
        grid[i][j] = 0;
      else if (grid_compare[i][j] == 3)
        grid[i][j] = 1;
    }
  }
  gamaiobera(grid, grid_temp, grid_prev, cycle);
}
void gamaiobera(int** grid, int grid_temp[SIZE_Y][SIZE_X],
        int grid_prev[SIZE_Y][SIZE_X], int cycle) {
  int same_quantity = 0;
  int prev_quantity = 0;
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      if (grid[i][j] == grid_temp[i][j]) {
        same_quantity++;
      }
      if ( grid[i][j] == grid_prev[i][j] && cycle > 3 ) {
        prev_quantity++;
      }
    }
  }
  if (same_quantity == (SIZE_X*SIZE_Y) || prev_quantity == (SIZE_X*SIZE_Y)) {
    printw("\nAll organisms are either dead or stationary\nPress any key to exit");
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    endwin();
    free(grid);
    exit(0);
  }
}
int count(int **grid, int i, int j) {
  int count = 0;
  if (grid[(i + SIZE_Y + 1) % SIZE_Y][(j + SIZE_X) % SIZE_X] == 1)
    count++;
  if (grid[(i + SIZE_Y - 1) % SIZE_Y][(j + SIZE_X) % SIZE_X] == 1)
    count++;
  if (grid[(i + SIZE_Y) % SIZE_Y][(j + SIZE_X + 1) % SIZE_X] == 1)
    count++;
  if (grid[(i + SIZE_Y) % SIZE_Y][(j + SIZE_X-1) % SIZE_X] == 1)
    count++;
  if (grid[(i + SIZE_Y + 1) % SIZE_Y][(j + SIZE_X + 1) % SIZE_X] >= 1)
    count++;
  if (grid[(i + SIZE_Y + 1) % SIZE_Y][(j + SIZE_X - 1) % SIZE_X] >= 1)
    count++;
  if (grid[(i + SIZE_Y - 1) % SIZE_Y][(j + SIZE_X + 1) % SIZE_X] >= 1)
    count++;
  if (grid[(i + SIZE_Y - 1) % SIZE_Y][(j + SIZE_X - 1) % SIZE_X] >= 1)
    count++;
  return count;
}
void print(int** grid) {
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      if (grid[i][j] == 1) {
        printw("*");
      }  else {
        printw(".");
        }
    }
    printw("\n");
  }
}
