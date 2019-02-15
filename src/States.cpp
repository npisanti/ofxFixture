
#include "States.h"

ofx::fixture::States::States(){
    
    bTouching = false;
    
    multiple.parameters.setName("update multiple");
    multiple.headsSelector.setName( "select heads" );
    multiple.headsSelector.add( multiple.selectAllHeads.set("select all", false) );
    multiple.parameters.add( multiple.headsSelector );
    multiple.dimmersSelector.setName( "select dimmers" );
    multiple.dimmersSelector.add( multiple.selectAllDimmers.set("select all", false) );
    multiple.parameters.add( multiple.dimmersSelector );

    multiple.parameters.add( multiple.dimmer.set("dimmer", 0.0f, 0.0f, 1.0f) );
    multiple.parameters.add( multiple.zoom.set("zoom", 0.0f, 0.0f, 1.0f) );
    multiple.parameters.add( multiple.red.set("red", 255.0f, 0.0f, 255.0f) );
    multiple.parameters.add( multiple.green.set("green", 255.0f, 0.0f, 255.0f) );
    multiple.parameters.add( multiple.blue.set("blue", 255.0f, 0.0f, 255.0f) );
    multiple.parameters.add( multiple.white.set("white", 255.0f, 0.0f, 255.0f) );
    
    multiple.selectAllHeads.addListener( this, &States::onSelectAllHeads );
    multiple.selectAllDimmers.addListener( this, &States::onSelectAllDimmers );
    
    multiple.dimmer.addListener( this, &States::onDimmerChange );
    multiple.zoom.addListener( this, &States::onZoomChange );
    multiple.red.addListener( this, &States::onColorChange );
    multiple.green.addListener( this, &States::onColorChange );
    multiple.blue.addListener( this, &States::onColorChange );
    multiple.white.addListener( this, &States::onColorChange );
    
    bTargetAdded = false;
    
}


void ofx::fixture::States::init( std::string tag ){
    
    if( snapshots.count( tag ) == 0 ){
        bTouching = true;
        
        snapshots.insert( std::make_pair( tag, SystemSnapshot() ));
        SystemSnapshot & snap = snapshots[tag];

        snap.heads.resize( heads.size() );
        for( size_t i=0; i<heads.size(); ++i ){
            snap.heads[i].init( *heads[i] );
            snap.system.add( snap.heads[i].parameters );
        }

        snap.dimmers.resize( dimmers.size() );
        for( size_t i=0; i<dimmers.size(); ++i ){
            snap.dimmers[i].init( *dimmers[i] );
            snap.system.add( snap.dimmers[i].parameters );
        }

        std::string path = ofToDataPath( "snapshots/"+tag+".json" );

        ofFile file( path );
        if( file.exists() ){
            ofLogVerbose() << "[ofx::fixture::States] found file for state name = "<<tag<<", loading";
            ofJson json = ofLoadJson( path );
            ofDeserialize( json, snap.system );
        }else{
            ofLogVerbose() << "[ofx::fixture::States] ile not found for state name = "<<tag;
        }                
    }else{
        ofLogVerbose() << "[ofx::fixture::States] tag "<< tag <<" already initialized";
    }
}


