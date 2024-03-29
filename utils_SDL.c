﻿#include "utils_SDL.h"
#include "graph.h"

SDL_Surface * init(char * p_heightmapPath, SDL_Surface **p_heightmap, char * p_windowName)
{
	SDL_Surface *window;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("[-] ERROR - %s\n", SDL_GetError());
		return NULL;
	}
	
	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("[-] ERROR - Failed to initialise SDL_Image (%s)\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}

	*p_heightmap = IMG_Load(p_heightmapPath);

	window = SDL_SetVideoMode((*p_heightmap)->w, (*p_heightmap)->h, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
	if (!window)
	{
		printf("[-] ERROR - %s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}

	SDL_WM_SetCaption(p_windowName, NULL);

	return window;
}

int interaction(SDL_Event *p_e, struct Graph_s* graph, struct Node_s** A, struct Node_s** B, bool* start, bool* search)
{
	int quit = 0;
	int mouse_x, mouse_y;
	

	while (SDL_PollEvent(p_e))
	{
		switch (p_e->type)
		{
			// Appuie sur la croix rouge de la fenêtre
			case SDL_QUIT:
				quit = 1;
				break;

			// Appuie sur une touche du clavier
			case SDL_KEYDOWN:
				switch (p_e->key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = 1;
						break;

					default:
						break;
				}
				break;

			// Relachement d'une touche du clavier
			case SDL_KEYUP:
				switch (p_e->key.keysym.sym)
				{
					default:
						break;
				}
				break;

			// Appuie sur un bouton de la souris
			case SDL_MOUSEBUTTONDOWN:
				switch (p_e->button.button)
				{
					case SDL_BUTTON_LEFT:
							SDL_GetMouseState(&mouse_x, &mouse_y);
							if (*start) {
								*A = getNodeFromGraph(graph, mouse_y, mouse_x);
								*start = false;
							}
							else {
								*B = getNodeFromGraph(graph, mouse_y, mouse_x);
								*start = true;
								*search = true;
							}
				
						break;

					case SDL_BUTTON_RIGHT:
						break;

					case SDL_BUTTON_MIDDLE:
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
	}

	return quit;
}

int update(struct Graph_s* graph, struct Node_s* A, struct Node_s* B, struct Node_s** antecedentarray, int* distancearray, int* idx, SDL_Surface* heightmap, SDL_Surface* copy, SDL_Surface* window, bool* search)
{
	struct Edge_s* to_draw = NULL;
	struct Node_s* S = getNodeWithMinimalDistance(graph->nodesarray, distancearray, graph->m_nbnodes);

	if (*idx >= 0 && *idx < graph->m_nbnodes)
		getShortestpathV2(graph, A, B, S, antecedentarray, distancearray, &to_draw);

	while (to_draw)
	{
		_SDL_SetPixel(copy, to_draw->m_node->m_col, to_draw->m_node->m_row, mapValue(distancearray[S->m_index], 255, 10), 0, 0);
		to_draw = to_draw->m_next;
	}

	if (B == S)
	{
		copy = copy_surface(heightmap);
		int f = B->m_index;
		while (antecedentarray[f] != A)
		{
			_SDL_SetPixel(copy, antecedentarray[f]->m_col, antecedentarray[f]->m_row, 0, 255, 0);
			draw(window, copy);
			f = antecedentarray[f]->m_index;
		}
		*search = false;
	}

	draw(window, copy);

	*idx = *idx + 1;
	return 0;
}

SDL_Surface* copy_surface(SDL_Surface* surface)
{
	SDL_Surface* copy;
	copy = SDL_CreateRGBSurface(0, surface->w, surface->h, 24, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

	if (copy == NULL || surface == NULL)
		return NULL;

	copy = SDL_DisplayFormatAlpha(surface);

	return copy;
}

int draw(SDL_Surface *p_window, SDL_Surface *p_image)
{
	SDL_FillRect(p_window, NULL, SDL_MapRGBA(p_window->format, 0, 0, 0, 0));
	SDL_BlitSurface(p_image, NULL, p_window, NULL);

	SDL_Flip(p_window);

	return 0;
}

void _SDL_SetPixel(SDL_Surface* p_affichage, int p_x, int p_y, const Uint8 p_red, const Uint8 p_green, const Uint8 p_blue)
{
	Uint32 color = SDL_MapRGB(p_affichage->format, p_red, p_green, p_blue);
	const Uint8 Bpp = p_affichage->format->BytesPerPixel;

	// test si le pixel est dans la fen�tre avant de le dessiner si besoin
	if ((p_x >= 0 && p_x < p_affichage->clip_rect.w) && (p_y >= 0 && p_y < p_affichage->clip_rect.h))
		*(Uint32*)((Uint8*)p_affichage->pixels + p_y * p_affichage->pitch + p_x * Bpp) = color;

}
