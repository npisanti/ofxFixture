
#pragma once 

#include "Dimmer.h"

namespace ofx { namespace fixture { 

class States;

class Head : public Dimmer {
    friend class States;
    
public:
    Head();

    void setup( ofxDmx & dmx, int channel, int universe=1 ) override;

    void setTarget( glm::vec3 pos ){ target = pos; }
    void setColor( const ofColor & color, bool alphaAsWhite=false );

    ofParameter<float> zoom;
    
    ofParameterGroup color;
        ofParameter<float> red;
        ofParameter<float> green;
        ofParameter<float> blue;
        ofParameter<float> white;

    ofParameter<float> pan;
    ofParameter<float> tilt;
    
    ofParameterGroup extras;

    ofParameter<bool> chaseTarget;
    ofParameter<glm::vec3> target;

    std::string fixtureName() override;
    
    void update() override;
    void draw() override;
    
    void enableLight();
    void disableLight();
    
    // call this to send messages on exit
    virtual void close(){}
    
protected: // to be called from subclasses

    // those have to be used in the constructor 
    void setPanRange( float min, float max );
    void setTiltRange( float min, float max );

    
    // adds a custom ofParameter to snapshot management 
    void addExtra( ofParameter<float> & parameter );
    void addExtra( ofParameter<int> & parameter );
    void addExtra( ofParameter<bool> & parameter );

    
    // specificationCh is the channel in the fixture's dmx specifation
    void setDmx( int specificationCh, int value ){
        dmx->setLevel( specificationCh-1 + channel, value, universe );
    }

    void setDmxDimmer16bit( int coarseChannel, int fineChannel ){
        int dim = dimmer * 65535;
        int dim0 = (dim >> 8) & 0x00ff;
        int dim1 =  dim       & 0x00ff;
        dmx->setLevel( channel+coarseChannel-1, dim0,	universe );		
        dmx->setLevel( channel+fineChannel-1, 	dim1,	universe );		
    }

    void setDmxPan16bit( int coarseChannel, int fineChannel){
        int value = ofMap( pan, panMin, panMax, 65535, 0 );
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        dmx->setLevel( channel+coarseChannel-1, coarse,	universe );		
        dmx->setLevel( channel+fineChannel-1, 	fine,	universe );	
    }

    void setDmxPan16bitReversed( int coarseChannel, int fineChannel){
        int value = ofMap( pan, panMin, panMax, 0, 65535 );
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        dmx->setLevel( channel+coarseChannel-1, coarse,	universe );		
        dmx->setLevel( channel+fineChannel-1, 	fine,	universe );	
    }

    void setDmxTilt16bit( int coarseChannel, int fineChannel ){
        int value = ofMap( tilt, tiltMin, tiltMax, 0, 65535 ); 
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        dmx->setLevel( channel+coarseChannel-1, coarse,	universe );		
        dmx->setLevel( channel+fineChannel-1, 	fine,	universe );	
    }

    void setDmxTilt16bitReversed( int coarseChannel, int fineChannel ){
        int value = ofMap( tilt, tiltMin, tiltMax, 65535, 0 );
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        dmx->setLevel( channel+coarseChannel-1, coarse,	universe );		
        dmx->setLevel( channel+fineChannel-1, 	fine,	universe );	
    }


    // ---------- those are for customizing simulated light ---------
    // sets spot concentration as light.setSpotConcentration()
    void setSimulationConcentration( float value );
    // sets the light cutoff at the min and max zoom values
    void setSimulationLightCutoffRange( float zoomMin, float zoomMax );
    // this is a trick, make light more distant to make the spot larger
    void setSimulationLightDistanceRange( float zoomMax );
    
    
private:
    float cutoffMin, cutoffMax, lightDistMin, lightDistMax;
    float panMin, panMax, tiltMin, tiltMax;

    ofNode head;       
    ofBoxPrimitive box;
    ofBoxPrimitive label;
    ofCylinderPrimitive spot;
    ofCylinderPrimitive tip;
    ofLight light;

    void onTargetChange( glm::vec3 & value );

    void setOrientation( float tilt, float pan );
    float panAngle( glm::vec3 v);
    float tiltAngle( glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
        
    std::vector<ofParameter<float>*> fOptionals; 
    std::vector<ofParameter<int>*>   iOptionals;
    std::vector<ofParameter<bool>*>  bOptionals;
    
    bool bHasExtra;
    
};

}}
