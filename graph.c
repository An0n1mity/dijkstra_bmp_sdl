#include "graph.h"

struct Graph_s* createGraphFromSurface(SDL_Surface* surface)
{
	struct Graph_s* graph = (struct Graph_s*)malloc(sizeof(struct Graph_s));

	int width = surface->w;
	int height = surface->h;
	graph->m_nbnodes = width * height;
	graph->m_width = width;

	// Array of pixels from bmp
	uint8_t* pixelsarray = getPixelsarrayFromSurface(surface);
	// Nodes array
	struct Node_s** nodesarray = createNodesarrayFromPixelarray(pixelsarray, width, height);
	graph->nodesarray = nodesarray;
	
	graph->nodelist = nodesarray[0];


	return graph;
}

struct Node_s* getNodeWithMinimalDistance(struct Node_s** nodesarray, int* distancearray, bool* trackingarray, int size)
{
	int min = 100000;
	struct Node_s* S = NULL;
	for (size_t i = 0; i < size; i++)
	{
		if (distancearray[i] < min && (!trackingarray[i]))
		{
			min = distancearray[i];
			S = nodesarray[i];
		}
	}

	return S;

}

void getShortestpath(struct Graph_s* graph, struct Node_s* A, struct Node_s* B, SDL_Surface* surface, SDL_Surface* window)
{
	SDL_Surface* copy = copy_surface(surface);

	bool* trackingarray = createTrackingarray(A, graph->m_nbnodes);
	struct Node_s** antecedentsarray = createAntecedentsarray(A, graph->m_nbnodes);
	int* distancearray = createDistancearray(A, graph->m_nbnodes);
	struct Node_s* S;
	int j;

	for (size_t i = 0; i < graph->m_nbnodes; i++)
	{
		S = getNodeWithMinimalDistance(graph->nodesarray, distancearray, trackingarray, graph->m_nbnodes);
		j = S->m_index;
		trackingarray[j] = true;

		if (S == B)
			break;

		_SDL_SetPixel(surface, graph->nodesarray[j]->m_col, graph->nodesarray[j]->m_row, 255, 0, 0);
		draw(window, surface);

			struct Edge_s* cursor = S->m_neighbours;
			while (cursor)
			{
				if (!trackingarray[cursor->m_node->m_index]) {
					_SDL_SetPixel(surface, cursor->m_node->m_col, cursor->m_node->m_row, 255, 0, 0);
					draw(window, surface);

					if (distancearray[j] + cursor->m_distance < distancearray[cursor->m_node->m_index])
					{
						distancearray[cursor->m_node->m_index] = distancearray[j] + cursor->m_distance;
						antecedentsarray[cursor->m_node->m_index] = S;
					}
				}
				cursor = cursor->m_next;
			}
	}

	draw(window, copy);

	int f = B->m_index;
	while (antecedentsarray[f])
	{
		_SDL_SetPixel(copy, antecedentsarray[f]->m_col, antecedentsarray[f]->m_row, 0, 255, 0);
		draw(window, copy);
		f = antecedentsarray[f]->m_index;
	}

}

bool* createTrackingarray(struct Node_s* A, int size)
{
	bool* boolsarray = (bool*)calloc(size, sizeof(bool));
	if(boolsarray)
		boolsarray[A->m_index] = true;

	return boolsarray;

}

int* createDistancearray(struct Node_s* A, int size)
{
	int* distancearray = (int*)malloc(sizeof(int)*size);
	if(distancearray)
		memset(distancearray, 1, sizeof(int) * size);

	struct Edge_s* cursor = A->m_neighbours;
	while (cursor)
	{
		distancearray[cursor->m_node->m_index] = cursor->m_distance;
		cursor = cursor->m_next;
	}

	return distancearray;
	
}

struct Node_s** createAntecedentsarray(struct Node_s* A, int size)
{
	struct Node_s** antecendantsarray = (struct Node_s**)malloc(sizeof(struct Node_s*) * size);
	for (size_t i = 0; i < size; i++)
		antecendantsarray[i] = A;
	
	return antecendantsarray;
}

uint8_t* getPixelsarrayFromSurface(SDL_Surface* surface)
{
	int width = surface->w;
	int height = surface->h;
	int pitch = surface->pitch;
	int bpp = surface->format->BytesPerPixel;

	uint8_t* pixelsarray = (uint8_t*)malloc(sizeof(uint8_t) * height * width);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			Uint8* pixel = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				pixelsarray[width * y + x] = pixel[0] << 16 | pixel[1] << 8 | pixel[2];
			else
				pixelsarray[width * y + x] = pixel[0] | pixel[1] << 8 | pixel[2] << 16;

			//printf("%hhx\n", pixelsarray[width * y + x]);
		}
	}

	return pixelsarray;
}

