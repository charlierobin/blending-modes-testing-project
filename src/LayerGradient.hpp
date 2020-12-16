#ifndef LayerGradient_hpp
#define LayerGradient_hpp

#include "Layer.hpp"

class LayerGradient: public Layer {
    
public:
    
    LayerGradient();
    LayerGradient ( cinder::JsonTree );
    
    cinder::SurfaceRef render() override;
    
//    bool customGUI( int, bool ) override;
    
//    void customJSON( cinder::JsonTree * ) override;
    
private:
    
//    cinder::Color colour_;
    
};

#endif /* LayerGradient_hpp */
