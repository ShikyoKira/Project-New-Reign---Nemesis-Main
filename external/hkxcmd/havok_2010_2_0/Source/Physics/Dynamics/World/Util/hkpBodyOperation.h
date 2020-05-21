/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_BODY_OPERATION_H
#define HK_DYNAMICS2_BODY_OPERATION_H

class hkpRigidBody;

class hkpBodyOperation : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_WORLD);

		enum ExecutionState
		{
			EXECUTE_SINGLE_THREADED,
			EXECUTE_MULTI_THREADED,
		};

		struct UpdateInfo
		{
			UpdateInfo(): m_bodyIsDeleted(false), m_bodyIsInWorld(true) {}

			hkBool m_bodyIsDeleted;
			hkBool m_bodyIsInWorld;
		};

	public:

		hkpBodyOperation() { }

		virtual ~hkpBodyOperation() { }

		virtual void execute( hkpRigidBody* body, UpdateInfo& updateInfo ) = 0;
};

#endif // HK_DYNAMICS2_BODY_OPERATION_H

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