void addNeighboursToNodesarray(struct Node_s** nodesarray, int width, int height)
{
	int row, col;
	struct Node_s* neighbour = NULL;
	for (size_t i = 0; i < width * height; i++)
	{
		//printf("%hhx\n", nodesarray[i]->m_color);
		row = i / width;
		col = i % width;

		// Up neighbours
		if (row + 1 < height) {
			neighbour = getNodeFromNodesarray(nodesarray, row + 1, col, width);
			addNeighboursToNode(nodesarray[i], neighbour, abs(neighbour->m_color - nodesarray[i]->m_color));
		}
		// Down neighbours
		if (row - 1 >= 0) {
			neighbour = getNodeFromNodesarray(nodesarray, row - 1, col, width);
			addNeighboursToNode(nodesarray[i], neighbour, abs(neighbour->m_color - nodesarray[i]->m_color));
		}
		// Right neighbours
		if (col + 1 < width) {
			neighbour = getNodeFromNodesarray(nodesarray, row, col + 1, width);
			addNeighboursToNode(nodesarray[i], neighbour, abs(neighbour->m_color - nodesarray[i]->m_color));
		}
		// Column neighbours
		if (col - 1 >= 0) {
			neighbour = getNodeFromNodesarray(nodesarray, row, col - 1, width);
			addNeighboursToNode(nodesarray[i], neighbour, abs(neighbour->m_color - nodesarray[i]->m_color));
		}

		if (i + 1 < width * height)
			nodesarray[i]->m_next = nodesarray[i + 1];

	}
}

struct Node_s* createNodeFromPixel(int idx, int row, int col, uint8_t pixel)
{
	struct Node_s* node = (struct Node_s*)calloc(1, sizeof(struct Node_s));
	node->m_index = idx;
	node->m_row = row;
	node->m_col = col;
	node->m_color = pixel;
	node->m_nbneighbours = 0;
	node->flag = false;
	node->m_pathvalue = 100000;

	return node;
}

void addNode(struct Node_s** nodes, struct Node_s* node_to_add)
{
	if (!*nodes)
	{
		*nodes = node_to_add;
		return;
	}
	
	node_to_add->m_next = *nodes;
	*nodes = node_to_add;

}

struct Node_s** createNodesarrayFromPixelarray(uint8_t* pixelarray, int width, int height)
{
	struct Node_s** nodesarray = (struct Node_s**)calloc(height*width, sizeof(struct Node_s*));
	for (size_t i = 0; i < width*height; i++)
		nodesarray[i] = createNodeFromPixel(i, i / width, i % width, pixelarray[i]);

	addNeighboursToNodesarray(nodesarray, width, height);

	return nodesarray;
}

struct Edge_s* createEdge(struct Node_s* node, int distance)
{
	struct Edge_s* edge = (struct Edge_s*)calloc(1, sizeof(struct Edge_s));

	if (edge)
	{
		edge->m_node = node;
		edge->m_distance = distance;
	}

	return edge;
}

void addEdge(struct Edge_s** edge, struct Edge_s* edge_to_add)
{
	if (!*edge)
	{
		*edge = edge_to_add;
		return;
	}

	edge_to_add->m_next = *edge;
	*edge = edge_to_add;
}

void addNeighboursToNode(struct Node_s* A, struct Node_s* B, int distance)
{
	addEdge(&A->m_neighbours, createEdge(B, distance));
	A->m_nbneighbours++;
}

struct Node_s* getNodeFromNodesarray(struct Node_s** nodesarray, int row, int col, int width)
{
	return nodesarray[row * width + col];
}

int getDistanceBetweenNodes(struct Node_s* A, struct Node_s* B)
{
	int dy = A->m_row - B->m_row;
	int dx = A->m_col - B->m_col;
	int dz = A->m_color - B->m_col;

	 
}

struct Node_s* getNodeFromGraph(struct Graph_s* graph, int row, int col)
{
	int idx = row * graph->m_width + col;
	struct Node_s* cursor = graph->nodelist;
	while (idx >= 0)
	{
		cursor = cursor->m_next;
		idx--;
	}

	return cursor;
}