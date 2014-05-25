#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Json.h"

#include "msEpoch.h"

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
    
private:
    ci::openni::Camera mCamera;
    gl::Texture mTex;
    bool mRecording;
    bool mColor;
    Font mFont;
};


void DepthRecorderApp::prepareSettings(Settings *settings)
{
    settings->setWindowSize( 640, 480 );
    settings->setFrameRate( 30 );
}

void DepthRecorderApp::setup()
{
    try {

        JsonTree mode = JsonTree(loadAsset("mode.json"));
        mColor = mode.getChild("mode").getValue<string>() == "color";
        
    } catch (exception& e) {
        console() << "Unable to parse json, defaulting to depth: " << e.what() << endl;
        mColor = false;
    }

    if(mColor) {
        mCamera.setup( ci::openni::Camera::SENSOR_COLOR );
    } else {
        mCamera.setup( ci::openni::Camera::SENSOR_DEPTH );
    }
    
    mRecording = false;
    
    mFont = Font("Arial", 30);
}

void DepthRecorderApp::keyDown( KeyEvent event )
{
    if(event.getChar() == ' ') {
        stringstream ss;
        ss << MSEpoch();
        DataTargetRef target;
        
        if( mRecording ) {
            target = writeFile( getDocumentsDirectory() / fs::path("_Depth") / (ss.str() + "--STOPPED" ));
            
        } else {
            target = writeFile( getDocumentsDirectory() / fs::path("_Depth") / (ss.str() + "--STARTING" ));
        }
        
        target->getStream()->write(" ");
        
        mRecording = ! mRecording;
    }
}

void DepthRecorderApp::mouseDown( MouseEvent event )
{
}

void DepthRecorderApp::update()
{
    mCamera.update();
    mTex = mColor? mCamera.getColorTex() : mCamera.getDepthTex();
    
    stringstream ss;
    ss << MSEpoch();
    
    if(mRecording) {
        // I haven't looked inside but I'm sure the camera keeps a surface around, so let's just
        // use their ImageSource interface
        Surface surf = mCamera.getDepthImage();
        writeImage( getDocumentsDirectory() / fs::path("_Depth") / (ss.str() + ".png"), surf );
    }
    
}

void DepthRecorderApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    
    gl::draw( mTex, Rectf( 0, 0, 640, 480 ) );
    
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
