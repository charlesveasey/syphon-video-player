#include "testApp.h"

const int width = 2048;
const int height = 2048;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true); 
	ofEnableSmoothing();
    
	ofSetWindowTitle("Syphon Video Player");
	mainOutputSyphonServer.setName("Screen Output");
    individualTextureSyphonServer.setName("Video Output");
    
    loop = false;
    preview = false;
    useAlpha = true;
    frameByframe = true;
    publishScreen = true;
    ofSetFullscreen(false);
    ofBackground(0,0,0);
	ofSetFrameRate(30);    

    
    // layout
    float bgX = 0.0;
    float bgY = 0.0;

    float bgBarX = 0.0;
    float bgBarY = 0.0;    
    
    float sliderX = 75;
    float sliderY = 30;
    
    float timeX = 480;
    float timeY = 45;

    float pauseX = 33;
    float pauseY = 30;
    
    float playX = 33;
    float playY = 30;
    
    float openX = 685;
    float openY = 33;
    
    float loopX = 727;
    float loopY = 35;
    
    
    // background images
    bg.loadImage("graphics/bg.png");
    barBg.loadImage("graphics/barBg.png");    

    
    
    // setup ui canvas
    gui = new ofxUICanvas(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    
    // font
    gui->setFont("Open_Sans/OpenSans-Regular.ttf");
    gui->setFontSize(OFX_UI_FONT_LARGE, 18);            
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 12);           
    gui->setFontSize(OFX_UI_FONT_SMALL, 8);

    
    gui->setPosition(5, 5);

    
    // create widgits 
    
    
    bgImage = new ofxUIImage(bgX, bgY, 992, 445, &bg, "BGIMAGE", false);
    bgBarImage = new ofxUIImage(bgBarX, bgBarY, 780, 89, &barBg, "BGBARIMAGE", false);
    pauseImage = new ofxUIImageButton(pauseX, pauseY, 30, 30, false, "graphics/pause.png", "PAUSE");
    playImage = new ofxUIImageButton(playX, playY, 30, 30, false, "graphics/play.png", "PLAY");
    openBtn = new ofxUIImageButton(openX, openY, 24, 22, false, "graphics/open.png", "OPEN");
    loopBtn = new ofxUIImageToggle(loopX, loopY, 22, 22, false, "graphics/loop.png", "LOOP");
    
    pSlider = new ofxUIImageSlider(sliderX, sliderY, 500.0, 30.0, 0.0, 100.0, 0.0, "slider.png", "PSLIDER");
    pSlider->setLabelVisible(false);
    
    time = new ofxUILabel(timeX, timeY, 300.0, "TIME", "00:00:00", OFX_UI_FONT_SMALL);
   
    
    

    
    //open = new ofxUIImageButton(0, 0, 30, 30, false, "open.png", "open");    
    
    
    // add widgets
    gui->addWidget(loopBtn);
    gui->addWidget(openBtn);
    gui->addWidget(pSlider);
    gui->addWidget(time);
    gui->addWidget(pauseImage);
    gui->addWidget(playImage);
    gui->addWidget(bgBarImage);
    
    
    
    ofColor cc;
    cc.set(155, 155, 155);      
    time->setColorFill(cc);    
    
    time->setLabel("00:00:00");
    
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
    //gui->loadSettings("GUI/guiSettings.xml");     
    
    pSlider->setDrawOutline(false); 
    pSlider->setDrawOutlineHighLight(false);
    pSlider->setDrawFill(false);
    pSlider->setDrawFillHighLight(false);   
 
    ofColor c;
    c.set(255,255,255);   
    
    
    pauseImage->setColorFill(c);
    pauseImage->setColorBack(c);
    playImage->setColorFill(c);
    playImage->setColorBack(c);
    openBtn->setColorFill(c);
    openBtn->setColorBack(c);
    
    ofColor c2;
    c2.set(200,200,200);
    
    
    pauseImage->setColorFillHighlight(c2);
    pauseImage->setColorOutlineHighlight(c2);   
    playImage->setColorFillHighlight(c2);
    playImage->setColorOutlineHighlight(c2);   
    openBtn->setColorFillHighlight(c2);
    openBtn->setColorOutlineHighlight(c2); 
    

    
    ofColor c3;
    c3.set(0,0,0);    
    
    
    loopBtn->setColorPadded(c);
    loopBtn->setColorFill(c);
    loopBtn->setColorFillHighlight(c);
    loopBtn->setColorOutlineHighlight(c);
    loopBtn->setColorBack(c3);
    
    
    pauseImage->setVisible(false);
   
    
    
    

    // #1 Play videos with an alpha channel. ---------------------------
	// ofQTKitPlayer videos encoded with Alpha channels (e.g. Animation Codec etc).
    // The pixel format MUST be enabled prior to loading!
	// If an alpha channels is not used, setting a non-alpha pixel format
    // (e.g. OF_PIXELS_RGB) will increase performance.
	
    if (useAlpha) {
        vid.setPixelFormat(OF_PIXELS_RGBA);
        tex.allocate(width, height, GL_RGBA);                    
    }
    else {
        vid.setPixelFormat(OF_PIXELS_RGB);
        tex.allocate(width, height, GL_RGB);        
    }
    
    
	//# 2 Decode Modes and video loading. -------------------------------
	// If you don't need direct access to pixel data, you can use ofQTKitPlayer's
    // highly optimized internal texture management system.  These mode are not
    // available when using the cross-platform ofVideoPlayer.
    
	// Texture only is fastest, but no pixel access allowed.
   
    //decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
    
	// Pixels and texture together is faster than PIXEL_ONLY and manually uploaded textures.
    
    decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
	
    
	
    //# 3 Synchronous seeking. ---------------------------
    // Like the legacy player, by default frames are available immediately for display
    // via setFrame().  In many cases when random access via setFrame() is not required,
    // significant speed increases can be gained by disabling synchronous seeking
    // particularly when playing high bitrate HD video.
    
    // In "asynchronous" seek mode, individual frames can still be randomly accessed,
    // but the pixels may not be available immediately without calling update() first.
    
    vid.setSynchronousSeeking(true);
    
    
 
    
}



