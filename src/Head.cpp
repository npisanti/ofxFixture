
#include "Head.h"

ofx::fixture::Head::Head(){

    bHasExtra = false;
    
    // object init
    tiltMin = -180; 
    tiltMax = 180;
    panMin = -180; 
    panMax = 180;
    
    // default orientation
    orientation = glm::vec3( 180, 180, 0);

    // object internal graphics
    head.setPosition( 0.0f, 0.0f, 0.0f );
    head.setParent( node );  
      
    box.setParent(node);
    box.setPosition( 0, -15, 0 );
    box.set( 30, 10, 30 );

    label.setParent(node);
    label.setPosition(0, -15, 20);
    label.set( 20, 3, 1 );
    
    spot.setResolution( 16, 4 );
    spot.set( 10, 25 );
    spot.setPosition( 0, -5, 0 );
        
    spot.setParent( head );
    spot.tiltDeg(180);
    tip.setParent( spot );
    tip.setPosition( 0, -15, 0 );
    tip.set( 12, 4 );

    
    parameters.add( zoom.set("zoom", 0.0f, 0.0f, 1.0f) );
    
    color.setName( "color" );
    color.add( red.set("red", 255.0f, 0.0f, 255.0f) );
    color.add( green.set("green", 255.0f, 0.0f, 255.0f) );
    color.add( blue.set("blue", 255.0f, 0.0f, 255.0f) );
    color.add( white.set("white", 0.0f, 0.0f, 255.0f) );
    parameters.add( color );
    
    parameters.add( pan.set("pan", 0.0f, panMin, panMax) );
    parameters.add( tilt.set("tilt", 90.0f, tiltMin, tiltMax) );

    parameters.add( chaseTarget.set("chase target", false) );    
    target.addListener( this, &Head::onTargetChange);
    parameters.add( target.set("target", glm::vec3(0, 0, 0 ), glm::vec3(0,0,0), boundaries ) );
    
    fOptionals.clear();
    iOptionals.clear();
    bOptionals.clear();
}

void ofx::fixture::Head::setup( ofxDmx & dmx, int channel, int universe ) {
    target.set( target.getName(), target.get(), glm::vec3(0, 0, 0), boundaries );  
    Dimmer::setup( dmx, channel, universe );
    dimmer.setName("dimmer");
    if( bHasExtra ){
        extras.setName("extras");
        parameters.add( extras );
    }
}

void ofx::fixture::Head::setColor( const ofColor & color, bool alphaAsWhite ){
    red = color.r;
    green = color.g;
    blue = color.b;
    if(alphaAsWhite){
        white = color.a;
    }
}

void ofx::fixture::Head::setPanRange( float min, float max ){
    panMin = min;
    panMax = max;
    pan.set("pan", pan.get(), panMin, panMax);
}

void ofx::fixture::Head::setTiltRange( float min, float max ){
    tiltMin = min;
    tiltMax = max;
    tilt.set( "tilt", tilt.get(), tiltMin, tiltMax );
}

std::string ofx::fixture::Head::fixtureName(){
    return "virtual head";
}

void ofx::fixture::Head::update(){

}

void ofx::fixture::Head::draw(){

    head.setOrientation( glm::vec3(0,0,0) );
    head.panDeg( pan );
    head.tiltDeg( tilt );

    ofColor color( red, green, blue );

    // draw spot spot
    ofSetColor( 100 );
    spot.draw();

    ofSetColor( color*dimmer, 120 );     
    box.draw();

    ofSetColor( color*dimmer );  
    label.draw();
    tip.draw();
    
    if(chaseTarget){
        ofDrawSphere( target, 5 );
        ofDrawLine( node.getPosition(), target ); 
    }
    
    if( bDrawAddress ){
        ofSetColor( 255);    
        ofDrawBitmapString( address, node.getPosition());             
    }

}

void ofx::fixture::Head::setDmx( int specificationCh, int value ){
    dmx->setLevel( specificationCh-1 + channel, value, universe );
}


void ofx::fixture::Head::setDmxDimmer16bit( int coarseChannel, int fineChannel ){
	int dim = dimmer * 65535;
	int dim0 = (dim >> 8) & 0x00ff;
	int dim1 =  dim       & 0x00ff;
	dmx->setLevel( channel+coarseChannel-1, dim0,	universe );		
	dmx->setLevel( channel+fineChannel-1, 	dim1,	universe );		
}

void ofx::fixture::Head::setDmxPan16bit( int coarseChannel, int fineChannel, bool reverse ){
    int value;
    switch( reverse ){
        case false: value = ofMap( pan, panMin, panMax, 65535, 0 ); break;
        case true:  value = ofMap( pan, panMin, panMax, 0, 65535 ); break;
    }
    int coarse = (value >> 8) & 0x00ff;
    int fine =    value       & 0x00ff;
    dmx->setLevel( channel+coarseChannel-1, coarse,	universe );		
	dmx->setLevel( channel+fineChannel-1, 	fine,	universe );	
}

