#pragma once
#ifndef MAZE_H
#define MAZE_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include <algorithm>

#define INF 0x3f3f3f3f 

using namespace std;
/*
 * Single Node in graph
 * Include list of adjacent Graph_Nodes with distance to them
 */
class Graph_Node
{
private:
	cv::Point2i center;		//Node's coordinates
	vector<pair<Graph_Node*, int >> adjacent_table;		//adjacent Nodes with distance to them

public:
	/**
	 *	@brief	:	Create Node - define center point
	 */
	Graph_Node(cv::Point2i & point);

	/**
	 *	@brief	:	Look for adjacent Graph_Nodes and add them to adjacent_table
	 *	@return	:	void
	 */
	void fill_adjacent_table(vector<Graph_Node*> &);
	
	/**
	 *	@brief	:	Calculate distance between 2 Graph_Nodes
	 *	@return :	Distance
	 */
	double getDistance(Graph_Node * next);
	
	void print_graph();

	/**
	 *	@brief	:	Getter for center point		
	 *	@return	:	Node's center
	 */
	cv::Point2i * getCenter();

	/**
	 *	@brief	:	Getter for adjacent_table
	 *	@return	:	Node's neighbours
	 */
	vector<pair<Graph_Node*, int >> * getAdjTable();
};


/*
 * Finds way out of defined maze
 * uses Dikstra algorythm to solve problem
 */
class Maze
{
private:
	//first and last node in maze
	Graph_Node * start_node;
	Graph_Node * end_node;

	//next node to be visited by tank
	Graph_Node * curr_node;

	//checklist
	vector<Graph_Node*> * unvisited;
	vector<Graph_Node*> * visited;
	
	//edges of graph
	map<Graph_Node*, pair<int,Graph_Node*>> distance;

public:
	/**
	 *	@brief	:	Find start and end node
	 */
	Maze(vector<Graph_Node*> &);
	~Maze();

	/**
	 *	@brief	:	Draw way out of a maze
	 *	@return :	void
	 */
	void draw_solution(cv::Mat&);

	/**
	 *	@brief	:	Use Diktra algorithm to find way out of maze
	 *	@return :	vector with nodes in proper order
	 */
	vector<Graph_Node*> use_dikstra();
	vector<Graph_Node*> use_a_star();

	/**
	 *	@brief	:	Calculate distance from tank center to line
					between 2 nearest nodes
	 *	@return :	tuple with distance to closest node, perpendicular distance to line, turn direction
	 */
	tuple<double, double, bool, int> calc_dist_to_line(cv::Point2i & tank_pos, int &angle);

	void draw_line_to_node(cv::Mat & frame, cv::Point2i & tank_pos);
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

/*	@!brief : calculate linear coeffs connecting 2 points (Ax + Bx + C = 0)
 *	@param p1 : coordinates of first node in graph
 *	@param p2 : coordinates of second node in graph
 *	@return : tuple with line coeffs 
 */
tuple<double, double, double> calc_line_coeffs(cv::Point2i * p1, cv::Point2i * p2);


#endif // MAZE_H