
#include "Head.h"

ofx::fixture::Head::Head(){

    // object init
    tiltMin = -180; 
    tiltMax = 180;
    panMin = -180; 
    panMax = 180;
    
    // for light simulation 
    cutoffMin = 3.0f;
    cutoffMax = 35.0f;
    lightDistMin = 0.0f;
    lightDistMax = 20.0f;
    
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
    //spot.tiltDeg(180);
    tip.setParent( spot );
    tip.setPosition( 0, 15, 0 );
    tip.set( 12, 4 );

    if( bSetupGLLights ){
        light.setParent( tip );   
        light.tiltDeg(-90);
        light.setSpotlight();
        light.setSpotConcentration( 10 );
        light.setup();
    }
    
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

}

void ofx::fixture::Head::setup( ofxDmx & dmx, int channel, int universe, std::string name ) {
    target.set( target.getName(), target.get(), glm::vec3(0, 0, 0), boundaries );  
    Dimmer::setup( dmx, channel, universe, name );
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

void ofx::fixture::Head::enableLight(){ 
    
    //light.setSpotlightCutOff( 2+50*zoom*zoom );
    light.setSpotlightCutOff( ofMap( zoom, 0.0f, 1.0f, cutoffMin, cutoffMax ) );
    light.setPosition( 0, ofMap( zoom, 0.0f, 1.0f, lightDistMin, lightDistMax ), 0 );
    
    float r = red + white;
    r = (r<255.0f) ? r : 255.0f;
    float g = green + white;
    g = (g<255.0f) ? g : 255.0f;
    float b = blue + white;
    b = (b<255.0f) ? b : 255.0f;

    light.setDiffuseColor( ofColor(r, g, b)*dimmer );
    //light.setSpecularColor( ofColor(r, g, b) );
    
    light.enable(); 
}

void ofx::fixture::Head::disableLight(){ 
    light.disable(); 
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

}

// --------------------- internal functions -------------------------

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
    
    // i had to modify this to get it working 
    if(v3.x <= 0.f && v3.y <= 0.f) {
        angle = abs(angle-90.f);
    } else if(v3.x <= 0.f && v3.y >= 0.f) {
        angle = angle + 90.f;
    } else if(v3.x >= 0.f && v3.y >= 0.f) {
        angle = abs(angle-90.f) - 180.f;
    }else{
        angle = angle - 90.0f;
    }
    
    // voodoo code, still doesn't work for 1°-45°
    if( orientation.get().y != 0.0f ){ 
        angle += -orientation.get().y + 180.0f; 
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
    
    //angle += -orientation.get().x + 180.0f;
    
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

void ofx::fixture::Head::setSimulationConcentration( float value ){
    light.setSpotConcentration( value );
}

void ofx::fixture::Head::setSimulationLightCutoffRange( float zoomMin, float zoomMax ){
    cutoffMin = zoomMin;
    cutoffMax = zoomMax;
}

void ofx::fixture::Head::setSimulationLightDistanceRange( float zoomMax ){
    lightDistMin = 0.0f;
    lightDistMax = zoomMax;
}

void ofx::fixture::Head::draw2D( float scale ){
    static const float side =     12.0f;
    static constexpr float half = 12.0f * 0.5f;
    float x = position.get().x * scale - half;
    float y = position.get().z * scale - half;
    ofSetColor( red, blue, green, 255 * dimmer );
    ofFill();
    ofDrawCircle( x, y, side );
    ofSetColor(255);
    ofNoFill();
    ofDrawCircle( x, y, side );
}
