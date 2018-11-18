#pragma once
#ifndef MAZE_H
#define MAZE_H

#include<iostream>
#include<opencv2/opencv.hpp>

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
};


/*
 * Finds way out of defined maze
 * uses Dikstra algorythm to solve problem
 */
class Maze
{
private:
	//list of all Graph_Nodes
	std::vector<Graph_Node*> * graph;
	Graph_Node * start_node;
	Graph_Node * end_node;

public:
	//find start and end node
	Maze(std::vector<Graph_Node*> *);

	~Maze();
	std::vector<Graph_Node*> use_dikstra();
	std::vector<Graph_Node*> use_a_star();
	void draw_solution(std::vector<Graph_Node*>);
};



#endif // MAZE_H