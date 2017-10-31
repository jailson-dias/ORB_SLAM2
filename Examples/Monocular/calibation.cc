#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>


#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

#include <fstream>
#include <sstream>


using namespace std;
using namespace cv;

const float calibrationSquareDimension = 0.001; // metros
const float arucoSquareDimension = 0.01; // metros
const Size chessboardDimension = Size(9, 6);

void createArucoMarkers() {
    Mat outputMarker;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    for (int i = 0; i < 50; i++){
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}

void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f> &corners){
    for(int i = 0; i< boardSize.height; i++) {
        for (int j = 0; j< boardSize.width; j++) {
            corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
        }
    }
}

void getChessboardCorners(vector<Mat> images, vector<vector<Point2f> > &allFoundCorners, bool showResults = false) {
    for(vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*iter, Size(9, 6), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
        if (found) {
            allFoundCorners.push_back(pointBuf);
        }

        if (showResults) {
            drawChessboardCorners(*iter, Size(9,6),pointBuf, found);
            imshow("Looking for Corners", *iter);
            waitKey(0);
        }
    }
}

int main() {
    // cout << "teste" << endl;
    // createArucoMarkers();
    // cout << "teste" << endl;
    Mat frame;
    Mat drawToFrame;

    Mat cameraMatrix = Mat::eye(3,3, CV_64F);

    Mat distanceCoefficients;

    vector<Mat> savedImages;

    vector<vector<Point2f> > markerCorners, rejectedCandidates;

    VideoCapture vid(0);

    if(!vid.isOpened()) {
        return 5;
    }

    int framesPerSecond = 20;

    namedWindow("Webcam", CV_WINDOW_AUTOSIZE);

    while(true) {
        if(!vid.read(frame)) {
            break;
        }

        vector<Vec2f> foundPoints;
        bool found = false;

        found = findChessboardCorners(frame, chessboardDimension, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
        frame.copyTo(drawToFrame);
        drawChessboardCorners(drawToFrame, chessboardDimension, foundPoints, found);

        if(found) {
            imshow("Webcam", drawToFrame);

        } else {
            imshow("Webcam", frame);
        }

        char character = waitKey(1000/framesPerSecond);
    }

    return 0;
}