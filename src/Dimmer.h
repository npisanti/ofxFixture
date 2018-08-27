
#pragma once 

#include "ofMain.h"
#include "ofxDmx.h"

namespace ofx { namespace fixture { 

class Dimmer {

public:
    Dimmer();
    
    void setup( ofxDmx & dmx, int channel, int universe=1 );


    virtual std::string fixtureName();
    virtual void init(){}; // to send out default dmx messages
    virtual void update();
    virtual void draw();
    
    ofParameterGroup installation;
    ofParameter<glm::vec3> position;
    ofParameter<glm::vec3> orientation;

    ofParameterGroup parameters;
    ofParameter<float> dimmer;
    ofParameter<float> zoom;
    
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

