#include "Maze.h"


Graph_Node::Graph_Node(cv::Point2i & point)
{
	center = point;
	//at moment of creating this Graph_Node object other Graph_Node objects are not created
	adjacent_table = nullptr;
}

void Graph_Node::fill_adjacent_table(std::vector<Graph_Node> & all_nodes)
{

}

int Graph_Node::getDistance(Graph_Node & next)
{
	return 0;
}

void Graph_Node::print_graph()
{
}




Maze::Maze()
{

}


Maze::~Maze()
{
}

