/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_COLLIDE2_BROAD_PHASE_HANDLE_H
#define HK_COLLIDE2_BROAD_PHASE_HANDLE_H


extern const class hkClass hkpBroadPhaseHandleClass;

/// An identifier that is used to link the broadphase with the user application.
/// Each object that is to be added to the broadphase must have a hkpBroadPhaseHandle.
/// For example, the hkpCollidable class has a member of type hkpTypedBroadPhaseHandle
/// (which directly derives from hkpBroadPhaseHandle),
/// allowing Havok rigid bodies to be used with the broadphase. Alternatively, if you are
/// using the broadphase on its own, you can create your own hkpBroadPhaseHandle subclasses.
class hkpBroadPhaseHandle
{
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpBroadPhaseHandle);

		hkUint32 m_id; // +nosave

		hkpBroadPhaseHandle(): m_id(0){}

	public:

		hkpBroadPhaseHandle( class hkFinishLoadedObjectFlag flag ){}

};


#endif // HK_COLLIDE2_BROAD_PHASE_HANDLE_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
