
#include "LightBar.h"

std::string ofx::fixture::LightBar::fixtureName() {
    return "light bar";
}

void ofx::fixture::LightBar::setNumLights( int num, bool reverse  ){
    
    bReverse = reverse;
    
    lights.resize( num );
    lv.resize( num );
    tips.resize(num);
    
    box.setParent( node );
    box.set( num*10, 5, 5 );
    box.setPosition( 0, 0, 0 );
    box.tiltDeg(-90);
    
    for( size_t i=0; i<lv.size(); ++i ){

        lv[i].setSpotlightCutOff( 6.0f );
        lv[i].setDiffuseColor( ofColor(255, 255, 120)*dimmer );
        
        tips[i].setParent( box );
        tips[i].setPosition( -(num-1)*5 + ofMap( i, 0, num-1, 0, (num-1)*10 ), 4, 0 );
        tips[i].set( 2.5, 2 );    

        lv[i].setParent( tips[i] );   
        lv[i].tiltDeg(-90);
        lv[i].setSpotlight();
        lv[i].setSpotConcentration( 10 );
        lv[i].setSpotlightCutOff( 5.0f );
        lv[i].setup();
        
        addOption(lights[i].set("light "+ofToString(i), 1.0f, 0.0f, 1.0f) );
    }

}


void ofx::fixture::LightBar::update(){
    if(bReverse){
        for(size_t i=0; i<lights.size(); ++i ){
            setDmx( lights.size()-i, dimmer*lights[i]*255.0f );
        }        
    }else{
        for(size_t i=0; i<lights.size(); ++i ){
            setDmx( i+1, dimmer*lights[i]*255.0f );
        }        
    }
}


void ofx::fixture::LightBar::draw(){
    ofSetColor(0);
    box.draw();

    for( size_t i=0; i<tips.size(); ++i ){
        ofSetColor( ofColor(255, 255, 150)*dimmer*lights[i] );
        tips[i].draw();
    }   
    
}


void ofx::fixture::LightBar::enableLight(){ 
    for( size_t i=0; i<lv.size() && i<8; ++i ){
        lv[i].setDiffuseColor( ofColor(255, 255, 120)*dimmer*lights[i] );
        lv[i].enable();
    } 
}

void ofx::fixture::LightBar::disableLight(){ 
    for( size_t i=0; i<lv.size() && i<8; ++i ){
        lv[i].disable();
    } 
}
