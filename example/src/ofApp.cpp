#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "moving spot simulation" );
	ofSetVerticalSync(true);

    // stage dimensions, I like to think it as centimeters
    float sw = 1200.0f;
    float sh = 500.0f;
    float sd = 800.0f;
    
    simulation.setStage( sw, sh, sd );
    simulation.setGraphics( 20, 20, 760, 760 );
    
    head.setup( dmx, 1 );
    head.position.set( glm::vec3( sw*0.5f, sh, sd*0.5f) );
    simulation.add( head );
    
    
    positions.setup("positions", "positions.xml", ofGetWidth()-440, 20 );
    positions.add( head.installation );
    gui.setup("panel", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( head.parameters );
}

//--------------------------------------------------------------
void ofApp::update(){
    head.setTarget( glm::vec3( 
            ofNoise(ofGetElapsedTimef()*0.08f, 0) * 1200.0f, 
            0.0f,
            ofNoise(ofGetElapsedTimef()*0.08f, 2) * 800.0f
    ));
    
    simulation.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    simulation.draw();
    
    positions.draw();
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
