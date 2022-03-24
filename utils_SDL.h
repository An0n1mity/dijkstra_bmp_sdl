#pragma once

#include "..//SDL-1.2.15/include/SDL_image.h"
#include "../SDL-1.2.15/include/SDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int draw(SDL_Surface *p_window, SDL_Surface *p_image);
SDL_Surface * init(char * p_heightmapPath, SDL_Surface **p_heightmap, char * p_windowName);
int interaction(SDL_Event* p_e, SDL_Surface* window, SDL_Surface* surface, struct Graph_s* graph, struct Node_s** A, struct Node_s** B, bool* start);
int update();
SDL_Surface* copy_surface(SDL_Surface* surface);
void _SDL_SetPixel(SDL_Surface* p_affichage, int p_x, int p_y, const Uint8 p_red, const Uint8 p_green, const Uint8 p_blue);
