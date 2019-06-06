#include "myimgproc.h"

using namespace cv;
using namespace std;

Mat Myimgproc::fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Mat Myimgproc::img_tracking;
Ptr<BackgroundSubtractor> Myimgproc::pMOG2; //MOG2 Background subtractor

//used by findContours method
vector<vector<Point>> Myimgproc::contours;
vector<Vec4i> Myimgproc::hierarchy;

//used to extract maze lines
Mat Myimgproc::src_HSV;
Mat Myimgproc::dst;


/*	@!bief : enable background subtraction
 *	@return : void
 */
void Myimgproc::init()
{
	//create Background Subtractor objects
	pMOG2 = createBackgroundSubtractorMOG2(500, 150.0, false); //MOG2 approach
}

/*	@!bief : Create all opencv windows to show results 
 *	@return : void
 */
void Myimgproc::createAllWindows()
{
	//global windows
	namedWindow("src"       , WINDOW_NORMAL);
	namedWindow("dst"       , WINDOW_NORMAL);
	namedWindow("Frame"     , WINDOW_NORMAL);
	namedWindow("BS_MOG_2"  , WINDOW_NORMAL);
}

/*  @!brief : Get tank position
 *	mark it with contour 
 *  uses background substraction algorithm
 *
 *	@param frame : newest frame from camera
 *	@return : tuple of center point of tank rectangle contour and angle
 */
tuple<Point2i, int> Myimgproc::processImages(Mat & frame)
{
	//update the background model
	pMOG2->apply(frame, fgMaskMOG2);

	/*noise reduction routine*/
	Mat element_erosion = getStructuringElement(MORPH_RECT,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	Mat element_dilation = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));

	erode(fgMaskMOG2, fgMaskMOG2, element_erosion);
	dilate(fgMaskMOG2, fgMaskMOG2, element_dilation);
	dilate(fgMaskMOG2, fgMaskMOG2, element_dilation);
//	dilate(fgMaskMOG2, fgMaskMOG2, element_dilation);
	/*-----------------------*/

	//find tank contour
	findContours(fgMaskMOG2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	// vector for rectangles
	vector<RotatedRect> minRect(contours.size());

	//draw all found countours 
	for (int i = 0; i < contours.size(); i++)
	{
		//uncomment if precise contour is needed
		Scalar color = Scalar(0,255,128);
		drawContours(frame, contours, i, color, 8, LINE_8, hierarchy, 0);
		
		Scalar color2 = Scalar(0, 0, 255);		//red
		
		// Find the rotated rectangle for each contour
		minRect[i] = minAreaRect(Mat(contours[i]));
		//4 points of rectangle
		Point2f rect_points[4];
		//draw rectangle
		minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
		{
			line(frame, rect_points[j], rect_points[(j + 1) % 4], color2, 2, LINE_8);
		}
		
		//TODO: get rid of noise rectangles, calculate angle of tank in range 0-360 deg
		cout << minRect[i].angle << endl;
	}
		
	//show the current frame and the fg masks
	imshow("Frame", frame);
	imshow("BS_MOG_2", fgMaskMOG2);

	if (minRect.size() != 0)
		return	make_tuple(minRect[0].center, minRect[0].angle);
	else
		return make_tuple(Point(0, 0), 0);
}

/*	@!bief: Extract maze lines from first image
 *			and draw them 
 *	@param frame : 
 *	@return : void
 */	
