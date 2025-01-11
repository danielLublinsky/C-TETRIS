/* Main.c */
#include <stdio.h>
#include <stdlib.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct
{
    int scoreVal;
    int **game_board;
    int *active_figure;
} game_state_t;

void initialize_board(game_state_t *game_state) {
    game_state->game_board = (int **)malloc(BOARD_HEIGHT * sizeof(int *));
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        game_state->game_board[i] = (int *)malloc(BOARD_WIDTH * sizeof(int));
    }

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            game_state->game_board[i][j] = 0;  
        }
    }
}

void free_board(game_state_t *game_state) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        free(game_state->game_board[i]);
    }
    free(game_state->game_board);
}

void print_board(game_state_t *game_state) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf("%d ", game_state->game_board[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, int* argv[]) {
    game_state_t game_state;

    game_state.scoreVal = 0;
    initialize_board(&game_state);
    print_board(&game_state);
    free_board(&game_state);

    return 0;
}