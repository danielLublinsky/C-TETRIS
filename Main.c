/* Main.c */
#include <stdio.h>
#include <stdlib.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MAX_FIGURE_WIDTH 5
#define MAX_FIGURE_HEIGHT 5

typedef struct
{
    int scoreVal;
    int **game_board;
    int figure_x, figure_y;
} game_state_t;


//demo
int active_figure[MAX_FIGURE_HEIGHT][MAX_FIGURE_WIDTH] = {
    {1, 1, 1, 1, 0},  
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

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

void display_figure(game_state_t *game_state){
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int y = 0; y < MAX_FIGURE_WIDTH; y++ ) {
            int x_pos = game_state->figure_x + i;
            int y_pos = game_state->figure_y + y;

            if (x_pos >= 0 && x_pos < BOARD_HEIGHT && y_pos >= 0 && y_pos < BOARD_WIDTH) {
                if (game_state->game_board[x_pos][y_pos] == 0 && active_figure[i][y] != 0) {
                    game_state->game_board[x_pos][y_pos] = active_figure[i][y];
                }
                else if (game_state->game_board[x_pos][y_pos] != 0 && active_figure[i][y] != 0) {
                    printf("Collision detected\n");
                    free_board(game_state);
                    exit(1);
                }
            } else {
                printf("Out of bounds\n");
                free_board(game_state);
                exit(1);
            }

        }
    }
}
void update_board(game_state_t *game_state) {
    //system("clear");
    display_figure(game_state);
    print_board(game_state);
}


void move_figure(game_state_t *game_state) {
    game_state->figure_y++;
    update_board(game_state);
}

int main() {
    game_state_t game_state;
    game_state.scoreVal = 0;
    game_state.figure_x = 0;
    game_state.figure_y = 0;

    initialize_board(&game_state);

    while (1)
    {
        update_board(&game_state);
        getchar();
    }
    
    free_board(&game_state);
    return 0;
}