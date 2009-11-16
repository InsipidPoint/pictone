#include <iostream>
#include <vector>
#include "Camera.h"
#include "Detector.h"

#include "SineWave.h"
#include "Plucked.h"
#include "RtAudio.h"

using namespace std;
using namespace stk;

const char *WINDOW_NAME = "Pictone";

int g_notes[30];
int g_t = 0;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    Plucked *sine = (Plucked *) dataPointer;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    
    if (g_notes[g_t/90]>=0) {
        if(g_t%90==0) {
            sine->setFrequency(g_notes[g_t/90]);
            sine->pluck(1);
        }
        g_t++;
    } else {
        g_t = 0;
    }

    
    for ( unsigned int i=0; i<nBufferFrames; i++ )
        *samples++ = sine->tick();
    
    return 0;
}

bool resultPredicate(const pair<CvRect,Detector::Type>& d1, const pair<CvRect,Detector::Type>& d2) {
    return d1.first.x < d2.first.x;
}

int main(int argc, char **argv) {
    
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 44100.0 );
    
    Plucked plucked(200);
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
    
    plucked.setFrequency(440.0);
    plucked.pluck(1);
    
    try {
        dac.startStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
        return 0;
    }
    
    g_notes[0] = -1;
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
            sort(r.begin(), r.end(), resultPredicate);
            int i = 0;
            for (i = 0; i < r.size(); i++) {
                g_notes[i] = 200+100*r.at(i).second;
            }
            g_notes[i] = -1;
            
            cvShowImage(WINDOW_NAME, small_img);
            cvWaitKey(999999);
            g_notes[0] = -1;
            g_t = 0;
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
