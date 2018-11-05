#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
   
    ofSetWindowTitle( "head subclassing" );
	ofSetVerticalSync(true);
    
    int numChannels = 512;
	dmx.connect( "ttyUSB0", numChannels ); // use the name
	//dmx.connect(0); // or use a number
     
    gui.setup("settings", "settings.xml", ofGetWidth()-220, 20 );
    //positions.setup("positions", "positions.xml", ofGetWidth()-440, 20 );

    // -------------------- simulation ------------------------------  
   
    // stage dimensions, I like to think it as centimeters
    float sw = 600.0f;
    float sh = 300.0f;
    float sd = 400.0f;
    
    fixtures.setup( dmx, sw, sh, sd );
    fixtures.simulation.setGraphics( 20, 20, 900, 800 );
    
    
    // -------------------- head ------------------------------------
    head.setup( dmx, 1 );
    head.position.set( glm::vec3( sw*0.5f, sh, sd*0.75f) );
    
    head.dimmer = 1.0f;

    fixtures.add( head ); 
    
    gui.add( fixtures.positions );
    gui.add( fixtures.controls );

}

//--------------------------------------------------------------
void ofApp::update(){

    fixtures.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    fixtures.draw();
    
    //positions.draw();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    // close all the fixtures
    // sometimes you don't want to call this,
    // for example if you don't want to shut the lamps off
    // when you just need to modify the program and restart it
    fixtures.close(); 
    
    // remember to disconnect the dmx
    dmx.disconnect();
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
