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
        
        
        ofEasyCam camera;

        // head variables 
        glm::vec3 orientation;
        glm::vec3 position;
        

        
        
        ofPlanePrimitive floor;
        ofPlanePrimitive wall;

        ofxDmx dmx;
        ofx::fixture::Dimmer dimmer;
        ofx::fixture::Head head;
        
        ofxPanel gui;
};
