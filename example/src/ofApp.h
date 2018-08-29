#pragma once


#include "ofMain.h"
#include "ofxFixture.h"
#include "ofxGui.h"

class ofApp: public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        ofx::fixture::Simulation simulation;

        ofxDmx dmx;
        std::vector<ofx::fixture::Head> heads;
        std::vector<ofx::fixture::Dimmer> dimmers;
        
        ofx::fixture::States states;
        
        ofxPanel gui;
        ofxPanel positions;
        
        ofParameter<bool> bTargetDemo;

        int transitioning;
        float cursor;
};
