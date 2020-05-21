/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SIMULATION_ISLAND_VIEWER_H
#define HK_UTILITIES2_SIMULATION_ISLAND_VIEWER_H

#include <Common/Base/hkBase.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpWorldViewerBase.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Common/Visualize/Shape/hkDisplayAABB.h>


class hkDebugDisplayHandler;
class hkpWorld;

	/// Displays the bounding boxes of all simulation islands in the world. The the boxes will
	/// be blue for active simulation islands and green for inactive simulation islands.
class hkpSimulationIslandViewer : public hkpWorldViewerBase, protected hkpWorldPostSimulationListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Creates a hkpSimulationIslandViewer.
		static hkProcess* HK_CALL create( const hkArray<hkProcessContext*>& contexts );

			/// Registers the hkpSimulationIslandViewer with the hkViewerFactory.
		static void HK_CALL registerViewer();

			/// Gets the tag associated with this viewer type
		virtual int getProcessTag() { return m_tag; }

		static inline const char* HK_CALL getName() { return "Simulation Islands"; }

			// Control the state of the simulation island viewer
		hkBool m_showActiveIslands;
		hkBool m_showInactiveIslands;

	protected:

		hkpSimulationIslandViewer( const hkArray<hkProcessContext*>& contexts );
		virtual ~hkpSimulationIslandViewer();

		virtual void postSimulationCallback(hkpWorld* world );
		virtual void worldAddedCallback( hkpWorld* world );
		virtual void worldRemovedCallback( hkpWorld* world );

		hkArray<hkDisplayAABB> m_inactiveIslandDisplayGeometries;
		hkArray<hkDisplayAABB> m_activeIslandDisplayGeometries;

		static int m_tag;
};

#endif	// HK_UTILITIES2_SIMULATION_ISLAND_VIEWER_H

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
