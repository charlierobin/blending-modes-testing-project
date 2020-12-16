#include "LayerCheckerBoard.hpp"

#include "cinder/ip/Checkerboard.h"

using namespace ci;

LayerCheckerBoard::LayerCheckerBoard() : Layer( "Checkerboard", "LayerCheckerBoard" )
{
}

LayerCheckerBoard::LayerCheckerBoard ( JsonTree json ) : Layer( json )
{
}

SurfaceRef LayerCheckerBoard::render()
{
    SurfaceRef surface = Surface::create( ip::checkerboard( 640, 480 ) );
    
    return surface;
}

bool LayerCheckerBoard::customGUI( bool dirty )
{
//    if ( ImGui::ColorPicker3( "Colour", &colour_ ) )
//    {
//        dirty = true;
//    }
    
    return dirty;
}

void LayerCheckerBoard::customJSON( JsonTree * json )
{
//    json->addChild( JsonTree( "colour_r", colour_.r ) );
//    json->addChild( JsonTree( "colour_g", colour_.g ) );
//    json->addChild( JsonTree( "colour_b", colour_.b ) );
}



