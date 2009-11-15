#include <iostream>
#include "Camera.h"

using namespace std;

const char *WINDOW_NAME = "Pictone";


int main(int argc, char **argv) {
    
    cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    
    const IplImage *current_frame;
    IplImage *gray = cvCreateImage(cvSize(640,480), 8, 1);
    IplImage *small_img = cvCreateImage(cvSize(640,480), 8, 3);
    CvMemStorage *storage = cvCreateMemStorage(0);
    Camera cam;
    
    while((current_frame = cam.GetFrame())) {
        cvResize(current_frame, small_img, CV_INTER_LINEAR);
       // cvFlip(small_img, small_img, 1);
        cvCvtColor(small_img, gray, CV_BGR2GRAY);
        //    cvEqualizeHist(gray,gray);
        
        cvShowImage(WINDOW_NAME, gray);
        char key = cvWaitKey(10);
        if (key == ' ') {   // capture
//            cvAdaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7);
            cvCanny(gray, gray, 60, 50);
            CvSeq *contours = NULL;
            cvFindContours(gray, storage, &contours);
//            cvZero( gray );
            if( contours ) {
                cout << "here\n";
                cvDrawContours(gray, contours, cvScalarAll(255), cvScalarAll(255), 100);
                cvBoundingRect(contours);
            }
            
            cvShowImage(WINDOW_NAME, gray);
            cvWaitKey(999999);
        } else if(key == 'q') {
            break;
        }
    }
    
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
    cvReleaseMemStorage(&storage);
    return 0;
}
