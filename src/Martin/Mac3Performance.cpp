
#include "Mac3Performance.h"

ofx::fixture::Martin::Mac3Performance::Mac3Performance(){
    setPanRange( -270, 270 );
    setTiltRange( -134, 134 );
    setMaxDmxChannel( 40 );
    
    addOption( temperature.set("color temperature", 0.0f, 0.0f, 1.0f) );
    addOption( focus.set("focus", 0.1f, 0.0f, 1.0f) );
    addOption( goboSelect.set("gobo select", 0, 0, 5) );
    addOption( goboRotate.set("gobo rotate", 0.0f, 0.0f, 1.0f) );
    addOption( goboAnimate.set("gobo animate", 0.0f, 0.0f, 1.0f) );

    bladeAmounts.resize(4);
    bladeAngles.resize(4);
    for(size_t i=0; i<4; ++i ){
        addOption( bladeAmounts[i].set("blade "+ofToString(i)+ " amount", 0.0f, 0.0f, 1.0f) );
        addOption( bladeAngles[i].set("blade "+ofToString(i)+ " angle", 0.5f, 0.0f, 1.0f) );
    }
}

std::string ofx::fixture::Martin::Mac3Performance::fixtureName(){
    return "Mac3 Perf";
}

void ofx::fixture::Martin::Mac3Performance::init(){
    setDmx( 39, 45 ); // lamp on
    
    // this is to avoid multiple lamp on messages
    // that could draw too much current
    ofSleepMillis( 50 ); 
    
    setDmx( 1, 255 ); // shutter open
}


void ofx::fixture::Martin::Mac3Performance::close(){
    
    setDmx( 39, 50 ); // lamp off
    setDmx( 1, 0 ); // shutter close
    setDmx( 30, 0 ); // iris to 0
    
    // reset to zero position 
    dimmer = 0.0f;
    pan = 0.0f;
    tilt = 0.0f;
    setDmxDimmer16bit( 2, 3 );
    setDmxPan16bit( 35, 36 );
    setDmxTilt16bit( 37, 38 );

}


void ofx::fixture::Martin::Mac3Performance::update(){
    
    setDmxDimmer16bit( 2, 3 );
    setDmxPan16bit( 35, 36 );
    setDmxTilt16bit( 37, 38 );

    setDmx( 33, (1.0f-zoom)*255.f ); // zoom
    //setDmx( 30, (1.0f-zoom)*200.f ); // iris 
    setDmx( 31, focus*255.f );
    
    setDmx( 4, 255.0f - red );   // cyan
    setDmx( 5, 255.0f - green ); // magenta
    setDmx( 6, 255.0f - blue );  // yellow
    setDmx( 7, 255.0f * temperature ); // color temperature
    
    setDmx( 9, 5 + 5*goboSelect ); 
    setDmx( 10, goboRotate*255 );
    setDmx( 26, 110 + (1.0f-goboAnimate)*145 );
    
    for( int i=0; i<4; ++i ){
        setDmx( 12 + i*2, bladeAmounts[i]*255);
        setDmx( 13 + i*2, bladeAngles[i]*255);
    }

}
