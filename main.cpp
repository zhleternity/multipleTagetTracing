// ObjectTrackingCPP.cpp

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h> // it may be necessary to change or remove this line if not using Windows
#include <io.h>
#include <direct.h>

#include "Blob.h"
#include "videoPro.h"
#include "ImageVideoTrans.h"

#define SHOW_STEPS            // un-comment or comment this line to show steps or not




#if 0
int main(){
	/*
	char* inputDir = "D:/zhlWorkDocs/mot/jpda_m/Data/PETS/PETS09/Images/S2/L2/";
	char* videoName = "S2L2.avi";
	char* outDir = "D:/zhlWorkDocs/video/";
	int frames = ImageToVideo(outDir, videoName, inputDir, 0, 300, 768, 576, "frame_", 24, 1, CAP_PROP_FOURCC);
	std::cout << "total frames " << frames << " have been write to video." << std::endl;
	*/

	char* videoName1 = "D:/zhlWorkDocs/mot/OpenCV_3_Multiple_Object_Tracking_by_Image_Subtraction_Cpp-master/OpenCV_3_Multiple_Object_Tracking_by_Image_Subtraction_Cpp-master/768x576.avi";
	char* outDir1 = "D:/zhlWorkDocs/image_sequences/1/";
	ImageVideoTrans iv;
	int images = iv.VideoToImage(videoName1, outDir1, ".jpg", 100);
	cout << "total frames have been extracted from video." << endl;
    int p;
	std::cin >> p;
	return 0;
}
#endif


