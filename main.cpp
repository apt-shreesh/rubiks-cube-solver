#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin=0, smin=0, vmin=0;
int hmax=179, smax=255, vmax=255;

Scalar lowerWhite(0, 0, 50),upperWhite(179, 30, 180);
Scalar lowerYellow(20, 100, 100),upperYellow(35, 255, 255);
Scalar lowerPink(140, 50, 100),upperPink(170, 255, 255);
Scalar lowerOrange(10, 100, 100),upperOrange(15, 255, 255);
Scalar lowerBlue(90, 50, 100),upperBlue(120, 255, 255);
Scalar lowerGreen(35, 50, 100),upperGreen(85, 255, 255);

vector<string> colors = {"W","Y","P","O","B","G"};
vector<Scalar> lowerColors = {lowerWhite,lowerYellow,lowerPink,lowerOrange,lowerBlue,lowerGreen};
vector<Scalar> upperColors = {upperWhite,upperYellow,upperPink,upperOrange,upperBlue,upperGreen};
/*
void onChange(int val,void* userdata){
    cout << "Trackbar position:" << val << endl;
}
*/

int main(){
    
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cerr << "[ERROR]:Camera unable to be opened" << endl;
        return -1;
    }

    /*
    namedWindow("TrackBar Window",WINDOW_AUTOSIZE);
    
    createTrackbar("HUE min","TrackBar Window",&hmin,179,onChange);
    createTrackbar("HUE max","TrackBar Window",&hmax,179,onChange);
    createTrackbar("Saturation min","TrackBar Window",&smin,255,onChange);
    createTrackbar("Saturation max","TrackBar Window",&smax,255,onChange);
    createTrackbar("Value min","TrackBar Window",&vmin,255,onChange);
    createTrackbar("Value max","TrackBar Window",&vmax,255,onChange);
    */

    Mat frame,mframe,hsvframe,mask,filtered;
    vector<vector<Point>> contours;

    while (true){
        cap >> frame;

        if (frame.empty()){
            cerr << "[ERROR]:Frame empty" << endl;
            return -1;
        }

        flip(frame,mframe,1);

        cvtColor(mframe, hsvframe, COLOR_BGR2HSV);

        for(size_t i=0;i<colors.size();i++){
                inRange(hsvframe,lowerColors[i],upperColors[i],mask);
                //Look up RETR_EXTERNAL & CHAIN_APPROX_SIMPLE
                findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

                for(size_t j=0;j<contours.size();j++){
                    drawContours(mframe,contours,i,upperColors[i],2);
                    Rect box = boundingRect(contours[j]);
                    putText(mframe,colors[i],{box.x,box.y-10},FONT_HERSHEY_COMPLEX,0.6,upperColors[i],2);
                }
        }

        //bitwise_and(mframe,mframe,filtered,mask);

        imshow("Camera Feed",mframe);
        //imshow("Masked Feed",mask);
        //imshow("Filtered Feed",filtered);

        /*
        
        Mat whiteMask, yellowMask, pinkMask, orangeMask, blueMask, greenMask;
        
        inRange(hsvframe, lowerWhite, upperWhite, whiteMask);
        inRange(hsvframe, lowerYellow, upperYellow, yellowMask);
        inRange(hsvframe, lowerPink, upperPink, pinkMask);
        inRange(hsvframe, lowerOrange, upperOrange, orangeMask);
        inRange(hsvframe, lowerBlue, upperBlue, blueMask);
        inRange(hsvframe, lowerGreen, upperGreen, greenMask);
        
        Mat mirroredFrame,mirroredWhiteMask;

        flip(frame,mirroredFrame,1);
        flip(whiteMask,mirroredWhiteMask,1);
        imshow("Original image", mirroredFrame);
        imshow("White Mask", mirroredWhiteMask);

        imshow("Yellow Mask", yellowMask);
        imshow("Pink Mask", pinkMask);
        imshow("Orange Mask", orangeMask);
        imshow("Blue Mask", blueMask);
        imshow("Green Mask", greenMask);
        */

        if (waitKey(30) == 'q') break;

    }

    cap.release();
    destroyAllWindows();
    return 0;
}