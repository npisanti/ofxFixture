
#include "Dimmer.h"


ofx::fixture::Dimmer::Dimmer(){
    
    parameters.setName( "fixture base class" );
    parameters.add( dimmer.set("dimmer", 0.0f, 0.0f, 255.0f) );

    position.addListener( this, &Dimmer::onPositionChanged);
    parameters.add( position.set("position", glm::vec3(0, 0, 0 ), glm::vec3(-600, -600, -600), glm::vec3(600, 600, 600) ) );

    orientation.addListener( this, &Dimmer::onOrientationChanged);
    parameters.add( orientation.set("orientation", glm::vec3(180, 180,0 ), glm::vec3(0, 0, 0), glm::vec3(360, 360, 0) ) );

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
    
    parameters.setName( fixtureName() + " ch" + ofToString(channel)+" u"+ofToString(universe) );
}

void ofx::fixture::Dimmer::onPositionChanged( glm::vec3 & value ){
    node.setPosition( value );
}

void ofx::fixture::Dimmer::onOrientationChanged( glm::vec3 & value ){
    node.setOrientation( value );
}
