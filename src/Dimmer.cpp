
#include "Dimmer.h"

bool ofx::fixture::Dimmer::bDrawAddress = false;

ofx::fixture::Dimmer::Dimmer(){
    parameters.setName( "fixture base class" );
    parameters.add( dimmer.set("dimmer", 0.0f, 0.0f, 1.0f) );
    parameters.add( zoom.set("zoom", 0.0f, 0.0f, 1.0f) );

    installation.setName( "fixture base class pos" );
    position.addListener( this, &Dimmer::onPositionChanged);
    installation.add( position.set("position", glm::vec3(0, 0, 0 ), glm::vec3(-600, -600, -600), glm::vec3(600, 600, 600) ) );
    orientation.addListener( this, &Dimmer::onOrientationChanged);
    installation.add( orientation.set("orientation", glm::vec3(180, 180,0 ), glm::vec3(0, 0, 0), glm::vec3(360, 360, 0) ) );
}

std::string ofx::fixture::Dimmer::fixtureName(){
    return "dimmer";
}

void ofx::fixture::Dimmer::update(){
    dmx->setLevel( channel, int( dimmer ), universe );
}

void ofx::fixture::Dimmer::draw(){
    // todo : add dimmer graphics 
}
    
void ofx::fixture::Dimmer::setup( ofxDmx & dmx, int channel, int universe ){
    this->channel = channel;
    this->universe = universe;
    this->dmx = &dmx;
    
    address = "u"+ofToString(universe) + "ch" + ofToString(channel);
    
    parameters.setName( address+" " + fixtureName() );
    installation.setName( "pos "+address+" " + fixtureName());
    
    init();
}

void ofx::fixture::Dimmer::onPositionChanged( glm::vec3 & value ){
    node.setPosition( value );
}

void ofx::fixture::Dimmer::onOrientationChanged( glm::vec3 & value ){
    node.setOrientation( value );
}
