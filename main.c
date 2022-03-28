#include "utils_SDL.h"
#include "graph.h"

struct global_s {
	bool m_search;
	struct Node_s* m_A;
	struct Node_s* m_B;
	struct Node_s** m_antecedentsarray;
	struct Graph_s* m_graph;
	SDL_Surface* m_surface;
	SDL_Surface* m_window;
	int* m_distancearray;
	SDL_mutex* m_search_lock;
};

void processFunction(void* global)
{
	struct global_s* gb = (struct global_s*)global;
	int idx = 0;
	SDL_Surface* copy = copy_surface(gb->m_surface);
	SDL_LockMutex(gb->m_search_lock);
	while (gb->m_search) {
		if (!gb->m_antecedentsarray)
			gb->m_antecedentsarray = createAntecedentsarray(gb->m_A, gb->m_graph->m_nbnodes);
		if (!gb->m_distancearray)
			gb->m_distancearray = createDistancearray(gb->m_A, gb->m_graph->m_nbnodes);
		update(gb->m_graph, gb->m_A, gb->m_B, gb->m_antecedentsarray, gb->m_distancearray, &idx, gb->m_surface, copy, gb->m_window, &gb->m_search);

	}
	SDL_UnlockMutex(gb->m_search_lock);
}

int main(int argc, char ** argv)
{
	SDL_Event e;
	SDL_Surface * window = NULL;
	SDL_Surface * heightmap = NULL;
	SDL_Surface* copy = NULL; 
	SDL_Thread* thread = NULL;

	bool start = true;
	bool search = false;
	struct Node_s* A = NULL;
	struct Node_s* B = NULL;
	struct Node_s** antecedentsarray = NULL;
	int* distancearray = NULL;
	int idx = 0;

	window = init("./data/heightmap__.bmp", &heightmap, "basecode");
	copy = copy_surface(heightmap);
	struct Graph_s* graph = createGraphFromSurface(heightmap);

	struct global_s* global = (struct global_s*)malloc(sizeof(struct global_s));
	global->m_graph = graph;
	global->m_window = window;
	global->m_surface = copy;
	global->m_antecedentsarray = antecedentsarray;
	global->m_distancearray = distancearray;
	global->m_search = false;
	global->m_search_lock = SDL_CreateMutex();

	if (!window)
		return EXIT_FAILURE;
	
	draw(window, heightmap);
	while (!interaction(&e, graph, &A, &B, &start, &global->m_search))
	{
		// If search spawn a new thread for calculation and rendering
		if (global->m_search) {
			global->m_A = A; global->m_B = B;
			if(!thread)
				thread = SDL_CreateThread(processFunction, (void*)global);/*
			if(!antecedentsarray)
				antecedentsarray = createAntecedentsarray(A, graph->m_nbnodes);
			if(!distancearray)
				distancearray = createDistancearray(A, graph->m_nbnodes);
			update(graph, A, B, antecedentsarray, distancearray, &idx, heightmap, copy, window, &search);
			if (!search) {
				copy = copy_surface(heightmap);
				for (size_t i = 0; i < graph->m_nbnodes; i++)
				{
					graph->nodesarray[i]->flag = false;
				}
				free(distancearray);
				free(antecedentsarray);
				distancearray = NULL;
				antecedentsarray = NULL;
				idx = 0;
			}
			*/
		}
		else {
			if (thread) {
				SDL_KillThread(thread);
				for (size_t i = 0; i < global->m_graph->m_nbnodes; i++)
				{
					global->m_graph->nodesarray[i]->flag = false;
				}
				free(global->m_distancearray);
				free(global->m_antecedentsarray);
				global->m_distancearray = NULL;
				global->m_antecedentsarray = NULL;
				thread = NULL;
			}

		}
		
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}
