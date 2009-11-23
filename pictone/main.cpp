#include <iostream>
#include <vector>
#include "Camera.h"
#include "Detector.h"
#include "Synth.h"

#include "Clarinet.h"
#include "RtAudio.h"

using namespace std;
using namespace stk;

MultiSynth synth;

const char *WINDOW_NAME = "Pictone";

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
//    Clarinet *sine = (Clarinet *) dataPointer;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    
    for ( unsigned int i=0; i<nBufferFrames; i++ )
        *samples++ = synth.tick(); //sine->tick();
    
    return 0;
}

bool resultPredicate(const pair<CvRect,Detector::Type>& d1, const pair<CvRect,Detector::Type>& d2) {
    return d1.first.x < d2.first.x;
}

int main(int argc, char **argv) {
    
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 44100.0 );
    
    Clarinet plucked(200);
    RtAudio dac;
    
    // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 1;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    try {
        dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&plucked );
    }
    catch ( RtError &error ) {
        error.printMessage();
        return 0;
    }
    
//    plucked.noteOn(440, 1);
    
    try {
        dac.startStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
        return 0;
    }
    ////////
    
    cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    
    const IplImage *current_frame;
    IplImage *small_img = cvCreateImage(cvSize(640,480), 8, 3);
    Camera cam;
    MomentDetector detector;
    
    while((current_frame = cam.GetFrame())) {
        cvResize(current_frame, small_img, CV_INTER_LINEAR);
        cvRectangle(small_img, cvPoint(80,80), cvPoint(560,400), CV_RGB(0,255,0), 1);
        cvShowImage(WINDOW_NAME, small_img);
        char key = cvWaitKey(10);
        if (key == ' ') {   // capture
            Detector::Result r = detector.detect(small_img);
            synth.init(r);
            cvShowImage(WINDOW_NAME, small_img);
            cvWaitKey(999999);
            synth.clear();
        } else if(key == 'q') {
            break;
        }
    }
    
    cvReleaseImage( &small_img );
    
    ////////////
    // Shut down the output stream.
    try {
        dac.closeStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    
    return 0;
}
