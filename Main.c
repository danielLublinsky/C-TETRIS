/* Main.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include "FigureManager.h"


#define BOARD_WIDTH 13
#define BOARD_HEIGHT 25
#define PROG_TICK 400000

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

            if (y_pos < 0 || x_pos < 0 ) {
                printf("Out of bounds\n");
                free_game(game_state);
                exit(1);
            }

            if (y_pos < BOARD_HEIGHT && x_pos < BOARD_WIDTH) {
                if (game_state->game_board[y_pos][x_pos] == '#' && game_state->active_figure[i][j] == '*') {
                    clear_figure(game_state);
                    game_state->figure_y--;
                    write_figure(game_state);
                    prep_new_figure(game_state);
                    return;
                } else if (y_pos == BOARD_HEIGHT-1 && game_state->active_figure[i][j] == '*') {
                    write_figure(game_state);
                    prep_new_figure(game_state);
                    return;
                } else if (game_state->game_board[y_pos][x_pos] == ' ') {
                    game_state->game_board[y_pos][x_pos] = game_state->active_figure[i][j];
                }
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
    game_state->figure_x = rand() % (5 + 1); 
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

void on_left_arrow(game_state_t *game_state) {
   clear_figure(game_state);
   if (game_state->figure_x > 0){game_state->figure_x--;}
   update_board(game_state);
}

void on_right_arrow(game_state_t *game_state) {
    clear_figure(game_state);
    if (game_state->figure_x < BOARD_WIDTH){game_state->figure_x++;}
    update_board(game_state);
}
void on_space_press(game_state_t *game_state) {
    clear_figure(game_state);
    rotate_figure(game_state->active_figure);
    update_board(game_state);
}

int get_keypress_nonblocking() {
    struct timeval timeout;
    fd_set readfds;

    timeout.tv_sec = 0; 
    timeout.tv_usec = 0; 

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            return ch;
        }
    }
    return 0;
}

void set_terminal_mode() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag &= ~ICANON;          
    new_settings.c_lflag &= ~ECHO;            
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);  
}

void restore_terminal_mode() {
    struct termios old_settings;
    tcgetattr(STDIN_FILENO, &old_settings);  
    old_settings.c_lflag |= ICANON;           
    old_settings.c_lflag |= ECHO;          
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); 
}

int main() {
    game_state_t game_state;
    game_state.figure_x = 3;
    game_state.figure_y = 0;
    initialize_figure(&game_state);
    initialize_board(&game_state);
    
    set_terminal_mode();

    char ch;
    int drop_counter = 0;

    while (1) {
        ch = get_keypress_nonblocking();

        if (ch) { 
            switch (ch) {
                case 67: 
                    on_right_arrow(&game_state);
                    break;
                case 68:  
                    on_left_arrow(&game_state);
                    break;
                case 32: 
                    on_space_press(&game_state);
                    break;
            }
        }

        if (drop_counter >= PROG_TICK / 1000) { 
            drop_figure(&game_state);
            drop_counter = 0; 
        }

        usleep(1000);
        drop_counter++;
    }

    free_game(&game_state);
    restore_terminal_mode();
    return 0;
}