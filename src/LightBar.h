
#pragma once 

#include "Dimmer.h"

namespace ofx { namespace fixture { 

class LightBar : public Dimmer {

public:

    void setNumLights( int num, bool reverse=false );
    std::string fixtureName() override;

    void update() override;
    void draw() override;

    void enableLight();
    void disableLight();

    std::vector<ofParameter<float>> lights; 

private:
    
    bool bReverse;
    
    std::vector<ofLight> lv;
    ofBoxPrimitive box;
    std::vector<ofCylinderPrimitive> tips;

};
    
}}
