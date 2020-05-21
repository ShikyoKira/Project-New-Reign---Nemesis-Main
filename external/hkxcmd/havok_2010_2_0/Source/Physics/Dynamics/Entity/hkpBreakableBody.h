/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_BREAKABLE_BODY_H
#define HK_DYNAMICS2_BREAKABLE_BODY_H


extern const class hkClass hkpBreakableBodyClass;

class hkpRigidBody;


/// This is an interface class to Havok Destruction's hkdBreakableBody. Its sole purpose
/// right now is to provide access to the hkdBreakableBody's clone function.
class hkpBreakableBody : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE_CLASS);
		HK_DECLARE_REFLECTION();

			// Default constructor.
		hkpBreakableBody()
		:	hkReferencedObject()
		{
		}

		hkpBreakableBody(class hkFinishLoadedObjectFlag flag)
		:	hkReferencedObject(flag)
		{
		}

			// Destructor.
		virtual ~hkpBreakableBody()
		{
		}

			/// Clone this Breakable Body. See the documentation for hkdBreakableBody for details.
		virtual hkpBreakableBody* cloneBreakableBody(hkpRigidBody* clonedRigidBody = HK_NULL) const = 0;
};


#endif // HK_DYNAMICS2_BREAKABLE_BODY_H

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
