#pragma once
#ifndef MAZE_H
#define MAZE_H

#include<iostream>
#include<opencv2/opencv.hpp>

#define INF 0x3f3f3f3f 
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
	std::vector<std::pair<Graph_Node*, int >> adjacent_table;

public:
	//create Graph_Node - define center point
	Graph_Node(cv::Point2i & point);

	//looks for adjacent Graph_Nodes and add them to adjacent_table
	void fill_adjacent_table(std::vector<Graph_Node*> &);
	
	//calculate distance between 2 Graph_Nodes
	double getDistance(Graph_Node * next);
	
	void print_graph();

	//getter for center point
	cv::Point2i * getCenter();

	//getter for adjacent_table
	std::vector<std::pair<Graph_Node*, int >> * getAdjTable() { return &adjacent_table; };
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
	std::vector<Graph_Node*> * unvisited;
	std::vector<Graph_Node*> * visited;
	
	//edges
	std::map<Graph_Node*, int> distance;

public:
	//find start and end node
	Maze(std::vector<Graph_Node*> *);
	~Maze();

	//looks for
	std::vector<Graph_Node*> find_closest();
	
	//draw way out of maze
	void draw_solution(std::vector<Graph_Node*>);

	//solving maze
	std::vector<Graph_Node*> use_dikstra();
	std::vector<Graph_Node*> use_a_star();
};



#endif // MAZE_H