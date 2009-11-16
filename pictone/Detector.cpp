/*
 *  Detector.cpp
 *  pictone
 *
 *  Created by Shiwei Song on 11/16/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */

#include "Detector.h"

MomentDetector::MomentDetector() {
    CvMemStorage *storage = cvCreateMemStorage(0);
    IplImage *t = cvLoadImage("triangle_up.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    CvSeq *s = NULL;
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    t = cvLoadImage("square.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    t = cvLoadImage("slant.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    t = cvLoadImage("pin.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
    
    t = cvLoadImage("star.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cvCanny(t, t, 100, 50);
    cvFindContours(t, storage, &s, sizeof(CvContour), CV_RETR_TREE);
    templates.push_back(s);
}

Detector::Result MomentDetector::detect(IplImage *img) {
    IplImage *gray = cvCreateImage(cvSize(640,480), 8, 1);
    cvCvtColor(img, gray, CV_BGR2GRAY);
    cvSetImageROI(gray, cvRect(82, 82, 476, 316));
    
    Result r;
    
    CvMemStorage *storage = cvCreateMemStorage(0);
    cvCanny(gray, gray, 100, 50);
    CvSeq *contours = NULL;
    cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_TREE);
    
    while(contours) {
        cvDrawContours(img, contours, cvScalarAll(255), cvScalarAll(128), 0);
        CvRect rect = cvBoundingRect(contours);
        rect.x += 82;
        rect.y += 82;
        cvRectangle(img, cvPoint(rect.x,rect.y), cvPoint(rect.x+rect.width,rect.y+rect.height), CV_RGB(255,0,0), 1);
        
        double minS = 99999;
        int minT = -1;
        for (unsigned i = 0; i < templates.size(); i++) {
            double score = cvMatchShapes(contours, templates.at(i), CV_CONTOURS_MATCH_I1);
            if (score < minS) {
                minS = score;
                minT = i;
            }
        }
        
        pair<CvRect,Type> p = make_pair(cvBoundingRect(contours), Type(minT));
        r.push_back(p);
        
        char buf[50];
        sprintf(buf,"%d",minT);
        CvFont font;
        double hScale=0.5;
        double vScale=0.5;
        int lineWidth=1;
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, hScale,vScale,0,lineWidth);
        cvPutText(img, buf, cvPoint(rect.x+rect.width,rect.y+rect.height), &font, cvScalar(255,255,0));
        
        contours = contours->h_next;
    }
    
    cvReleaseImage(&gray);
    cvClearMemStorage(storage);
    return r;
}
