#include "Maze.h"
#include <math.h>

Graph_Node::Graph_Node(cv::Point2i & point)
{
	center = point;
}

void Graph_Node::fill_adjacent_table(std::vector<Graph_Node*> & all_nodes)
{
	std::vector<std::pair<Graph_Node*, int> > adjacent;

	//iterate over all nodes to find adjacent ones
	for (int i = 0; i < all_nodes.size(); ++i)
	{
		int distance = this->getDistance(all_nodes[i]);
		//ignore self
		if ( distance == 0 || distance > 120)
		{
			continue;
		}
		//add to adjacent table
		else
		{
			adjacent.push_back(std::make_pair(all_nodes[i], distance));
		}
	}
	std::copy(adjacent.begin(), adjacent.end(), std::back_inserter(this->adjacent_table));
}

double Graph_Node::getDistance(Graph_Node * next)
{
	return (int)sqrt( pow( (this->center.x - next->center.x), 2) + 
		pow( (this->center.y - next->center.y), 2));
}

void Graph_Node::print_graph()
{
	printf("\nNode Center: (%i,%i)\nAdjacent nodes:\n", this->center.x, this->center.y);

	for (unsigned char i = 0; i < this->adjacent_table.size(); ++i)
	{
		printf("Node : [%p, %i]\n", this->adjacent_table[i].first,
			this->adjacent_table[i].second);
	}
}

cv::Point2i * Graph_Node::getCenter()
{
	return &center;
}



Maze::Maze(std::vector<Graph_Node*> * g) : distance()
{
	unvisited = g;
	
	//iterate over all nodes
	//first encountered node in 1st line is end node 
	//first encountered node in last line is start node 
	for (int i = 0; i < g->size(); ++i)
	{
		if (g -> operator[](i)->getCenter()->y == 40)
		{
			end_node = g -> operator[](i);
			break;
		}
		else if (g -> operator[](i)->getCenter()->y == 40)
		{
			start_node = g -> operator[](i);
			break;
		}
	}

	visited = new std::vector<Graph_Node*>[g->size()];

	//set all distances to INFINITY
	for (std::vector<Graph_Node*>::iterator it = g->begin(); it != g->end(); ++it)
	{
		distance.insert(std::make_pair(*it, INF));
	}
}

Maze::~Maze()
{
}

std::vector<Graph_Node*> Maze::use_dikstra()
{
	typedef std::pair<Graph_Node*, int> GPair;
	/* priority queue for dikstra algorithm
	   stores 
	*/
	std::priority_queue< GPair, std::vector<GPair>, std::greater<GPair>> pq;
	
	/*
	throw start_node to visited vector
	delete from unvisited 
	set distance to 0 for starting node, rest init with infinity
	*/
	for (std::vector<Graph_Node*>::iterator it = unvisited->begin(); it != unvisited->end(); ++it)
	{
		if (*it == start_node)
		{
			visited->push_back(*it);
			unvisited->erase(it);
			distance[*it] = 0;
			pq.push(std::make_pair(*it,distance[*it]));		//add to priority queue
		}
		//init all distances with infinity
		distance[*it] = INF;
	}

	Graph_Node * top_node;

	/*
		loop until all nodes are visited
		1. get top Node in priority queue
		2. update distance to nodes
		3. add nodes with distance to pq
		4. mark node as visited
		5. pop top node from pq
	*/
	while (unvisited->size() != 0)
	{
		//top node
		top_node = pq.top().first;
		
		//update distances
		for (std::vector<GPair>::iterator it = top_node->getAdjTable->begin(); it != top_node->getAdjTable->end(); ++it)
		{
			distance[it->first] = it->second;
			//add next moves to pq
			pq.push(*it);
		}

		//mark node as visited
		visited->push_back(top_node);
		//delete node from unvisited
		for (std::vector<Graph_Node*>::iterator it = unvisited->begin(); it != unvisited->end(); ++it)
		{
			if (*it == start_node)	unvisited->erase(it);
		}

		pq.pop();
	}



	return std::vector<Graph_Node*>();
}

std::vector<Graph_Node*> Maze::use_a_star()
{
	return std::vector<Graph_Node*>();
}

void Maze::draw_solution(std::vector<Graph_Node*>)
{
}

