
#include "Robin600.h"

ofx::fixture::Robe::Robin600::Robin600(){
    setPanRange( -225, 225 );
    setTiltRange( -150, 150 );
}

std::string ofx::fixture::Robe::Robin600::fixtureName(){
    return "Robin 600";
}

void ofx::fixture::Robe::Robin600::init(){
	setDmx( 6, 89 );  // disabled blackout while pan/tilt moving
	setDmx( 35, 255 );  // shutter open     
    
    // todo : set highest speed for panning and tilting 
}

void ofx::fixture::Robe::Robin600::close(){

	setDmx( 35, 9 );  // shutter close     

    // resets to zero position
    pan = 0.0f;
    tilt = 0.0f; 
    dimmer = 0.0f;
    setDmxPan16bit( 1, 2 );
    setDmxTilt16bit( 3, 4 );
    setDmxDimmer16bit( 36, 37 );

}

void ofx::fixture::Robe::Robin600::update(){
    
    setDmxPan16bit( 1, 2 );
    setDmxTilt16bit( 3, 4 );
    setDmxDimmer16bit( 36, 37 );

    float zones[3];
    zones[0] = 1.0f;
    zones[1] = ofMap( zoom, 0.0f,  0.15f, 0.0f, 1.0f, true );
    zones[2] = ofMap( zoom, 0.15f, 0.30f, 0.0f, 1.0f, true );
    float zoomval = ofMap( zoom, 0.3f, 1.0f, 0, 255, true );

	for( int i=0; i<3; ++i ){
        setDmx( 7+i*8,  red * zones[i] );
        setDmx( 9+i*8,  green * zones[i]  );
        setDmx( 11+i*8, blue * zones[i]  );
        setDmx( 13+i*8, white * zones[i]  );
	}

    setDmx( 33, zoomval );
}