void ofx::fixture::Head::setDmxTilt16bit( int coarseChannel, int fineChannel, bool reverse ){
    int value;
    switch( reverse ){
        case false: value = ofMap( tilt, tiltMin, tiltMax, 0, 65535 ); break;
        case true:  value = ofMap( tilt, tiltMin, tiltMax, 65535, 0 ); break;
    }
    int coarse = (value >> 8) & 0x00ff;
    int fine =    value       & 0x00ff;
    dmx->setLevel( channel+coarseChannel-1, coarse,	universe );		
	dmx->setLevel( channel+fineChannel-1, 	fine,	universe );	
}


// --------------------- internal functions -------------------------
void ofx::fixture::Head::setTarget( glm::vec3 pos ){
    target.set( pos );
}

void ofx::fixture::Head::onTargetChange( glm::vec3 & value ){
    
    if(chaseTarget){
        const glm::vec3 & orientation = this->orientation.get();
        const glm::vec3 & position = this->position.get(); 
        const glm::vec3 & target = this->target.get();
        

        glm::quat qr = glm::quat( - orientation.x, glm::vec3(1.0f, 0.0f, 0.0f) );
        glm::quat qp = glm::quat( - orientation.y, glm::vec3(0.0f, 1.0f, 0.0f) );
        glm::quat qy = glm::quat( - orientation.z, glm::vec3(0.0f, 0.0f, 1.0f) );

        glm::quat qt = qr * qp * qy; // total

        // needed the old mat4 for this
        ofMatrix4x4 m4;
        m4.setRotate( qt );
        glm::mat4 m = m4;
        
        //glm::mat4 m = glm::toMat4( qt ); // this isn't working
        
        glm::vec4 translate_v = glm::vec4(  target.x - position.x,
                                            target.y - position.y,
                                            target.z - position.z, 
                                            0.0f);
        
        glm::vec4 targetRelative = translate_v * m;

        glm::vec3 v1 = glm::vec3(0, 0, 0);
        glm::vec3 v2 = glm::vec3(targetRelative.x, targetRelative.y, targetRelative.z);
        glm::vec3 v3 = glm::vec3(targetRelative.x, 0, targetRelative.z);
        
        pan  = panAngle(v3);
        tilt = tiltAngle(v1, v2, v3);        
    }

}

float ofx::fixture::Head::panAngle( glm::vec3 v){
    
    glm::vec2 v1 = glm::vec2(0, 0);
    glm::vec2 v2 = glm::vec2(v.x, 0);
    glm::vec2 v3 = glm::vec2(v.x, v.z);
    
    float theta;
    float d1 = glm::distance(v1, v2 );
    float d2 = glm::distance(v2, v3 );
    float d3 = glm::distance(v3, v1 );
    
    theta = (d1*d1 + d3*d3 - d2*d2) / (2 * d1 * d3);
    
    float angle = acos(theta) * (180.0f/PI);
    
    if( v3.x == 0) {
        angle = 0.0f;
        return angle;
    }
    
    if(v3.x <= 0.f && v3.y <= 0.f) {
        angle = abs(angle-90.f);
    } else if(v3.x <= 0.f && v3.y >= 0.f) {
        angle = angle + 90.f;
    } else if(v3.x >= 0.f && v3.y >= 0.f) {
        angle = abs(angle-90.f) - 180.f;
    }else{
        angle = angle - 90.0f;
    }

    //--calculate nearest angle to actual pan and check boundaries---
    float mindist = abs( pan-angle );
    float minangle = angle;
    
    float angledown = angle - 360.0f;
    while( angledown > panMin ){
        float dist = abs( pan-angledown );
        if( dist < mindist ){
            mindist = dist;
            minangle = angledown;
        }
        angledown -= 360.0f;
    }
    
    float angleup = angle + 360.0f;
    while( angleup < panMax ){
        float dist = abs( pan-angleup );
        if( dist < mindist ){
            mindist = dist;
            minangle = angleup;
        }
        angleup += 360.0f;
    }
    
    return minangle;

}

float ofx::fixture::Head::tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
    
    float theta;
    float d1 = glm::distance( v1, v2 );
    float d2 = glm::distance( v2, v3 );
    float d3 = glm::distance( v3, v1 );
    
    theta = (d1*d1 + d3*d3 - d2*d2) / (2 * d1 * d3);
    float angle = acos(theta) * (180/PI);
    
    angle -= 90.0f;
    
    // change this with optimization for nearest point in range
    if( angle >= tiltMin && angle <= tiltMax)
    {
        return angle;
    }
    else
    {
        if( angle >= tiltMin ) { 
            return tiltMax; 
        }else{
            return tiltMin;
        }
    }
}

void ofx::fixture::Head::addExtra( ofParameter<float> & parameter ){
    extras.add( parameter );
    fOptionals.push_back( &parameter );
    bHasExtra = true;
}

void ofx::fixture::Head::addExtra( ofParameter<int> & parameter ){
    extras.add( parameter );
    iOptionals.push_back( &parameter );
    bHasExtra = true;
}

void ofx::fixture::Head::addExtra( ofParameter<bool> & parameter ){
    extras.add( parameter );
    bOptionals.push_back( &parameter );
    bHasExtra = true;
}

