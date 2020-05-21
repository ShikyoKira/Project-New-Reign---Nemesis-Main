/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_AGENT_REGISTER_UTIL_H
#define HK_COLLIDE2_AGENT_REGISTER_UTIL_H


class hkpCollisionDispatcher;

	/// A helper class that allows you to register all Havok collision agents the collision dispatcher.
class hkpAgentRegisterUtil
{
	public:
			/// Registers all collision agents with the collision dispatcher.
			/// This also links in all agents, even if you are not using them.
			/// However it serves as a good starting point for your development,
			/// and later you could implement your own version.
			/// Note: this function also calls registerAlternateShapeTypes
		static void HK_CALL registerAllAgents(hkpCollisionDispatcher* dis);

		static void HK_CALL _registerBvTreeAgents(hkpCollisionDispatcher* dis);
	
		static void HK_CALL _registerTerminalAgents( hkpCollisionDispatcher* dis);

		static void HK_CALL _registerListAgents( hkpCollisionDispatcher* dis);

};

#endif // HK_COLLIDE2_AGENT_REGISTER_UTIL_H

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
