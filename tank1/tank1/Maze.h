#pragma once
#ifndef MAZE_H
#define MAZE_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include <algorithm>

#define INF 0x3f3f3f3f 

using namespace std;
/*
 * Single Graph_Node in graph
 * Include list of adjacent Graph_Nodes with distance to them
 */
class Graph_Node
{
private:
	//
	cv::Point2i center;
	
	//adjacent Graph_Nodes with distance to them
	vector<pair<Graph_Node*, int >> adjacent_table;

public:
	//create Graph_Node - define center point
	Graph_Node(cv::Point2i & point);

	//looks for adjacent Graph_Nodes and add them to adjacent_table
	void fill_adjacent_table(vector<Graph_Node*> &);
	
	//calculate distance between 2 Graph_Nodes
	double getDistance(Graph_Node * next);
	
	void print_graph();

	//getter for center point
	cv::Point2i * getCenter();

	//getter for adjacent_table
	vector<pair<Graph_Node*, int >> * getAdjTable();
};


/*
 * Finds way out of defined maze
 * uses Dikstra algorythm to solve problem
 */
class Maze
{
private:
	//first and last node
	Graph_Node * start_node;
	Graph_Node * end_node;

	//checklists
	vector<Graph_Node*> * unvisited;
	vector<Graph_Node*> * visited;
	
	//edges
	map<Graph_Node*, pair<int,Graph_Node*>> distance;

public:
	//find start and end node
	Maze(vector<Graph_Node*> &);
	~Maze();

	//draw way out of a maze
	void draw_solution(cv::Mat&);

	//solving maze
	vector<Graph_Node*> use_dikstra();
	vector<Graph_Node*> use_a_star();
};

/*
 * Used in priority queue as a comparator to decide
 * order of nodes to visit
 * smaller distance is prioritized
*/
class Compare_Node
{
	typedef pair<Graph_Node*, int> GPair;

public:
	bool operator()(GPair x, GPair y)
	{
		return x.second > y.second;
	}
};

#endif // MAZE_H