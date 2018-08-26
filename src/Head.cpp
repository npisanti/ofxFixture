
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
    box.setPosition( 0, -10, 0 );
    box.set( 20, 5, 20 );

    bottom.setParent(node);
    bottom.setPosition( 0, -14, 9 );
    bottom.set( 10, 1, 2 );

    label.setParent(node);
    label.setPosition(0, -10, 10);
    label.set( 10, 3, 1 );
    spot.set( 10, 15, 6 );
    spot.setParent( head );
    spot.rotateDeg( 90.0f, glm::vec3( 1, 0, 0) );

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

    // draw spot spot
    ofSetColor(ofColor::white);
    spot.draw();
    ofSetColor( ofColor::black );
    spot.setScale(1.002f);
    spot.drawWireframe();
    spot.setScale(1.f);  

    ofSetColor( 255, 120 );
    box.draw();
    ofSetColor( ofColor::red ); // this should be of the light color
    label.draw();
    bottom.draw();
    
    // draw target
    ofSetColor( ofColor::red ); // use fixture light color
    ofDrawSphere( target, 5 );
    ofDrawLine( node.getPosition(), target );

}

void ofx::fixture::Head::setDmx( int specificationCh, int value ){
    dmx->setLevel( specificationCh-1 + channel, value, universe );
}


// --------------------- internal functions -------------------------
void ofx::fixture::Head::setTarget( glm::vec3 pos ){
    
    const glm::vec3 & orientation = this->orientation.get();
    const glm::vec3 & position = this->position.get();

    target.set(pos);    
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
    
    if( angle >= tiltMin && angle <= tiltMax)
    {
        return angle;
    }
    else
    {
        return 0;
    }
}
