#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
   
    ofSetWindowTitle( "spots and snapshots" );
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
    
    
    // this enable the use of GL lightning
    // openGL supports only up to 8 lights
    // so you can use this only with less then 8 fixtures 
    manager.simulation.enableLighting();
    
    // ==============================================================  
    
    // use this if you don't want simulated graphics 
    // manager.setup( dmx, sw, sh, sd, false ); 
    
    manager.simulation.setStage( sw, 500, sd ); // stage can be smaller than boundaries
    manager.simulation.setGraphics( 20, 20, 900, 800 ); 
    manager.simulation.setDrawAddress( true );
    
    // use this to change the floor or wall color
    // manager.simulation.setFloorColor( ofColor(40) );
    // manager.simulation.setWallColor( ofColor(255, 200, 200 ) );

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
    
    
    // ------------ add fixtures to manager -------------------------
    for( auto & fixt : spots ){ manager.add( fixt ); }


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
    
    
    std::string info = "1 - store snapshot one\n";
    info +="2 - store snapshot two\n";
    info +="3 - store to origin snapshot\n";
    info += "q - recall snapshot one\n";
    info += "w - recall snapshot two\n";
    info += "e - recall origin snapshot\n";
    info += "r - store origin and transition to snapshot one\n";
    info += "t - store origin and transition to snapshot two\n";
    info += "y - transition from snapshot one to snapshot two\n";
    
    ofDrawBitmapString( info, 30, 40 );
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
