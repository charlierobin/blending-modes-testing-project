#ifndef LayerFile_hpp
#define LayerFile_hpp

#include "Layer.hpp"

class LayerFile: public Layer {
    
public:
    
    LayerFile( std::string filepath = "" );
    LayerFile ( cinder::JsonTree );
    
    cinder::SurfaceRef render() override;
    
    bool customGUI( int, bool ) override;
    
    void customJSON( cinder::JsonTree * ) override;
    
private:
    
    std::string filepath_;
    
    bool error_;
    
};

#endif /* LayerFile_hpp */
