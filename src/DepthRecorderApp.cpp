#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "CinderOpenNI.h"
#include "Cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DepthRecorderApp : public AppNative {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
	void update();
	void draw();
    void shutdown();
    unsigned long long MSEpoch();
    
private:
    ci::openni::Camera mCamera;
    gl::Texture mDepthTex;
    gl::Texture mColorTex;
    bool mRecording;
    bool mColor;
    
    Font mFont;

};

unsigned long long DepthRecorderApp::MSEpoch()
{
#if defined(WIN32) || defined(WIN64)
    
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER ts;
    
    ts.HighPart = ft.dwHighDateTime;
    ts.LowPart = ft.dwLowDateTime;
    
    unsigned long long time = ts.QuadPart;
    time /= 10000;
    //cout << time << endl;
    
    return time;
    
#else
    
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    unsigned long long millisecondsSinceEpoch =
    (unsigned long long)(tv.tv_sec) * 1000 +
    (unsigned long long)(tv.tv_usec) / 1000;
    
    // fake the MS
    return millisecondsSinceEpoch;
    
#endif
}

void DepthRecorderApp::prepareSettings(Settings *settings)
{
    settings->setWindowSize( 1280, 480 );
    settings->setFrameRate( 30 );
}

void DepthRecorderApp::setup()
{
    mCamera.setup( ci::openni::Camera::SENSOR_DEPTH | ci::openni::Camera::SENSOR_COLOR );
    mRecording = false;
    mColor = false;
    
    mFont = Font("Arial", 30);
}

void DepthRecorderApp::keyDown( KeyEvent event )
{
    if(event.getChar() == ' ') {
        mRecording = ! mRecording;
    }
    if(event.getChar() == 'c') {
        mColor = ! mColor;
    }
}

void DepthRecorderApp::mouseDown( MouseEvent event )
{
}

void DepthRecorderApp::update()
{
    mCamera.update();
    mDepthTex = mCamera.getDepthTex();
    if(mColor) {
        mColorTex = mCamera.getColorTex();
    }
    
    stringstream ss;
    ss << MSEpoch();
    
    if(mRecording) {
        writeImage( getDocumentsDirectory() / fs::path("_Depth") / (ss.str() + "-depth.png"), mDepthTex );
        if(mColor) {
            writeImage( getDocumentsDirectory() / fs::path("_Depth") / (ss.str() + "-color.jpg"), mColorTex );
        }
    }
    
}

void DepthRecorderApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    
    gl::draw( mDepthTex, Rectf( 0, 0, 640, 480 ) );
    //gl::draw( camera.getRawDepthTex(), Rectf( 0, 0, 640, 480 ) );
    if(mColor) {
        gl::draw( mColorTex, Rectf( 640, 0, 1280, 480 ) );
    }
    
    stringstream ss;
    ss.precision(1);
    ss << std::fixed << getAverageFps();
    cinder::gl::drawString(ss.str(), Vec2f(10,10), mRecording? Colorf(1,0,0) : Colorf(1,1,1), mFont );
    
}

void DepthRecorderApp::shutdown()
{
    console() << "Shutting down" << endl;
    mCamera.close();
    mCamera.shutdown();
}

CINDER_APP_NATIVE( DepthRecorderApp, RendererGl )
