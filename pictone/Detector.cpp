/*
 *  Detector.cpp
 *  pictone
 *
 *  Created by Shiwei Song on 11/16/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */

#include "Detector.h"
#include <iostream>

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
        
        std::cout << rect.x << " " << minS << endl;
        Shape s;
        if (minS > 1) {
            minT = -1;
//            CvMemStorage *tmp = cvCreateMemStorage(0);
//            CvSeq *poly = cvApproxPoly(contours, sizeof(CvContour), tmp, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.01);
//            cvDrawContours(img, poly, CV_RGB(0,255,0), CV_RGB(0,255,0), 0);
            std::cout << contours->total << " here\n";
            CvPoint prev = cvPoint(999, 0);
            bool search = true;
            int j;
            for (int i = 0; i < contours->total; i++) {
                CvPoint *p = CV_GET_SEQ_ELEM(CvPoint, contours, i);
                if (prev.x >= p->x && search) {
                    prev = *p;
                } else if (search) {
                    search = false;
                    j = i-1;
                    s.pts.push_back(prev);
                    s.pts.push_back(*p);
                } else if (i-j+1 < contours->total/2) {
                    s.pts.push_back(*p);
                }

                //std::cout << "(" << p->x << "," << p->y << ") ";
            }
            
//            for (int i = 0; i < s.pts.size(); i++) {
//                std::cout << "(" << s.pts.at(i).x << "," << s.pts.at(i).y << ") ";
//            }
        }

        s.rect = cvBoundingRect(contours);
        s.type = Type(minT);
        r.push_back(s);
        
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
