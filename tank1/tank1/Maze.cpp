#include "Maze.h"
#include <math.h>

Graph_Node::Graph_Node(cv::Point2i & point)
{
	center = point;
}

void Graph_Node::fill_adjacent_table(vector<Graph_Node*> & all_nodes)
{
	vector<pair<Graph_Node*, int> > adjacent;

	//iterate over all nodes to find adjacent ones
	for (int i = 0; i < all_nodes.size(); ++i)
	{
		int distance = (int)this->getDistance(all_nodes[i]);
		//ignore self
		if ( distance == 0 || distance > 120)
		{
			continue;
		}
		//add to adjacent table
		else
		{
			adjacent.push_back(make_pair(all_nodes[i], distance));
		}
	}
	copy(adjacent.begin(), adjacent.end(), back_inserter(this->adjacent_table));
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

vector<pair<Graph_Node*, int>>* Graph_Node::getAdjTable()
{
	return &adjacent_table;
}




Maze::Maze(vector<Graph_Node*> & all_nodes) 
{
	vector<Graph_Node*> * g = &all_nodes;
	unvisited = new vector<Graph_Node*> (*g);
	try
	{
		if (unvisited->size() == 0 || unvisited == nullptr)
			throw "unvisited table not initiated";
	
	//iterate over all nodes
	//first encountered node in 1st line is end node 
	//first encountered node in last line is start node 
		for (int i = 0; i < g->size(); ++i)
		{
			if (g -> operator[](i)->getCenter()->y == 40 && end_node == nullptr)
			{
				end_node = g -> operator[](i);
			}
			else if (g -> operator[](i)->getCenter()->y == 1000) //hardcoded last sliding window line!!
			{
				start_node = g -> operator[](i);
				break;
			}
		}
		if (end_node == nullptr || start_node == nullptr)
			throw "Start or end point not found!";

		visited = new vector<Graph_Node*>[g->size()];

		//set all distances to INFINITY
		for (vector<Graph_Node*>::iterator it = g->begin(); it != g->end(); ++it)
		{
			distance.insert(make_pair(*it, make_pair(INF, nullptr)));
		}
		
		if (distance.size() == 0)
			throw "Distance table not initiated";
	}
	catch (const char * msg)
	{
		cerr << msg;
	}
}

Maze::~Maze()
{
}

vector<Graph_Node*> Maze::use_dikstra()
{
	typedef pair<Graph_Node*, int> GPair;

	/* priority queue for dikstra algorithm
	   stores 
	*/
	priority_queue< GPair, vector<GPair>, Compare_Node> pq;
	
	/*
	throw start_node to visited vector
	delete from unvisited 
	set distance to 0 for starting node
	*/
	for (auto it = unvisited->begin(); it != unvisited->end(); ++it)
	{
		if (*it == start_node)
		{
			visited->push_back(*it);
			distance.at(*it).first = 0;
			pq.push(make_pair(*it,distance[*it].first));		//add to priority queue
			unvisited->erase(it);
			break;
		}
	}

	Graph_Node * top_node;

	/*
		loop until all nodes are visited
		5. pop top node from pq
	*/
	while (unvisited->size() != 0)
	{
		top_node = pq.top().first;		
		//1. get top Node from priority queue
		//loop on adjacecent nodes to top node
		for (vector<GPair>::iterator it = top_node->getAdjTable()->begin(); it != top_node->getAdjTable()->end(); ++it)
		{
			//distance to node in which i am NOW + what i see ahead is less then distance i know then update
			if (distance[top_node].first + it->second < distance[it->first].first)
			{
				distance[it->first].first = distance[top_node].first + it->second;		//2. update distance to nodes
				distance[it->first].second = top_node;									//2.1 remember previous node
			}

			//check if node is in unvisited
			if (find(unvisited->begin(), unvisited->end(), it->first) != unvisited->end())
			{
				pq.push(*it);  															//3. add unvisited node to pq
			}
		}

		
		visited->push_back(top_node);													//4. mark node as visited
		//delete node from unvisited
		auto it = find(unvisited->begin(), unvisited->end(), top_node);
		if (it != unvisited->end())
		{
			unvisited->erase(it);								//5. delete from unvisited
		}

		pq.pop();																		//6. delete top_node from pq
		printf("visited size: %zi\n", visited->size());
	}



	return vector<Graph_Node*>();
}

vector<Graph_Node*> Maze::use_a_star()
{
	return vector<Graph_Node*>();
}

void Maze::draw_solution(cv::Mat& img)
{
	Graph_Node * node = end_node;
	Graph_Node * next = distance.at(end_node).second;
	while (true)
	{
		//check if arrived to start_node
		if (next == nullptr)
		{
			//draw last line from node to start node
			cv::line(img, *(node->getCenter()), *(start_node->getCenter()), cv::Scalar(255, 0, 0), 10, 8, 0);

			break;
		}
		else
		{
			//draw line from node to node
			cv::line(img, *(node->getCenter()), *(next->getCenter()), cv::Scalar(255,0,0), 10, 8, 0);
			
			//swap to new nodes 
			node = next;
			next = distance.at(node).second;		

			//cv::imshow("src", img);

		}

	}
}

