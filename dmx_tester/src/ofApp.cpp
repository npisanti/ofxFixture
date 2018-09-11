#include "ofApp.h"

void ofApp::setup() {
	
    ofxGuiSetDefaultWidth(512);
	
    int numChannels = 512;
	dmx.connect( "ttyUSB0", numChannels ); // use the name
	//dmx.connect(0); // or use a number

    values.resize(4);
     
    panel.setName("Panel");
    panel.setup("", "settings.xml", 10, 10 );   
    panel.add( values[0].setup("value A"));
    panel.add( values[1].setup("value B"));
    panel.add( values[2].setup("value C"));
    panel.add( values[3].setup("value D"));

    channelsGroup.setName("channels");
    channels.resize( values.size() );    

    channelsGroup.add( fixtureChannel.set( "fixture base channel", 1, 1, 512 ) );
    for( size_t i=0; i<channels.size(); ++i ){
        channelsGroup.add( channels[i].set( values[i].parameters.getName()+" chan", 1, 1, 128 ) );
    }

    panel.add( channelsGroup );
    
    panel.loadFromFile("settings.xml");
    
    ofBackground( 0 );
    ofSetWindowTitle( "dmx channel tester" );
}

void ofApp::update() { 
    for( size_t i=0; i<values.size(); ++i ){
        values[i].update( dmx, fixtureChannel + channels[i] - 1 );
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
