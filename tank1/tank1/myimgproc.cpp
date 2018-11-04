#include "myimgproc.h"

using namespace cv;
using namespace std;

void processImages(const char* firstFrameFilename)
{
	//create GUI windows
	namedWindow("Frame"   , WINDOW_NORMAL);
	namedWindow("BS_MOG_2", WINDOW_NORMAL);
	//namedWindow("Tracking", WINDOW_NORMAL);

	Mat frame; //current frame
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	Mat img_tracking;
	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor

	//create Background Subtractor objects
	pMOG2 = createBackgroundSubtractorMOG2(500, 150.0, false); //MOG2 approach

	//current image filename
	string fn(firstFrameFilename);
	
	//frame info
	int frameNumber = 0;
	string prefix = "data/img";
	string suffix = ".jpg";

	//read the first file of the sequence
	frame = imread(firstFrameFilename);
	if (!frame.data) 
	{
		//error in opening the first image
		cerr << "Unable to open first image frame: " << firstFrameFilename << endl;
		waitKey(0);
		exit(EXIT_FAILURE);
	}

	int a = 1; //input from keyboard
	for(int i = 1; i < 15; i++)
	{
		//update the background model
		pMOG2->apply(frame, fgMaskMOG2);

		/*noise reduction routine*/
		int erosion_size = 3;
		Mat element_erosion = getStructuringElement(MORPH_RECT,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));

		int dilation_size = 8;
		Mat element_dilation = getStructuringElement(MORPH_RECT,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));

		erode(fgMaskMOG2, fgMaskMOG2, element_erosion);
		dilate(fgMaskMOG2, fgMaskMOG2, element_dilation);
		dilate(fgMaskMOG2, fgMaskMOG2, element_dilation);
		/*-----------------------*/

		/*find tank contour*/
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(fgMaskMOG2, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		
		for (size_t i = 0; i < contours.size(); i++)
		{
		Scalar color = Scalar(0,0,0);
		drawContours(frame, contours, 0, color, 10, LINE_8, hierarchy, 0);
		}
		

		//show the current frame and the fg masks
		imshow("Frame", frame);
		imshow("BS_MOG_2", fgMaskMOG2);

		//search for the next image in the sequence
		ostringstream oss;
		oss << (frameNumber + i);
		string nextFrameNumberString = oss.str();
		string nextFrameFilename = prefix + nextFrameNumberString + suffix;
		
		a = waitKey(30);
		//read the next frame
		frame = imread(nextFrameFilename);
		if (frame.empty()) {
			//error in opening the next image in the sequence
			cerr << "Unable to open image frame: " << nextFrameFilename << endl;
			break;
			//exit(EXIT_FAILURE);
		}
		//update the path of the current frame
		fn.assign(nextFrameFilename);
	}
	waitKey(0);

}

/*Extract maze lines from first image
* and draw them 
*/
void draw_maze()
{
	//define min and max values for color extraction 
	const int max_value_H = 105;
	const int max_value = 220;
	int low_H = 90, low_S = 60, low_V = 50;
	int high_H = max_value_H, high_S = max_value, high_V = max_value;

	//Mat object for raw image in BGR, raw image in HSV, binary image after color extract
	Mat src, src_HSV, dst;
	
	src = imread("data/img0.jpg", 1);

	//change image from BGR to HSV & extract color
	cvtColor(src, src_HSV, COLOR_BGR2HSV);
	inRange(src_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), dst);

	//dilation for reducing number of contours
	int dilation_size = 8;
	Mat element_dilation = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(dst, dst, element_dilation);

	//vectors for contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//draw each contour separately
	for (int i = 0; i < contours.size(); i++)
	{
		cout << i;
		Scalar color = Scalar(0, 0, 255);
		drawContours(src, contours, i, color, 20, LINE_8, hierarchy, 0);
	}

	//show results
	imshow("src", src);
	imshow("dst", dst);
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