//--------------------------	------------------------------------
void testApp::update(){    
    if (vid.isLoaded()) {
        vid.update();
        
        if (vid.getIsMovieDone()) {
            vid.stop();
            pauseImage->setVisible(false);
            playImage->setVisible(true);         
        }
        
        pSlider->setValue(vid.getPositionInSeconds());
        pSlider->update();
        updateTime();

    }
}


void testApp::updateTime(){
    int input_seconds;
    input_seconds = vid.getPositionInSeconds();
    
    string hours;
    string minutes;
    string seconds;
    
    hours = ofToString( (input_seconds / 60 / 60) % 24 );
    minutes = ofToString( (input_seconds / 60) % 60 );
    seconds = ofToString( input_seconds % 60 );    

    if (hours.length() < 2)
        hours = "0" + hours;
    if (minutes.length() < 2)
        minutes = "0" + minutes;
    if (seconds.length() < 2)
        seconds = "0" + seconds;   
    
    string str;
    
    str += hours;
    str += ":";
    str += minutes;  
    str += ":";    
    str += seconds;        
        
    time->setLabel(str);
}

//--------------------------------------------------------------
void testApp::draw(){
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //bg.draw(0,0);
    //barBg.draw(0,0);
    
    ofSetHexColor(0xFFFFFF);   
    
    if (vid.isLoaded()) {
       tex.loadData(vid.getPixelsRef());
        if (preview)
            vid.draw(0,0,512,512*aspectRatio);          
    }
        
    individualTextureSyphonServer.publishTexture(&tex);
    
    if (publishScreen)
        mainOutputSyphonServer.publishScreen();    
}


void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");     
    delete gui; 	
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
        
    if(e.widget->getName() == "PSLIDER")
    {
        vid.setPosition(pSlider->getValue());
        vid.update();
        updateTime();
    } 
    else if(e.widget->getName() == "PLAY")
    {
        //playImage->setVisible(!vid.isPlaying());
        //pauseImage->setVisible(vid.isPlaying());
    }
    else if(e.widget->getName() == "PAUSE")
    {
        //playImage->setVisible(vid.isPlaying());        
        //pauseImage->setVisible(!vid.isPlaying());
    
    }     
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    switch(key){
        case 'm':
            frameByframe=!frameByframe;
            vid.setPaused(frameByframe);
            break;
        case OF_KEY_LEFT:
            vid.previousFrame();
            break;
        case OF_KEY_RIGHT:
            vid.nextFrame();
            break;
        case '0':
            vid.firstFrame();
            break;
        case 32:
            if (vid.isPlaying())
                vid.stop();
            else 
                vid.play();
            
            playImage->setVisible(!vid.isPlaying());
            pauseImage->setVisible(vid.isPlaying());
            
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'o':
            //Open the Open File Dialog
            ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a file"); 
            
            //Check if the user opened a file
            if (openFileResult.bSuccess){
                ofLogVerbose("User selected a file");

                //We have a file, check it and process it
                processOpenFileSelection(openFileResult);
                
            }else {
                ofLogVerbose("User hit cancel");
            }
            break;            
    }
}



void testApp::keyReleased(int key){}



void testApp::processOpenFileSelection(ofFileDialogResult openFileResult){
	//ofLogVerbose("getName(): "  + openFileResult.getName());
	//ofLogVerbose("getPath(): "  + openFileResult.getPath());
	
	ofFile file (openFileResult.getPath()); 
    
	if (file.exists()){
        vid.closeMovie();
        vid.loadMovie(openFileResult.getPath(), decodeMode);
        aspectRatio = vid.getHeight() / vid.getWidth();
        pSlider->setMax(vid.getDuration());
        //vid.play(); 
        
        // set loop state
        vid.setLoopState(OF_LOOP_NONE);
        
	}
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        vid.setPosition(pct);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	/*
    if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        float speed = (2 * pct - 1) * 5.0f;
        vid.setSpeed(speed);
	}
    */
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(!frameByframe){
        vid.setPaused(true);
	}
    
        if(playImage->isHit(x, y))
        {
            vid.play();
            playImage->setVisible(!vid.isPlaying());
            pauseImage->setVisible(vid.isPlaying());
        }
        else if(pauseImage->isHit(x, y))
        {
            vid.stop();
            pauseImage->setVisible(false);
            playImage->setVisible(true);        
        }
        else if (loopBtn->isHit(x, y)){

            if(loopBtn->getValue())
                vid.setLoopState(OF_LOOP_NONE);
            else 
                vid.setLoopState(OF_LOOP_NORMAL);
        }
        else if (openBtn->isHit(x, y)){
            //Open the Open File Dialog
            ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a file"); 
            
            //Check if the user opened a file
            if (openFileResult.bSuccess){
                ofLogVerbose("User selected a file");
                
                //We have a file, check it and process it
                processOpenFileSelection(openFileResult);
                
            }else {
                ofLogVerbose("User hit cancel");
            }            
        }
}    


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(!frameByframe){
        vid.setPaused(false);
	}
    
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}

