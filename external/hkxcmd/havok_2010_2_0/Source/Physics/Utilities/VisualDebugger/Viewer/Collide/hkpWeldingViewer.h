/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_WELDING_VIEWER_H
#define HK_UTILITIES2_WELDING_VIEWER_H

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpWorldViewerBase.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>

class hkDebugDisplayHandler;
class hkpWorld;
class hkListener;

	/// A class for viewing welding data
class hkpWeldingViewer : public hkpWorldViewerBase, public hkpWorldPostSimulationListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TOOLS);


		/// Creates a hkpConstraintViewer.
		static hkProcess* HK_CALL create( const hkArray<hkProcessContext*>& contexts );

		/// Registers the hkpConstraintViewer with the hkViewerFactory.
		static void HK_CALL registerViewer( void );

		/// Get the name of this viewer
		static inline const char* HK_CALL getName( void ) { return "Welding"; }


		/// Gets the tag associated with this viewer type
		virtual int getProcessTag( void ) { return m_tag; }

		void postSimulationCallback(hkpWorld* world);
		
	protected:
		hkpWeldingViewer(const hkArray<hkProcessContext*>& contexts);
		virtual ~hkpWeldingViewer();

		virtual void worldAddedCallback( hkpWorld* world );
		virtual void worldRemovedCallback( hkpWorld* world );

		static int m_tag;
};

#endif	// HK_UTILITIES2_CONTACT_POINT_VIEWER_H

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
