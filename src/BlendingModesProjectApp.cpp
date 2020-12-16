#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/JSON.h"

#include "cinder/CinderImGui.h"

#include "cinder/ip/Fill.h"

#include "LayerActions.h"

#include "LayerSolid.hpp"
#include "LayerFile.hpp"
#include "LayerCheckerBoard.hpp"
#include "LayerGradient.hpp"

#include <string.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class BlendingModesProjectApp : public App {
    
public:
    
    void setup() override;
    void update() override;
    void draw() override;
    
    void keyDown( KeyEvent event ) override;
    
    void cleanup() override;
    
private:
    
    Color backgroundColour_ = Color::black();
    
    bool backgroundExpanded_ = false;
    
    vector<Layer*> layers_;
    
    bool dirty_ = true;
    
    gl::TextureRef textureRender_;
};

void BlendingModesProjectApp::setup()
{
    ImGui::Initialize();
    
    string homeDir = getenv( "HOME" );
    
    ifstream jsonFile( homeDir + "/Desktop/test.json" );
    
    JsonTree json;
    
    if ( jsonFile.is_open() )
    {
        string jsonFileAsString( ( istreambuf_iterator<char>( jsonFile ) ), istreambuf_iterator<char>() );
        
        jsonFile.close();
        
        json = JsonTree( jsonFileAsString );
    }
    
    if ( json.getNodeType() == JsonTree::NODE_OBJECT )
    {
        JsonTree windowJSON = json.getChild( "window" );
        
        setWindowPos(
                     windowJSON.getValueForKey<int>( ".posX" ),
                     windowJSON.getValueForKey<int>( ".posY" )
                     );
        
        setWindowSize(
                      windowJSON.getValueForKey<int>( ".width" ),
                      windowJSON.getValueForKey<int>( ".height" )
                      );
        
        backgroundColour_ = Color(
                                  windowJSON.getValueForKey<float>( ".backgroundColour_r" ),
                                  windowJSON.getValueForKey<float>( ".backgroundColour_g" ),
                                  windowJSON.getValueForKey<float>( ".backgroundColour_b" )
                                  );
        
        backgroundExpanded_ = windowJSON.getValueForKey<bool>( ".backgroundExpanded" );
        
        JsonTree layersJSON = json.getChild( "layers" );
        
        for ( auto layerJSON : layersJSON )
        {
            layers_.push_back( Layer::newFrom( layerJSON ) );
        }
    }
}

void BlendingModesProjectApp::cleanup()
{
    JsonTree layersJSON = JsonTree::makeArray( "layers" );
    
    for ( auto layer : layers_ )
    {
        layersJSON.addChild( layer->asJSON() );
    }
    
    JsonTree json = JsonTree::makeObject();
    
    json.addChild( layersJSON );
    
    JsonTree windowJSON = JsonTree::makeObject( "window" );
    
    windowJSON.addChild( JsonTree( "posX", getWindowPosX() ) );
    windowJSON.addChild( JsonTree( "posY", getWindowPosY() ) );
    
    windowJSON.addChild( JsonTree( "width", getWindowWidth() ) );
    windowJSON.addChild( JsonTree( "height", getWindowHeight() ) );
    
    windowJSON.addChild( JsonTree( "backgroundColour_r", backgroundColour_.r ) );
    windowJSON.addChild( JsonTree( "backgroundColour_g", backgroundColour_.g ) );
    windowJSON.addChild( JsonTree( "backgroundColour_b", backgroundColour_.b ) );
    
    windowJSON.addChild( JsonTree( "backgroundExpanded", backgroundExpanded_ ) );
    
    json.addChild( windowJSON );
    
    string homeDir = getenv( "HOME" );
    
    json.write( homeDir + "/Desktop/test.json" );
}

