#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DepthRecorderApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void DepthRecorderApp::setup()
{
}

void DepthRecorderApp::mouseDown( MouseEvent event )
{
}

void DepthRecorderApp::update()
{
}

void DepthRecorderApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( DepthRecorderApp, RendererGl )
