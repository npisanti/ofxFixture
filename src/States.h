
#pragma once

#include "ofMain.h"
#include "Dimmer.h"
#include "Head.h"

namespace ofx{ namespace fixture {
    
class States {
    friend class Manager;

public:
    States();

    void init( std::string tag );
    
    void store( std::string tag );
    void store();
    
    void recall();
    void recall( std::string tag );
    
    void transition( std::string tagA, std::string tagB, float pct );
    void transition( std::string tag, float pct );
    void transition( float pct ); // from origin to destination
     
    void fade( std::string tag, float pct ); // fade from zero
    void fade(float pct ); // fades origin snap ( 0.0f is black )
    void black(); // all the fixtures' dimmers to zero
    
    void storeOrigin();
    void storeDestination();    
    
    void lightsRecall();
    void lightsRecall( std::string tag );
    void lightsTransition( std::string tagA, std::string tagB, float pct );
    void lightsTransition( std::string tag, float pct );
    void lightsTransition( float pct ); // from origin to destination
    
    void lightsFade( std::string tag, float pct  ){ fade( tag, pct);}
    void lightsFade( float pct ){ fade(pct);}
    
    void degreesRecall();
    void degreesRecall( std::string tag );
    void degreesTransition( std::string tagA, std::string tagB, float pct );
    void degreesTransition( std::string tag, float pct );
    void degreesTransition( float pct ); // from origin to destination
     
    struct MultipleUpdater{
        ofParameterGroup parameters;
            ofParameterGroup headsSelector;
                ofParameter<bool> selectAllHeads;            
                std::vector<ofParameter<bool>> bHeads;
            ofParameterGroup dimmersSelector;            
                ofParameter<bool> selectAllDimmers;
                std::vector<ofParameter<bool>> bDimmers;
            ofParameter<float> dimmer;
            ofParameter<float> zoom;
            ofParameter<float> red;
            ofParameter<float> green;
            ofParameter<float> blue;
            ofParameter<float> white;
            ofParameter<glm::vec3> target;
    } multiple;
    
private: // ---------------------------------------------------------

    void add( Dimmer & dimmer );
    void add( Head & head );

    static inline float lerp( const ofParameter<float> & x0, const ofParameter<float> & x1, float pct ){
        return x0.get()*(1.0f-pct) + x1.get()*pct;
    }

    struct DimmerSnapshot{
        void init( Dimmer & dimmer );
        void store( Dimmer & dimmer );
        void fade( Dimmer & dimmer, float pct );
        void recall( Dimmer & dimmer );
        void mix( Dimmer & dimmer, DimmerSnapshot & other, float pct );
        
        ofParameterGroup parameters;
            ofParameter<float> dimmer;
            std::vector<ofParameter<float>> fOptions;
            std::vector<ofParameter<int>>   iOptions; 
            std::vector<ofParameter<bool>>  bOptions; 
    };
    
    struct HeadSnapshot{
        void init( Head & head );
        void store( Head & head );
        void recall( Head & head );
            void recallLight( Head & head );
            void recallDegrees( Head & head );
        void mix( Head & head, HeadSnapshot & other, float pct );
            void mixLight( Head & head, HeadSnapshot & other, float pct );
            void mixDegrees( Head & head, HeadSnapshot & other, float pct );

        void fade( Head & head, float pct );
        
        ofParameterGroup parameters;
            ofParameter<float> dimmer;
            ofParameter<float> zoom;
            ofParameter<float> pan;
            ofParameter<float> tilt;
            ofParameter<bool> chaseTarget;
            ofParameter<glm::vec3> target;
            ofParameter<float> red;
            ofParameter<float> green;
            ofParameter<float> blue;
            ofParameter<float> white;
            std::vector<ofParameter<float>> fOptions;
            std::vector<ofParameter<int>>   iOptions; 
            std::vector<ofParameter<bool>>  bOptions; 
    };
    
    struct SystemSnapshot{
        SystemSnapshot();
        
        ofParameterGroup system;
        std::vector<DimmerSnapshot> dimmers;
        std::vector<HeadSnapshot> heads;
    };
    
    SystemSnapshot origin;
    SystemSnapshot destination;
    
    std::map<std::string, SystemSnapshot> snapshots;
    
    std::vector<Head*> heads;
    std::vector<Dimmer*> dimmers;
    
    void store( SystemSnapshot & snap );
    void fade( SystemSnapshot & snap, float pct );
    
    void recall( SystemSnapshot & snap );
    void transition( SystemSnapshot & snapA, SystemSnapshot & snapB, float pct );
    
    void lightsRecall( SystemSnapshot & snap );
    void lightsTransition( SystemSnapshot & snapA, SystemSnapshot & snapB, float pct );
    
    void degreesRecall( SystemSnapshot & snap );
    void degreesTransition( SystemSnapshot & snapA, SystemSnapshot & snapB, float pct );
    
    bool bTouching;
    
    void onSelectAllHeads( bool & value );
    void onSelectAllDimmers( bool & value );
    void onZoomChange( float & value );
    void onColorChange( float & value );
    void onDimmerChange( float & value );
    void onTargetChange( glm::vec3 & value );
    
    bool bTargetAdded;
};
    
    
}}
