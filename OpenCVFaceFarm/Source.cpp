#include <opencv2/core/utility.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/tracking.hpp>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>

using namespace cv;


String face_xml = "C:/Users/Michael Dalton/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
void overlay(Mat frame);
void track(Mat frame, MultiTracker& tracker);


//Program start
int main(void)
{
	//cam
	VideoCapture feed(0);

	//basic mat
	Mat frame;

	//init tracker and track container
	MultiTracker tracker("KCF");
	std::vector<Rect> tracked_objects;

	//Load our pre-made xml
	if (!face_cascade.load(face_xml)) { std::cerr << "Error Loading face_xml\n"; return -1; };

	if (!feed.isOpened()) { std::cerr << "Error opening webcam\n"; return -1; }

	double width = feed.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = feed.get(CV_CAP_PROP_FRAME_HEIGHT);

	namedWindow("Test", CV_WINDOW_AUTOSIZE);
	namedWindow("Tracker", CV_WINDOW_AUTOSIZE);

	while (true)
	{
		bool capture = feed.read(frame);

		if (!capture)
		{
			std::cout << "Can't read from feed" << std::endl;
			break;
		}

		overlay(frame);

		//Manual way to break our loop if stuck
		char c = (char)waitKey(10);
		if (c == 27) { break; }
	}
	return 0;
}

void overlay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		rectangle(frame, faces[i], Scalar(0, 255, 255), 3, 8, 0);
		Mat faceROI = frame_gray(faces[i]);
	}
	//Display on window
	imshow("Test", frame);
}

//read up on move const. for frame. this could be gummy if i get a bunch
//of frame c.b.v's
void track(Mat frame, MultiTracker& tracker)
{
	tracker.update(frame);
	imshow("tracker", frame);
}
