
#include "Dimmer.h"

glm::vec3 ofx::fixture::Dimmer::boundaries = glm::vec3( 1600.0f, 800.0f, 1200.0f );

ofParameter<bool> ofx::fixture::Dimmer::bDrawAddress = ofParameter<bool>( "draw address", false );

ofx::fixture::Dimmer::Dimmer(){
    parameters.setName( "fixture base class" );
    parameters.add( dimmer.set("dimmer", 1.0f, 0.0f, 1.0f) );

    installation.setName( "fixture base class pos" );
    position.addListener( this, &Dimmer::onPositionChanged);
    installation.add( position.set("position", glm::vec3(0, 0, 0 ), glm::vec3(0,0,0), boundaries ) );
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
    
    // update to boundaries
    position.set( position.getName(), position.get(), glm::vec3(0, 0, 0), boundaries );    

    init();
}

void ofx::fixture::Dimmer::onPositionChanged( glm::vec3 & value ){
    node.setPosition( value );
}

void ofx::fixture::Dimmer::onOrientationChanged( glm::vec3 & value ){
    node.setOrientation( value );
}


void ofx::fixture::Dimmer::setBoundaries( glm::vec3 dimensions ){ boundaries = dimensions; }

void ofx::fixture::Dimmer::setBoundaries( float w, float h, float d ){ boundaries = glm::vec3( w, h, d ); }

const glm::vec3 & ofx::fixture::Dimmer::getBoundaries(){ return boundaries; }

const glm::vec3 & ofx::fixture::getBoundaries(){ return Dimmer::getBoundaries(); }
void ofx::fixture::setBoundaries( glm::vec3 dimensions  ){ Dimmer::setBoundaries( dimensions ); }
void ofx::fixture::setBoundaries( float w, float h, float d  ){ Dimmer::setBoundaries( w, h, d ); }
