#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);	
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void exit(); 
    void guiEvent(ofxUIEventArgs &e);
	void updateTime();
    
    bool    preview;
    bool    useAlpha;
    bool    frameByframe;
    bool    publishScreen;
    bool    splitScreen;
    float   aspectRatio;
	
    ofTexture       tex;
	ofxSyphonServer mainOutputSyphonServer;
	ofxSyphonServer individualTextureSyphonServer;
    ofQTKitPlayer   vid;
    ofQTKitDecodeMode decodeMode;
    ofxUICanvas *gui;
    ofxUIImageSlider *pSlider;
   
    ofxUIImageButton *openBtn;
    ofxUIImageToggle *loopBtn;

    
    ofxUILabel *time;
    ofxUIImage *bgImage;
    ofxUIImage *bgBarImage;
    ofxUIImageButton *pauseImage;
    ofxUIImageButton *playImage;

    
    ofImage bg;
    ofImage barBg;
    ofImage pause;
    
    bool loop;

};

#endif

