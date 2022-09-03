#pragma once

#include "GLView.h"
#include "vector"
#include "irrKlang.h"

namespace Aftr
{
   class Camera;

/**
   \class GLViewShardsOfGranius
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewShardsOfGranius : public GLView
{
public:
   static GLViewShardsOfGranius* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewShardsOfGranius();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createShardsOfGraniusWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );
   std::vector < std::vector<WO*>> scene1;
   std::vector < std::vector<WO*>> scene2;
   std::vector < std::vector<WO*>> scene3;
   std::vector<WO*> accessibleWOs;
   std::vector<WO*> accessibleTerrains;
   std::vector<irrklang::ISoundSource*> accessibleSounds; // dont think i need this but leave it for now
   irrklang::ISoundEngine* engine;
   static float masterVol;
   WO* acessibleWO;
   //WO* acessibleTerrain;
   static int woIndex;
   static std::string miniName;
   static std::string terrainName;
   static bool characterMovement;
   static std::string songAlias;
   static bool musicPause;
   static bool loadMini;
   //this is gonna need to be removed later 
   float xt, yt, zt;
   
protected:
   GLViewShardsOfGranius( const std::vector< std::string >& args );
   virtual void onCreate();   
};

/** \} */

} //namespace Aftr
