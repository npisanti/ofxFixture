
// the code for panAngle() and tiltAngle() is ported to glm from Shohei Fujimoto's oF_movingLightHead_object
// https://github.com/sfjmt/oF_movingLightHead_object

#pragma once 

#include "Dimmer.h"

namespace ofx { namespace fixture { 

class States;

class Head : public Dimmer {

    
public:
    Head();

    void setup( ofxDmx & dmx, int channel, int universe=1, std::string name="" ) override;

    void setTarget( glm::vec3 pos ){ chaseTarget=true; target = pos; }
    void setColor( const ofColor & color, bool alphaAsWhite=false );

    ofParameter<float> zoom;
    
    ofParameterGroup color;
        ofParameter<float> red;
        ofParameter<float> green;
        ofParameter<float> blue;
        ofParameter<float> white;

    ofParameter<float> pan;
    ofParameter<float> tilt;

    ofParameter<bool> chaseTarget;
    ofParameter<glm::vec3> target;

    std::string fixtureName() override;
    
    void draw() override;
    void draw2D( float scale ) override;
    
    void enableLight() override;
    void disableLight() override;
    
    // call this to send messages on exit
    virtual void close(){}
    
    void avoidJump( float angle ); // for moving without jump when possible
    
protected: // to be called from subclasses

    // those have to be used in the constructor 
    void setPanRange( float min, float max );
    void setTiltRange( float min, float max );
    

    void setDmxDimmer16bit( int coarseChannel, int fineChannel ){
        int dim = dimmer * 65535;
        int dim0 = (dim >> 8) & 0x00ff;
        int dim1 =  dim       & 0x00ff;
        setDmx( coarseChannel, dim0 );
        setDmx( fineChannel, dim1 );	
    }

    void setDmxPan16bit( int coarseChannel, int fineChannel){
        int value = ofMap( pan, panMin, panMax, 65535, 0 );
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        setDmx( coarseChannel, coarse );
        setDmx( fineChannel, fine );
    }

    void setDmxPan16bitReversed( int coarseChannel, int fineChannel){
        int value = ofMap( pan, panMin, panMax, 0, 65535 );
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        setDmx( coarseChannel, coarse );
        setDmx( fineChannel, fine );
    }

    void setDmxTilt16bit( int coarseChannel, int fineChannel ){
        int value = ofMap( tilt, tiltMin, tiltMax, 0, 65535 ); 
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        setDmx( coarseChannel, coarse );
        setDmx( fineChannel, fine );
    }

    void setDmxTilt16bitReversed( int coarseChannel, int fineChannel ){
        int value = ofMap( tilt, tiltMin, tiltMax, 65535, 0 );
        int coarse = (value >> 8) & 0x00ff;
        int fine =    value       & 0x00ff;
        setDmx( coarseChannel, coarse );
        setDmx( fineChannel, fine );
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

};

}}
