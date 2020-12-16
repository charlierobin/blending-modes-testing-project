#include "LayerFile.hpp"

using namespace ci;
using namespace std;

LayerFile::LayerFile( string filepath ) :  Layer( "File", "LayerFile" )
{
    filepath_ = filepath;
}

LayerFile::LayerFile ( cinder::JsonTree json ) : Layer( json )
{
    //    JsonTree customJSON = json.getChild( "custom" );
    //    filepath_ = customJSON.getValueForKey<string>( ".filepath" );
    
    //    same as...
    
    filepath_ = json.getValueForKey<string>( ".custom.filepath" );
    
    fs::path testPath( filepath_ );
    
    error_ = false;
    
    if( ! fs::exists( testPath ) ) error_ = true;
}

SurfaceRef LayerFile::render()
{
    if ( filepath_ == "" ) return nullptr;
    
    fs::path testPath( filepath_ );
    
    if ( ! fs::exists( testPath ) )
    {
        error_ = true;
        
        return nullptr;
    }
    
    error_ = false;
    
    SurfaceRef surface = Surface::create( loadImage( testPath.native() ) );
    
    return surface;
}

bool LayerFile::customGUI( bool dirty )
{
    vec4 filePathTextColour = vec4( 0, 1, 0, 1 );
    
    if ( error_ ) filePathTextColour = vec4( 1, 0, 0, 1 );

    ImGui::TextColored( filePathTextColour, "%s", filepath_.c_str() );
    
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
