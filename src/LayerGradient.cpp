#include "LayerGradient.hpp"

#include "cinder/ip/Fill.h"

using namespace ci;

LayerGradient::LayerGradient() : Layer( "Gradient", "LayerGradient" )
{
    startColour_ = Color( "darkmagenta" );
    endColour_ = Color( "lavender" );
    
    // https://www.december.com/html/spec/colorsvg.html
}

LayerGradient::LayerGradient ( JsonTree json ) : Layer( json )
{
    JsonTree customJSON = json.getChild( "custom" );
    
    startColour_ = Color(
                    customJSON.getValueForKey<float>( ".startColour_r" ),
                    customJSON.getValueForKey<float>( ".startColour_g" ),
                    customJSON.getValueForKey<float>( ".startColour_b" )
                    );
    
    endColour_ = Color(
                         customJSON.getValueForKey<float>( ".endColour_r" ),
                         customJSON.getValueForKey<float>( ".endColour_g" ),
                         customJSON.getValueForKey<float>( ".endColour_b" )
                         );
}

SurfaceRef LayerGradient::render()
{
    SurfaceRef surface = Surface::create( 640, 480, false );
    
    ip::fill( surface.get(), Color( "red" ) );
    
    float distance = 480;
    
    vec3 step = vec3( ( endColour_.r - startColour_.r ) / distance, ( endColour_.g - startColour_.g ) / distance, ( endColour_.b - startColour_.b ) / distance );
    
    Surface::Iter iter = surface->getIter();
    
    Color paintColour = startColour_;
    
    while ( iter.line() )
    {
        while ( iter.pixel() )
        {
            iter.r() = 255 * paintColour.r;
            iter.g() = 255 * paintColour.g;
            iter.b() = 255 * paintColour.b;
        }
        
        paintColour = paintColour + step;
    }
    
    return surface;
}

bool LayerGradient::customGUI( bool dirty )
{
    if ( ImGui::ColorPicker3( "Start colour", &startColour_ ) )
    {
        dirty = true;
    }
    
    if ( ImGui::ColorPicker3( "End colour", &endColour_ ) )
    {
        dirty = true;
    }
    
    return dirty;
}

void LayerGradient::customJSON( JsonTree * json )
{
    json->addChild( JsonTree( "startColour_r", startColour_.r ) );
    json->addChild( JsonTree( "startColour_g", startColour_.g ) );
    json->addChild( JsonTree( "startColour_b", startColour_.b ) );
    
    json->addChild( JsonTree( "endColour_r", endColour_.r ) );
    json->addChild( JsonTree( "endColour_g", endColour_.g ) );
    json->addChild( JsonTree( "endColour_b", endColour_.b ) );
}




