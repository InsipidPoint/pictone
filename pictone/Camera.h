/*
 *  Camera.h
 *  pictone
 *
 *  Created by Shiwei Song on 11/14/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */
#pragma once
#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>

class Camera {
public:
    Camera(const char *filename = NULL);
    ~Camera();
    
    const IplImage *GetFrame();
    
private:
    CvCapture *camera;
};