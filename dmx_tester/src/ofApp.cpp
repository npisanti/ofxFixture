#include "ofApp.h"

void ofApp::setup() {
	
    ofxGuiSetDefaultWidth(512);
	
    int numChannels = 512;
	dmx.connect( "ttyUSB0", numChannels ); // use the name
	//dmx.connect(0); // or use a number
     
    channels.resize( 4 );
     
    panel.setName("Panel");
    panel.setup("", "settings.xml", 10, 10 );   
    panel.add( channels[0].setup("channel A"));
    panel.add( channels[1].setup("channel B"));
    panel.add( channels[2].setup("channel C"));
    panel.add( channels[3].setup("channel D"));

    ofBackground( 0 );
    ofSetWindowTitle( "dmx channel tester" );
}

void ofApp::update() { 
    for( auto & channel : channels ){
        channel.update( dmx );
    }   
	dmx.update();
}

void ofApp::draw() {
    panel.draw();
}


void ofApp::keyPressed( int key ) {
	switch( key ){

        case 'r':
            dmx.clear();
        break;
	}
}

void ofApp::exit() {
	dmx.clear();
	dmx.update();
	cout<<"clearing dmx...\n";
	ofSleepMillis( 500 );
	dmx.disconnect();
}
