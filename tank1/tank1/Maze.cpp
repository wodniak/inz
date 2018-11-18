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




Maze::Maze()
{

}


Maze::~Maze()
{
}

