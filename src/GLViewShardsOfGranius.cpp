#include "GLViewShardsOfGranius.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "WOImGui.h" //GUI Demos also need to #include "AftrImGuiIncludes.h"
#include "AftrImGuiIncludes.h"
#include "AftrGLRendererBase.h"

#include "irrKlang.h"
#include "cstdlib"
#include "string"
#include "algorithm"
using namespace Aftr;

float GLViewShardsOfGranius::masterVol = 1.0f;
int GLViewShardsOfGranius::woIndex = 0;
std::string GLViewShardsOfGranius::miniName = "";
std::string GLViewShardsOfGranius::terrainName = "";
bool GLViewShardsOfGranius::characterMovement = false;
std::string GLViewShardsOfGranius::songAlias = "";
bool GLViewShardsOfGranius::musicPause = false;
bool GLViewShardsOfGranius::loadMini = false;
//WO* GLViewShardsOfGranius::woToAdd = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");

GLViewShardsOfGranius* GLViewShardsOfGranius::New( const std::vector< std::string >& args )
{
   GLViewShardsOfGranius* glv = new GLViewShardsOfGranius( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}


GLViewShardsOfGranius::GLViewShardsOfGranius( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewShardsOfGranius::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewShardsOfGranius::onCreate() is invoked after this module's LoadMap() is completed.

}


void GLViewShardsOfGranius::onCreate()
{
   //GLViewShardsOfGranius::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
}


GLViewShardsOfGranius::~GLViewShardsOfGranius()
{
   //Implicitly calls GLView::~GLView()
    //might need to do something here with the preloadvector or something idk
}


void GLViewShardsOfGranius::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.
   engine->setSoundVolume(masterVol);
   /*if (woIndex < accessibleWOs.size()) {
       acessibleWO = accessibleWOs[woIndex];
       miniName = acessibleWO->getLabel();
   }*/
   /*if (loadMini) {
       loadMini = false;
       accessibleWOs.push_back(woToAdd);
   }*/
   /*if (add) {
       add = false;
       ManagerGLView::getGLView()->getWorldContainer()->push_back(accessibleWOs[0].wo);
   }*/
}


void GLViewShardsOfGranius::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewShardsOfGranius::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
   
}


void GLViewShardsOfGranius::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewShardsOfGranius::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewShardsOfGranius::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );

   if (key.keysym.sym == SDLK_1) {
       WO* wo = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl", Vector(0.1f, 0.1f, 0.1f));
       wo->setPosition(Vector(2.5f, 2.0f, 5.0f));
       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
       wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
       wo->setLabel("pc1");
       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
       accessibleWOs.push_back(wo);
   }

   if (key.keysym.sym == SDLK_SPACE) {
       //acessibleWO->setPosition(Vector(xt, yt, zt));
   }

   if( key.keysym.sym == SDLK_n )
   {
       engine->stopAllSounds();
       engine->play2D(songAlias.c_str());
   }
   if (key.keysym.sym == SDLK_p)
   {
       if (musicPause) {
           engine->play2D(songAlias.c_str());
           musicPause = false;
       }
       else {
           engine->stopAllSounds();
           musicPause = true;
       }
   }
   if (!characterMovement) {
       if (key.keysym.sym == SDLK_w) {
           acessibleWO->moveRelative(Vector(5, 0, 0));
       }
       if (key.keysym.sym == SDLK_s) {
           acessibleWO->moveRelative(Vector(-5, 0, 0));
       }
       if (key.keysym.sym == SDLK_a) {
           acessibleWO->moveRelative(Vector(0, 5, 0));
       }
       if (key.keysym.sym == SDLK_d) {
           acessibleWO->moveRelative(Vector(0, -5, 0));
       }
       if (key.keysym.sym == SDLK_q) {
           //acessibleWO->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
           acessibleWO->rotateAboutRelY((-90.0f * PI) / 180.0f);
       }
       if (key.keysym.sym == SDLK_e) {
           //acessibleWO->rotateAboutGlobalZ((-90.0f * PI) / 180.0f);
           acessibleWO->rotateAboutRelY((90.0f * PI) / 180.0f);
       }
   }
   else {
       if (key.keysym.sym == SDLK_w) {
           acessibleWO->moveRelative(Vector(5, 0, 0));
       }
       if (key.keysym.sym == SDLK_s) {
           acessibleWO->moveRelative(Vector(-5, 0, 0));
       }
       if (key.keysym.sym == SDLK_a) {
           acessibleWO->moveRelative(Vector(0, 5, 0));
       }
       if (key.keysym.sym == SDLK_d) {
           acessibleWO->moveRelative(Vector(0, -5, 0));
       }
       if (key.keysym.sym == SDLK_q) {
           acessibleWO->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
       }
       if (key.keysym.sym == SDLK_e) {
           acessibleWO->rotateAboutGlobalZ((-90.0f * PI) / 180.0f);
       }
   }
}


