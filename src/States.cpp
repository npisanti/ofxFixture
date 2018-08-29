
#include "States.h"

ofx::fixture::States::States(){
    bTouching = false;
    
    // search for snapshot directory here 
}


void ofx::fixture::States::touch( std::string tag ){
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

    // if tag exists, deserialize to snap here 
}


void ofx::fixture::States::store( std::string tag ){
    if( snapshots.count( tag ) >0 ){
        SystemSnapshot & snap = snapshots[tag]; 
        store( snap );
        
        // serialize snap here
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given tag not initialized, use touch( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::recall( std::string tag ){
    if( snapshots.count( tag ) >0 ){
        recall( snapshots[tag] );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given tag not initialized, use touch( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::transition( std::string tagA, std::string tagB, float pct ){
    if( snapshots.count( tagA ) >0 && snapshots.count( tagB ) ){
        transition( snapshots[tagA], snapshots[tagB], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given tag not initialized, use touch( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::transition( std::string tag, float pct ){
    if( snapshots.count( tag ) >0 ){
        transition( untagged, snapshots[tag], pct );
    }else{
        ofLogWarning()<< "[ofx::fixture::States] snapshot with given tag not initialized, use touch( std::string tag ) before storing or recalling";
    }
}

void ofx::fixture::States::store(){
    store ( untagged );
}

void ofx::fixture::States::recall(){
    recall( untagged );
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
    
ofx::fixture::States::SystemSnapshot::SystemSnapshot(){
    system.setName("system snapshot");
}
        
void ofx::fixture::States::add( Dimmer & dimmer ){
    if( bTouching ){
        ofLogWarning()<< "[ofx::fixture::States] calling add() after touch() can lead to segmentation faults";
    }
    dimmers.push_back( &dimmer );
    untagged.dimmers.emplace_back();
    untagged.dimmers.back().init( dimmer );
    untagged.system.add( untagged.dimmers.back().parameters );
}

void ofx::fixture::States::add( Head & head ){
    if( bTouching ){
        ofLogWarning()<< "[ofx::fixture::States] calling add() after touch() can lead to segmentation faults";
    }
    heads.push_back( &head );
    untagged.heads.emplace_back();
    untagged.heads.back().init( head );
    untagged.system.add( untagged.heads.back().parameters );
}
    
void ofx::fixture::States::DimmerSnapshot::init( Dimmer & dimmer ){
    parameters.setName( dimmer.parameters.getName() );
    
    this->dimmer.set( dimmer.dimmer.getName(), dimmer.dimmer.get(), dimmer.dimmer.getMin(), dimmer.dimmer.getMax() );
    parameters.add( this->dimmer );
    
    this->zoom.set( dimmer.zoom.getName(), dimmer.zoom.get(), dimmer.zoom.getMin(), dimmer.zoom.getMax() );
    parameters.add( this->zoom );
}

void ofx::fixture::States::DimmerSnapshot::store( Dimmer & dimmer ){
    this->dimmer = dimmer.dimmer.get();
    this->zoom = dimmer.zoom.get(); 
}

void ofx::fixture::States::DimmerSnapshot::recall( Dimmer & dimmer ){
    dimmer.dimmer = this->dimmer.get();
    dimmer.zoom = this->zoom.get();
}

void ofx::fixture::States::DimmerSnapshot::mix( Dimmer & dimmer, DimmerSnapshot & other, float pct ){
    dimmer.dimmer = lerp( this->dimmer, other.dimmer, pct );
    dimmer.zoom = lerp( this->zoom, other.zoom, pct );
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
    head.dimmer = dimmer.get();
    head.zoom = zoom.get();
    head.red = red.get();
    head.green = green.get();
    head.blue = blue.get();
    head.white = white.get();
    head.pan = pan.get();
    head.tilt = tilt.get();
    head.chaseTarget = chaseTarget.get();
    head.target = target.get();
    for( size_t i = 0; i<fOptions.size(); ++i ){ head.fOptionals[i]->set( fOptions[i].get()); }
    for( size_t i = 0; i<iOptions.size(); ++i ){ head.iOptionals[i]->set( iOptions[i].get()); }
    for( size_t i = 0; i<bOptions.size(); ++i ){ head.bOptionals[i]->set( bOptions[i].get()); }
}

void ofx::fixture::States::HeadSnapshot::mix( Head & head, HeadSnapshot & other, float pct ){
    head.dimmer = lerp( dimmer, other.dimmer, pct );
    head.zoom = lerp( zoom, other.zoom, pct );
    head.red = lerp( red, other.red, pct );
    head.green = lerp( green, other.green, pct );
    head.blue = lerp( blue, other.blue, pct );
    head.white = lerp( white, other.white, pct );
    head.pan = lerp( pan, other.pan, pct );
    head.tilt = lerp( tilt, other.tilt, pct );
    head.chaseTarget = chaseTarget.get();
    head.target = glm::mix( target.get(), other.target.get(), pct );
    for( size_t i = 0; i<fOptions.size(); ++i ){ 
        head.fOptionals[i]->set( lerp(fOptions[i].get(), other.fOptions[i].get(), pct) ); 
    }
    for( size_t i = 0; i<iOptions.size(); ++i ){ 
        head.iOptionals[i]->set( lerp( float(iOptions[i].get()), float(other.iOptions[i].get()), pct ) ); 
    }
    for( size_t i = 0; i<bOptions.size(); ++i ){ head.bOptionals[i]->set( bOptions[i].get()); }
}
