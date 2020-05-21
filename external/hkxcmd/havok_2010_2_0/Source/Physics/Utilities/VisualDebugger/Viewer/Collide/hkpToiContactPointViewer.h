/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_TOI_VIEWER_H
#define HK_UTILITIES2_TOI_VIEWER_H

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpWorldViewerBase.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

class hkDebugDisplayHandler;
class hkpWorld;
class hkListener;

	/// Draws TOI contact points in the VDB. Contact points are drawn as red
	/// arrows. See the userguide for more information.
class hkpToiContactPointViewer
	: public hkpWorldViewerBase
	, public hkpContactListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		virtual void init();

		static const char* getName() { return "Contact Points (TOI)"; }
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);
		static void HK_CALL registerViewer();

		int getProcessTag(void);

		virtual void contactPointCallback( const hkpContactPointEvent& event );

	protected:
		hkpToiContactPointViewer(const hkArray<hkProcessContext*>& contexts);
		virtual ~hkpToiContactPointViewer();
		virtual void worldAddedCallback  ( hkpWorld* world );
		virtual void worldRemovedCallback( hkpWorld* world );

		static int s_tag;
};

#endif	// HK_UTILITIES2_TOI_VIEWER_H

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
