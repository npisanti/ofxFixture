#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
   
    ofSetWindowTitle( "moving spot simulation" );
	ofSetVerticalSync(true);
    
    gui.setup("settings", "settings.xml", ofGetWidth()-220, 20 );
    positions.setup("positions", "positions.xml", ofGetWidth()-440, 20 );
 
    transitioning = 0;
    cursor = 0.0f;



	dmx.connect( "ttyUSB0", 512 ); // use the name
	//dmx.connect(0); // or use a number

    // -------------------- set up manager --------------------------  
   
    // simulated world boundaries, I like to think it as centimeters
    float sw = 1200.0f;
    float sh = 600.0f;
    float sd = 800.0f;
    
    // ==== WARNING!!! call this function before all the others =====
    manager.setup( dmx, sw, sh, sd ); 
    // ==============================================================  
    
    // use this if you don't want simulated graphics 
    // manager.setup( dmx, sw, sh, sd, false ); 
    
    manager.simulation.setStage( sw, 500, sd ); // stage can be smaller than boundaries
    manager.simulation.setGraphics( 20, 20, 900, 800 ); 
    manager.simulation.setDrawAddress( true );
    
    // use this to change the floor or wall color
    // manager.simulation.setFloorColor( ofColor(40) );
    // manager.simulation.setWallColor( ofColor(255, 200, 200 ) );
    
    // -------------------- heads -----------------------------------
    heads.resize(2);
    heads[0].setup( dmx, 1 );
    heads[0].position.set( glm::vec3( sw*0.25f, sh, sd*0.5f) );
    heads[1].setup( dmx, 41 );
    heads[1].position.set( glm::vec3( sw*0.75f, sh, sd*0.5f) );
 
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
    
    // --------------------- light bar ------------------------------
    bar.setNumLights( 10 );
    bar.setup( dmx, 270 );

    
    // ------------ add fixtures to manager -------------------------
    for( auto & fixt : heads ){ manager.add( fixt ); }
    for( auto & fixt : spots ){ manager.add( fixt ); }
    manager.add( bar );

    // ------------------ states ------------------------------------
    // init your snapshot only after you'added all the heads 
    // and spots to the state manager 
    manager.snapshots.init( "one" );
    manager.snapshots.init( "two" );
    
    // adds misc parameters to gui ----------------------------------
    gui.add( manager.controls );
    gui.add( ofx::fixture::Dimmer::bDrawAddress ); // static ofParameter<bool>
    gui.add( bTargetDemo.set("target demo", true) );
    gui.add( manager.multiple );
    gui.minimizeAll();
    
    positions.add( manager.positions );
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
                manager.snapshots.transition("one", cursor); 
            break;
            
            case 2:
                // transition from temp snapshot 
                manager.snapshots.transition("two", cursor);
            break;
            
            case 3:
                // transition between two snapshots
                manager.snapshots.transition("one", "two", cursor);
            break;
            
        }        
        if( cursor == 1.0f){ 
            transitioning = 0; 
        }
        cursor += 0.003f;
        if(cursor>1.0f){ cursor = 1.0f; }
    }

    manager.update( false ); // don't send dmx in this example
    
    // usually you call this
    // manager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    manager.draw();
    
    positions.draw();
    gui.draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch( key ){

        case '1': 
            std::cout << "storing snapshot one\n";
            manager.snapshots.store( "one"); 
        break;
        
        case '2': 
            std::cout << "storing snapshot two\n";
            manager.snapshots.store( "two"); 
        break;
        
        case '3': 
            std::cout << "storing to temp snapshot\n";
            manager.snapshots.store(); 
        break;
        
        case 'q': 
            std::cout << "recalling snapshot one\n";
            manager.snapshots.recall( "one"); 
            bTargetDemo=false; 
        break;
        
        case 'w': 
            std::cout << "recalling snapshot two\n";
            manager.snapshots.recall( "two"); 
            bTargetDemo=false; 
        break;
        
        case 'e': 
            std::cout << "recalling temp snapshot\n";
            manager.snapshots.recall(); 
            bTargetDemo=false; 
        break;
        
        case 'r': 
            std::cout<< "storing origin and transitioning to snapshot one\n";
            manager.snapshots.storeOrigin(); // it is the same as .store() 
            cursor = 0.0f;
            transitioning=1; 
            bTargetDemo=false; 
        break;
        
        case 't': 
            std::cout<< "storing origin and transitioning to snapshot two\n";
            manager.snapshots.store(); // it is the same as .storeOrigin() 
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
