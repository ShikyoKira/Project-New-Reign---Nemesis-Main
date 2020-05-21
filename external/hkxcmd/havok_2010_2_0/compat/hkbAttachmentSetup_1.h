#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbAttachmentSetup : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbAttachmentSetup(void) {}
   HK_FORCE_INLINE hkbAttachmentSetup( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   enum AttachmentType
   {
      ATTACHMENT_TYPE_KEYFRAME_RIGID_BODY = 0,
      ATTACHMENT_TYPE_BALL_SOCKET_CONSTRAINT = 1,
      ATTACHMENT_TYPE_RAGDOLL_CONSTRAINT = 2,
      ATTACHMENT_TYPE_SET_WORLD_FROM_MODEL = 3,
      ATTACHMENT_TYPE_NONE = 4,
   };

   // Properties
   hkReal m_blendInTime;
   hkReal m_moveAttacherFraction;
   hkReal m_gain;
   hkReal m_extrapolationTimeStep;
   hkReal m_fixUpGain;
   hkReal m_maxLinearDistance;
   hkReal m_maxAngularDistance;
   hkEnum<AttachmentType,hkInt8> m_attachmentType;
};
extern const hkClass hkbAttachmentSetupClass;

