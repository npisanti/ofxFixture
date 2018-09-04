
#include "TestHead.h"


ofx::fixture::Vendor::TestHead::TestHead(){
    // add your parameter and setup here
    
    
    // also set pan and tilt ranges
    setPanRange( 0, 360 );
    setTiltRange( -180, 180 );
}

std::string ofx::fixture::Vendor::TestHead::fixtureName(){
    return "test head";
}

void ofx::fixture::Vendor::TestHead::init(){
    // send init dmx values here with sendDmx()
    
}

void ofx::fixture::Vendor::TestHead::update(){
    // send dmx values here with sendDmx()

}
