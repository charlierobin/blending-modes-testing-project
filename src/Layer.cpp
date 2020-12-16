#include "Layer.hpp"

#include "LayerFile.hpp"
#include "LayerSolid.hpp"
#include "LayerCheckerBoard.hpp"
#include "LayerGradient.hpp"

#include "cinder/ip/Blend.h"

#include "cinder/ip/Screen.h"
#include "cinder/ip/Add.h"
#include "cinder/ip/Subtract.h"
#include "cinder/ip/Multiply.h"
#include "cinder/ip/Divide.h"
#include "cinder/ip/Difference.h"
#include "cinder/ip/Lighten.h"
#include "cinder/ip/Darken.h"
#include "cinder/ip/Hue.h"
#include "cinder/ip/Color.h"
#include "cinder/ip/Saturation.h"
#include "cinder/ip/Luminosity.h"

using namespace ci;
using namespace std;

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
    else if ( type == "LayerCheckerBoard" )
    {
        layer = new LayerCheckerBoard( json );
    }
    else if ( type == "LayerGradient" )
    {
        layer = new LayerGradient( json );
    }
    else
    {
        // whoops, exception?
    }
    
    return layer;
}

Layer::Layer( std::string name, std::string type )
{
    id_ = generateUUID();
    
    layerType_ = type;
    
    layerName_ = name;
    
    visible_ = true;
    
    blendMode_ = "Normal";
    
    expanded_ = false;
}

Layer::Layer( JsonTree json )
{
    id_ = json.getValueForKey<string>( ".id" );
    
    layerType_ = json.getValueForKey<string>( ".type" );
    
    layerName_ = json.getValueForKey<string>( ".name" );
    
    visible_ = json.getValueForKey<bool>( ".visible" );
    
    blendMode_ = json.getValueForKey<string>( ".blendMode" );
    
    expanded_ = json.getValueForKey<bool>( ".expanded" );
}

string Layer::generateUUID() {
    
    char buffer[128];
    
    std::string result = "";
    
    FILE* pipe = popen( "uuidgen", "r" );
    
    if ( ! pipe ) throw runtime_error( "popen() failed" );
    
    try
    {
        while ( ! feof( pipe ) )
        {
            if ( fgets( buffer, 128, pipe ) != NULL ) result += buffer;
        }
    }
    catch (...)
    {
        pclose( pipe );
        throw;
    }
    
    pclose( pipe );
    
    result.erase( result.find_last_not_of( " \n\r\t" ) + 1 );
    
    return result;
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
    else if ( blendMode_ == "Divide" )
    {
        ip::divide( surface, *layer );
    }
    else if ( blendMode_ == "Difference" )
    {
        ip::difference( surface, *layer );
    }
    else if ( blendMode_ == "Lighten" )
    {
        ip::lighten( surface, *layer );
    }
    else if ( blendMode_ == "Darken" )
    {
        ip::darken( surface, *layer );
    }
    else if ( blendMode_ == "Hue" )
    {
        ip::hue( surface, *layer );
    }
    else if ( blendMode_ == "Colour" )
    {
        ip::color( surface, *layer );
    }
    else if ( blendMode_ == "Saturation" )
    {
        ip::saturation( surface, *layer );
    }
    else if ( blendMode_ == "Luminosity" )
    {
        ip::luminosity( surface, *layer );
    }
    else
    {
        ip::blend( surface, *layer );
    }
}

bool Layer::customGUI( bool dirty )
{
    return dirty;
}

LayerActions Layer::gui( bool isNotFirst, bool isNotLast )
{
    ImGui::PushID( &id_ );
    
    bool dirty = false;
    bool moveUp = false;
    bool moveDown = false;
    bool deleteMe = false;
    
    ImGui::SetNextItemOpen( expanded_ );
    
    expanded_ = ImGui::TreeNode( ( layerName_.c_str() ) );
    
    if ( expanded_ )
    {
        if ( ImGui::Button( "Delete?" ) )
        {
            ImGui::OpenPopup( "Delete layer?" );
        }
        
        if ( ImGui::BeginPopupModal( "Delete layer?" ) )
        {
            ImGui::Text( "Are you sure? You cannot undo this action." );
            
            if ( ImGui::Button( "Yes" ) )
            {
                ImGui::CloseCurrentPopup();
                
                deleteMe = true;
            }
            
            ImGui::SameLine();
            
            if ( ImGui::Button( "No" ) )
            {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        if ( isNotFirst )
        {
            ImGui::SameLine();
            
            if ( ImGui::Button( "Move up" ) ) moveUp = true;
        }
        
        if ( isNotLast )
        {
            ImGui::SameLine();
            
            if ( ImGui::Button( "Move down" ) ) moveDown = true;
        }
        
        ImGui::SameLine();
        
        if ( ImGui::Checkbox( "Visible", &visible_ ) ) dirty = true;
        
        ImGui::Separator();
        
        const char* names[] = { "Normal", "Screen", "Add", "Subtract", "Multiply", "Divide", "Difference", "Lighten", "Darken", "Hue", "Colour", "Saturation", "Luminosity" };
        
        ImGui::Text( "Blend mode:" );
        
        if ( ImGui::Button( blendMode_.c_str() ) ) ImGui::OpenPopup( "blend_modes_popup" );
        
        if ( ImGui::BeginPopup( "blend_modes_popup" ) )
        {
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
        
        dirty = customGUI( dirty );
        
        ImGui::TreePop();
    }
    
    ImGui::PopID();
    
    return { dirty, moveUp, moveDown, deleteMe };
}

void Layer::customJSON( JsonTree * json )
{
}

JsonTree Layer::asJSON()
{
    JsonTree json = JsonTree::makeObject();
    
    json.addChild( JsonTree( "id", id_ ) );
    json.addChild( JsonTree( "type", layerType_ ) );
    
    json.addChild( JsonTree( "name", layerName_ ) );
    json.addChild( JsonTree( "visible", visible_ ) );
    json.addChild( JsonTree( "blendMode", blendMode_ ) );
    
    json.addChild( JsonTree( "expanded", expanded_ ) );
    
    JsonTree custom = JsonTree::makeObject( "custom" );
    
    customJSON( &custom );
    
    json.addChild( custom );
    
    return json;
}





