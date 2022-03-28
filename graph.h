#pragma once

#include "utils_SDL.h"
#include <stdint.h>
#include <stdbool.h>

struct Node_s {
	int m_index;
	int m_row;
	int m_col;
	unsigned char m_color;
	struct Edge_s* m_neighbours;
	int m_nbneighbours;
	bool flag;
	int m_pathvalue;

	struct Node_s* m_next;
};

struct Edge_s {
	struct Node_s* m_node;
	float m_distance;
	
	struct edge_s* m_next;
};

struct Graph_s {
	struct Node_s* nodelist;
	struct Node_s** nodesarray;
	int m_nbnodes;
	int m_width;
};

struct Graph_s* createGraphFromSurface(SDL_Surface* surface);
struct Node_s* createNodeFromPixel(int idx, int row, int col, uint8_t pixel);
struct Node_s** createNodesarrayFromPixelarray(uint8_t* pixelarray, int width, int height);
void addNode(struct Node_s** nodes, struct Node_s* node_to_add);
void addEdge(struct Edge_s** edge, struct Edge_s* edge_to_add);
void addNeighboursToNode(struct Node_s* A, struct Node_s* B, int distance);
struct Edge_s* createEdge(struct Node_s* node, int distance);
struct Node_s* getNodeFromNodesarray(struct Node_s** nodesarray, int row, int col, int width);
void addNeighboursToNodesarray(struct Node_s** nodesarray, int width, int height);
uint8_t* getPixelsarrayFromSurface(SDL_Surface* surface);
struct Node_s** createAntecedentsarray(struct Node_s* A, int size);
struct Node_s* getNodeFromGraph(struct Graph_s* graph, int row, int col);
bool* createTrackingarray(struct Node_s* A, int size);
int* createDistancearray(struct Node_s* A, int size);
struct Node_s* getNodeWithMinimalDistance(struct Node_s** nodesarray, int* distancearray, int size);
int mapValue(int value, int heighest_value, int lowest_value);

void getShortestpath(struct Graph_s* graph, struct Node_s* A, struct Node_s* B, SDL_Surface* surface, SDL_Surface* window);
void getShortestpathV2(struct Graph_s* graph, struct Node_s* A, struct Node_s* B, struct Node_s* S, struct Node_s** antecedentsarray, int* distancearray, struct Edge_s** to_draw);
