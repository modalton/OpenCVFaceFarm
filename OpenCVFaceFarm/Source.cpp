#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>


#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>

using namespace cv;

String video = "C:/Users/Michael Dalton/Documents/Visual Studio 2015/Projects/OpenCVFaceFarm/Tempdata/Kakkmaddafakka.mp4";
String face_xml = "C:/Users/Michael Dalton/Desktop/opencv_build/install/etc/haarcascades/haarcascade_frontalface_alt.xml";
String face_xml1 = "C:/Users/Michael Dalton/Desktop/opencv_build/install/etc/haarcascades/haarcascade_frontalface_alt2.xml";
String face_xml2 = "C:/Users/Michael Dalton/Desktop/opencv_build/install/etc/haarcascades/haarcascade_frontalface_alt_tree.xml";
String face_xml3 = "C:/Users/Michael Dalton/Desktop/opencv_build/install/etc/haarcascades/haarcascade_frontalface_default.xml";

void overlay(Mat frame, std::vector<CascadeClassifier>& face_cascade);
void track(Mat frame, MultiTracker& tracker);


//Program start
int main(void)
{
	std::vector<CascadeClassifier> face_cascade(4);
	std::vector<String> xmls = {face_xml, face_xml1, face_xml2, face_xml3};
	//cam
	VideoCapture feed(video);

	//basic mats
	Mat frame;
	Mat trackerframe;

	//init tracker and track container
	//MultiTracker tracker("KCF");
	//std::vector<Rect2d> tracked_objects;
	

	//Load our pre-made xml
	//if (!face_cascade.load(face_xml)) { std::cerr << "Error Loading face_xml\n"; return -1; };
	for (int i = 0; i<face_cascade.size(); i++)
	{
		//if (!face_cascade[i].load(xmls[i])) { std::cerr << "Error loading xmls\n"; return -1; }
	}
	if (!feed.isOpened()) { std::cerr << "Error opening webcam\n"; return -1; }

	double width = feed.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = feed.get(CV_CAP_PROP_FRAME_HEIGHT);

	namedWindow("Test", CV_WINDOW_AUTOSIZE);
	//namedWindow("Tracker Window", CV_WINDOW_AUTOSIZE);
	//feed >> trackerframe;
	//selectROI("Tracker Window", trackerframe, tracked_objects);
	//if (tracked_objects.size() <= 0) { return 0; }
	//tracker.add(trackerframe, tracked_objects);

	while (true)
	{
		bool capture = feed.read(frame);
		bool trackcapture = feed.read(trackerframe);

		if (!capture || !trackcapture)
		{
			std::cout << "Can't read from feed" << std::endl;
			break;
		}

		overlay(frame, face_cascade);
		//track(trackerframe, tracker);

		//Manual way to break our loop if stuck
		char c = (char)waitKey(10);
		if (c == 27) { break; }
	} 
	return 0;
}

void overlay(Mat frame, std::vector<CascadeClassifier>& face_cascade)
{ 
	std::vector<std::vector<Rect>> faces(face_cascade.size());
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	for (int i = 0; i<face_cascade.size(); i++)
	{
		face_cascade[i].detectMultiScale(frame_gray, faces[i], 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	}
	
	for (int j = 0; j < faces.size(); j++)
	{
		for (int i = 0; i < faces[j].size; i++)
		{
			Point center(faces[j][i].x + faces[j][i].width / 2, faces[j][i].y + faces[j][i].height / 2);
			rectangle(frame, faces[j][i], Scalar(0, 50*j, 255), 3, 8, 0);
			Mat faceROI = frame_gray(faces[j][i]);
		}
	}
	//Display on window
	imshow("Test", frame);
}

//read up on move const. for frame. this could be gummy if i get a bunch
//of frame c.b.v's
void track(Mat frame, MultiTracker& tracker)
{
	tracker.update(frame);
	for (unsigned i = 0; i < tracker.objects.size(); i++)
	{
		rectangle(frame, tracker.objects[i], Scalar(255, 0, 0), 2, 1);
	}
	imshow("Tracker Window", frame);
}
