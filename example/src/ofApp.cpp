#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
   
    ofSetWindowTitle( "moving spot simulation" );
	ofSetVerticalSync(true);
    
    gui.setup("settings", "settings.xml", ofGetWidth()-220, 20 );
    positions.setup("positions", "positions.xml", ofGetWidth()-440, 20 );
 
    transitioning = 0;
    cursor = 0.0f;
    
    // -------------------- simulation ------------------------------  
   
    // stage dimensions, I like to think it as centimeters
    float sw = 1200.0f;
    float sh = 600.0f;
    float sd = 800.0f;
    
    // boundaries for fixtures position and targets
    ofx::fixture::setBoundaries( sw, sh, sd );
    
    simulation.setStage( sw, 500, sd ); // stage can be smaller than boundaries
    simulation.setGraphics( 20, 20, 900, 800 );
    simulation.setDrawAddress( true );
    
    // use this to change the floor or wall color
    // simulation.setFloorColor( ofColor(40) );
    // simulation.setWallColor( ofColor(255, 200, 200 ) );
    
    // -------------------- heads -----------------------------------
    heads.resize(2);
    heads[0].setup( dmx, 1 );
    heads[0].position.set( glm::vec3( sw*0.25f, sh, sd*0.5f) );
    heads[1].setup( dmx, 41 );
    heads[1].position.set( glm::vec3( sw*0.75f, sh, sd*0.5f) );
 
    for( auto & head : heads ){ 
        simulation.add( head ); 
        states.add( head );
        positions.add( head.installation );
        gui.add( head.parameters );
    }
      
    // -------------------- spots ---------------------------------
    spots.resize(5);
    for( size_t i=0; i<spots.size(); ++i ){
        spots[i].position.set( glm::vec3( sw*0.25f*i, sh, sd*0.5f) );
    }
    spots[0].setup( dmx, 79 );
    spots[1].setup( dmx, 84 );
    spots[2].setup( dmx, 85 );
    spots[3].setup( dmx, 89 );
    spots[4].setup( dmx, 97 );
    
    for( auto & spot : spots ){ 
        simulation.add( spot ); 
        states.add( spot );
        positions.add( spot.installation );
        gui.add( spot.dimmer ); // spots have just one parameter, so we add that
        spot.dimmer = 0.0f;
    }

    bar.setup( dmx, 270 );
    simulation.add( bar );
    states.add( bar );
    positions.add( bar.installation );
    gui.add( bar.parameters );
    // ------------------ states ------------------------------------
    // init your snapshot only after you'added all the heads 
    // and spots to the state manager 
    states.init( "one" );
    states.init( "two" );
    
    // adds misc parameters to gui ----------------------------------
    gui.add( ofx::fixture::Dimmer::bDrawAddress ); // static ofParameter<bool>
    gui.add( bTargetDemo.set("target demo", true) );
    
    gui.add( states.multiple.parameters );
    gui.minimizeAll();
    
    positions.minimizeAll();
    positions.loadFromFile( "positions.xml" );

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if( bTargetDemo ){ // chase a noise generated target 
        for( auto & head : heads ){
            head.setTarget( glm::vec3( 
                    ofNoise(ofGetElapsedTimef()*0.08f, 0) * 1200.0f, 
                    0.0f,
                    ofNoise(ofGetElapsedTimef()*0.08f, 2) * 800.0f
            ));        
        }        
    }
    
    // transition routines 
    if(transitioning){
        switch( transitioning ){
            case 1:
                // transition from temp snapshot 
                states.transition("one", cursor); 
            break;
            
            case 2:
                // transition from temp snapshot 
                states.transition("two", cursor);
            break;
            
            case 3:
                // transition between two snapshots
                states.transition("one", "two", cursor);
            break;
            
        }        
        if( cursor == 1.0f){ 
            transitioning = 0; 
        }
        cursor += 0.003f;
        if(cursor>1.0f){ cursor = 1.0f; }
    }

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

        case '1': 
            std::cout << "storing snapshot one\n";
            states.store( "one"); 
        break;
        
        case '2': 
            std::cout << "storing snapshot two\n";
            states.store( "two"); 
        break;
        
        case '3': 
            std::cout << "storing to temp snapshot\n";
            states.store(); 
        break;
        
        case 'q': 
            std::cout << "recalling snapshot one\n";
            states.recall( "one"); 
            bTargetDemo=false; 
        break;
        
        case 'w': 
            std::cout << "recalling snapshot two\n";
            states.recall( "two"); 
            bTargetDemo=false; 
        break;
        
        case 'e': 
            std::cout << "recalling temp snapshot\n";
            states.recall(); 
            bTargetDemo=false; 
        break;
        
        case 'r': 
            std::cout<< "storing origin and transitioning to snapshot one\n";
            states.storeOrigin(); // it is the same as .store() 
            cursor = 0.0f;
            transitioning=1; 
            bTargetDemo=false; 
        break;
        
        case 't': 
            std::cout<< "storing origin and transitioning to snapshot two\n";
            states.store(); // it is the same as .storeOrigin() 
            cursor = 0.0f;
            transitioning=2; 
            bTargetDemo=false; 
        break;
        
        case 'y': 
            std::cout<< "transitioning from snapshot one to snapshot two\n";
            cursor = 0.0f;
            transitioning=3; 
            bTargetDemo=false; 
        break;
        

        
    }
    
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
