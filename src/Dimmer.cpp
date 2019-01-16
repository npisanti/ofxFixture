
#include "Dimmer.h"

glm::vec3 ofx::fixture::Dimmer::boundaries = glm::vec3( 1600.0f, 800.0f, 1200.0f );

ofParameter<bool> ofx::fixture::Dimmer::bDrawAddress = ofParameter<bool>( "draw address", false );
bool ofx::fixture::Dimmer::bSetupGLLights = false;

ofx::fixture::Dimmer::Dimmer(){

    changeDimmerName = false;
    bHasOptions = false;
    
    parameters.setName( "fixture base class" );
    parameters.add( dimmer.set("dimmer", 0.0f, 0.0f, 1.0f) );

    installation.setName( "fixture base class pos" );
    installation.add( armed.set("armed dmx", true ) );
    position.addListener( this, &Dimmer::onPositionChanged);
    installation.add( position.set("position", glm::vec3(0, 0, 0 ), glm::vec3(0,0,0), boundaries ) );
    orientation.addListener( this, &Dimmer::onOrientationChanged);
    installation.add( orientation.set("orientation", glm::vec3(180, 180, 0 ), glm::vec3(0, 0, 0), glm::vec3(360, 360, 360) ) );
    
    fOptionals.clear();
    iOptionals.clear();
    bOptionals.clear();
    
    dmxBoundaryAdd = 0;
}

std::string ofx::fixture::Dimmer::fixtureName(){
    return "dimmer";
}

void ofx::fixture::Dimmer::setup( ofxDmx & dmx, int channel, int universe, std::string name ){
    this->channel = channel;
    this->universe = universe;
    this->dmx = &dmx;
    
    address = "u"+ofToString(universe) + "ch" + ofToString(channel);
    
    if( name=="" ){
        parameters.setName( address+" " + fixtureName() );
        if(changeDimmerName) dimmer.setName( address+" " + fixtureName() );
        installation.setName( address+" " + fixtureName());
    }else{
        parameters.setName( name );
        if(changeDimmerName) dimmer.setName( name );
        installation.setName( name );
    }
    
    // update to boundaries
    position.set( position.getName(), position.get(), glm::vec3(0, 0, 0), boundaries );    

    if( bHasOptions ){
        options.setName("options");
        parameters.add( options );
    }
    
    init();
    
}

void ofx::fixture::Dimmer::onPositionChanged( glm::vec3 & value ){
    node.setPosition( value );
}

void ofx::fixture::Dimmer::onOrientationChanged( glm::vec3 & value ){
    node.setOrientation( value );
}

void ofx::fixture::Dimmer::setMaxDmxChannel( int max ){
    dmxBoundaryAdd = max-1;
}

void ofx::fixture::Dimmer::setBoundaries( glm::vec3 dimensions ){ boundaries = dimensions; }

void ofx::fixture::Dimmer::setBoundaries( float w, float h, float d ){ boundaries = glm::vec3( w, h, d ); }

const glm::vec3 & ofx::fixture::Dimmer::getBoundaries(){ return boundaries; }

const glm::vec3 & ofx::fixture::getBoundaries(){ return Dimmer::getBoundaries(); }
void ofx::fixture::setBoundaries( glm::vec3 dimensions  ){ Dimmer::setBoundaries( dimensions ); }
void ofx::fixture::setBoundaries( float w, float h, float d  ){ Dimmer::setBoundaries( w, h, d ); }


void ofx::fixture::Dimmer::addOption( ofParameter<float> & parameter ){
    options.add( parameter );
    fOptionals.push_back( &parameter );
    bHasOptions = true;
}

void ofx::fixture::Dimmer::addOption( ofParameter<int> & parameter ){
    options.add( parameter );
    iOptionals.push_back( &parameter );
    bHasOptions = true;
}

void ofx::fixture::Dimmer::addOption( ofParameter<bool> & parameter ){
    options.add( parameter );
    bOptionals.push_back( &parameter );
    bHasOptions = true;
}

void ofx::fixture::Dimmer::checkDmxCollision( const Dimmer & other ){
   
    bool collision = false;
    
    int min0 = channel;
    int max0 = channel + dmxBoundaryAdd;
    
    int min1 = other.channel;
    int max1 = other.channel + other.dmxBoundaryAdd;
    
    if( (max0>=min1 && min0<max1) || (max1>=min0 && min1<max0) ){ collision = true; }

    if( universe!= other.universe ){ collision = false; }
    
    if(collision){
        ofLogWarning() << "[ofxFixture] dmx address collision between fixtures "<<parameters.getName() <<" and "<<other.parameters.getName()<<"\n";
        ofLogWarning() << "disarming fixture "<<parameters.getName() <<"\n";
        armed = false;
    }
}

void ofx::fixture::Dimmer::draw2D( float scale ){
    static const float side =     8.0f;
    static constexpr float half = 8.0f * 0.5f;
    float x = position.get().x * scale  - half;
    float y = position.get().z * scale - half;
    ofSetColor( 255 * dimmer );
    ofFill();
    ofDrawRectangle( x, y, side, side );
    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle( x, y, side, side );
}
