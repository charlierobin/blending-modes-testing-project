#include "LayerSolid.hpp"

#include "cinder/ip/Fill.h"

using namespace ci;

LayerSolid::LayerSolid( Color colour ) : Layer( "Solid", "LayerSolid" )
{
    colour_ = colour;
}

LayerSolid::LayerSolid ( JsonTree json ) : Layer( json )
{
    JsonTree customJSON = json.getChild( "custom" );
    
    colour_ = Color(
                    customJSON.getValueForKey<float>( ".colour_r" ),
                    customJSON.getValueForKey<float>( ".colour_g" ),
                    customJSON.getValueForKey<float>( ".colour_b" )
                    );
}

SurfaceRef LayerSolid::render()
{
    SurfaceRef surface = Surface::create( 640, 480, false );
    
    ip::fill( surface.get(), colour_ );
    
    return surface;
}

bool LayerSolid::customGUI( bool dirty )
{
    if ( ImGui::ColorPicker3( "Colour", &colour_ ) ) dirty = true;
    
    return dirty;
}

void LayerSolid::customJSON( JsonTree * json )
{
    json->addChild( JsonTree( "colour_r", colour_.r ) );
    json->addChild( JsonTree( "colour_g", colour_.g ) );
    json->addChild( JsonTree( "colour_b", colour_.b ) );
}

