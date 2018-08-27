
#pragma once 

#include "ofMain.h"
#include "Dimmer.h"
#include "Head.h"

namespace ofx{ namespace fixture {
    
class Simulation {

public:  
    Simulation();

    void setStage( float stageWidht, float stageHeight, float stageDepth );
    void setGraphics( int x, int y, int w, int h );
    void moveGraphics( int x, int y );

    void add( Head & head );
    void add( Dimmer & dimmer );   
    
    void update();
    void draw();
    
    void enable();
    void disable();
    
    void setDrawAddress( bool drawAddress );
    void toggleDrawAddress();
    
private:
    glm::vec3 stage;

    ofEasyCam camera;
    ofFbo fbo;

    ofPlanePrimitive floor;
    ofPlanePrimitive wall;

    std::vector<Dimmer*> dimmers;
    std::vector<Head*>   heads;
    
    int gx;
    int gy;
    
    bool bActive;
    
};
    
    
}}
