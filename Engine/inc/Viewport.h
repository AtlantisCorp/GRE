//
//  Viewport.h
//  GRE
//
//  Created by Jacques Tronconi on 09/03/2016.
//
//

#ifndef GRE_Viewport_h
#define GRE_Viewport_h

#include "Pools.h"
#include "Scene.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A simple Viewport object.
///
/// The Viewport is defined using ratio. This means that for example,
/// if you set a width ratio of 0.5f, when RenderContext calls
/// onBordersChanged() with the RenderContext new width of 400 px, the
/// width given by getSurface().width will be 400*0.5f = 200.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Viewport
{
public:
    
    Viewport(const std::string& name, float top = 0.0f, float left = 0.0f, float width = 1.0f, float height = 1.0f, bool activated = true);
    virtual ~Viewport();
    
    Viewport& operator = (const Viewport& rhs);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Viewport. This function should be called by the
    /// RenderContext object.
    //////////////////////////////////////////////////////////////////////
    virtual void onBordersChanged(const Surface& parentSurface);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Name of this Viewport.
    //////////////////////////////////////////////////////////////////////
    const std::string& getName() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Viepwort should be rendered.
    //////////////////////////////////////////////////////////////////////
    bool isActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface covered by this Viewport.
    //////////////////////////////////////////////////////////////////////
    const Surface& getSurface() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Selects a Scene for this Viewport.
    //////////////////////////////////////////////////////////////////////
    void selectScene(const Scene& scene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'true' if this Viewport has a Scene object linked to.
    //////////////////////////////////////////////////////////////////////
    bool hasScene() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object linked to this Viewport if it has
    /// one, or Scene::Null if not.
    //////////////////////////////////////////////////////////////////////
    const Scene& getScene() const;
    
    /// @brief An empty const vector of Viewport.
    static const std::vector<Viewport> EmptyVector;
    
protected:
    
    /// @brief Name of this Viewport.
    std::string _mName;
    
    /// @brief Is this Viewport activated ?
    /// Default value is 'true'.
    bool _mActivated;
    
    /// @brief Width border ratio.
    float _mBorderWidth;
    
    /// @brief Height border ratio.
    float _mBorderHeight;
    
    /// @brief Top margin ratio.
    float _mBorderTop;
    
    /// @brief Left margin ratio.
    float _mBorderLeft;
    
    /// @brief Surface object updated with Viewport::onBordersChanged().
    Surface _mSurface;
    
    /// @brief Holds the Scene that might have been selected by the User to draw this
    /// Viewport object.
    Scene _mScene;
};

GreEndNamespace

#endif
