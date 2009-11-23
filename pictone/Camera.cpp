/*
 *  Camera.cpp
 *  pictone
 *
 *  Created by Shiwei Song on 11/14/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */

#include "Camera.h"

Camera::Camera(const char *filename) {
    if(filename)
        camera = cvCaptureFromAVI(filename);
    else
        camera = cvCreateCameraCapture(CV_CAP_ANY);
    
    if(!camera)
        abort();
}

Camera::~Camera() {
    cvReleaseCapture(&camera);
}

const IplImage * Camera::GetFrame() {
    return cvQueryFrame(camera);
}
