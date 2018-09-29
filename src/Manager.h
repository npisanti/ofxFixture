
#pragma once

#include "ofxDmx.h"
#include "Simulation.h"
#include "States.h"
#include "Spot.h"

namespace ofx { namespace fixture {
    
class Manager {
    
public:
    Manager();

    void setup( ofxDmx & dmx, float worldWidth, float worldHeight, float worldDepth, bool useSimulation = true );
    
    void add( Dimmer & dimmer );
    void add( Head & head );
    void add( Spot & spot );
    
    void update( bool sendDmx = true );
    
    void draw();
    
    void close();
    
    Simulation  simulation;
    States      snapshots;
    
    ofParameterGroup parameters;
        ofParameterGroup positions;
        ofParameterGroup controls;
        ofParameterGroup & multiple;
private:

    void checkForCollision( Dimmer & fixt );

    bool useSimulation;
    ofxDmx * dmx;
    
};
    
}}
