/*
 *  Detector.h
 *  pictone
 *
 *  Created by Shiwei Song on 11/16/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */

#pragma once
#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>
#include <vector>

using namespace std;

class Detector {
public:
    enum Type {
        UNKNOWN = -1,
        TRIANGLE_UP = 0,
        SQUARE, // 1
        SLANT,  // 2
        PIN,    // 3
        STAR,    // 4
    };
    typedef vector<pair<CvRect,Type> > Result;
    
    virtual Result detect(IplImage *img) { Result r; return r; };
};

class MomentDetector : public Detector {
public:
    MomentDetector();
    ~MomentDetector() {};
    Result detect(IplImage *img);
    
private:
    vector<CvSeq *> templates;
};
