#include "Layer.hpp"

#include "LayerFile.hpp"
#include "LayerSolid.hpp"

#include "cinder/ip/Blend.h"
#include "cinder/ip/Screen.h"
#include "cinder/ip/Add.h"
#include "cinder/ip/Subtract.h"
#include "cinder/ip/Multiply.h"

using namespace ci;
using namespace std;

//class LayerFile;
//class LayerSolid;

Layer * Layer::newFrom( cinder::JsonTree json )
{
    string type = json.getValueForKey<string>( ".type" );
    
    Layer * layer = nullptr;
    
    if ( type == "LayerFile" )
    {
        layer = new LayerFile( json );
    }
    else if ( type == "LayerSolid" )
    {
        layer = new LayerSolid( json );
    }
    else
    {
        // whoops, exception
    }
    
    return layer;
}

Layer::Layer( std::string name, std::string type )
{
    layerType_ = type;
    
    layerName_ = name;
    
    visible_ = true;
    
    blendMode_ = "Normal";
}

Layer::Layer( JsonTree json )
{
    layerType_ = json.getValueForKey<string>( ".type" );
    
    layerName_ = json.getValueForKey<string>( ".name" );
    
    visible_ = json.getValueForKey<bool>( ".visible" );

    blendMode_ = json.getValueForKey<string>( ".blendMode" );
}

SurfaceRef Layer::render()
{
    return nullptr;
}

void Layer::compositeOnTo( Surface * surface )
{
    if ( ! visible_ ) return;
    
    SurfaceRef layer = render();
    
    if ( layer == nullptr ) return;
    
    if ( blendMode_ == "Screen" )
    {
        ip::screen( surface, *layer );
    }
    else if ( blendMode_ == "Add" )
    {
        ip::add( surface, *layer );
    }
    else if ( blendMode_ == "Subtract" )
    {
        ip::subtract( surface, *layer );
    }
    else if ( blendMode_ == "Multiply" )
    {
        ip::multiply( surface, *layer );
    }
    else
    {
        ip::blend( surface, *layer );
    }
}

bool Layer::customGUI( int i, bool dirty )
{
    return dirty;
}

bool Layer::gui( int i )
{
    ImGui::PushID( i );
    
    bool dirty = false;
    
    if ( ImGui::TreeNode( ( layerName_.c_str() ) ) )
    {
        if ( ImGui::Checkbox( "Visible", &visible_ ) )
        {
            dirty = true;
        }
        
        const char* names[] = { "Normal", "Screen", "Add", "Subtract", "Multiply" };
        
        if ( ImGui::Button( blendMode_.c_str() ) ) ImGui::OpenPopup( "blend_modes_popup" );
        
        if ( ImGui::BeginPopup( "blend_modes_popup" ) )
        {
            ImGui::Text( "Blend modes" );
            
            ImGui::Separator();
            
            for ( auto name : names )
            {
                if ( ImGui::Selectable( name ) )
                {
                    blendMode_ = name;
                    
                    dirty = true;
                }
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::Separator();
        
        dirty = customGUI( i, dirty );
        
        ImGui::TreePop();
    }
    
    ImGui::PopID();
    
    return dirty;
}

void Layer::customJSON( JsonTree * json )
{
}

JsonTree Layer::asJSON()
{
    JsonTree json = JsonTree::makeObject();
    
    json.addChild( JsonTree( "type", layerType_ ) );
    
    json.addChild( JsonTree( "name", layerName_ ) );
    json.addChild( JsonTree( "visible", visible_ ) );
    json.addChild( JsonTree( "blendMode", blendMode_ ) );
    
    JsonTree custom = JsonTree::makeObject( "custom" );
    
    customJSON( &custom );
    
    json.addChild( custom );
    
    return json;
}





