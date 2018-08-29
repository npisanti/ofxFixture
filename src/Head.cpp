
#include "Head.h"

ofx::fixture::Head::Head(){

    //  object init
    tiltMin = 0; 
    tiltMax = 180;
    panMin = 0; 
    panMax = 360;
    
    // default orientation
    orientation = glm::vec3( 180, 180, 0);

    // object internal graphics
    head.setPosition( 0.0f, 0.0f, 0.0f );
    head.setParent( node );  
      
    box.setParent(node);
    box.setPosition( 0, -15, 0 );
    box.set( 40, 10, 40 );

    label.setParent(node);
    label.setPosition(0, -15, 20);
    label.set( 20, 3, 1 );
    spot.set( 10, 30, 6 );
    
    spot.setParent( head );
    spot.rotateDeg( 90.0f, glm::vec3( 1, 0, 0) );
    
    color.setName( "color" );
    color.add( red.set("red", 255.0f, 0.0f, 255.0f) );
    color.add( green.set("green", 255.0f, 0.0f, 255.0f) );
    color.add( blue.set("blue", 255.0f, 0.0f, 255.0f) );
    color.add( white.set("white", 0.0f, 0.0f, 255.0f) );
    parameters.add( color );
    
    parameters.add( chaseTarget.set("chase target", true) );
    
    parameters.add( pan.set("pan", 0.0f, panMin, panMax) );
    parameters.add( tilt.set("tilt", 0.0f, tiltMin, tiltMax) );
    
    target.addListener( this, &Head::onTargetChange);
    parameters.add( target.set("target", glm::vec3(0, 0, 0 ), glm::vec3(-600, -600, -600), glm::vec3(600, 600, 600) ) );
    
    fOptionals.clear();
    iOptionals.clear();
    bOptionals.clear();
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
    head.panDeg( pan-90 );
    head.tiltDeg( tilt );

    ofColor color( red, green, blue );

    // draw spot spot
    ofSetColor(ofColor::white);
    spot.draw();
    ofSetColor( ofColor::black );
    spot.setScale(1.002f);
    spot.drawWireframe();
    spot.setScale(1.f);  

    ofSetColor( color, 120 );
    box.draw();
    ofSetColor( color ); // this should be of the light color
    label.draw();
    
    if(chaseTarget){
        // draw target
        ofSetColor( color ); // use fixture light color
        ofDrawSphere( target, 5 );
        ofDrawLine( node.getPosition(), target ); 
        if( bDrawAddress ){
            ofSetColor( 255);    
            ofDrawBitmapString( address, target );              
        }
    }
    if( bDrawAddress ){
        ofSetColor( 255);    
        ofDrawBitmapString( address, node.getPosition());             
    }

}

void ofx::fixture::Head::setDmx( int specificationCh, int value ){
    dmx->setLevel( specificationCh-1 + channel, value, universe );
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
        angle = 90.0f;
        return angle;
    }
    
    if(v3.x <= 0.f && v3.y <= 0.f) {
        angle = abs(angle-90.f) + 90.f;
    } else if(v3.x <= 0.f && v3.y >= 0.f) {
        angle = angle + 180.f;
    } else if(v3.x >= 0.f && v3.y >= 0.f) {
        angle = abs(angle-90.f) + 270.f;
    }
    
    if( angle >= panMin && angle <= panMax )
    {
        return angle;
    }
    else
    {
        return 0;
    }
}

float ofx::fixture::Head::tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
    
    float theta;
    float d1 = glm::distance( v1, v2 );
    float d2 = glm::distance( v2, v3 );
    float d3 = glm::distance( v3, v1 );
    
    theta = (d1*d1 + d3*d3 - d2*d2) / (2 * d1 * d3);
    float angle = acos(theta) * (180/PI);
    
    
    // change this with optimization for nearest point in range
    if( angle >= tiltMin && angle <= tiltMax)
    {
        return angle;
    }
    else
    {
        return 0;
    }
}

void ofx::fixture::Head::addOptional( ofParameter<float> & parameter ){
    fOptionals.push_back( &parameter );
}

void ofx::fixture::Head::addOptional( ofParameter<int> & parameter ){
    iOptionals.push_back( &parameter );
}

void ofx::fixture::Head::addOptional( ofParameter<bool> & parameter ){
    bOptionals.push_back( &parameter );
}
