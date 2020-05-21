#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include <Common/Base/Types/Geometry/LocalFrame/hkLocalFrame.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include "hkbCharacter_2.h"

class hkbHandle : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbHandle(void) {}
   HK_FORCE_INLINE hkbHandle( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_frame(flag)
   , m_rigidBody(flag)
   , m_character(flag)
      {}
   // Properties
   hkRefPtr<hkLocalFrame> m_frame;
   hkRefPtr<hkpRigidBody> m_rigidBody;
   hkRefPtr<hkbCharacter> m_character;
   hkInt16 m_animationBoneIndex;
};
extern const hkClass hkbHandleClass;

