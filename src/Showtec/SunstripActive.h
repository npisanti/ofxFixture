
#pragma once

#include "../LightBar.h"

namespace ofx { namespace fixture { namespace Showtec {
    
class SunstripActive : public LightBar {

public:
    SunstripActive(){ 
        // 8 lights, reversed DMX control
        setNumLights(8, true);
    }
    
};
    
}}}
