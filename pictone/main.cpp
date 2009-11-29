#include <iostream>
#include <vector>
#include <GLUT/glut.h>
#include "Camera.h"
#include "Detector.h"
#include "Synth.h"

#include "Clarinet.h"
#include "RtAudio.h"

using namespace std;
using namespace stk;

const char *WINDOW_NAME = "Pictone";

const IplImage *current_frame;
IplImage *small_img;
IplImage *tex_img;
Camera cam;
MomentDetector detector;
MultiSynth synth;
bool g_detect = false;

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc( );
void displayFunc( );
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void initialize( );

// width and height of the window
GLsizei g_width = 640;
GLsizei g_height = 480;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    register StkFloat *samples = (StkFloat *) outputBuffer;
    
    for ( unsigned int i=0; i<nBufferFrames; i++ )
        *samples++ = synth.tick();
    
    return 0;
}

//-----------------------------------------------------------------------------
// Name: initialize( )
// Desc: sets initial OpenGL states
//       also initializes any application data
//-----------------------------------------------------------------------------
void initialize()
{
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_FLAT );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // draw in wireframe
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    // enable lighting for front
    glLightModeli( GL_FRONT_AND_BACK, GL_TRUE );
    // material have diffuse and ambient lighting 
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );
}

//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    g_width = w; g_height = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( 90.0, (GLfloat) w / (GLfloat) h, .1, 50.0 );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
    // position the view point
    gluLookAt( 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}

//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    Detector::Result r;
    switch( key ) {
        case ' ':
            if (g_detect) {
                synth.clear();
                g_detect = false;
            } else {
                r = detector.detect(small_img);
                synth.init(r);
                g_detect = true;
            }   
            break;

        case 'Q':
        case 'q':
            exit(1);
            break;
    }
    
    glutPostRedisplay( );
}

//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // when left mouse button is down, move left
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        // when right mouse button down, move right
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    
    glutPostRedisplay( );
}

//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
    
    // reshape
    // reshapeFunc( g_width, g_height );
}

//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glPushMatrix();

    if((current_frame = cam.GetFrame()) && !g_detect) {
        cvResize(current_frame, small_img, CV_INTER_LINEAR);
        cvRectangle(small_img, cvPoint(80,80), cvPoint(560,400), CV_RGB(0,255,0), 1);
    }
    
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture (GL_TEXTURE_2D, 13);
//    glTexImage2D(GL_TEXTURE_2D,        //target
//                 0,                    //level
//                 GL_RGB,               //internalFormat
//                 small_img->width,         //width
//                 small_img->height,        //height
//                 0,                    //border
//                 GL_BGR,               //format
//                 GL_UNSIGNED_BYTE,     //type
//                 small_img->imageData);    //pointer to image data
//    glBegin (GL_QUADS);
//    glTexCoord2f (0.0, 0.0);
//    glVertex3f (0.0, 0.0, 0.0);
//    glTexCoord2f (1.0, 0.0);
//    glVertex3f (10.0, 0.0, 0.0);
//    glTexCoord2f (1.0, 1.0);
//    glVertex3f (10.0, 10.0, 0.0);
//    glTexCoord2f (0.0, 1.0);
//    glVertex3f (0.0, 10.0, 0.0);
//    glEnd ();

//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    gluOrtho2D(0.0,g_width,0.0,g_height);
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glLoadIdentity();
    
    cvConvertImage(small_img, tex_img, CV_CVTIMG_FLIP);
    glDrawPixels(tex_img->width,tex_img->height,GL_BGR,GL_UNSIGNED_BYTE,tex_img->imageData);
    
//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
    
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
    
    glPopMatrix();
    
    // flush and swap
    glFlush( );
    glutSwapBuffers( );
}

int main(int argc, char **argv) {
        
    // initialize GLUT
    glutInit( &argc, argv );
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow(WINDOW_NAME);
    
    // set the idle function - called when idle
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    
    // do our own initialization
    initialize();
    
    //
    
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
    
    try {
        dac.startStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
        return 0;
    }
    ////////
    
    small_img = cvCreateImage(cvSize(640,480), 8, 3);
    tex_img = cvCreateImage(cvSize(640,480), 8, 3);
    
    glutMainLoop();
    
    cvReleaseImage(&small_img);
    cvReleaseImage(&tex_img);
    
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
