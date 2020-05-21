/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_MOPP_MODIFIER_H
#define HK_COLLIDE2_MOPP_MODIFIER_H

	/// Interface used by a MOPP machine to add/remove terminals from an existing MOPP.
	/// Read hkMoppModifyVirtualMachine_queryAabb for details
class hkpMoppModifier
{
	//+hk.MemoryTracker(ignore=True)
	public:

		virtual ~hkpMoppModifier() { }

			/// Implementations of this function should return true for every terminal, which should be removed
		virtual hkBool shouldTerminalBeRemoved( unsigned int id, const unsigned int *properties ) = 0;

			/// This function will be called for every subtree which has only 'to-be-removed' children
			/// Changing the MOPP code at this address to zero will remove those children.
		virtual void addTerminalRemoveInfo( int relativeMoppAddress ) = 0;
};


#endif // HK_COLLIDE2_MOPP_MODIFIER_H

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