void ofx::fixture::States::store( std::string tag ){
    if( snapshots.count( tag ) >0 ){
        SystemSnapshot & snap = snapshots[tag]; 
        store( snap );
        
        ofJson json;
        
        ofSerialize( json, snap.system );
        std::string path = ofToDataPath( "snapshots/"+tag+".json" );
        ofSavePrettyJson( path, json );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::recall( std::string tag ){
    if( snapshots.count( tag ) >0 ){
        recall( snapshots[tag] );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::lightsRecall( std::string tag ){
    if( snapshots.count( tag ) >0 ){
        lightsRecall( snapshots[tag] );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::degreesRecall( std::string tag ){
    if( snapshots.count( tag ) >0 ){
        degreesRecall( snapshots[tag] );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::transition( std::string tagA, std::string tagB, float pct ){
    if( snapshots.count( tagA ) >0 && snapshots.count( tagB ) ){
        transition( snapshots[tagA], snapshots[tagB], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::lightsTransition( std::string tagA, std::string tagB, float pct ){
    if( snapshots.count( tagA ) >0 && snapshots.count( tagB ) ){
        lightsTransition( snapshots[tagA], snapshots[tagB], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::degreesTransition( std::string tagA, std::string tagB, float pct ){
    if( snapshots.count( tagA ) >0 && snapshots.count( tagB ) ){
        degreesTransition( snapshots[tagA], snapshots[tagB], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::transition( std::string tag, float pct ){
    if( snapshots.count( tag ) >0 ){
        transition( origin, snapshots[tag], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::lightsTransition( std::string tag, float pct ){
    if( snapshots.count( tag ) >0 ){
        lightsTransition( origin, snapshots[tag], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::degreesTransition( std::string tag, float pct ){
    if( snapshots.count( tag ) >0 ){
        degreesTransition( origin, snapshots[tag], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::fade( std::string tag, float pct ){
    if( snapshots.count( tag ) >0 ){
        fade( snapshots[tag], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given state name not initialized, use init( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::fade( float pct ){
    fade( origin, pct );
}

void ofx::fixture::States::black(){
    for( size_t i=0; i<heads.size(); ++i ){
        heads[i]->dimmer = 0.0f;
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        dimmers[i]->dimmer = 0.0f;
    }
}
    
void ofx::fixture::States::transition( float pct ){
    transition( origin, destination, pct );
}
    
void ofx::fixture::States::lightsTransition( float pct ){
    lightsTransition( origin, destination, pct );
}
    
void ofx::fixture::States::degreesTransition( float pct ){
    degreesTransition( origin, destination, pct );
}

void ofx::fixture::States::store(){
    store ( origin );
}

void ofx::fixture::States::storeOrigin(){
    store ( origin );
}

void ofx::fixture::States::storeDestination(){
    store ( destination );
}

void ofx::fixture::States::recall(){
    recall( origin );
}

void ofx::fixture::States::lightsRecall(){
    lightsRecall( origin );
}

void ofx::fixture::States::degreesRecall(){
    degreesRecall( origin );
}

void ofx::fixture::States::store( SystemSnapshot & snap ){
    for( size_t i=0; i<heads.size(); ++i ){
        snap.heads[i].store( *heads[i] );
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        snap.dimmers[i].store( *dimmers[i] );
    }
}

void ofx::fixture::States::recall( SystemSnapshot & snap ){
    for( size_t i=0; i<heads.size(); ++i ){
        snap.heads[i].recall( *heads[i] );
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        snap.dimmers[i].recall( *dimmers[i] );
    }
}

void ofx::fixture::States::transition( SystemSnapshot & snapA, SystemSnapshot & snapB, float pct ){
    for( size_t i=0; i<heads.size(); ++i ){
        snapA.heads[i].mix( *heads[i], snapB.heads[i], pct );
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        snapA.dimmers[i].mix( *dimmers[i], snapB.dimmers[i], pct );
    }
}

void ofx::fixture::States::lightsRecall( SystemSnapshot & snap ){
    for( size_t i=0; i<heads.size(); ++i ){
        snap.heads[i].recallLight( *heads[i] );
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        snap.dimmers[i].recall( *dimmers[i] );
    }
}

void ofx::fixture::States::lightsTransition( SystemSnapshot & snapA, SystemSnapshot & snapB, float pct ){
    for( size_t i=0; i<heads.size(); ++i ){
        snapA.heads[i].mixLight( *heads[i], snapB.heads[i], pct );
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        snapA.dimmers[i].mix( *dimmers[i], snapB.dimmers[i], pct );
    }
}

void ofx::fixture::States::degreesRecall( SystemSnapshot & snap ){
    for( size_t i=0; i<heads.size(); ++i ){
        snap.heads[i].recallDegrees( *heads[i] );
    }
}

void ofx::fixture::States::degreesTransition( SystemSnapshot & snapA, SystemSnapshot & snapB, float pct ){
    for( size_t i=0; i<heads.size(); ++i ){
        snapA.heads[i].mixDegrees( *heads[i], snapB.heads[i], pct );
    }
}

void ofx::fixture::States::fade( SystemSnapshot & snap, float pct ){
    for( size_t i=0; i<heads.size(); ++i ){
        snap.heads[i].fade( *heads[i], pct );
    }
    for( size_t i=0; i<dimmers.size(); ++i ){
        snap.dimmers[i].fade( *dimmers[i], pct );
    }
}
    
ofx::fixture::States::SystemSnapshot::SystemSnapshot(){
    system.setName("system snapshot");
}
        
void ofx::fixture::States::add( Dimmer & dimmer ){
    if( bTouching ){
        ofLogWarning()<< "[ofx::fixture::States] calling add() after init() can lead to segmentation faults";
    }
    dimmers.push_back( &dimmer );
    origin.dimmers.emplace_back();
    origin.dimmers.back().init( dimmer );
    origin.system.add( origin.dimmers.back().parameters );
    destination.dimmers.emplace_back();
    destination.dimmers.back().init( dimmer );
    destination.system.add( destination.dimmers.back().parameters );
    
    multiple.bDimmers.emplace_back();
    multiple.bDimmers.back().set( dimmer.parameters.getName(), false );
    multiple.dimmersSelector.add( multiple.bDimmers.back() );
}

void ofx::fixture::States::add( Head & head ){
    if( bTouching ){
        ofLogWarning()<< "[ofx::fixture::States] calling add() after init() can lead to segmentation faults";
    }
    heads.push_back( &head );
    origin.heads.emplace_back();
    origin.heads.back().init( head );
    origin.system.add( origin.heads.back().parameters );
    destination.heads.emplace_back();
    destination.heads.back().init( head );
    destination.system.add( destination.heads.back().parameters );
    
    multiple.bHeads.emplace_back();
    multiple.bHeads.back().set( head.parameters.getName(), false );
    multiple.headsSelector.add( multiple.bHeads.back() );
    
    if(!bTargetAdded){
        multiple.parameters.add( multiple.target.set("target", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), getBoundaries()));
        multiple.target.addListener( this, &States::onTargetChange );
        bTargetAdded = true;
    }
}
    
void ofx::fixture::States::DimmerSnapshot::init( Dimmer & dimmer ){
    parameters.setName( dimmer.parameters.getName() );
    
    this->dimmer.set( dimmer.dimmer.getName(), dimmer.dimmer.get(), dimmer.dimmer.getMin(), dimmer.dimmer.getMax() );
    parameters.add( this->dimmer );
    
    fOptions.resize( dimmer.fOptionals.size() );
    for( size_t i = 0; i<fOptions.size(); ++i ){
        fOptions[i].set( dimmer.fOptionals[i]->getName(), dimmer.fOptionals[i]->get(), dimmer.fOptionals[i]->getMin(), dimmer.fOptionals[i]->getMax());
    }
    
    iOptions.resize( dimmer.iOptionals.size() );
    for( size_t i = 0; i<iOptions.size(); ++i ){
        iOptions[i].set( dimmer.iOptionals[i]->getName(), dimmer.iOptionals[i]->get(), dimmer.iOptionals[i]->getMin(), dimmer.iOptionals[i]->getMax());
    }
    
    bOptions.resize( dimmer.bOptionals.size() );
    for( size_t i = 0; i<bOptions.size(); ++i ){
        bOptions[i].set( dimmer.bOptionals[i]->getName(), dimmer.bOptionals[i]->get(), dimmer.bOptionals[i]->getMin(), dimmer.bOptionals[i]->getMax());
    }
}

void ofx::fixture::States::DimmerSnapshot::store( Dimmer & dimmer ){
    this->dimmer = dimmer.dimmer.get();
    for( size_t i = 0; i<fOptions.size(); ++i ){ fOptions[i] = dimmer.fOptionals[i]->get(); }
    for( size_t i = 0; i<iOptions.size(); ++i ){ iOptions[i] = dimmer.iOptionals[i]->get(); }
    for( size_t i = 0; i<bOptions.size(); ++i ){ bOptions[i] = dimmer.bOptionals[i]->get(); }
}

void ofx::fixture::States::DimmerSnapshot::recall( Dimmer & dimmer ){
    dimmer.dimmer = this->dimmer.get();
    for( size_t i = 0; i<fOptions.size(); ++i ){ dimmer.fOptionals[i]->set( fOptions[i].get()); }
    for( size_t i = 0; i<iOptions.size(); ++i ){ dimmer.iOptionals[i]->set( iOptions[i].get()); }
    for( size_t i = 0; i<bOptions.size(); ++i ){ dimmer.bOptionals[i]->set( bOptions[i].get()); }
}

void ofx::fixture::States::DimmerSnapshot::mix( Dimmer & dimmer, DimmerSnapshot & other, float pct ){
    dimmer.dimmer = lerp( this->dimmer, other.dimmer, pct );
    for( size_t i = 0; i<fOptions.size(); ++i ){ 
        dimmer.fOptionals[i]->set( lerp(fOptions[i].get(), other.fOptions[i].get(), pct) ); 
    }
    for( size_t i = 0; i<iOptions.size(); ++i ){ 
        dimmer.iOptionals[i]->set( lerp( float(iOptions[i].get()), float(other.iOptions[i].get()), pct ) ); 
    }
    for( size_t i = 0; i<bOptions.size(); ++i ){ dimmer.bOptionals[i]->set( bOptions[i].get()); }
}

void ofx::fixture::States::DimmerSnapshot::fade( Dimmer & dimmer, float pct ){
    dimmer.dimmer = this->dimmer*pct;
}

void ofx::fixture::States::HeadSnapshot::init( Head & head ){

    parameters.setName( head.parameters.getName() );

    dimmer.set( head.dimmer.getName(), head.dimmer.get(), head.dimmer.getMin(), head.dimmer.getMax() );    
    parameters.add( dimmer );    
    
    zoom.set( head.zoom.getName(), head.zoom.get(), head.zoom.getMin(), head.zoom.getMax() );    
    parameters.add( zoom );

    red.set( head.red.getName(), head.red.get(), head.red.getMin(), head.red.getMax() );    
    parameters.add( red );

    green.set( head.green.getName(), head.green.get(), head.green.getMin(), head.green.getMax() );    
    parameters.add( green );

    blue.set( head.blue.getName(), head.blue.get(), head.blue.getMin(), head.blue.getMax() );    
    parameters.add( blue );

    white.set( head.white.getName(), head.white.get(), head.white.getMin(), head.white.getMax() );    
    parameters.add( white );

    pan.set( head.pan.getName(), head.pan.get(), head.pan.getMin(), head.pan.getMax() );    
    parameters.add( pan );

    tilt.set( head.tilt.getName(), head.tilt.get(), head.tilt.getMin(), head.tilt.getMax() );    
    parameters.add( tilt );
    
    chaseTarget.set( head.chaseTarget.getName(), head.chaseTarget.get() );    
    parameters.add( chaseTarget );
    
    target.set( head.target.getName(), head.target.get(), head.target.getMin(), head.target.getMax() );    
    parameters.add( target );
    
    fOptions.resize( head.fOptionals.size() );
    for( size_t i = 0; i<fOptions.size(); ++i ){
        fOptions[i].set( head.fOptionals[i]->getName(), head.fOptionals[i]->get(), head.fOptionals[i]->getMin(), head.fOptionals[i]->getMax());
    }
    
    iOptions.resize( head.iOptionals.size() );
    for( size_t i = 0; i<iOptions.size(); ++i ){
        iOptions[i].set( head.iOptionals[i]->getName(), head.iOptionals[i]->get(), head.iOptionals[i]->getMin(), head.iOptionals[i]->getMax());
    }
    
    bOptions.resize( head.bOptionals.size() );
    for( size_t i = 0; i<bOptions.size(); ++i ){
        bOptions[i].set( head.bOptionals[i]->getName(), head.bOptionals[i]->get(), head.bOptionals[i]->getMin(), head.bOptionals[i]->getMax());
    }
}
    
void ofx::fixture::States::HeadSnapshot::store( Head & head ){
    dimmer = head.dimmer.get();
    zoom = head.zoom.get();
    red = head.red.get();
    green = head.green.get();
    blue = head.blue.get();
    white = head.white.get();
    pan = head.pan.get();
    tilt = head.tilt.get();
    chaseTarget = head.chaseTarget.get();
    target = head.target.get();
    for( size_t i = 0; i<fOptions.size(); ++i ){ fOptions[i] = head.fOptionals[i]->get(); }
    for( size_t i = 0; i<iOptions.size(); ++i ){ iOptions[i] = head.iOptionals[i]->get(); }
    for( size_t i = 0; i<bOptions.size(); ++i ){ bOptions[i] = head.bOptionals[i]->get(); }
}

void ofx::fixture::States::HeadSnapshot::recall( Head & head ){
    recallLight( head );
    recallDegrees( head );
}

void ofx::fixture::States::HeadSnapshot::recallLight( Head & head ){
    head.dimmer = dimmer.get();
    head.zoom = zoom.get();
    head.red = red.get();
    head.green = green.get();
    head.blue = blue.get();
    head.white = white.get();
    for( size_t i = 0; i<fOptions.size(); ++i ){ head.fOptionals[i]->set( fOptions[i].get()); }
    for( size_t i = 0; i<iOptions.size(); ++i ){ head.iOptionals[i]->set( iOptions[i].get()); }
    for( size_t i = 0; i<bOptions.size(); ++i ){ head.bOptionals[i]->set( bOptions[i].get()); }
}

void ofx::fixture::States::HeadSnapshot::recallDegrees( Head & head ){
    head.pan = pan.get();
    head.tilt = tilt.get();
    head.chaseTarget = chaseTarget.get();
    head.target = target.get();
}

void ofx::fixture::States::HeadSnapshot::mix( Head & head, HeadSnapshot & other, float pct ){
    mixLight( head, other, pct );
    mixDegrees( head, other, pct );
}

void ofx::fixture::States::HeadSnapshot::mixLight( Head & head, HeadSnapshot & other, float pct ){
    head.dimmer = lerp( dimmer, other.dimmer, pct );
    head.zoom = lerp( zoom, other.zoom, pct );
    head.red = lerp( red, other.red, pct );
    head.green = lerp( green, other.green, pct );
    head.blue = lerp( blue, other.blue, pct );
    head.white = lerp( white, other.white, pct );
    for( size_t i = 0; i<fOptions.size(); ++i ){ 
        head.fOptionals[i]->set( lerp(fOptions[i].get(), other.fOptions[i].get(), pct) ); 
    }
    for( size_t i = 0; i<iOptions.size(); ++i ){ 
        head.iOptionals[i]->set( lerp( float(iOptions[i].get()), float(other.iOptions[i].get()), pct ) ); 
    }
    for( size_t i = 0; i<bOptions.size(); ++i ){ head.bOptionals[i]->set( bOptions[i].get()); }
}

void ofx::fixture::States::HeadSnapshot::mixDegrees( Head & head, HeadSnapshot & other, float pct ){
    head.pan = lerp( pan, other.pan, pct );
    head.tilt = lerp( tilt, other.tilt, pct );
    head.chaseTarget = other.chaseTarget.get();
    head.target = glm::mix( target.get(), other.target.get(), pct );
}

void ofx::fixture::States::HeadSnapshot::fade( Head & head, float pct ){
    head.dimmer = this->dimmer * pct;
}


void ofx::fixture::States::onSelectAllHeads( bool & value ){
    for( auto & flag : multiple.bHeads ){ flag = value; }
}

void ofx::fixture::States::onSelectAllDimmers( bool & value ){
    for( auto & flag : multiple.bDimmers ){ flag = value; }
}


void ofx::fixture::States::onDimmerChange( float & value ){
    for( size_t i=0; i<dimmers.size(); ++i ){
        if( multiple.bDimmers[i] ){ dimmers[i]->dimmer = value; }
    }
    for( size_t i=0; i<heads.size(); ++i ){
        if( multiple.bHeads[i] ){ heads[i]->dimmer = value; }
    }
}

void ofx::fixture::States::onZoomChange( float & value ){
    for( size_t i=0; i<heads.size(); ++i ){
        if( multiple.bHeads[i] ){ heads[i]->zoom = value; }
    }
}

void ofx::fixture::States::onColorChange( float & value ){
    for( size_t i=0; i<heads.size(); ++i ){
        if( multiple.bHeads[i] ){ 
            heads[i]->red = multiple.red; 
            heads[i]->green = multiple.green; 
            heads[i]->blue = multiple.blue; 
            heads[i]->white = multiple.white; 
        }
    }
}

void ofx::fixture::States::onTargetChange( glm::vec3 & value ){
    for( size_t i=0; i<heads.size(); ++i ){
        if( multiple.bHeads[i] ){
            heads[i]->target = value; 
            heads[i]->chaseTarget = true;
        }
    }
}
