#ifndef LayerSolid_hpp
#define LayerSolid_hpp

#include "Layer.hpp"

class LayerSolid: public Layer {
    
public:
    
    LayerSolid( cinder::Color colour = cinder::Color::black() );
    LayerSolid ( cinder::JsonTree );
    
    cinder::SurfaceRef render() override;
    
    bool customGUI( int, bool ) override;
    
    void customJSON( cinder::JsonTree * ) override;
    
private:
    
    cinder::Color colour_;
    
};

#endif /* LayerSolid_hpp */
