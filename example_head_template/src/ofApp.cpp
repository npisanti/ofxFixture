#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
   
    ofSetWindowTitle( "head subclassing" );
	ofSetVerticalSync(true);
    
    int numChannels = 512;
	dmx.connect( "ttyUSB0", numChannels ); // use the name
	//dmx.connect(0); // or use a number
     
    gui.setup("settings", "settings.xml", ofGetWidth()-220, 20 );
    positions.setup("positions", "positions.xml", ofGetWidth()-440, 20 );

    // -------------------- simulation ------------------------------  
   
    // stage dimensions, I like to think it as centimeters
    float sw = 600.0f;
    float sh = 300.0f;
    float sd = 400.0f;
    
    // boundaries for fixtures position and targets
    ofx::fixture::setBoundaries( sw, sh, sd );
    
    simulation.setStage( sw, 500, sd ); // stage can be smaller than boundaries
    simulation.setGraphics( 20, 20, 900, 800 );

    
    // -------------------- head ------------------------------------
    head.setup( dmx, 1 );
    head.position.set( glm::vec3( sw*0.5f, sh, sd*0.75f) );

    simulation.add( head ); 
    
    positions.add( head.installation );
    gui.add( head.parameters );

}

//--------------------------------------------------------------
void ofApp::update(){
    
    head.update();
    
    // updates simulation fbo
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
    switch( key ){
        
    }    
}

//--------------------------------------------------------------
void ofApp::exit(){
	dmx.clear();
	dmx.update();
	cout<<"clearing dmx...\n";
	ofSleepMillis( 500 );
	dmx.disconnect();
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
