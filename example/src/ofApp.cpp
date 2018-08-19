#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofEnableDepthTest();
	ofSetCircleResolution(64);


    //  object init
    minTilt = 0; 
    maxTilt = 180;
    minPan = 0; 
    maxPan = 360;
    position.y = 250.0f;
    orientation.x = 180; // tilt
    orientation.y = 180; // pan 
    
    base.setPosition( position );
    base.setOrientation( orientation );

    // object internal graphics
    head.setPosition( 0.0f, 0.0f, 0.0f );
    head.setParent( base );  
      
    box.setParent(base);
    box.setPosition( 0, -10, 0 );
    box.set( 20, 5, 20 );

    bottom.setParent(base);
    bottom.setPosition( 0, -14, 9 );
    bottom.set( 10, 1, 2 );

    label.setParent(base);
    label.setPosition(0, -10, 10);
    label.set( 10, 3, 1 );
    spot.set( 10, 15, 6 );
    spot.setParent( head );
    spot.rotateDeg( 90.0f, glm::vec3( 1, 0, 0) );

    // walls
    floor.set( 600, 600 );
    floor.rotateDeg( 90.0f, glm::vec3(1,0,0) );

    wall.set( 600, 300 );
    wall.setPosition( 0, 150, -300 );

    // easy cam
    camera.setPosition( glm::vec3(0.0f, 0.5f, 1.0f )); // up, front
    camera.setTarget( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    
    ofSetWindowTitle( "moving spot simulation" );
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    setTarget( glm::vec3( 
            ofSignedNoise(ofGetElapsedTimef()*0.08f, 0) * 200.0f, 
            0.0f,
            ofSignedNoise(ofGetElapsedTimef()*0.08f, 2) * 200.0f
    ));

    //tilt = ofNoise( ofGetElapsedTimef()*0.08f, 0) * 180.f;
    //pan  = ofNoise( ofGetElapsedTimef()*0.08f, 2) * 360.f;

	
    //ofEnableLighting();
    camera.begin();

        //head.lookAt( target, {0.f,1.f,0.f} );
        
        head.setOrientation( glm::vec3(0,0,0) );
        head.panDeg( pan-90 );
        head.tiltDeg( tilt );
        
        //head.setOrientation( glm::vec3( pan-90, tilt, 0 ) );

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
        
        // draw lines in the floor borders
        ofSetColor(0);
        ofDrawLine( -300, 0, -299, 300, 0, -299 ); 

        // draw target
        ofSetColor( ofColor::red ); // use fixture light color
        ofDrawSphere( target, 5 );
        ofDrawLine( base.getPosition(), target );
        
        // draw grid 

        ofSetColor( 40 );
        floor.draw();
        wall.draw();
        
        ofSetColor( 255 );
        ofDrawSphere( 0, 0, 0, 5);
        
    camera.end();
    //ofDisableLighting();
    
    
    std::string info = "target x = ";
    info += ofToString( target.x );
    info += "\n";
    info += "target y = ";
    info += ofToString( target.y );
    info += "\n";
    info += "target z = ";
    info += ofToString( target.z );
    info += "\n";    
    info += "pan = ";
    info += ofToString( pan );
    info += "\n";    
    info += "tilt = ";
    info += ofToString( tilt ); 
    ofSetColor(255);
    ofDrawBitmapString( info, 20, 20 );
}

void ofApp::setTarget( glm::vec3 pos ){
    
    target = pos;

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

float ofApp::panAngle( glm::vec3 v){
    
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
    
    if( angle >= minPan && angle <= maxPan )
    {
        return angle;
    }
    else
    {
        return 0;
    }
}

float ofApp::tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
    
    float theta;
    float d1 = glm::distance( v1, v2 );
    float d2 = glm::distance( v2, v3 );
    float d3 = glm::distance( v3, v1 );
    
    theta = (d1*d1 + d3*d3 - d2*d2) / (2 * d1 * d3);
    float angle = acos(theta) * (180/PI);
    
    if( angle >= minTilt && angle <= maxTilt)
    {
        return angle;
    }
    else
    {
        return 0;
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