void BlendingModesProjectApp::keyDown( KeyEvent event )
{
    if ( event.getCode() == KeyEvent::KEY_s )
    {
        layers_.push_back( new LayerSolid() );
        
        dirty_ = true;
    }
    else if ( event.getCode() == KeyEvent::KEY_f )
    {
        layers_.push_back( new LayerFile() );
        
        dirty_ = true;
    }
    else if ( event.getCode() == KeyEvent::KEY_c )
    {
        layers_.push_back( new LayerCheckerBoard() );
        
        dirty_ = true;
    }
    else if ( event.getCode() == KeyEvent::KEY_g )
    {
        layers_.push_back( new LayerGradient() );
        
        dirty_ = true;
    }
}

void BlendingModesProjectApp::update()
{
    if ( ! dirty_ ) return;
    
    dirty_ = false;
    
    Surface final( 640, 480, false );
    
    ip::fill( &final, backgroundColour_ );
    
    for ( auto layer : layers_ )
    {
        layer->compositeOnTo( &final );
    }
    
    textureRender_ = gl::Texture::create( final );
}

void BlendingModesProjectApp::draw()
{
    gl::clear( backgroundColour_ );
    
    gl::draw( textureRender_ );
    
    ImGui::Begin( "Layers" );
    
    ImGui::Text( "Add a new:" );
    
    ImGui::SameLine();
    
    ImGui::SetCursorPosY( ImGui::GetCursorPosY() - 2 );
    
    if ( ImGui::Button( "File" ) )
    {
        layers_.push_back( new LayerFile() );
        
        dirty_ = true;
    }
    
    ImGui::SameLine();
    
    if ( ImGui::Button( "Solid" ) )
    {
        layers_.push_back( new LayerSolid() );
        
        dirty_ = true;
    }
    
    ImGui::SameLine();
    
    if ( ImGui::Button( "Checkerboard" ) )
    {
        layers_.push_back( new LayerCheckerBoard() );
        
        dirty_ = true;
    }
    
    ImGui::SameLine();
    
    if ( ImGui::Button( "Gradient" ) )
    {
        layers_.push_back( new LayerGradient() );
        
        dirty_ = true;
    }
    
    ImGui::Separator();
    ImGui::Separator();
    
    Layer * toMoveUp = nullptr;
    Layer * toMoveDown = nullptr;
    Layer * toDelete = nullptr;
    
    for ( auto layer = layers_.rbegin(); layer != layers_.rend(); ++layer )
    {
        LayerActions response = ( *layer )->gui( layer != layers_.rbegin(), next( layer ) != layers_.rend() );
        
        if ( response.dirty ) dirty_ = true;
        
        if ( response.deleteLayer ) toDelete = ( *layer );
        
        if ( response.moveUp ) toMoveUp = ( *layer );
        
        if ( response.moveDown ) toMoveDown = ( *layer );
    }
    
    ImGui::Separator();
    ImGui::Separator();
    
    ImGui::SetNextItemOpen( backgroundExpanded_ );
    
    backgroundExpanded_ = ImGui::TreeNode( ( "Background" ) );
    
    if ( backgroundExpanded_ )
    {
        if ( ImGui::ColorPicker3( "Background", &backgroundColour_ ) ) dirty_ = true;
        
        ImGui::TreePop();
    }
    
    ImGui::End();
    
    if ( toDelete != nullptr )
    {
        layers_.erase( remove( layers_.begin(), layers_.end(), toDelete ), layers_.end() );
        
        delete toDelete;
        
        dirty_ = true;
    }
    
    if ( toMoveUp != nullptr )
    {
        auto iter = find( layers_.begin(), layers_.end(), toMoveUp );
        
        long oldIndex = std::distance( layers_.begin(), iter );
        
        long newIndex = oldIndex + 1;
        
        rotate( layers_.begin() + oldIndex, layers_.begin() + oldIndex + 1, layers_.begin() + newIndex + 1);
        
        dirty_ = true;
    }
    
    if ( toMoveDown != nullptr )
    {
        auto iter = find( layers_.begin(), layers_.end(), toMoveDown );
        
        long oldIndex = std::distance( layers_.begin(), iter );
        
        long newIndex = oldIndex - 1;
        
        std::rotate( layers_.rend() - oldIndex - 1, layers_.rend() - oldIndex, layers_.rend() - newIndex );
        
        dirty_ = true;
    }
}

CINDER_APP( BlendingModesProjectApp, RendererGl )




