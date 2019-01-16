
#pragma once 

#include "ofMain.h"
#include "ofxDmx.h"

namespace ofx { namespace fixture { 

class Dimmer {
    friend class States;
    friend class Simulation;
    friend class Manager;
    
public:
    Dimmer();
    
    virtual void setup( ofxDmx & dmx, int channel, int universe=1, std::string name="" );

    virtual std::string fixtureName();
    virtual void init(){} // to send out default dmx messages
    virtual void update(){}
    virtual void draw(){}
    virtual void draw2D( float scale );
    
    // enable and disable lights here, used by simulation
    virtual void enableLight(){}
    virtual void disableLight(){};
    
    
    ofParameterGroup installation;
    ofParameter<bool> armed;
    ofParameter<glm::vec3> position;
    ofParameter<glm::vec3> orientation;

    ofParameterGroup parameters;
    ofParameter<float> dimmer;
    ofParameterGroup options;
    
    
    static ofParameter<bool> bDrawAddress;
    static bool bSetupGLLights;
    
    
    static void setBoundaries( glm::vec3 dimensions );
    static void setBoundaries( float w, float h, float d );
    static const glm::vec3 & getBoundaries();

protected:
    // specificationCh is the channel in the fixture's dmx specifation
    inline void setDmx( int specificationCh, int value ){
        if(armed) dmx->setLevel( specificationCh-1 + channel, value, universe );
    }

    // adds a custom ofParameter to snapshot management 
    void addOption( ofParameter<float> & parameter );
    void addOption( ofParameter<int> & parameter );
    void addOption( ofParameter<bool> & parameter );

    // you have to set this for automatic dmx collision detection
    void setMaxDmxChannel( int max );

    ofNode node; 
    
    std::string address;

    static glm::vec3 boundaries;

    // if you set this to true the dimmer parameter name will change
    // to the same name as the fixture name
    bool changeDimmerName; 

private: 
	int channel;
	int universe;
    ofxDmx * dmx;

    void onPositionChanged( glm::vec3 & value );
    void onOrientationChanged( glm::vec3 & value );
            
    std::vector<ofParameter<float>*> fOptionals; 
    std::vector<ofParameter<int>*>   iOptionals;
    std::vector<ofParameter<bool>*>  bOptionals;
    bool bHasOptions;

    int dmxBoundaryAdd;
    void checkDmxCollision( const Dimmer & other );
};    

const glm::vec3 & getBoundaries();
void setBoundaries( glm::vec3 dimensions );
void setBoundaries( float w, float h, float d  );
    
}}
