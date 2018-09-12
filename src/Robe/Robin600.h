
#pragma once

#include "ofxFixture.h"

namespace ofx{ namespace fixture{ namespace Robe{

class Robin600 : public ofx::fixture::Head {

public:

    Robin600();

    std::string fixtureName() override;
    
    void init() override;
    void update() override;
    void close() override;
    
};

}}}
