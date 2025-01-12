/* Main.c */
#include <stdio.h>
#include <stdlib.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MAX_FIGURE_WIDTH 3
#define MAX_FIGURE_HEIGHT 3

typedef struct
{
    char **game_board;
    int figure_x, figure_y;
} game_state_t;

//demo
char active_figure[MAX_FIGURE_HEIGHT][MAX_FIGURE_WIDTH] = {
    {'*', '*', ' '},  
    {'*', ' ', ' '},
    {'*', ' ', ' '}
};

void initialize_board(game_state_t *game_state) {
    game_state->game_board = (char **)malloc(BOARD_HEIGHT * sizeof(char *));
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        game_state->game_board[i] = (char *)malloc(BOARD_WIDTH * sizeof(char));
    }

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            game_state->game_board[i][j] = ' ';  
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
            printf("%c ", game_state->game_board[i][j]);
        }
        printf("\n");
    }
}

void display_figure(game_state_t *game_state){
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int j = 0; j < MAX_FIGURE_WIDTH; j++) {
            int y_pos = game_state->figure_y + i; 
            int x_pos = game_state->figure_x + j;

            if (y_pos >= 0 && y_pos < BOARD_HEIGHT && x_pos >= 0 && x_pos < BOARD_WIDTH) {
                if (game_state->game_board[y_pos][x_pos] == ' ') {
                    game_state->game_board[y_pos][x_pos] = active_figure[i][j];  // Correct figure placement
                }
                else if (game_state->game_board[y_pos][x_pos] == '#' && active_figure[i][j] == '*') {
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

void clear_figure(game_state_t *game_state) {
     for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int j = 0; j < MAX_FIGURE_WIDTH; j++) {
            int y_pos = game_state->figure_y + i; 
            int x_pos = game_state->figure_x + j;
            
            if (y_pos >= 0 && y_pos < BOARD_HEIGHT && x_pos >= 0 && x_pos < BOARD_WIDTH) {
                if(game_state->game_board[y_pos][x_pos] != '#') {
                    game_state->game_board[y_pos][x_pos] = ' '; 
                }
            }
        }
    }
}

void update_board(game_state_t *game_state) {
    system("clear");
    display_figure(game_state);
    print_board(game_state);
}


void drop_figure(game_state_t *game_state) {
    clear_figure(game_state);
    game_state->figure_y++;
    update_board(game_state);
}

int main() {
    game_state_t game_state;
    game_state.figure_x = 2;
    game_state.figure_y = 2;

    initialize_board(&game_state);

    while (1)
    {
        drop_figure(&game_state);
        getchar();
    }
    
    free_board(&game_state);
    return 0;
}