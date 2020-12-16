#ifndef LayerGradient_hpp
#define LayerGradient_hpp

#include "Layer.hpp"

class LayerGradient: public Layer {
    
public:
    
    LayerGradient();
    LayerGradient ( cinder::JsonTree );
    
    cinder::SurfaceRef render() override;
    
    bool customGUI( bool ) override;
    
    void customJSON( cinder::JsonTree * ) override;
    
private:
    
    cinder::Color startColour_;
    cinder::Color endColour_;
    
};

#endif /* LayerGradient_hpp */
