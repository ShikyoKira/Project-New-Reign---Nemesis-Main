#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbCharacter_2.h"
#include "hkbCharacterSetup_2.h"
#include "hkbBehaviorGraph_1.h"
#include "hkbProjectData_2.h"

class hkbCharacter : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacter(void) {}
   HK_FORCE_INLINE hkbCharacter( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_nearbyCharacters(flag)
   , m_name(flag)
   , m_ragdollDriver(flag)
   , m_characterControllerDriver(flag)
   , m_footIkDriver(flag)
   , m_handIkDriver(flag)
   , m_setup(flag)
   , m_behaviorGraph(flag)
   , m_projectData(flag)
   , m_animationBindingSet(flag)
   , m_raycastInterface(flag)
   , m_world(flag)
   , m_eventQueue(flag)
   , m_worldFromModel(flag)
      {}
   // Properties
   hkArray<hkbCharacter*> m_nearbyCharacters;
   hkInt16 m_currentLod;
   hkInt16 m_numTracksInLod;
   hkStringPtr m_name;
   hkRefVariant m_ragdollDriver;
   hkRefVariant m_characterControllerDriver;
   hkRefVariant m_footIkDriver;
   hkRefVariant m_handIkDriver;
   hkRefPtr<hkbCharacterSetup> m_setup;
   hkRefPtr<hkbBehaviorGraph> m_behaviorGraph;
   hkRefPtr<hkbProjectData> m_projectData;
   hkRefVariant m_animationBindingSet;
   hkRefVariant m_raycastInterface;
   hkRefVariant m_world;
   hkRefVariant m_eventQueue;
   hkRefVariant m_worldFromModel;
   //hkSimpleArray<hkRefVariant> m_poseLocal;
   const void** m_poseLocal;
   int m_numPoseLocal; //+overridetype(hkSimpleArray<void*>)
   hkBool m_deleteWorldFromModel;
   hkBool m_deletePoseLocal;
};
extern const hkClass hkbCharacterClass;

