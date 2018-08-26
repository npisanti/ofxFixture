
#pragma once 

#include "Dimmer.h"

namespace ofx { namespace fixture { 

class Head : public Dimmer {
    
public:
    Head();

    std::string fixtureName() override;
    void update() override;
    void draw() override;
    
    void setTarget( glm::vec3 pos );

    ofParameter<float> tilt;
    ofParameter<float> pan;
    ofParameter<bool> bTarget;
    ofParameter<glm::vec3> target;

protected: // to be called from subclasses

    // specificationCh is the channel in the fixture's dmx specifation
    void setDmx( int specificationCh, int value );

    // those have to be set in the constructor 
    float panMin, panMax, tiltMin, tiltMax;

private:

    ofNode head;       
    ofBoxPrimitive box;
    ofBoxPrimitive label;
    ofBoxPrimitive bottom;
    ofBoxPrimitive spot;

    void setOrientation( float tilt, float pan );
    float panAngle( glm::vec3 v);
    float tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
        
};

}}


