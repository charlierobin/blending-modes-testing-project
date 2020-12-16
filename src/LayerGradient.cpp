#include "LayerGradient.hpp"

#include "cinder/ip/Fill.h"

using namespace ci;

LayerGradient::LayerGradient() : Layer( "Gradient", "LayerGradient" )
{
}

LayerGradient::LayerGradient ( JsonTree json ) : Layer( json )
{
}

SurfaceRef LayerGradient::render()
{
    SurfaceRef surface = Surface::create( 640, 480, false );
    
    ip::fill( surface.get(), Color( "red" ) );
    
    // https://www.december.com/html/spec/colorsvg.html
    
    Color startColour = Color( "darkmagenta" );
    Color endColour = Color( "lavender" );
    
    float stepR = ( endColour.r - startColour.r ) / 480.0;
    float stepG = ( endColour.g - startColour.g ) / 480.0;
    float stepB = ( endColour.b - startColour.b ) / 480.0;
    
    Surface::Iter iter = surface->getIter();
    
    Color paintColour = startColour;
    
    while ( iter.line() )
    {
        while ( iter.pixel() )
        {
            iter.r() = 255 * paintColour.r;
            iter.g() = 255 * paintColour.g;
            iter.b() = 255 * paintColour.b;
        }
        
        paintColour = Color( paintColour.r + stepR, paintColour.g + stepG, paintColour.b + stepB );
    }
    
    return surface;
}

//bool LayerGradient::customGUI( int i, bool dirty )
//{
    //    if ( ImGui::ColorPicker3( "Colour", &colour_ ) )
    //    {
    //        dirty = true;
    //    }
    
//    return dirty;
//}

//void LayerGradient::customJSON( JsonTree * json )
//{
    //    json->addChild( JsonTree( "colour_r", colour_.r ) );
    //    json->addChild( JsonTree( "colour_g", colour_.g ) );
    //    json->addChild( JsonTree( "colour_b", colour_.b ) );
//}




