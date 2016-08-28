//
//  CustomScene.h
//  GRE
//
//  Created by Jacques Tronconi on 07/04/2016.
//
//
/*
#ifndef GRE_CustomScene_h
#define GRE_CustomScene_h

#include "Scene.h"

#include "CustomNode.h"
#include "MeshCustomNode.h"
#include "CameraCustomNode.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief An Experimental Tree to manage 3D-space efficiently.
///
/// The Gre::CustomScenePrivate proceed with Gre::CustomNodePrivate
/// that allows us to go in any neighbouring directions.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC CustomScenePrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    CustomScenePrivate(const std::string& name);
    virtual ~CustomScenePrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Root Node of this Scene.
    //////////////////////////////////////////////////////////////////////
    virtual CustomNode getRoot();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Default Active CameraCustomNode.
    //////////////////////////////////////////////////////////////////////
    void setDefaultActiveCamera(const CameraCustomNode& node);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Create a Default Pass given its name and number.
    /// If passNumber is already taken, return Pass::Null.
    //////////////////////////////////////////////////////////////////////
    Pass createAndAddPass(const std::string& name, const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds given custom created pass.
    /// This function takes ownership of the Pointer. Also, it may return
    /// Pass::Null if an error occured.
    //////////////////////////////////////////////////////////////////////
    Pass addPass(PassPrivate* customPass);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
    Pass getPassByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the pass at given number.
    //////////////////////////////////////////////////////////////////////
    Pass getPassByNumber(const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the first Pass with given name.
    //////////////////////////////////////////////////////////////////////
    void removePassByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the Pass with given number.
    //////////////////////////////////////////////////////////////////////
    void removePassByNumber(const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every active Pass, sorted by PassNumber from first
    /// to last.
    //////////////////////////////////////////////////////////////////////
    PassList getActivePasses() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls every methods registered in the Listener.
    /// When overwriting this method, if you still want to add actions, use
    /// ::onEvent(e) to call this class function which correctly calls every
    /// methods.
    ///
    /// @note In this class the onEvent() method transmit the Events to
    /// the Root Node. 
    //////////////////////////////////////////////////////////////////////
    virtual void onEvent (const Event& e);
    
private:
    
    /// @brief The Root Node, as a persistent ResourceUser object.
    CustomNode _mRootNode;
    
    /// @brief The default CameraCustomNode. This Node is used to render the
    /// Camera.
    CameraCustomNode _mDefaultActiveCamera;
    
    /// @brief Holds every Pass in the Scene.
    PassPrivateOwnedList _mPasses;
    
    /// @brief Catalog to Pass objects by name.
    std::map<PassNumber, std::weak_ptr<PassPrivate> > _mPassesByNumber;
};

GreEndNamespace

#endif
*/