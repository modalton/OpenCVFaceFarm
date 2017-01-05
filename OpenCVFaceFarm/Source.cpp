#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/dnn.hpp>


#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace cv;
using namespace cv::dnn;

String video = "C:/Users/Michael Dalton/Documents/Visual Studio 2015/Projects/OpenCVFaceFarm/Tempdata/Kakkmaddafakka.mp4";
String modelTxt = "C:/Users/Michael Dalton/Documents/Visual Studio 2015/Projects/OpenCVFaceFarm/Tempdata/bvlc_googlenet.prototxt";
String modelBin = "C:/Users/Michael Dalton/Documents/bvlc_googlenet.caffemodel";
String img = "C:/Users/Michael Dalton/Documents/Visual Studio 2015/Projects/OpenCVFaceFarm/Tempdata/hongkong.jpg";

void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}
std::vector<String> readClassNames(const char *filename = "synset_words.txt")
{
	std::vector<String> classNames;
	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << filename << std::endl;
		exit(-1);
	}
	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back(name.substr(name.find(' ') + 1));
	}
	fp.close();
	return classNames;
}

//Program start
int main(void)
{
	//basic mat
	Mat frame;

	//cam
	//VideoCapture feed(video);

	Net net = dnn::readNetFromCaffe(modelTxt, modelBin);

	if (net.empty()) { std::cerr << "Cant intialize net" << std::endl; }

	namedWindow("GoogLeNet", CV_WINDOW_AUTOSIZE);

	frame = imread(img);
	if (img.empty()) { std::cerr << "img prob" << std::endl; }

	resize(frame, frame, Size(224, 224));
	dnn::Blob inputBlob = dnn::Blob::fromImages(img);

	net.setBlob(".data", inputBlob);
	net.forward();

	dnn::Blob probLayer = net.getBlob("prob");

	int id;
	double classProb;
	getMaxClass(probLayer, &id, &classProb);

	std::vector<String> classNames = readClassNames();
	std::cout << "Bestest Class:" << classNames.at(id);
	//Lets do one img before vid impl

	/*--- INITIALIZE VIDEOWRITER
	VideoWriter writer;
	int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
	double fps = 60.0;                          // framerate of the created video stream
	String filename = "C:/Users/Michael Dalton/Documents/Visual Studio 2015/Projects/OpenCVFaceFarm/Tempdata/alt_tree.avi";             // name of the output video file
	writer.open(filename, codec, fps, frame.size(), true);

	if (!writer.isOpened()) { std::cerr << "couldnt open\n"; return -1; }

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
	} */


	return 0;
}