void Myimgproc::draw_maze(Mat & frame)
{
	//define min and max values for color extraction 
	const int max_value_H = 105;
	const int max_value = 220;
	int low_H = 90, low_S = 60, low_V = 50;
	int high_H = max_value_H, high_S = max_value, high_V = max_value;
	
	//change image from BGR to HSV & extract color
	cvtColor(frame, src_HSV, COLOR_BGR2HSV);
	 inRange(src_HSV, Scalar(0,0,0), Scalar(180, 255, 30), dst);
	//inRange(src_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), dst);

	//dilation for reducing number of contours
	int dilation_size = 8;
	Mat element_dilation = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(dst, dst, element_dilation);
	erode(dst, dst, element_dilation);
	erode(dst, dst, element_dilation);

	//find all contours
	findContours(dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//draw each contour separately
	for (int i = 0; i < contours.size(); i++)
	{
		cout << i;
		Scalar color = Scalar(0, 0, 255);
		drawContours(frame, contours, i, color, 2, LINE_8, hierarchy, 0);
	}

	//show results
	imshow("src", frame);
	imshow("dst", dst);
}

/*	@!brief : 
 *	
 *	@return : pointer to initialized Maze object  
 */
Maze * Myimgproc::create_graph2()
{
	//sliding window params
	int window_rows = 30;
	int window_cols = window_rows;
	int step = window_rows;

	//dst with grid
	Mat grid = dst.clone();

	//need vector because we dont know how many there will be nodes...
	vector<Graph_Node*> all_nodes;

	//iterate over image with sliding window
	for (int row = 0; row <= dst.rows - window_rows; row += step)
	{
		for (int col = 0; col <= dst.cols - window_cols; col += step)
		{
			Rect windows(col, row, window_rows, window_cols);
			if (check_empty(windows, dst))
			{
				//draw
				rectangle(grid, windows, Scalar(255), 1, 8, 0);
				printf("Top-left: (%i %i)		bottom-right: (%i %i)\n",
					windows.tl().x, windows.tl().y, windows.br().x, windows.br().y);
				//draw point just to show nodes
				Point2i curr_point = middle_point(windows);
				circle(grid, curr_point, 1, Scalar(255), -1);

				//init node
				all_nodes.push_back(new Graph_Node(curr_point));
			}
			else
			{
				//draw
				rectangle(grid, windows, Scalar(127), -1);
			}
		}
	}
	imshow("Frame", grid);
	
	//Create adjacent table for each Node
	for (int i = 0; i < all_nodes.size(); ++i)
	{
		all_nodes[i]->fill_adjacent_table(all_nodes);
		all_nodes[i]->print_graph();
	}
	//init maze
	Maze * maze = new Maze(all_nodes);
	return maze;
}

/*	@!brief : 
 *	for each rectangle 
 *	check if it intersects with maze line
 *	return true if not intersect
 *	
 *	@param windows : reference to 
 *	@param frame : reference to 
 */
bool Myimgproc::check_empty(Rect & windows, Mat & frame)
{
	int col_left  = windows.tl().y;
	int col_right = windows.br().y;
	int row_left  = windows.tl().x;
	int row_right = windows.br().x;
	
	//w/o crashes at right edge detect in 2nd for loop
	if (row_right == 640)
	{
		row_right -= 1;
	}
	if (col_right == 480)
	{
		col_right -= 1;
	}

	printf("EMPTY:: Top-left: (%i %i)		bottom-right: (%i %i)\n",
		windows.tl().x, windows.tl().y, windows.br().x, windows.br().y);
	//horizontal rect lines
	for (int col = windows.tl().x; col < windows.br().x; ++col)
	{
			//upper edge
		if ( (int)frame.at<uchar>(Point2i(col, col_left)) ||
			//lower edge
			 (int)frame.at<uchar>(Point2i(col, col_right)) )
		{
			cout << "found!";
			return false;
		}
		
	}
	//vertical rect lines
	for (int row = windows.tl().y; row < windows.br().y; ++row)
	{
			//left edge
		if ( (int)frame.at<uchar>(Point2i(row_left, row)) ||
			//right edge
			 (int)frame.at<uchar>(Point2i(row_right, row)) )
		{
			cout << "found edge!";
			return false;
		}
	}
	//means that rectangle represent road
	return true;
}

Point2i Myimgproc::middle_point(Rect & windows)
{
	return Point2i(windows.tl().x + windows.width / 2 , windows.tl().y + windows.width / 2);
}




/* Makes 3 channel histogram
 * Shows each channel and combined histogram
 * NOT IMPORTANT NOW
 */
void histogram_debug(void)
{
	Mat src;
	src = imread("data/img8.jpg", 1);

	// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	// Establish the number of bins
	int histSize = 256;

	// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	//threshold
	threshold(bgr_planes[0], bgr_planes[0], 130, 255, 0);
	threshold(bgr_planes[2], bgr_planes[2], 160, 255, 0);


	// Display
	namedWindow("calcHist Demo", WINDOW_NORMAL);
	namedWindow("Blue channel", WINDOW_NORMAL);
	namedWindow("Green channel", WINDOW_NORMAL);
	namedWindow("Red channel", WINDOW_NORMAL);
	namedWindow("Real", WINDOW_NORMAL);

	imshow("calcHist Demo", histImage);
	imshow("Blue channel", bgr_planes[0]);
	imshow("Green channel", bgr_planes[1]);
	imshow("Red channel", bgr_planes[2]);
	imshow("Real", src);

}

/*
 *  Get graph nodes
 *  find maze walls and extend them to full screen (canny + houghlines)
 *  find lines which are between maze walls
 *  find line crossings
 *  create Node objects and initialize them with found points
 */
 /*
 void Myimgproc::create_graph(Mat & frame)
 {
	 //detecting maze lines
	 vector<Vec2f> lines;

	 //lines in cartesian system
	 vector<pair<Point, Point>> lines_cart;
	 vector<pair<Point, Point>> lines_cart_vertical;
	 vector<pair<Point, Point>> lines_cart_horizontal;

	 //calculate lines
	 Canny(dst, dst, 100, 300, 3);
	 HoughLines(dst, lines, 1, CV_PI / 180, 170, 0, 0);

	 //iterate over lines in polar system
	 //reduce amount of lines by averaging similar lines

	 int polar_sum = 0;
	 for (size_t i = 0; i < lines.size(); i++)
	 {
		 polar_sum += lines[i][0];
		 if ()
		 lines.erase(lines.begin() + i);
	 }


	 //iterate over lines in polar system and change them to cartesian
	 //divide them into horizontal and vertical
	 for (size_t i = 0; i < lines.size(); i++)
	 {
		 //lines are in polar coordinate system
		 float rho = lines[i][0];
		 float theta = lines[i][1];

		 //change to cartesian coordinate system
		 Point pt1, pt2;
		 double a = cos(theta), b = sin(theta);
		 double x0 = a * rho, y0 = b * rho;
		 pt1.x = cvRound(x0 + 2000 * (-b));
		 pt1.y = cvRound(y0 + 2000 * (a));
		 pt2.x = cvRound(x0 - 2000 * (-b));
		 pt2.y = cvRound(y0 - 2000 * (a));

		 //adding line to general vector with lines in cartesian coordinate system
		 lines_cart.push_back(make_pair(pt1, pt2));

		 //determine whether line is vertical (slope > 0) or horizontal (slope ~= 0)
		 //calculating slope with formula:
		 // slope = (y2 - y1) / (x2 - x1)
 //TODO : CAN OCCUR ERROR IF X2 - X1 = 0 - MAKE EXCEPTION
		 int slope = (pt2.y - pt1.y) / (pt2.x - pt1.x);

		 //check if it is horizontal
		 if (slope < 0.5 && slope > -0.5)
		 {
			 //add to horizontal lines
			 lines_cart_horizontal.push_back(make_pair(pt1, pt2));
			 printf("horizontal -- Cart: (%d %d) (%d %d)		Polar: (%f %f)		Slope: %i\n"
				 , pt1.x, pt1.y, pt2.x, pt2.y, lines[i][0], lines[i][1], slope);

		 }
		 //line is vertical
		 else
		 {
			 //add to vertical lines
			 lines_cart_vertical.push_back(make_pair(pt1, pt2));
		 printf("vertical -- Cart: (%d %d) (%d %d)		Polar: (%f %f)		Slope: %i\n"
			 ,pt1.x, pt1.y, pt2.x, pt2.y, lines[i][0], lines[i][1], slope);
		 }

		 //drawing
		 line(frame, pt1, pt2, Scalar(0, 255, 0), 8, LINE_8);

	 }


	 //thresholding values
	 int min_dist = 40;
	 int max_dist = 100;

	 // iterate over vertical lines in cartesian system
	 // to find lines which will be in the middle of maze path
	 for (int i = 0; i < lines_cart_vertical.size(); i++)
	 {




	 }

	 //print all created lines
	 imshow("src", frame);
 }
 */