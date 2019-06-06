#include "tankGUI.h"
#include <opencv2/imgproc.hpp>

const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", window_detection_name, high_V);
}

tankGUI::tankGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//connect with tank
	tank = new Tank(port_name);
	QPixmap pixmap(ui.tankReadyLabel->size());
	if (tank->isConnected()) {
		cout << "Connection Established" << endl;
		QColor color(0, 255, 0);
		pixmap.fill(color);
	} else {
		QColor color(255, 0, 0);
		pixmap.fill(color);
		cout << "ERROR, check port name";
	}
		ui.tankReadyLabel->setPixmap(pixmap);

	Myimgproc::createAllWindows();	//open all opencv windows 
	cap	  = new VideoCapture(0);	//camera handle
	frame = new Mat();

	if (!cap->isOpened())		// Check if camera opened successfully 
	{
		cout << "Error opening video stream or file" << endl;
		assert(false);
	}

	//get 1st frame, show it and wait for user to click on it
	cap->operator>>(*frame);
	Mat src_HSV;
	cvtColor(*frame, src_HSV, COLOR_BGR2RGB);
	ui.imageLabel->setPixmap(QPixmap::fromImage(
		QImage(src_HSV.data, src_HSV.cols, src_HSV.rows, src_HSV.step, QImage::Format_RGB888)));
	Myimgproc::init();

	//set connections
	connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runButton_onClick()));
	connect(ui.getColorButton, SIGNAL(clicked()), this, SLOT(getColorImage_onClick()));
}

void tankGUI::mousePressEvent(QMouseEvent *event)
{
	QPoint pos = mapToParent(event->pos());
	double x = pos.x();
	double y = pos.y();

	if (x > ui.imageLabel->pos().rx() && y > ui.imageLabel->pos().ry()) {
		x = x - ui.imageLabel->pos().rx();
		y = y - ui.imageLabel->pos().ry();

		QImage img = ui.imageLabel->pixmap()->toImage();
		QColor hsvColor = img.pixel(x, y);
		QPixmap pixmap(ui.imageLabel->size());
		pixmap.fill(hsvColor);
		ui.extractedColorLabel->setPixmap(pixmap);

		int r, g, b;
		hsvColor.getHsv(&r, &g, &b);
		color1 = Scalar(b, g, r); // swap RGB-->BGR
	}
}

void tankGUI::showOnLabel(QLabel * label, Mat * img)
{
	//showing Mat in QLabel
	//cvtColor(*img, *img, 2);	//CV_BGR2RGB probably
	label->setPixmap(QPixmap::fromImage(
		QImage(img->data, img->cols, img->rows, img->step, QImage::Format_RGB888)));
}


//
void tankGUI::getColorImage_onClick()
{/*
	namedWindow(window_capture_name);
	namedWindow(window_detection_name);
	// Trackbars to set thresholds for HSV values
	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
	Mat frame_HSV, frame_threshold;

		// Convert from BGR to HSV colorspace
		cvtColor(*frame, frame_HSV, COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
		// Show the frames
		imshow(window_capture_name, *frame);
		imshow(window_detection_name, frame_threshold);
 */

	
	//change image from BGR to HSV & extract color
	Mat src_HSV, dst;
	Scalar color2 = Scalar(
		color1[0] + 50,
		250,
		250 );

	cvtColor(*frame, src_HSV, COLOR_BGR2HSV);

	//inRange(src_HSV, Scalar(72,0,0), Scalar(250, 250, 250), dst);
	inRange(src_HSV, Scalar(70,0,0), Scalar(120,250,250), dst);
	imshow("dst", dst);

	//dilation for reducing number of contours
	int dilation_size = 2;
	Mat element_dilation = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(dst, dst, element_dilation);
	
	erode(dst, dst, element_dilation);
	erode(dst, dst, element_dilation);
	erode(dst, dst, element_dilation);
	erode(dst, dst, element_dilation);

	//imshow("src", dst);

	ui.contoursLabel->setPixmap(QPixmap::fromImage(
	QImage(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Indexed8)));
	

	cap->operator>>(*frame);
	Mat hh;
	cvtColor(*frame, hh, COLOR_BGR2HSV);
	//ui.imageLabel->setPixmap(QPixmap::fromImage(
	//	QImage(hh.data, hh.cols, hh.rows, hh.step, QImage::Format_RGB888)));

	Myimgproc::dst = dst;
}



void tankGUI::runButton_onClick()
{
	//create maze and calculate shortest path
	maze = Myimgproc::create_graph2();
	maze->use_dikstra();
	maze->draw_solution(*frame);

	int corner;

	//loop until end of video
	while (tank->isConnected())
	{
		if (frame_number % 3 == 0)			//every nth frame update steering
		{
			//STEERING ROUTINE
			tie(tank_position, angle) = Myimgproc::processImages(*frame);			//keep track of tank center & angle
			tie(dist_to_line, cross_track_error, tank_turn_right, corner) = maze->calc_dist_to_line(tank_position, angle);		//calc distance to node
			//tank->steer_auto(cross_track_error, tank_position, tank_turn_right, corner);		//send steering values to robot

			maze->draw_line_to_node(*frame, tank_position);		//just draw line
			maze->draw_solution(*frame);
			this->showOnLabel(ui.imageLabel, frame);
		}
		tank->steer();
		//get next frame
		frame_number++;
		cap->operator>>(*frame);
		waitKey(100);		//wait between frames
	}
}


