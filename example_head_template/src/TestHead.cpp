
#include "TestHead.h"


ofx::fixture::Vendor::TestHead::TestHead(){
    // add your custom parameter and setup here
    
    
    // sets pan and tilt ranges
    setPanRange( -225, 225 );
    setTiltRange( -150, 150 );
    
    // sets the max dmx channel used in dmx specification
    // this is needed for automatic address collision warning 
    setMaxDmxChannel( 20 ); 
}

std::string ofx::fixture::Vendor::TestHead::fixtureName(){
    return "test head";
}

void ofx::fixture::Vendor::TestHead::init(){
    // send init dmx values here with setDmx()
    // usually you have to find a way to enable lamps, open shutters, etc
    // and also set the dmx pan and tilt direction to a state compatible with ofxFixture
    
    // CHECKLIST OF SETUP MESSAGES TO SEND: 
    // 1) power on lamp / open shutter
    //      test that light is emitted
    // 2) set tilt reverse so that a tilt of -90 points away from the label 
    //       test it with the pan at 0
    // 3) set pan reverse so that the head rotate in the correct direction
    
}

void ofx::fixture::Vendor::TestHead::update(){
    // send dmx values here with setDmx()

}
