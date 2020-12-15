#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/JSON.h"

#include "cinder/CinderImGui.h"

#include "cinder/ip/Fill.h"

#include "LayerSolid.hpp"
#include "LayerFile.hpp"

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
    
    vector<Layer*> layers_;
    
    bool dirty_ = true;
    
    gl::TextureRef textureRender_;
};

void BlendingModesProjectApp::setup()
{
    ImGui::Initialize();
    
    JsonTree json;
    
    ifstream jsonFile( "/Users/charlie/Desktop/test.json" );
    
    if ( jsonFile.is_open() )
    {
        std::string jsonFileAsString( ( std::istreambuf_iterator<char>( jsonFile ) ), std::istreambuf_iterator<char>() );
        
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
    
    json.addChild( windowJSON );
    
    json.write( "/Users/charlie/Desktop/test.json" );
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
    
    if ( ImGui::BeginMenu( "New" ) )
    {
        if ( ImGui::MenuItem( "File", "Ctrl+F" ) )
        {
            layers_.push_back( new LayerFile() );
            
            dirty_ = true;
        }
        
        if ( ImGui::MenuItem( "Solid", "Ctrl+S" ) )
        {
            layers_.push_back( new LayerSolid() );
            
            dirty_ = true;
        }
        
        ImGui::EndMenu();
    }
    
    int i = 0;
    
    for ( auto layer = layers_.rbegin(); layer != layers_.rend(); ++layer )
    {
        if( (*layer)->gui( i ) ) dirty_ = true;
        
        i++;
    }
    
    ImGui::Separator();
    ImGui::Separator();
    
    if ( ImGui::TreeNode( ( "Background" ) ) )
    {
        if( ImGui::ColorPicker3( "Background", &backgroundColour_ ) )
        {
            dirty_ = true;
        }
        
        ImGui::TreePop();
    }
    
    ImGui::End();
}

CINDER_APP( BlendingModesProjectApp, RendererGl )




