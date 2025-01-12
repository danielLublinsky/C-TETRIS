/* FigureManager.h */
#ifndef FIGUREMANAGER_H
#define FIGUREMANAGER_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_FIGURE_WIDTH 3
#define MAX_FIGURE_HEIGHT 3

const char figure_templates[3][MAX_FIGURE_HEIGHT][MAX_FIGURE_WIDTH] = {
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
    }
};

void generate_new_figure(char **figure) {
    srand(time(NULL));
    int template_index = rand() % (2 + 1); 
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

#endif
