#pragma once


#include "ofMain.h"
#include "ofxFixtures.h"


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
        
        ofNode base; 
        ofNode head;       
        
        ofBoxPrimitive box;
        ofBoxPrimitive label;
        ofBoxPrimitive bottom;
        ofBoxPrimitive spot;


        // head variables 
        glm::vec3 orientation;
        glm::vec3 position;
        glm::vec3 target;
        float pan;
        float tilt;
        
        ofParameter<float> minPan, maxPan, minTilt, maxTilt;
        
        void setTarget( glm::vec3 pos );
        void setOrientation( float tilt, float pan );
        float panAngle( glm::vec3 v);
        float tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
        
        ofPlanePrimitive floor;
        ofPlanePrimitive wall;
    
};
