
#pragma once 

#include "Dimmer.h"

namespace ofx { namespace fixture { 

class States;

class Head : public Dimmer {
    friend class States;
    
public:
    Head();

    void setup( ofxDmx & dmx, int channel, int universe=1 ) override;

    void setTarget( glm::vec3 pos );
    void setColor( const ofColor & color, bool alphaAsWhite=false );

    ofParameter<float> zoom;
    
    ofParameterGroup color;
        ofParameter<float> red;
        ofParameter<float> green;
        ofParameter<float> blue;
        ofParameter<float> white;

    ofParameter<float> pan;
    ofParameter<float> tilt;

    ofParameter<bool> chaseTarget;
    ofParameter<glm::vec3> target;

    std::string fixtureName() override;
    
    void update() override;
    void draw() override;
    

protected: // to be called from subclasses

    // specificationCh is the channel in the fixture's dmx specifation
    void setDmx( int specificationCh, int value );

    // those have to be used in the constructor 
    void setPanRange( float min, float max );
    void setTiltRange( float min, float max );
    
    // adds a custom ofParameter to snapshot management 
    void addOptional( ofParameter<float> & parameter );
    void addOptional( ofParameter<int> & parameter );
    void addOptional( ofParameter<bool> & parameter );
    
private:
    float panMin, panMax, tiltMin, tiltMax;

    ofNode head;       
    ofBoxPrimitive box;
    ofBoxPrimitive label;
    ofCylinderPrimitive spot;

    void onTargetChange( glm::vec3 & value );

    void setOrientation( float tilt, float pan );
    float panAngle( glm::vec3 v);
    float tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
        
    std::vector<ofParameter<float>*> fOptionals; 
    std::vector<ofParameter<int>*>   iOptionals;
    std::vector<ofParameter<bool>*>  bOptionals;
    
};

}}


