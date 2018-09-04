#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxGui.h"


class DmxChannel {
public:
    ofParameterGroup & setup( std::string name ){
        parameters.setName( name );
        parameters.add( bActive.set("send dmx", false) );
        parameters.add( channel.set( "channel", 1, 1, 512 ) );
        parameters.add( value.set(   "value "  ,    0,   0, 255 ) );

        parameters.add( autoCycle.set("auto cycle", false) );
        parameters.add( minval.set(   "auto min"  , 0,   0, 255 ) );
        parameters.add( maxval.set(   "auto max"  , 255, 0, 255 ) );
        parameters.add( speed.set("cycle speed", 0.5f, 0.5f, 3.0f) );
        
        return parameters;
    }
    
    void update( ofxDmx & dmx ){
        if(bActive){
            if (autoCycle) { 
                value = ofMap( sin( ofGetElapsedTimef()*speed ), -1.0f, 1.0f, minval, maxval ); 
            }
            dmx.setLevel( channel, value );

            dmx.update();                        
        }
    }

    ofParameterGroup parameters;
        ofParameter<bool> bActive;
        ofParameter<int> channel;
        ofParameter<int> value;
        ofParameter<int> minval;
        ofParameter<int> maxval;
        ofParameter<float> speed;
        ofParameter<bool> autoCycle;        
};


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed( int key );
	
	ofxDmx dmx;
    
    ofxPanel panel;

    std::vector<DmxChannel> channels;
};
