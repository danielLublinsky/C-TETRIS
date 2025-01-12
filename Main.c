/* Main.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "FigureManager.h"


#define BOARD_WIDTH 7
#define BOARD_HEIGHT 10
#define PROG_TICK 500000

typedef struct {
    char **game_board;
    char **active_figure;
    int figure_x, figure_y;
} game_state_t;


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

void initialize_figure(game_state_t *game_state) {
    game_state->active_figure = (char **)malloc(MAX_FIGURE_HEIGHT * sizeof(char *));
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        game_state->active_figure[i] = (char *)malloc(MAX_FIGURE_WIDTH * sizeof(char));
    }
    generate_new_figure(game_state->active_figure);
}


void free_game(game_state_t *game_state) {
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        free(game_state->active_figure[i]);
    }
    free(game_state->active_figure);

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
                if (y_pos == BOARD_HEIGHT-1 && game_state->active_figure[i][j] == '*') {
                    write_figure(game_state);
                    prep_new_figure(game_state);
                    return;
                } else if (game_state->game_board[y_pos][x_pos] == '#' && game_state->active_figure[i][j] == '*') {
                    clear_figure(game_state);
                    game_state->figure_y--;
                    write_figure(game_state);
                    prep_new_figure(game_state);
                    return;
                }  else if (game_state->game_board[y_pos][x_pos] == ' ') {
                    game_state->game_board[y_pos][x_pos] = game_state->active_figure[i][j];
                }
            } else {
                printf("Out of bounds\n");
                free_game(game_state);
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

void write_figure(game_state_t *game_state){
     for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int j = 0; j < MAX_FIGURE_WIDTH; j++) {
            int y_pos = game_state->figure_y + i; 
            int x_pos = game_state->figure_x + j;
            
            if (y_pos >= 0 && y_pos < BOARD_HEIGHT && x_pos >= 0 && x_pos < BOARD_WIDTH) {
                if(game_state->active_figure[i][j] == '*') {
                    game_state->game_board[y_pos][x_pos] = '#'; 
                }
            }
        }
    }
}

void prep_new_figure(game_state_t *game_state){
    generate_new_figure(game_state->active_figure);
    game_state->figure_y = 0;
    game_state->figure_x = rand() % (3 + 1); 
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
    game_state.figure_x = 3;
    game_state.figure_y = 0;
    initialize_figure(&game_state);
    initialize_board(&game_state);
    
    while (1)
    {
        drop_figure(&game_state);
        usleep(PROG_TICK);   
    }
    
    free_game(&game_state);
    return 0;
}