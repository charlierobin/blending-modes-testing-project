#ifndef Layer_hpp
#define Layer_hpp

#include "cinder/Json.h"
#include "cinder/CinderImGui.h"

#include "LayerActions.h"

class Layer {
    
public:
    
    static Layer * newFrom( cinder::JsonTree );
    
    Layer( std::string, std::string );
    Layer ( cinder::JsonTree );
    
    LayerActions gui( bool, bool );
    
    virtual bool customGUI( bool );
    
    void compositeOnTo( cinder::Surface * );
    
    virtual cinder::SurfaceRef render();

    cinder::JsonTree asJSON();

    virtual void customJSON( cinder::JsonTree * );
    
private:
    
    std::string layerType_;
    
    std::string generateUUID();
    
    bool expanded_;
    
protected:
    
    std::string id_;
    
    std::string layerName_;
    bool visible_;
    std::string blendMode_;
};

#endif /* Layer_hpp */
