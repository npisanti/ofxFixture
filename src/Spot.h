
#pragma once 

#include "Dimmer.h"

namespace ofx { namespace fixture { 

class Spot : public Dimmer {

public:
    Spot();
    
    std::string fixtureName() override;

    void draw() override;

    void enableLight();
    void disableLight();

    ofParameter<float> zoom;
    
    void setTarget( glm::vec3 pos ){ node.lookAt(pos); }

private:
    ofLight light;
    ofCylinderPrimitive spot;
    ofCylinderPrimitive tip;

};
    
}}
