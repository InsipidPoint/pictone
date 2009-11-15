#include <iostream>
#include <vector>
#include "Camera.h"

using namespace std;

const char *WINDOW_NAME = "Pictone";

vector<CvSeq *> templates;

void loadTemplates() {
    CvMemStorage *storage = cvCreateMemStorage(0);
    IplImage *t = cvLoadImage("triangle_up.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    CvSeq *s = NULL;
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    storage = cvCreateMemStorage(0);
    t = cvLoadImage("slant.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    storage = cvCreateMemStorage(0);
    t = cvLoadImage("square.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    storage = cvCreateMemStorage(0);
    t = cvLoadImage("pin.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    storage = cvCreateMemStorage(0);
    t = cvLoadImage("cross.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    storage = cvCreateMemStorage(0);
    t = cvLoadImage("star.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
}

int matchTemplates(CvSeq *contour) {
    double min = 99999;
    int minT = -1;
    for (unsigned int i = 0; i < templates.size(); i++) {
        double score = cvMatchShapes(contour, templates.at(i), CV_CONTOURS_MATCH_I3);
        if (score < min) {
            min = score;
            minT = i;
        }
    }
//    if (min > 1) {
//        return -1;
//    }
    return minT;
}

void cleanupContour(CvSeq *contours) {
//    while(contours) {
//        
//    }
}

int main(int argc, char **argv) {
    
    cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    
    loadTemplates();
    
    const IplImage *current_frame;
    IplImage *gray = cvCreateImage(cvSize(640,480), 8, 1);
    IplImage *small_img = cvCreateImage(cvSize(640,480), 8, 3);
    CvMemStorage *storage = cvCreateMemStorage(0);
    Camera cam;
    
    IplImage *tri = cvLoadImage("star.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(tri, tri, 100, 50);
    CvSeq *tric = NULL;
    cvFindContours(tri, storage, &tric, sizeof(CvContour), CV_RETR_TREE);    
    
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
            cvDrawContours(small_img, tric, cvScalarAll(255), cvScalarAll(128), 0);
            while( contours ) {            
                cvDrawContours(small_img, contours, cvScalarAll(255), cvScalarAll(128), 0);
                CvRect rect = cvBoundingRect(contours);
                cvRectangle(small_img, cvPoint(rect.x,rect.y), cvPoint(rect.x+rect.width,rect.y+rect.height), CV_RGB(255,0,0), 1);
                
                char buf[50];
                sprintf(buf,"%d",matchTemplates(contours));
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
