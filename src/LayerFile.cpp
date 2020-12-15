#include "LayerFile.hpp"

using namespace ci;
using namespace std;

LayerFile::LayerFile( string filepath ) :  Layer( "File", "LayerFile" )
{
    filepath_ = filepath;
}

LayerFile::LayerFile ( cinder::JsonTree json ) : Layer( json )
{
//    JsonTree custom = json.getChild( "custom" );
    
    filepath_ = json.getValueForKey<string>( ".custom.filepath" );
};

SurfaceRef LayerFile::render()
{
    if ( filepath_ == "" ) return nullptr;
    
    SurfaceRef surface = Surface::create( loadImage( filepath_ ) );
    
    return surface;
}

bool LayerFile::customGUI( int i, bool dirty )
{
    if ( ImGui::Button( "Select file..." ) )
    {
        fs::path result = app::getOpenFilePath();
        
        if ( result != "" )
        {
            filepath_ = result;
            
            layerName_ = result.filename();
            
            dirty = true;
        }
    }
    
    return dirty;
}

void LayerFile::customJSON( JsonTree * json )
{
    json->addChild( JsonTree( "filepath", filepath_ ) );
}
