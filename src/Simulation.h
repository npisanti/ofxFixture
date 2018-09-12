
#pragma once 

#include "ofMain.h"
#include "Dimmer.h"
#include "Head.h"

namespace ofx{ namespace fixture {
    
class Simulation {

public:  
    Simulation();

    void setStage();
    void setStage( float stageWidht, float stageHeight, float stageDepth );
    
    void setGraphics( int x, int y, int w, int h );
    void moveGraphics( int x, int y );

    void add( Dimmer & fixt ); 
    
    virtual void drawObjects(){};
    
    void update();
    void draw();
    
    void enable();
    void disable();
    
    void setDrawAddress( bool drawAddress );
    void toggleDrawAddress();
    
private:

    ofEasyCam camera;
    ofFbo fbo;

    ofPlanePrimitive floor;
    ofPlanePrimitive wall;
    
    ofMaterial material;

    std::vector<Dimmer*> fixtures;
    
    int gx;
    int gy;
    
    bool bActive;
    
};
    
    
}}
