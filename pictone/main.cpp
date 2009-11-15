#include <iostream>
#include "Camera.h"

using namespace std;

const char *WINDOW_NAME = "Pictone";

void contour() {
    const IplImage *current_frame;
    IplImage *gray = cvCreateImage(cvSize(640,480), 8, 1);
    IplImage *small_img = cvCreateImage(cvSize(640,480), 8, 3);
    CvMemStorage *storage = cvCreateMemStorage(0);
    Camera cam;
    
    while((current_frame = cam.GetFrame())) {
        cvResize(current_frame, small_img, CV_INTER_LINEAR);
        cvCvtColor(small_img, gray, CV_BGR2GRAY);
        
        cvShowImage(WINDOW_NAME, gray);
        char key = cvWaitKey(10);
        if (key == ' ') {   // capture
            cvCanny(gray, gray, 60, 50);
            CvSeq *contours = NULL;
            cvFindContours(gray, storage, &contours);
            while( contours ) {
                cout << "here\n";
                cvDrawContours(gray, contours, cvScalarAll(255), cvScalarAll(255), 100);
                CvRect rect = cvBoundingRect(contours);
                cvRectangle(small_img, cvPoint(rect.x,rect.y), cvPoint(rect.x+rect.width,rect.y+rect.height), CV_RGB(255,0,0), 2);
                contours = contours->h_next;
            }
            
            cvShowImage(WINDOW_NAME, small_img);
            cvWaitKey(999999);
        } else if(key == 'q') {
            break;
        }
    }
    
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
    cvReleaseMemStorage(&storage);
}

int main(int argc, char **argv) {
    
    cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    cvNamedWindow ("template", CV_WINDOW_AUTOSIZE);
    
    const IplImage *current_frame;
    IplImage *gray = cvCreateImage(cvSize(640,480), 8, 1);
    IplImage *small_img = cvCreateImage(cvSize(640,480), 8, 3);
    CvMemStorage *storage = cvCreateMemStorage(0);
    Camera cam;
    
    IplImage *tri = cvLoadImage("triangle_up.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(tri, tri, 100, 50);
    CvSeq *tric = NULL;
    cvFindContours(tri, storage, &tric, sizeof(CvContour), CV_RETR_TREE);    
    cvDrawContours(tri, tric, cvScalarAll(255), cvScalarAll(128), 0);
    CvRect rect2 = cvBoundingRect(tric);
    cvRectangle(tri, cvPoint(rect2.x,rect2.y), cvPoint(rect2.x+rect2.width,rect2.y+rect2.height), CV_RGB(255,0,0), 1);
    cvShowImage("template", tri);
    
    while((current_frame = cam.GetFrame())) {
        cvResize(current_frame, small_img, CV_INTER_LINEAR);
        cvCvtColor(small_img, gray, CV_BGR2GRAY);
        
        cvShowImage(WINDOW_NAME, gray);
        char key = cvWaitKey(10);
        if (key == ' ') {   // capture            
            cvCanny(gray, gray, 100, 50);
            CvSeq *contours = NULL;
            cout << cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_TREE) << endl;
            cout << "similarity:\n";
            while( contours ) {            
                cvDrawContours(small_img, contours, cvScalarAll(255), cvScalarAll(128), 0);
                CvRect rect = cvBoundingRect(contours);
                cvRectangle(small_img, cvPoint(rect.x,rect.y), cvPoint(rect.x+rect.width,rect.y+rect.height), CV_RGB(255,0,0), 1);
                
                char buf[50];
                sprintf(buf,"%.3f",cvMatchShapes(contours, tric, CV_CONTOURS_MATCH_I3));
                CvFont font;
                double hScale=0.5;
                double vScale=0.5;
                int    lineWidth=1;
                cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, hScale,vScale,0,lineWidth);
                cvPutText (small_img,buf,cvPoint(rect.x+rect.width,rect.y+rect.height), &font, cvScalar(255,255,0));
                
                contours = contours->h_next;
            }
            
            cvShowImage(WINDOW_NAME, small_img);
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
