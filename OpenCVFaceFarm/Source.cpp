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
CascadeClassifier face_cascade;


void overlay(Mat frame);
void track(Mat frame, MultiTracker& tracker);


//Program start
int main(void)
{
	//basic mats
	Mat frame;
	Mat trackerframe;

	//cam
	VideoCapture feed(video);
	feed.read(frame);


	//init tracker and track container
	//MultiTracker tracker("KCF");
	//std::vector<Rect2d> tracked_objects;
	

	//Load our pre-made xml
	if (!face_cascade.load(face_xml1)) { std::cerr << "Error Loading face_xml\n"; return -1; };
	if (!feed.isOpened()) { std::cerr << "Error opening webcam\n"; return -1; }

	double width = feed.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = feed.get(CV_CAP_PROP_FRAME_HEIGHT);

	//namedWindow("Test", CV_WINDOW_AUTOSIZE);
	//namedWindow("Tracker Window", CV_WINDOW_AUTOSIZE);
	//feed >> trackerframe;
	//selectROI("Tracker Window", trackerframe, tracked_objects);
	//if (tracked_objects.size() <= 0) { return 0; }
	//tracker.add(trackerframe, tracked_objects);
	
	
	//--- INITIALIZE VIDEOWRITER
	VideoWriter writer;
	int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
	double fps = 60.0;                          // framerate of the created video stream
	String filename = "C:/Users/Michael Dalton/Documents/Visual Studio 2015/Projects/OpenCVFaceFarm/Tempdata/alt.avi";             // name of the output video file
	writer.open(filename, codec, fps, frame.size(), true);

	if (!writer.isOpened()) { std::cerr << "couldnt open\n"; return -1; }

	while (true)
	{
		bool capture = feed.read(frame);
		bool trackcapture = feed.read(trackerframe);

		if (!capture || !trackcapture)
		{
			std::cout << "Can't read from feed" << std::endl;
			break;
		}

		overlay(frame);
		writer.write(frame);
		//track(trackerframe, tracker);

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
	

	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		rectangle(frame, faces[i], Scalar(100, 100, 100), 3, 8, 0);
		Mat faceROI = frame_gray(faces[i]);
	}
	//Display on window*/
	//imshow("Test", frame);
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
