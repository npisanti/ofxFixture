
#pragma once

#include "ofxFixture.h"

// use the vendor name as namespace
// and the head model as class name

namespace ofx{ namespace fixture{ namespace Martin{

class Mac3Performance : public ofx::fixture::Head {

public:

    Mac3Performance();

    std::string fixtureName() override;
    
    void init() override;
    void update() override;
    void close() override;
    
    ofParameter<float> temperature;
    ofParameter<float> focus;
    ofParameter<int> goboSelect;
    ofParameter<float> goboRotate;
    ofParameter<float> goboAnimate;
    
    std::vector<ofParameter<float>> bladeAmounts;
    std::vector<ofParameter<float>> bladeAngles;

};

}}}
