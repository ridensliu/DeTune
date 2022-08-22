#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include "dButton.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void setupImage(std::string imgPath);
        float getColorWarmness(ofColor color);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioOut(ofSoundBuffer& output) override;
        double getSound(double freq);
		
    // ofxGui
    ofxPanel gui;
    ofxIntSlider intSlider;
    ofxFloatSlider floatSliderBlue;
    ofxFloatSlider floatSliderRed;
    ofxFloatSlider floatSliderGreen;
    ofxFloatSlider floatSlider1;
    ofxFloatSlider floatSlider2;
    ofxButton button;
    ofxToggle toggle;
    ofxButton loadButton;
    int upd_num;
    
    float nextEventSeconds = 0;
    int vertCount;
    ofxDelaunay delaunay;
    ofxDelaunay delaunay2d;
    ofEasyCam camera;
    ofLight light;
    ofTexture mTex;
    ofImage img;
    ofShader shaderColor;
    ofShader shaderWire;
    ofShader shaderBG;
    ofSoundStream soundStream;
    maxiOsc oscillator1;
    maxiOsc oscillator2;
    maxiOsc oscillator3;
    maxiOsc oscillator4;
    maxiClock clock;
    maxiEnv envelope;
    double scaleFreq[7] = {
        261.63, 293.66, 311.13, 349.23, 392.00, 415.30, 466.16
    };
    double tickTime;
    double dspTime = 0.0;
    int currentNote = 0;
    bool imgLoaded = false;
    float warmness = 0.5;
    bool skipTick = false;
    
    ofTrueTypeFont font;
    dButton loadDButton;
    dSlider wireSlider;
    dSlider volumeSlider;
    dSlider countSlider;
    int lastCount = 30;
    std::string imgPath = "";
    
    ofImage logo;
};
