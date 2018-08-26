
#pragma once 

#include "ofMain.h"
#include "ofxDmx.h"

namespace ofx { namespace fixture { 

class Dimmer{

public:
    Dimmer();
    
    void setup( ofxDmx & dmx, int channel, int universe=1 );

    virtual std::string fixtureName();
    virtual void update();
    virtual void draw();
    
    ofParameterGroup parameters;
    ofParameter<glm::vec3> position;
    ofParameter<glm::vec3> orientation;
    ofParameter<float> dimmer;

    
protected:
	int channel;
	int universe;
    ofxDmx * dmx;
    
    ofNode node; 
    
private: 
    void onPositionChanged( glm::vec3 & value );
    void onOrientationChanged( glm::vec3 & value );
};    
    
}}

