#pragma once
#include <Windows.h>

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include "ui_tankGUI.h"

#include "myimgproc.h"
#include "tank.h"
#include "Maze.h"

class tankGUI : public QMainWindow
{
	Q_OBJECT

public:
	tankGUI(QWidget *parent = Q_NULLPTR);
	void showOnLabel(QLabel * label, Mat * img);


public slots:
	void runButton_onClick();
	void getColorImage_onClick();

protected:
	virtual void mousePressEvent(QMouseEvent *event);

private:
	Ui::tankGUIClass ui;

	char port_name[9] = "\\\\.\\COM4";	//COM port name
	Tank * tank;		//handle to robot
	VideoCapture * cap;	//handle to camera
	Mat * frame;		//current frame from camera
	Maze * maze;

	Scalar color1;
	int frame_number = 0;
	Point2i tank_position;
	int angle;
	double dist_to_line, cross_track_error;
	bool tank_turn_right;
};
