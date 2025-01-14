/* FigureManager.h */
#ifndef FIGUREMANAGER_H
#define FIGUREMANAGER_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_FIGURE_WIDTH 3
#define MAX_FIGURE_HEIGHT 3
#define TEMPLATE_COUNT 10
typedef struct {
    char **figure_grid;
    int hight, width;
} active_figure_t;

const char figure_templates[TEMPLATE_COUNT][MAX_FIGURE_HEIGHT][MAX_FIGURE_WIDTH] = {
    {
        {'*', '*', ' '},
        {'*', ' ', ' '},
        {'*', ' ', ' '}
    },
    {
        {'*', '*', '*'},
        {' ', '*', ' '},
        {' ', '*', ' '}
    },
    {
        {'*', '*', '*'},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    },
    {
        {' ', '*', '*'},
        {'*', '*', ' '},
        {' ', ' ', ' '}
    },
    {
        {'*', '*', ' '},
        {' ', '*', '*'},
        {' ', ' ', ' '}
    },
    {
        {'*', '*', ' '},
        {'*', ' ', ' '},
        {' ', ' ', ' '}
    },
    {
        {'*', '*', ' '},
        {'*', '*', ' '},
        {' ', ' ', ' '}
    },
    {
        {'*', '*', '*'},
        {'*', ' ', '*'},
        {' ', ' ', ' '}
    },
    {
        {' ', '*', ' '},
        {'*', '*', '*'},
        {' ', '*', ' '}
    },
    {
        {' ', '*', ' '},
        {'*', '*', ' '},
        {' ', '*', ' '}
    }
};

void initialize_figure(active_figure_t *active_figure) {
    active_figure->figure_grid = (char **)malloc(MAX_FIGURE_HEIGHT * sizeof(char *));
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        active_figure->figure_grid[i] = (char *)malloc(MAX_FIGURE_WIDTH * sizeof(char));
    }
    generate_new_figure(active_figure->figure_grid);
    get_figure_size(active_figure);
}

void generate_new_figure(char **figure) {
    int template_index = rand() % (TEMPLATE_COUNT);
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int j = 0; j < MAX_FIGURE_WIDTH; j++) {
            figure[i][j] = figure_templates[template_index][i][j];
        }
    }
}

void rotate_figure(char **figure) {
    char rotated_figure[MAX_FIGURE_WIDTH][MAX_FIGURE_HEIGHT];

    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int j = 0; j < MAX_FIGURE_WIDTH; j++) {
            rotated_figure[j][MAX_FIGURE_HEIGHT - i - 1] = figure[i][j];
        }
    }

    for (int i = 0; i < MAX_FIGURE_WIDTH; i++) {
        for (int j = 0; j < MAX_FIGURE_HEIGHT; j++) {
            figure[i][j] = rotated_figure[i][j];
        }
    }
}

void get_figure_size(active_figure_t *active_figure) {
    int temp_hight = -1, temp_width = -1;
    for (int i = 0; i < MAX_FIGURE_HEIGHT; i++) {
        for (int j = 0; j < MAX_FIGURE_WIDTH; j++) {
            if (active_figure->figure_grid[i][j] == '*') {
                if (i > temp_hight) temp_hight = i;
                if (j > temp_width) temp_width = j;
            }
        }
    }
    active_figure->hight = temp_hight + 1;
    active_figure->width = temp_width + 1;
}

#endif
