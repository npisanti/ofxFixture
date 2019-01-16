
#include "Simulation.h"

ofx::fixture::Simulation::Simulation(){
    bActive = true;
    bUseLights = false;

    wallMaterial.setShininess( 50 );
    wallMaterial.setDiffuseColor(ofFloatColor::white);
    wallMaterial.setSpecularColor(ofColor(255, 255, 255, 255));
    wallColor = ofColor( 20 );

    floorMaterial.setShininess( 50 );
    floorMaterial.setDiffuseColor(ofFloatColor::white);
    floorColor = wallColor;

    // easy cam
    camera.setPosition( glm::vec3(0.0f, 0.5f, 0.5f )); // up, front
    camera.setTarget( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    
    setStage( 1600.0f, 800.0f, 1200.0f ); 
    
    bDrawFixturesName.set( "draw fixture name", false );

    customDrawRoutine = []() noexcept {};

    use3dGraphics = true;
}

void ofx::fixture::Simulation::enable3DGraphics( bool active ){
    use3dGraphics = active;
}

void ofx::fixture::Simulation::enableLighting(){
    bUseLights = true;
    ofSetSmoothLighting(true);
    Dimmer::bSetupGLLights = true;
}

void ofx::fixture::Simulation::disableLighting(){
    bUseLights = false;
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
    
    camera.setDistance( getBoundaries().x*1.2f );    

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

void ofx::fixture::Simulation::add( Dimmer & fixt ){
    fixtures.push_back( &fixt );
}

void ofx::fixture::Simulation::update(){
   
    if( bActive ){
        if( use3dGraphics ){ // ------------- 3D GRAPHICS
            camera.setTarget( glm::vec3( getBoundaries().x*0.5f, 0.0f, 0.0f ));
            
            fbo.begin();
            
                ofClear(0, 0, 0, 0);
                ofPushStyle();
                ofEnableDepthTest();

                camera.begin();

                    for( auto & fixt : fixtures ){
                        fixt->draw();
                        if( Dimmer::bDrawAddress ){
                            ofSetColor( 255);    
                            ofDrawBitmapString( fixt->address, fixt->node.getPosition());             
                        }
                        if( bDrawFixturesName ){
                            ofSetColor( 255, 0, 0 );    
                            ofDrawBitmapString( fixt->parameters.getName(), fixt->node.getPosition());             
                        }
                    }

                    if( bUseLights ){
                        
                        ofEnableLighting();
                        for( auto & fixt : fixtures ){
                            fixt->enableLight();
                        }                
                        
                        floorMaterial.begin();
                            floor.draw();
                        floorMaterial.end();

                        wallMaterial.begin();
                            wall.draw();
                        wallMaterial.end();
                        
                        customDrawRoutine();
                        
                        for( auto & fixt : fixtures ){
                            fixt->disableLight();
                        }
                        ofDisableLighting();      
                                    
                    }else{

                        ofSetColor( floorColor );
                        floor.draw();
                        ofSetColor( wallColor );
                        wall.draw();

                        // draw lines in the floor borders
                        ofSetColor(0);
                        ofSetLineWidth( 2.0f );
                        ofDrawLine( 0, 0, 2, getBoundaries().x, 0, 2 ); 

                        ofSetColor(255);
                        customDrawRoutine();
                    }

                    // draw the origin 
                    ofSetColor( 255 );
                    ofDrawSphere( 0, 0, 0, 5);
                    
                camera.end();

                ofDisableDepthTest();
                ofPopStyle();
            fbo.end();            
        }else{ // ------------- 2D GRAPHICS
            float scale = fbo.getWidth() / getBoundaries().x;
            fbo.begin();
            ofPushStyle();
                ofClear(0, 0, 0, 0);
                for( auto & fixt : fixtures ){
                    fixt->draw2D( scale );
                    
                    float x = fixt->node.getPosition().x * scale + 6.0f;
                    float y = fixt->node.getPosition().z * scale;
                    if( Dimmer::bDrawAddress ){
                        ofSetColor( 255);    
                        ofDrawBitmapString( fixt->address, x,y );             
                    }
                    if( bDrawFixturesName ){
                        ofSetColor( 255, 0, 0 );    
                        ofDrawBitmapString( fixt->parameters.getName(), x, y ); 
                    }
                }
            ofPopStyle();
            fbo.end();  
        }
        

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
    
void ofx::fixture::Simulation::setWallColor( ofColor color ){ 
    wallColor = color;
    wallMaterial.setDiffuseColor(color); 
}

void ofx::fixture::Simulation::setFloorColor( ofColor color ){ 
    floorColor = color;
    floorMaterial.setDiffuseColor(color); 
}
