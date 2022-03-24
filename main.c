#include "utils_SDL.h"
#include "graph.h"

int main(int argc, char ** argv)
{
	SDL_Event e;
	SDL_Surface * window = NULL;
	SDL_Surface * heightmap = NULL;

	bool start = true;
	struct Node_s* A = NULL;
	struct Node_s* B = NULL;


	window = init("./data/heightmap.bmp", &heightmap, "basecode");
	struct Graph_s* graph = createGraphFromSurface(heightmap);

	if (!window)
		return EXIT_FAILURE;
	
	while (!interaction(&e, window, heightmap, graph, &A, &B, &start))
	{
		update();

		draw(window, heightmap);
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}
