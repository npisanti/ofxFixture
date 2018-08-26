#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetCircleResolution(64);


    // walls
    floor.set( 600, 600 );
    floor.rotateDeg( 90.0f, glm::vec3(1,0,0) );

    wall.set( 600, 300 );
    wall.setPosition( 0, 150, -300 );

    // easy cam
    camera.setPosition( glm::vec3(0.0f, 0.5f, 1.0f )); // up, front
    camera.setTarget( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    
    ofSetWindowTitle( "moving spot simulation" );
    
    
    head.setup( dmx, 1 );
    head.position.set( glm::vec3(0, 225, 0) );
    
    gui.setup("panel", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( head.parameters );
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    head.setTarget( glm::vec3( 
            ofSignedNoise(ofGetElapsedTimef()*0.08f, 0) * 200.0f, 
            0.0f,
            ofSignedNoise(ofGetElapsedTimef()*0.08f, 2) * 200.0f
    ));

    //tilt = ofNoise( ofGetElapsedTimef()*0.08f, 0) * 180.f;
    //pan  = ofNoise( ofGetElapsedTimef()*0.08f, 2) * 360.f;

    ofEnableDepthTest();
    //ofEnableLighting();
    camera.begin();

        
        head.draw();
        
        
        // draw lines in the floor borders
        ofSetColor(0);
        ofDrawLine( -300, 0, -299, 300, 0, -299 ); 

        // draw grid 

        ofSetColor( 40 );
        floor.draw();
        wall.draw();
        
        ofSetColor( 255 );
        ofDrawSphere( 0, 0, 0, 5);
        
    camera.end();
    //ofDisableLighting();
    
    ofDisableDepthTest();
    


    gui.draw();
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
