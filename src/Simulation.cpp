
#include "Simulation.h"

ofx::fixture::Simulation::Simulation(){
    bActive = true;

    ofSetSmoothLighting(true);
    
    material.setShininess( 50 );
    material.setDiffuseColor(ofFloatColor::white);
    material.setSpecularColor(ofColor(255, 255, 255, 255));

    // easy cam
    camera.setPosition( glm::vec3(0.0f, 0.5f, 0.5f )); // up, front
    camera.setTarget( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    
    setStage( 1600.0f, 800.0f, 1200.0f ); 
}


void ofx::fixture::Simulation::setStage(){
    setStage( getBoundaries().x, getBoundaries().y, getBoundaries().z );
}

void ofx::fixture::Simulation::setStage( float stageWidht, float stageHeight, float stageDepth ){

    // walls
    floor.setResolution( 400, 400 );
    floor.setPosition(getBoundaries().x*0.5f, 0.0f, getBoundaries().z*0.5f );
    floor.set( stageWidht, stageDepth );
    floor.setOrientation( glm::vec3(0,0,0) );
    floor.rotateDeg( -90.0f, glm::vec3(1,0,0) );

    wall.setResolution( 300, 300  );
    wall.set( stageWidht, stageHeight );
    wall.setPosition( stageWidht*0.5f, stageHeight*0.5f, 0 );
    
    camera.setDistance( getBoundaries().x*1.1f );    

}

void ofx::fixture::Simulation::setGraphics( int x, int y, int w, int h ){
    fbo.allocate( w, h );
    moveGraphics( x, y );
}

void ofx::fixture::Simulation::moveGraphics( int x, int y ){
    gx = x;
    gy = y;
    camera.setControlArea( ofRectangle(gx, gy, fbo.getWidth(), fbo.getHeight() ) );    
}

void ofx::fixture::Simulation::add( Head & head ){
    heads.push_back( &head );
}

void ofx::fixture::Simulation::add( Dimmer & dimmer ){
    dimmers.push_back( &dimmer );
}

void ofx::fixture::Simulation::update(){
    if( bActive ){
        camera.setTarget( glm::vec3( getBoundaries().x*0.5f, 0.0f, 0.0f ));
        
        fbo.begin();
        
            ofClear(0, 0, 0, 0);
            ofPushStyle();
            ofEnableDepthTest();

            camera.begin();

                ofPushMatrix();
                //ofTranslate( -getBoundaries().x*0.5f, 0.0f, -getBoundaries().z*0.5f );
                for( auto & head : heads ){
                    head->draw();
                }
                
                for( auto & dimmer : dimmers ){
                    dimmer->draw();
                }
                
                // draw lines in the floor borders
                ofSetColor(0);
                ofSetLineWidth( 2.0f );
                ofDrawLine( 0, 0, 2, getBoundaries().x, 0, 2 ); 
                
                ofPopMatrix();

                // draw walls 
                ofSetColor(255);

                ofEnableLighting();

                for( auto & head : heads ){
                    head->enableLight();
                }                
                material.begin();
                    floor.draw();
                    wall.draw();
                material.end();
                
                drawObjects();
                
                for( auto & head : heads ){
                    head->disableLight();
                }

                ofDisableLighting();
            

                ofSetColor( 255 );
                ofDrawSphere( 0, 0, 0, 5);
                
            camera.end();

            ofDisableDepthTest();
            ofPopStyle();
        fbo.end();
    }
}

void ofx::fixture::Simulation::draw(){
    if( bActive ){
        ofPushStyle();
            ofSetColor(255);
            ofNoFill();
            ofDrawRectangle( gx, gy, fbo.getWidth(), fbo.getHeight() );
            ofSetColor(255);
            fbo.draw( gx, gy );
        ofPopStyle();        
    }
}
    
void ofx::fixture::Simulation::enable(){
    bActive = true;
    camera.enableMouseInput();
}

void ofx::fixture::Simulation::disable(){
    bActive = false;
    camera.disableMouseInput();
}
    
void ofx::fixture::Simulation::setDrawAddress( bool drawAddress ){
    Dimmer::bDrawAddress = drawAddress;
}
    
void ofx::fixture::Simulation::toggleDrawAddress(){
    Dimmer::bDrawAddress = ! Dimmer::bDrawAddress;
}
    
