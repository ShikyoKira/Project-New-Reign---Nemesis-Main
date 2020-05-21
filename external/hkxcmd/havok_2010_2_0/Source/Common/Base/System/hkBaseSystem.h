/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKBASESYSTEM_H
#define HKBASE_HKBASESYSTEM_H

/// An error report function to be passed to Havok when initializing it subsystems. In particular
typedef void (HK_CALL *hkErrorReportFunction)(const char* s, void* errorReportObject);

struct hkHardwareInfo
{
	int m_numThreads;
};

// Get information about the hardware configuration
void HK_CALL hkGetHardwareInfo( hkHardwareInfo& info );

class hkMemoryRouter;

/// This class is used to initialize the Havok subsystems (memory, singleton, etc).
namespace hkBaseSystem
{
		/// Initializes Havok's subsystems.
		/// Havok provides a number of memory systems, please consult hkMemoryInitUtil for more details.
		/// On error, the errorReportFunction is called with an error string and the errorReportObject pointer.
		/// init() sets in internal flag, so it may be called more than once but multiple init() and quit() calls
		/// do not nest.
	hkResult HK_CALL init( 
		hkMemoryRouter* memoryRouter, 
		hkErrorReportFunction errorReportFunction,
		void* errorReportObject = HK_NULL );

		/// Call for each active thread, sets the thread local router variable.
	hkResult HK_CALL initThread( hkMemoryRouter* threadAllocator );
		///
	hkResult HK_CALL quitThread();

		/// Quit the subsystems. It is safe to call multiple times.
		/// It cleans up in reverse order to init: default singletons, hkError,
		/// hkFileSystem and hkMemory
	hkResult HK_CALL quit();

		/// Checks if the system has already been initialized.
	hkBool HK_CALL isInitialized();

		/// Utility function to initialize all singletons, called from init().
		/// You do not need to call this method unless you have completely replaced
		/// the init method.
		/// All global non plain old data (POD) types are encapsulated in
		/// singletons. At program startup, static constructors put all these
		/// singletons and functions to create them into a linked list
		/// (see hkSingleton.h). At init time, we can then traverse the list,
		/// setting up singletons which have not been explicitly set up already.
	void HK_CALL initSingletons();

		/// Utility function to quit all singletons, called from quit().
		/// You do not need to call this method unless you have completely replaced
		/// the quit method.
	void HK_CALL quitSingletons();
}

#endif // HKBASE_HKBASESYSTEM_H

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
