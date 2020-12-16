#ifndef LayerCheckerBoard_hpp
#define LayerCheckerBoard_hpp

#include "Layer.hpp"

class LayerCheckerBoard: public Layer {
    
public:
    
    LayerCheckerBoard();
    LayerCheckerBoard ( cinder::JsonTree );
    
    cinder::SurfaceRef render() override;
    
    bool customGUI( int, bool ) override;
    
    void customJSON( cinder::JsonTree * ) override;
    
private:
    
    cinder::Color colour_;
    
};

#endif /* LayerCheckerBoard_hpp */
