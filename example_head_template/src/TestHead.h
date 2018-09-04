
#pragma once

#include "ofxFixture.h"

// use the vendor name as namespace
// and the head model as class name

namespace ofx{ namespace fixture{ namespace Vendor{

class TestHead : public ofx::fixture::Head {

public:

    TestHead();

    std::string fixtureName() override;
    
    void init() override;
    void update() override;
    
};

}}}
