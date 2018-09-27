
#pragma once

#include "../LightBar.h"

namespace ofx { namespace fixture { namespace Showtec {
    
class SunstripActive : public LightBar {

public:
    SunstripActive(){ 
        // 10 lights, reversed DMX control
        setNumLights(10, true);
    }
    
};
    
}}}