void GLViewShardsOfGranius::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}




void Aftr::GLViewShardsOfGranius::loadMap()
{
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 15,15,10 );
   xt = 0;
   yt = 0;
   zt = 0;

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string gridGrass(ManagerEnvironmentConfiguration::getLMM() + "/models/grassFloor400x400_pp.wrl");
   std::string redDragon(ManagerEnvironmentConfiguration::getLMM() + "models/Dragon.stl");
   std::string Vampire(ManagerEnvironmentConfiguration::getLMM() + "models/Strahd.stl");
   std::string rainday(ManagerEnvironmentConfiguration::getLMM() + "sounds/rainday.ogg");
   //std::string track(ManagerEnvironmentConfiguration::getSMM() + "/sounds/space.ogg");
   
   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg" );
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

   {
      //Create a light
      float ga = 0.1f; //Global Ambient Light level for this module
      ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
      WOLight* light = WOLight::New();
      light->isDirectionalLight( true );
      light->setPosition( Vector( 0, 0, 100 ) );
      //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
      //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
      light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
      light->setLabel( "Light" );
      worldLst->push_back( light );
   }

   {
      //Create the SkyBox
      WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->setLabel( "Sky Box" );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      worldLst->push_back( wo );
   }

   { 
      ////Create the infinite grass plane (the floor)
      WO* wo = WO::New( gridGrass, Vector( 1, 1, 1 ), MESH_SHADING_TYPE::mstFLAT );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      wo->upon_async_model_loaded( [wo]()
         {
            ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
            grassSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 5.0f );
            grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
            grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
            grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
            grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
         } );
      wo->setLabel( "Grass" );
      worldLst->push_back( wo );
   }

   {
       //to ensure you dont crash preload a cube into acessibleWo
       WO* wo = WO::New(shinyRedPlasticCube, Vector(0.1f, 0.1f, 0.1f));
       wo->setPosition(Vector(0.0f, 0.0f, -4.8f));
       wo->setLabel("");
       worldLst->push_back(wo);
       acessibleWO = wo;
   }
   
   
   {
       using namespace irrklang;
       engine = createIrrKlangDevice();
       //ISound* s = engine->play2D(track.c_str(), true);
       ISoundSource* rain = engine->addSoundSourceFromFile(rainday.c_str());
       engine->addSoundSourceAlias(rain, "rain");
       //need to load in all of the possible sounds and give them a allias in the engine so that it can be eaily played
   }
 
   
   //Make a Dear Im Gui instance via the WOImGui in the engine... This calls
   //the default Dear ImGui demo that shows all the features... To create your own,
   //inherit from WOImGui and override WOImGui::drawImGui_for_this_frame(...) (among any others you need).
   WOImGui* gui = WOImGui::New( nullptr );
   gui->setLabel( "My Gui" );
   gui->subscribe_drawImGuiWidget(
      [this, gui]() //this is a lambda, the capture clause is in [], the input argument list is in (), and the body is in {}
      {
           class MyGui : public WOImGui {
           public:
               static void draw_AftrImGui_Demo(WOImGui* gui, GLViewShardsOfGranius* g)
               {
                   // Menu Flags
                       static bool terrainMenu = false;
                       static bool tilesMenu = false;
                       static bool objectsMenu = false;
                       static bool minisMenu = false;
                       static bool aberationsMenu = false;
                       static bool beastsMenu = false;
                       static bool celestialsMenu = false;
                       static bool constructsMenu = false;
                       static bool dragonsMenu = false;
                       static bool metalicMenu = false;
                       static bool chromaticMenu = false; 
                       static bool gemMenu = false;
                       static bool elementalsMenu = false;
                       static bool feysMenu = false;
                       static bool fiendsMenu = false;
                       static bool demonsMenu = false;
                       static bool devilsMenu = false;
                       static bool giantsMenu = false;
                       static bool humanoidsMenu = false;
                       static bool banditsMenu = false;
                       static bool koboldsMenu = false;
                       static bool noblesMenu = false;
                       static bool goblinsMenu = false;
                       static bool pcMenu = false;
                       static bool monstrocitiesMenu = false;
                       static bool oozesMenu = false;
                       static bool plantsMenu = false;
                       static bool undeadMenu = false;
                       static bool generalMenu = false;
                       static bool combatMenu = false;
                       static bool tavernMenu = false;
                       static bool adventureMenu = false;
                       static bool ambienceMenu = false;
                       static int selectedMini = 0;
                       static int selectedTerrain = 0;
                       static int selectedScene;
                       static std::string selectedSong = "";
                       static std::vector<WorldContainer* > scenes;
                       static std::string label = "";
                       static int miniCount = 0;
                       static int terrainCount = 0;
                       static bool saveFlag = true;
                   ImGui::Begin("Scene Builder");
                   if (!minisMenu) {
                    ImGui::Checkbox("Terrain", &terrainMenu);
                    if (terrainMenu) {
                        ImGui::Checkbox("Tiles", &tilesMenu);
                        if (tilesMenu) {
                            if (ImGui::Button("Stone Wall")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Tiles/stoneWall.stl", Vector(0.1f, 0.1f, 0.1f));
                                wo->setPosition(Vector(0, 0, 1.5f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Wall" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                            if (ImGui::Button("Wood Floor")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Tiles/woodfloor.stl", Vector(0.2f, 0.2f, 0.2f));
                                wo->setPosition(Vector(5, 5, 0.0f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Wood Floor" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                        }
                        ImGui::Checkbox("Objects", &objectsMenu);
                        if (objectsMenu) {
                            if (ImGui::Button("Crate")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Objects/Crate.stl", Vector(0.1f, 0.1f, 0.1f));
                                wo->setPosition(Vector(2.5f, 2.5f, 0.9f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Crate" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                            if (ImGui::Button("Gravestone")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Objects/gravestone.stl", Vector(0.1f, 0.1f, 0.1f));
                                wo->setPosition(Vector(2.5f, 2.5f, 1.5f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Gravestone" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                            if (ImGui::Button("Table")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Objects/Table.stl", Vector(0.2f, 0.2f, 0.2f));
                                wo->setPosition(Vector(5.0f, 5.0f, 1.4f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalY(PI);
                                std::stringstream slabel;
                                slabel << "Table" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                            if (ImGui::Button("Tree")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Objects/Tree.stl", Vector(0.2f, 0.2f, 0.2f));
                                wo->setPosition(Vector(5.0f, 5.0f, 11.0f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Tree" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                            if (ImGui::Button("Pillar")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Objects/pillar.stl", Vector(0.2f, 0.2f, 0.2f));
                                wo->setPosition(Vector(5.0f, 5.0f, 4.0f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Pillar" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                            if (ImGui::Button("Well")) {
                                WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Terrain/Objects/well.stl", Vector(0.2f, 0.2f, 0.2f));
                                wo->setPosition(Vector(5.0f, 5.0f, 6.0f));
                                wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                std::stringstream slabel;
                                slabel << "Well" << terrainCount;
                                terrainCount++;
                                label = slabel.str();
                                wo->setLabel(label);
                                ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                g->accessibleTerrains.push_back(wo);
                                g->acessibleWO = wo;
                                terrainName = label;
                            }
                        }
                    }
                   }
                   if (!terrainMenu) {
                       ImGui::Checkbox("Minis", &minisMenu);
                       if (minisMenu) {
                           ImGui::Checkbox("Aberations", &aberationsMenu);
                           if (aberationsMenu) {
                               if (ImGui::Button("Beholder")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Aberations/Beholder1.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(5.0f, 5.0f, 8.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Beholder" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Mind Flayer")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Aberations/Mindflayer.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 2.25f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Mindflayer" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Beasts", &beastsMenu);
                           if (beastsMenu) {
                               if (ImGui::Button("Brown Bear")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Beasts/BrownBear.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(5.0f, 5.0f, 3.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Brown Bear" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Dire Wolf")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Beasts/DireWolf.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(8.0f, 9.75f, 3.612f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Dire Wolf" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Wolf")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Beasts/Wolf.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 1.1f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Wolf" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               
                           }
                           ImGui::Checkbox("Celestials", &celestialsMenu);
                           if (celestialsMenu) {
                               if (ImGui::Button("Empyrean")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Celestials/Empyrean2.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 17.0f, 13.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Empyrean" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Solar")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Celestials/Solar.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(11.75f, 9.25f, 8.750f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Solar" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Constructs", &constructsMenu);
                           if (constructsMenu) {
                               if (ImGui::Button("Iron Golem")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Constructs/IronGolem.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(10.0f, 10.0f, 6.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Iron Golem" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Stone Knight")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Constructs/StoneKnight.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(7.0f, 7.0f, 4.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Iron Golem" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Dragons", &dragonsMenu);
                           if (dragonsMenu) {
                               ImGui::SameLine();
                               ImGui::Checkbox("Chromatic", &chromaticMenu);
                               if (chromaticMenu) {
                                   if (ImGui::Button("Adult Blue Dragon")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Chromatic/AdultBlueDragon.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                       wo->setPosition(Vector(7.5f, 12.0f, 12.0f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Adult Blue Dragon" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Adult Red Dragon")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Chromatic/RedDragonAdult.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                       wo->setPosition(Vector(7.0f, 12.0f, 7.5f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Adult Red Dragon" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Ancient Red Dragon")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Chromatic/AncientRedDragon.stl", Vector(0.16f, 0.16f, 0.16f));
                                       wo->setPosition(Vector(10.0f, 10.0f, 7.75f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Adult Red Dragon" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Tiamat")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Chromatic/Tiamat.stl", Vector(0.16f, 0.16f, 0.16f));
                                       wo->setPosition(Vector(10.75f, 10.0f, 14.25f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Tiamat" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Young Green Dragon")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Chromatic/GreenDragonYoung.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                       wo->setPosition(Vector(9.5f, 9.5f, 5.4f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Young Green Dragon" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Wyvern")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Chromatic/Wyvern.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                       wo->setPosition(Vector(9.25f, 11.25f, 6.75f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Wyvern" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                               ImGui::SameLine();
                               ImGui::Checkbox("Metalic", &metalicMenu);
                               if (metalicMenu) {
                                   if (ImGui::Button("Bahamut")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Metalic/Bahamut.stl", Vector(0.16f, 0.16f, 0.16f));
                                       wo->setPosition(Vector(10.0f, 10.0f, 13.5f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel; 
                                       slabel << "Bahamut" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Ancient Gold Dragon")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Metalic/AncientGoldDragon.stl", Vector(0.16f, 0.16f, 0.16f));
                                       wo->setPosition(Vector(10.0f, 9.5f, 12.1f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Ancient Gold Dragon" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Ancient Silver Dragon")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Dragons/Metalic/AncientSilverDragon.stl", Vector(0.16f, 0.16f, 0.16f));
                                       wo->setPosition(Vector(10.25f, 9.5f, 10.4f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Ancient Silver Dragon" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }

                           }
                           ImGui::Checkbox("Elementals", &elementalsMenu);
                           if (elementalsMenu) {
                               if (ImGui::Button("Air Elemental")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Elementals/AirElemental.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(9.75f, 9.75f, 6.25f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Air Elemental " << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Earth Elemental ")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Elementals/EarthElemental.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(9.75f, 9.75f, 6.25f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Earth Elemental " << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Fire Elemental ")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Elementals/FireElemental.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(10.5f, 10.5f, 6.25f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Fire Elemental " << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Water Elemental")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Elementals/WaterElemental.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(9.75f, 9.75f, 5.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Water Elemental " << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Feys", &feysMenu);
                           if (feysMenu) {
                               if (ImGui::Button("Centaur")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Feys/Centaur.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(11.0f, 9.75f, 5.75f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Centaur" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Displacer Beast")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Feys/DisplacerBeast.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(9.5f, 9.75f, 5.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Displacer Beast" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Drider")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Feys/DriderFemale.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(9.75f, 9.75f, 3.75f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Drider" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Green Hag")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Feys/GreenHag.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 4.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Green Hag" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Will'O'Wisp")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Feys/WillOWisp.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(2.0f, 2.0f, 2.4f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Will'O'Wisp" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Fiends", &fiendsMenu);
                           if (fiendsMenu) {
                               ImGui::SameLine();
                               ImGui::Checkbox("Demons", &demonsMenu);
                               if (demonsMenu) {
                                   if (ImGui::Button("Balor")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Fiends/Demons/Balor.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                       wo->setPosition(Vector(12.0f, 17.0f, 10.3f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Balor" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Orcus")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Fiends/Demons/Orcus.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                       wo->setPosition(Vector(12.0f, 17.0f, 8.75f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Orcus" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                               ImGui::SameLine();
                               ImGui::Checkbox("Devils", &devilsMenu);
                               if (devilsMenu) {
                                   if (ImGui::Button("Asmodeus")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Fiends/Devils/Asmodeus.stl", Vector(0.2f, 0.2f, 0.2f));
                                       wo->setPosition(Vector(5.0f, 5.0f, 6.9f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Asmodeus" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Chain Devil")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Fiends/Devils/ChainDevil.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.0f, 2.0f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Chain Devil" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Horned Devil")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Fiends/Devils/HornedDevil.stl", Vector(0.2f, 0.2f, 0.2f));
                                       wo->setPosition(Vector(7.75f, 4.75f, 10.85f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Horned Devil" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                           }
                           ImGui::Checkbox("Giants", &giantsMenu);
                           if (giantsMenu) {
                               if (ImGui::Button("Cloud Giant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/CloudGiant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 17.0f, 10.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Cloud Giant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Fire Giant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/FireGiant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(10.0f, 16.5f, 10.1f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Fire Giant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Frost Giant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/FrostGiant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 15.5f, 8.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Frost  Giant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Hill Giant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/HillGiant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 17.0f, 8.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Hill Giant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Oni")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/Oni.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(4.5f, 4.25f, 6.3f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Oni" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Stone Giant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/StoneGiant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 17.0f, 8.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Stone Giant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Storm Giant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/StormGiant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 17.0f, 15.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Storm Giant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Troll")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Giants/Troll.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(5.0f, 4.5f, 6.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Troll" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Humanoids", &humanoidsMenu);
                           if (humanoidsMenu) {
                               ImGui::SameLine();
                               ImGui::Checkbox("Bandits", &banditsMenu);
                               if(banditsMenu){
                                   if (ImGui::Button("Bandit")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Bandits/Bandit.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.8f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Bandit" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Crossbow Bandit")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Bandits/BanditCrossbow.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.3f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Crossbow Bandit" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Half-Orc Bandit Captain")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Bandits/HalfOrcBanditCaptain.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 2.1f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Half-Orc Bandit Captain" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Thug")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Bandits/Thug.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 2.1f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Thug" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                               ImGui::SameLine();
                               ImGui::Checkbox("Kobolds", &koboldsMenu);
                               if (koboldsMenu) {
                                   if (ImGui::Button("Kobold")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Kobolds/Kobold.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.5f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Kobold" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Kobold Chief")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Kobolds/KoboldChiefWings.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 2.25f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Kobold Chief" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Kobold Rogue")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Kobolds/KoboldRogue.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.25f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Kobold" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Kobold Sorceror")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Kobolds/KoboldSorceror.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.75f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Kobold Sorceror" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                               ImGui::SameLine();
                               ImGui::Checkbox("Noble", &noblesMenu);
                               if (noblesMenu) {
                                   //if (ImGui::Button("Female Noble")) {
                                   //    WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Noble/noble-female-a.stl", Vector(0.1f, 0.1f, 0.1f));
                                   //    wo->setPosition(Vector(2.5f, 2.5f, 2.25f));
                                   //    //wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   //    wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   //    std::stringstream slabel;
                                   //    slabel << "Female Noble" << miniCount;
                                   //    miniCount++;
                                   //    label = slabel.str();
                                   //    wo->setLabel(label);
                                   //    ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   //    g->accessibleWOs.push_back(wo);
                                   //    g->acessibleWO = wo;
                                   //}
                                   if (ImGui::Button("Male Noble")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Noble/Rahadin.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 2.0f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Female Noble" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                               ImGui::SameLine();
                               ImGui::Checkbox("Goblins", &goblinsMenu);
                               if (goblinsMenu) {
                                   if (ImGui::Button("Goblin Bow")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Goblins/GoblinBow.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.6f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Goblin Bow" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Goblin CrossBow")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Goblins/GoblinCrossbow.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.3f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Goblin Crossbow" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Goblin King")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Goblins/GoblinKing.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.7f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Goblin King" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Hobgoblin Warlord")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/Goblins/HobgoblinWarlord.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.0f, 1.75f, 2.2f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Hobgoblin Warlord" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                               ImGui::SameLine();
                               ImGui::Checkbox("PC's", &pcMenu);
                               if (pcMenu) {
                                   if (ImGui::Button("Human Fighter")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/PCs/HumanFighter.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 2.1f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Human Fighter" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Halfling Scorcerer")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/PCs/HalflingSorcerer.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 1.25f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Halfling Sorcerer" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                                   if (ImGui::Button("Human Warior")) {
                                       WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Humanoids/PCs/HumanWarior.stl", Vector(0.1f, 0.1f, 0.1f));
                                       wo->setPosition(Vector(2.5f, 2.5f, 2.0f));
                                       wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                       wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                       std::stringstream slabel;
                                       slabel << "Human Warior" << miniCount;
                                       miniCount++;
                                       label = slabel.str();
                                       wo->setLabel(label);
                                       ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                       g->accessibleWOs.push_back(wo);
                                       g->acessibleWO = wo;
                                   }
                               }
                           }
                           ImGui::Checkbox("Monstrocities", &monstrocitiesMenu);
                           if (monstrocitiesMenu) {
                               if (ImGui::Button("Behir")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Monstrocities/Behir.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(12.0f, 17.0f, 3.5f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Behir" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Bulette")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Monstrocities/Bulette.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(15.0f, 15.0f, 3.75f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Bulette" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Owl Bear")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Monstrocities/OwlBear.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(15.0f, 15.0f, 6.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Owl Bear" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Purple Worm")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Monstrocities/PurpleWorm.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(11.5f, 10.0f, 16.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Purple Worm" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Werewolf")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Monstrocities/Werewolf.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(2.0f, 2.0f, 3.25f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Werewolf" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }

                           }
                           ImGui::Checkbox("Oozes", &oozesMenu);
                           if (oozesMenu) {
                               if (ImGui::Button("Black Pudding")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Oozes/BlackPudding.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(5.0f, 5.0f, 5.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Black Pudding" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Gelatinous Cube")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Oozes/GelatinousCube.stl", Vector(0.2f, 0.2f, 0.2f));
                                   wo->setPosition(Vector(5.0f, 5.0f,5.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalY(PI);
                                   std::stringstream slabel;
                                   slabel << "Gelatinous Cube" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Plants", &plantsMenu);
                           if (plantsMenu) {
                               if (ImGui::Button("Needle Blight")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Plants/NeedleBlight.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 2.25f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Needle Blight" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Treant")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Plants/Treant.stl", Vector(0.1875f, 0.1875f, 0.1875f));
                                   wo->setPosition(Vector(12.0f, 17.5f, 13.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Treant" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Twig Blight")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Plants/TwigBlight.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 1.75f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Twig Blight" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                           }
                           ImGui::Checkbox("Undead", &undeadMenu);
                           if (undeadMenu) {
                               if (ImGui::Button("Acererak")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/Acererak.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 2.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Acererak" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Banshee")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/Banshee.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 2.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Banshee" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Mummy Lord")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/MummyLord.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 2.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Acererak" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Skeleton Archer")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/SkeletonArcher.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 2.4f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Skeleton Archer" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Skeleton Spear")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/SkeletonSpear.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 1.75f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Skeleton Spear" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Vampire")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/Strahd.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.0f, 2.0f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Vampire" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               if (ImGui::Button("Zombie")) {
                                   WO* wo = WO::New(ManagerEnvironmentConfiguration::getLMM() + "models/Minis/Undead/Zombie.stl", Vector(0.1f, 0.1f, 0.1f));
                                   wo->setPosition(Vector(2.5f, 2.5f, 1.8f));
                                   wo->rotateAboutGlobalX((-90.0f * PI) / 180.0f);
                                   wo->rotateAboutGlobalZ((90.0f * PI) / 180.0f);
                                   std::stringstream slabel;
                                   slabel << "Zombie" << miniCount;
                                   miniCount++;
                                   label = slabel.str();
                                   wo->setLabel(label);
                                   ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
                                   g->accessibleWOs.push_back(wo);
                                   g->acessibleWO = wo;
                               }
                               
                           }
                       }
                   }
                   ImGui::InputInt("Mini Selector", &selectedMini);
                   if (ImGui::Button("Select")) {
                       if (selectedMini < g->accessibleWOs.size()) {
                           g->acessibleWO = g->accessibleWOs[selectedMini];
                           miniName = g->acessibleWO->getLabel();
                       }
                   }
                   //ImGui::Text("Selected Mini: %s", miniName.c_str());

                   ImGui::InputInt("Terrain Selector", &selectedTerrain);
                   if (ImGui::Button("Select Terrain")) {
                       if(selectedTerrain < g->accessibleTerrains.size()){
                           g->acessibleWO = g->accessibleTerrains[selectedTerrain];
                           terrainName = g->acessibleWO->getLabel();
                       }
                   }
                   //ImGui::Text("Selected Terrain: %s", terrainName.c_str());
                   ImGui::Text("Selected Tokin: %s", g->acessibleWO->getLabel().c_str());
                   /*if (ImGui::Button("Save Scene")) {
                       scenes.push_back(ManagerGLView::getGLView()->getWorldContainer());
                   }*/
                   /*static float xt = 0;
                   static float yt = 0;
                   static float zt = 0;*/
                   /*ImGui::InputFloat("GLobal x", &(g->xt), 1.0f);
                   ImGui::InputFloat("GLobal y", &(g->yt), 1.0f);
                   ImGui::InputFloat("GLobal z", &(g->zt), 1.0f);
                   if (ImGui::Button("Move")) {
                       g->acessibleWO->setPosition(Vector(g->xt, g->yt, g->zt));
                   }*/
                   //ImGui::Checkbox("Move in Characters look direction", &characterMovement);
                   ImGui::InputInt("Scene Selector 1-3:", &selectedScene);
                   if (ImGui::Button("Save Scene")) {
                       std::vector<WO*> tempMinis;
                       std::vector<WO*> tempTerrain;
                       for (int i = 0; i < g->accessibleWOs.size(); i++) {
                           tempMinis.push_back(g->accessibleWOs.at(i));
                       }
                       for (int i = 0; i < g->accessibleTerrains.size(); i++) {
                           tempTerrain.push_back(g->accessibleTerrains.at(i));
                       }
                       if (selectedScene == 1) {
                           g->scene1.clear();
                           g->scene1.push_back(tempMinis);
                           g->scene1.push_back(tempTerrain);
                           saveFlag = true;
                       }
                       else if (selectedScene == 2) {
                           g->scene2.clear();
                           g->scene2.push_back(tempMinis);
                           g->scene2.push_back(tempTerrain);
                           saveFlag = true;
                       }
                       else if (selectedScene == 3) {
                           g->scene3.clear();
                           g->scene3.push_back(tempMinis);
                           g->scene3.push_back(tempTerrain);
                           saveFlag = true;
                       }
                       else {
                           saveFlag = false;
                       }
                       //clear the scene after saving it and only if it saved
                       if (saveFlag) {
                           while (g->accessibleWOs.size() > 0) {
                               int index = g->getWorldContainer()->getIndexOfWO(g->accessibleWOs.back());
                               g->getWorldContainer()->eraseViaWOIndex(index);
                               g->accessibleWOs.pop_back();
                           }
                           while (g->accessibleTerrains.size() > 0) {
                               int index = g->getWorldContainer()->getIndexOfWO(g->accessibleTerrains.back());
                               g->getWorldContainer()->eraseViaWOIndex(index);
                               g->accessibleTerrains.pop_back();
                           }
                           miniCount = 0;
                           terrainCount = 0;
                       }
                   }
                   if (!saveFlag) {
                       ImGui::Text("Failed to save please select scene 1-3");
                   }
                   if (ImGui::Button("Load Scene")) {
                       while (g->accessibleWOs.size() > 0) {
                           int index = g->getWorldContainer()->getIndexOfWO(g->accessibleWOs.back());
                           g->getWorldContainer()->eraseViaWOIndex(index);
                           g->accessibleWOs.pop_back();
                       }
                       while (g->accessibleTerrains.size() > 0) {
                           int index = g->getWorldContainer()->getIndexOfWO(g->accessibleTerrains.back());
                           g->getWorldContainer()->eraseViaWOIndex(index);
                           g->accessibleTerrains.pop_back();
                       }
                       if (selectedScene == 1) {
                           g->accessibleWOs = g->scene1.at(0);
                           g->accessibleTerrains = g->scene1.at(1);
                       }
                       else if (selectedScene == 2) {
                           g->accessibleWOs = g->scene2.at(0);
                           g->accessibleTerrains = g->scene2.at(1);
                       }
                       else if (selectedScene == 3) {
                           g->accessibleWOs = g->scene3.at(0);
                           g->accessibleTerrains = g->scene3.at(1);
                       }
                       for (int i = 0; i < g->accessibleWOs.size(); i++) {
                           g->getWorldContainer()->push_back(g->accessibleWOs.at(i));
                       }
                       for (int i = 0; i < g->accessibleTerrains.size(); i++) {
                           g->getWorldContainer()->push_back(g->accessibleTerrains.at(i));
                       }
                       miniCount = g->accessibleWOs.size();
                       terrainCount = g->accessibleTerrains.size();
                   }
                   if (ImGui::Button("Clear Scene")) {
                       while (g->accessibleWOs.size() > 0) {
                           int index = g->getWorldContainer()->getIndexOfWO(g->accessibleWOs.back());
                           g->getWorldContainer()->eraseViaWOIndex(index);
                           g->accessibleWOs.pop_back();
                       }
                       while (g->accessibleTerrains.size() > 0) {
                           int index = g->getWorldContainer()->getIndexOfWO(g->accessibleTerrains.back());
                           g->getWorldContainer()->eraseViaWOIndex(index);
                           g->accessibleTerrains.pop_back();
                       }
                       miniCount = 0;
                       terrainCount = 0;
                   }
                   ImGui::End();

                   //Gui for controling sound
                   ImGui::Begin("Sound Control");
                   ImGui::SliderFloat("Master Volume", &masterVol, 0.0f, 1.0f);
                   if (!combatMenu) {
                       ImGui::Checkbox("General", &generalMenu);
                       if (generalMenu) {
                           ImGui::Checkbox("Tavern", &tavernMenu);
                           if (tavernMenu) {

                           }
                           ImGui::Checkbox("Adventure", &adventureMenu);
                           if (adventureMenu) {

                           }
                           ImGui::Checkbox("Ambience", &ambienceMenu);
                           if (ambienceMenu) {
                               if (ImGui::Button("Rain")) {
                                   songAlias = "rain";
                               }
                           }
                       }
                   }
                   if (!generalMenu) {
                       ImGui::Checkbox("Combat", &combatMenu);
                       if (combatMenu) {

                       }
                   }
                   ImGui::Text("Selected Song: %s", songAlias.c_str());
                   ImGui::Text("P - pause/play \t N- play new selected track ");
                   
                   ImGui::End();

                   //Gui for controling dice
                   static int numDice = 1;
                   static int diceType = 1;
                   static int modifier = 0;
                   static int result = 0;
                   static bool advantage = false;
                   static bool disadvantage = false;
                   static bool physicalDice = false;
                   ImGui::Begin("Dice Roller");
                   ImGui::InputInt("# of Dice", &numDice);
                   ImGui::InputInt("Dice Type", &diceType);
                   ImGui::InputInt("Modifier", &modifier);
                   if (ImGui::Button("Roll")) {
                       result = 0;
                       if (diceType >= 1) {
                           for (size_t i = 0; i < numDice; i++) {
                               if (advantage) {
                                   result += std::max(rand() % diceType + 1, rand() % diceType + 1) ;
                               }
                               else if (disadvantage) {
                                   result += std::min(rand() % diceType + 1, rand() % diceType + 1) ;
                               }
                               else {
                                   result += rand() % diceType + 1;
                               }
                           }
                       }
                       result += modifier;
                   }
                   if (!disadvantage) {
                       ImGui::Checkbox("Advantage", &advantage);
                   }
                   if (!advantage) {
                       ImGui::Checkbox("Disadvantage", &disadvantage);
                   }
                   if (diceType == 20 && result - modifier == 20) {
                       ImGui::Text("Natural 20!!!");
                   }
                   ImGui::Text("Result: %dD%d + %d = %d", numDice, diceType, modifier, result);
                   ImGui::End();


               }
           };
               MyGui::draw_AftrImGui_Demo(gui, this);
      } );
   this->worldLst->push_back( gui);
}


void GLViewShardsOfGranius::createShardsOfGraniusWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWayPointSpherical::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
}
