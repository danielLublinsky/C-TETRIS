/* Main.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include "FigureManager.h"

#define BOARD_WIDTH 13
#define BOARD_HEIGHT 25
#define PROG_TICK 400000
#define FAST_DROP_TICK 80000

typedef struct {
    char **game_board;
    active_figure_t *active_figure;
    int figure_posX, figure_posY;
} game_state_t;

void initialize_board(game_state_t *game_state) {
    game_state->game_board = (char **)malloc(BOARD_HEIGHT * sizeof(char *));
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        game_state->game_board[i] = (char *)malloc(BOARD_WIDTH * sizeof(char));
        for (int j = 0; j < BOARD_WIDTH; j++) {
            game_state->game_board[i][j] = ' ';
        }
    }
}

void free_prog(game_state_t *game_state) {
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        free(game_state->active_figure->figure_grid[i]);
    }
    free(game_state->active_figure->figure_grid);

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

int check_collision(game_state_t *game_state) {
    for (int i = 0; i < game_state->active_figure->hight; i++) {
        for (int j = 0; j < game_state->active_figure->width; j++) {
            int y_pos = game_state->figure_posY + i;
            int x_pos = game_state->figure_posX + j;

            if (y_pos >= BOARD_HEIGHT || x_pos < 0 || x_pos >= BOARD_WIDTH) {
                if (game_state->active_figure->figure_grid[i][j] == '*') {
                    return 1;
                }
            } else if (y_pos >= 0 && game_state->active_figure->figure_grid[i][j] == '*' && game_state->game_board[y_pos][x_pos] == '#') {
                return 1;
            }
        }
    }
    return 0;
}

void display_figure(game_state_t *game_state) {
    for (int i = 0; i < game_state->active_figure->hight; i++) {
        for (int j = 0; j < game_state->active_figure->width; j++) {
            int y_pos = game_state->figure_posY + i;
            int x_pos = game_state->figure_posX + j;

            if (y_pos >= 0 && y_pos < BOARD_HEIGHT && x_pos >= 0 && x_pos < BOARD_WIDTH && game_state->active_figure->figure_grid[i][j] == '*') {
                game_state->game_board[y_pos][x_pos] = game_state->active_figure->figure_grid[i][j];
            }
        }
    }
}

void clear_figure(game_state_t *game_state) {
    for (int i = 0; i < game_state->active_figure->hight; i++) {
        for (int j = 0; j < game_state->active_figure->width; j++) {
            int y_pos = game_state->figure_posY + i;
            int x_pos = game_state->figure_posX + j;

            if (y_pos >= 0 && y_pos < BOARD_HEIGHT && x_pos >= 0 && x_pos < BOARD_WIDTH) {
                if (game_state->game_board[y_pos][x_pos] != '#') {
                    game_state->game_board[y_pos][x_pos] = ' ';
                }
            }
        }
    }
}

void write_figure(game_state_t *game_state) {
    for (int i = 0; i < game_state->active_figure->hight; i++) {
        for (int j = 0; j < game_state->active_figure->width; j++) {
            int y_pos = game_state->figure_posY + i;
            int x_pos = game_state->figure_posX + j;

            if (y_pos >= 0 && y_pos < BOARD_HEIGHT && x_pos >= 0 && x_pos < BOARD_WIDTH) {
                if (game_state->active_figure->figure_grid[i][j] == '*') {
                    game_state->game_board[y_pos][x_pos] = '#';
                }
            }
        }
    }
}

void prep_new_figure(game_state_t *game_state) {
    generate_new_figure(game_state->active_figure->figure_grid);
    get_figure_size(game_state->active_figure);
    game_state->figure_posY = 0;
    game_state->figure_posX = rand() % (BOARD_WIDTH - game_state->active_figure->width + 1);
}

void update_board(game_state_t *game_state) {
    system("clear");
    display_figure(game_state);
    print_board(game_state);
}

void drop_figure(game_state_t *game_state) {
    clear_figure(game_state);
    game_state->figure_posY++;

    if (check_collision(game_state)) {
        game_state->figure_posY--;
        write_figure(game_state);
        clear_full_rows(game_state);
        prep_new_figure(game_state);
    } else {
        update_board(game_state);
    }
}

void on_left_arrow(game_state_t *game_state) {
    clear_figure(game_state);
    game_state->figure_posX--;

    if (check_collision(game_state)) {
        game_state->figure_posX++;
    }

    update_board(game_state);
}

void on_right_arrow(game_state_t *game_state) {
    clear_figure(game_state);
    game_state->figure_posX++;

    if (check_collision(game_state)) {
        game_state->figure_posX--;
    }

    update_board(game_state);
}

void on_down_arrow(game_state_t *game_state, int *tick_speed) {
    *tick_speed = FAST_DROP_TICK;
}

void on_space_press(game_state_t *game_state) {
    clear_figure(game_state);
    rotate_figure(game_state->active_figure->figure_grid);
    get_figure_size(game_state->active_figure);

    if (check_collision(game_state)) {
        rotate_figure(game_state->active_figure->figure_grid);
        rotate_figure(game_state->active_figure->figure_grid);
        rotate_figure(game_state->active_figure->figure_grid);
        get_figure_size(game_state->active_figure);
    }

    update_board(game_state);
}

void clear_full_rows(game_state_t *game_state) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        int charCount = 0;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (game_state->game_board[i][j] != ' ') {
                charCount++;
            }
        }
        if (charCount == BOARD_WIDTH) {
            for (int y = i; y > 0; y--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    game_state->game_board[y][x] = game_state->game_board[y - 1][x];
                }
            }
            
            for (int x = 0; x < BOARD_WIDTH; x++) {
                game_state->game_board[0][x] = ' ';
            }
        }
    }
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
    game_state.figure_posX = 3;
    game_state.figure_posY = 0;

    game_state.active_figure = malloc(sizeof(active_figure_t));
    initialize_figure(game_state.active_figure);
    initialize_board(&game_state);

    set_terminal_mode();
    srand(time(NULL));

    char ch;
    int drop_counter = 0;
    int tick_speed = PROG_TICK;

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
                case 66:
                    on_down_arrow(&game_state, &tick_speed);
                    break;
                case 32:
                    on_space_press(&game_state);
                    break;
            }
        }

        if (drop_counter >= tick_speed / 1000) {
            drop_figure(&game_state);
            drop_counter = 0;
            tick_speed = PROG_TICK;
        }

        usleep(1000);
        drop_counter++;
    }

    free_prog(&game_state);
    restore_terminal_mode();
    return 0;
}