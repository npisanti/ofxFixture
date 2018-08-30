
#pragma once 

#include "ofMain.h"
#include "ofxDmx.h"

namespace ofx { namespace fixture { 

class Dimmer {

public:
    Dimmer();
    
    virtual void setup( ofxDmx & dmx, int channel, int universe=1 );

    virtual std::string fixtureName();
    virtual void init(){}; // to send out default dmx messages
    virtual void update();
    virtual void draw();
    
    ofParameterGroup installation;
    ofParameter<glm::vec3> position;
    ofParameter<glm::vec3> orientation;

    ofParameterGroup parameters;
    ofParameter<float> dimmer;
    
    static ofParameter<bool> bDrawAddress;
    
    
    static void setBoundaries( glm::vec3 dimensions );

    static void setBoundaries( float w, float h, float d );
    
    static const glm::vec3 & getBoundaries();


protected:
	int channel;
	int universe;
    ofxDmx * dmx;
    
    ofNode node; 
    
    std::string address;

    static glm::vec3 boundaries;

private: 
    void onPositionChanged( glm::vec3 & value );
    void onOrientationChanged( glm::vec3 & value );
};    

const glm::vec3 & getBoundaries();
void setBoundaries( glm::vec3 dimensions );
    
}}