#if 1
int main() {

	VideoCapture capVideo;
	VideoProcessor vp;
	char *img_name = new char[500];
	vector<string> imgs;

	cv::Mat imgFrame1;
	cv::Mat imgFrame2;


	vector<Blob> blobs;
#if 0

	capVideo.open("D:/zhlWorkDocs/mot/OpenCV_3_Multiple_Object_Tracking_by_Image_Subtraction_Cpp-master/OpenCV_3_Multiple_Object_Tracking_by_Image_Subtraction_Cpp-master/768x576.avi");

	if (!capVideo.isOpened()) {                                                 // if unable to open video file
		cout << "error reading video file!" << endl << endl;      // show error message
		_getch();                    // it may be necessary to change or remove this line if not using Windows
		return 0;                                                              // and exit program
	}

	if (capVideo.get(CAP_PROP_FRAME_COUNT) < 2) {
		cout << "error: video file must have at least two frames!";
		_getch();
		return 0;
	}
#endif

#if 1
	ImageVideoTrans iv;
	char* inputDir = "D:/zhlWorkDocs/mot/LDCT-master/LDCT-master/data_PETS09S2L1/images/";
	char* videoName = "3.avi";
	char* outDir = "D:/zhlWorkDocs/video/";
	double t = getTickCount();
	int frames = iv.ImageToVideo(outDir, videoName, inputDir, 1, 200, 768, 576, "0", 20, 1, CAP_PROP_FOURCC);
	cout << "ImageToVideo resumes: " << (getTickCount() - t) / getTickFrequency() <<"ms"<< endl;
	std::cout << "total frames " << frames << " have been write to video." << std::endl;
	char out_file[500] = "";
	sprintf(out_file,"%s%s",outDir,videoName);
	capVideo.open(out_file);
	if (!capVideo.isOpened()) {                                                 // if unable to open video file
		cout << "error reading video file!" << endl << endl;      // show error message
		_getch();                    // it may be necessary to change or remove this line if not using Windows
		return 0;                                                              // and exit program
	}

	if (capVideo.get(CAP_PROP_FRAME_COUNT) < 2) {
		cout << "error: video file must have at least two frames!";
		_getch();
		return 0;
	}
	
#endif
	
	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);


	char chCheckForEscKey = 0;

	bool blnFirstFrame = true;

	int frameCount = 2;

	while (capVideo.isOpened() && chCheckForEscKey != 27) {

		vector<Blob> currentFrameBlobs;

		cv::Mat imgFrame1Copy = imgFrame1.clone();
		cv::Mat imgFrame2Copy = imgFrame2.clone();
		//resize(imgFrame1Copy, imgFrame1Copy, cv::Size(480, ((float)imgFrame1Copy.rows / imgFrame1Copy.cols) * 480));
		//resize(imgFrame2Copy, imgFrame2Copy, cv::Size(480, ((float)imgFrame2Copy.rows / imgFrame2Copy.cols) * 480));

		cv::Mat imgDifference;
		cv::Mat imgThresh;
		//grayscal
		cvtColor(imgFrame1Copy, imgFrame1Copy, COLOR_BGR2GRAY);
		imshow("gray1", imgFrame1Copy);

		cvtColor(imgFrame2Copy, imgFrame2Copy, COLOR_BGR2GRAY);
		imshow("gray2", imgFrame2Copy);

		//Gauss blur
		GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
		imshow("blur1", imgFrame1Copy);
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);
		imshow("blur2", imgFrame2Copy);
		
		//diff
		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
		imshow("diff",imgDifference);

		threshold(imgDifference, imgThresh, 30, 255.0, THRESH_BINARY);

		imshow("imgThresh", imgThresh);
		
		cv::Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
		cv::Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, cv::Size(5, 5));
		cv::Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, cv::Size(7, 7));
		cv::Mat structuringElement9x9 = getStructuringElement(MORPH_RECT, cv::Size(9, 9));

		/*
		dilate(imgThresh, imgThresh, structuringElement7x7);
		erode(imgThresh, imgThresh, structuringElement3x3);
		*/
		//morphologic
		//5x5
		dilate(imgThresh, imgThresh, structuringElement5x5);
		imshow("dilate1",imgThresh);
		dilate(imgThresh, imgThresh, structuringElement5x5);
		imshow("dilate2", imgThresh);
		erode(imgThresh, imgThresh, structuringElement5x5);
		imshow("erode", imgThresh);
		//waitKey();

		cv::Mat imgThreshCopy = imgThresh.clone();

		vector<vector<cv::Point> > contours;
		//find CC
		findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		vector<cv::Point> pts;
		Blob bl(pts);

		bl.drawAndShowContours(imgThresh.size(), contours, "imgContours");
		//gt convex hull
		vector<vector<cv::Point> > convexHulls(contours.size());

		for (unsigned int i = 0; i < contours.size(); i++) {
			convexHull(contours[i], convexHulls[i]);
		}
	
		bl.drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		for (auto &convexHull : convexHulls) {
			Blob possibleBlob(convexHull);

			if (possibleBlob.currentBoundingRect.area() > 100 &&
				possibleBlob.dblCurrentAspectRatio >= 0.2 &&
				possibleBlob.dblCurrentAspectRatio <= 1.25 &&
				possibleBlob.currentBoundingRect.width > 20 &&
				possibleBlob.currentBoundingRect.height > 20 &&
				possibleBlob.dblCurrentDiagonalSize > 30.0 &&
				(cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.40) {
				currentFrameBlobs.push_back(possibleBlob);
			}
		}

		bl.drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

		if (blnFirstFrame == true) {
			for (auto &currentFrameBlob : currentFrameBlobs) {
				blobs.push_back(currentFrameBlob);
			}
		}
		else {
			bl.matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		}

		bl.drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

		imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

		bl.drawBlobInfoOnImage(blobs, imgFrame2Copy);

		imshow("imgFrame2Copy", imgFrame2Copy);

		//cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

		// now we prepare for the next iteration

		currentFrameBlobs.clear();

		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

		if ((capVideo.get(CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CAP_PROP_FRAME_COUNT)) {
			capVideo.read(imgFrame2);
		}
		else {
			cout << "end of video\n";
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = cv::waitKey(1);
	}

	if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
		waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);
}
#endif



