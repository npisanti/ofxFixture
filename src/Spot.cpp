
#include "Spot.h"

std::string ofx::fixture::Spot::fixtureName() {
    return "spot";
}

ofx::fixture::Spot::Spot(){

    spot.setParent( node );
    
    spot.setResolution( 6, 1 );
    spot.set( 10, 25 );
    spot.setPosition( 0, 0, 0 );
        
    spot.tiltDeg(90);
    tip.setParent( spot );
    tip.setPosition( 0, -15, 0 );
    tip.set( 12, 4 );    
    
    if( bSetupGLLights ){
        light.setParent( tip );   
        light.tiltDeg(-90);
        light.setSpotlight();
        light.setSpotConcentration( 10 );
        light.setup();
    }

    installation.add( zoom.set("zoom", 0.1f, 0.0f, 1.0f) );
}
    

void ofx::fixture::Spot::draw() {
    ofSetColor(0);
    spot.draw();
    
    ofSetColor( ofColor(255, 255, 150)*dimmer );
    spot.setScale( 1.01f );
    spot.drawWireframe();
    spot.setScale( 1.0f );
    tip.draw();
}


void ofx::fixture::Spot::enableLight(){ 
    light.setSpotlightCutOff( 2.0f + zoom*38.0f );
    light.setDiffuseColor( ofColor(255, 255, 120)*dimmer );
    light.enable(); 
}

void ofx::fixture::Spot::disableLight(){ 
    light.disable();
}
