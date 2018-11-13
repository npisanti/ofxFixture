
#include "Manager.h"

ofx::fixture::Manager::Manager() : multiple( snapshots.multiple.parameters ){

    parameters.setName( "fixture manager" );
        positions.setName( "fixtures positions" ); 
        parameters.add( positions );
        controls.setName( "fixtures controls" ); 
        parameters.add( controls );
        
}
    
void ofx::fixture::Manager::setup( ofxDmx & dmx, float worldWidth, float worldHeight, float worldDepth, bool useSimulation ){
    this->useSimulation = useSimulation;
    this->dmx = &dmx;
    
    ofx::fixture::setBoundaries( worldWidth, worldHeight, worldDepth );
    
    if( useSimulation ){
        simulation.setStage();
        simulation.setGraphics( 20, 20, 640, 480 ); 
    }else{
        simulation.disable();
    }
}

void ofx::fixture::Manager::add( Dimmer & dimmer ){
    checkForCollision( dimmer );
    
    simulation.add( dimmer );
    snapshots.add( dimmer );
    positions.add( dimmer.installation );
    controls.add( dimmer.parameters );
}

void ofx::fixture::Manager::add( Spot & spot ){
    checkForCollision( spot );
    
    simulation.add( spot );
    snapshots.add( spot );
    positions.add( spot.installation );
    controls.add( spot.dimmer );
}

void ofx::fixture::Manager::add( Head & head ){
    checkForCollision( head );
    
    simulation.add( head );
    snapshots.add( head );
    positions.add( head.installation );
    controls.add( head.parameters );

}

void ofx::fixture::Manager::update( bool sendDmx ){
    
    for( auto & fixt : snapshots.dimmers ){ fixt->update(); }
    for( auto & fixt : snapshots.heads ){ fixt->update(); }
    
    if( useSimulation ){ simulation.update(); }
    
    if( sendDmx ) dmx->update();
}

void ofx::fixture::Manager::draw(){
    simulation.draw();
}

void ofx::fixture::Manager::close(){
    
    ofLogNotice()<<"[ofxFixture] closing heads...";
    for( auto & fixt : snapshots.heads ){ fixt->close(); }

	ofLogNotice()<<"[ofxFixture] clearing dmx...";    
	dmx->clear();
	dmx->update();
	ofSleepMillis( 200 );

}

void ofx::fixture::Manager::checkForCollision( Dimmer & fixt ){
    
    for( auto & other : snapshots.dimmers ){ 
        fixt.checkDmxCollision( *other );
    }
    for( auto & other : snapshots.heads ){ 
        fixt.checkDmxCollision( *other );
    }
   
}
