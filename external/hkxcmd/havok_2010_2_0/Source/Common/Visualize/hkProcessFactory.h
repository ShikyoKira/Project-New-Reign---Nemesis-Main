/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_PROCESS_FACTORY_H
#define HK_VISUALIZE_PROCESS_FACTORY_H

class hkProcessScheduler;
class hkProcessContext;

/// This class stores a list of process which are available.
/// It is a singleton that store the registry of name to create
/// function. It will only allow a scheduler to create any though.
/// This is a thread safe singleton
class hkProcessFactory : public hkReferencedObject, public hkSingleton<hkProcessFactory>
{
	//+vtable(true)
		friend class hkSingleton<hkProcessFactory>;
		friend class hkProcessHandler;
		friend class hkServerProcessHandler;

	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		typedef class hkProcess* (HK_CALL *hkProcessCreationFunction)( const hkArray<hkProcessContext*>& contexts );

		hkProcessFactory();
		~hkProcessFactory();

			/// Registers a viewer with hkProcessFactory instance, returns a tag which should be used by
			/// the process to identify the data it produces.
		int registerProcess(const char* viewerName, 
			hkProcessCreationFunction creationFunction);
		
		const char* getProcessName(int id);

		int getProcessId(const char* name);

		
			/// Creates a process identified by 'processName'. Caller owns it.
			/// Be careful if you call this (normally a handler would call them and setup the processes)
		hkProcess* createProcess(const char *processName, hkArray<hkProcessContext*>& contexts);
		hkProcess* createProcess(int tag, hkArray<hkProcessContext*>& contexts);


	public:
		struct ProcessIdPair
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY, ProcessIdPair);
			hkStringPtr m_name;
			hkProcessCreationFunction m_processCreationFunction;
			int m_tag;
		};

		const hkArray<ProcessIdPair>& getName2CreationFunction() const { return m_name2creationFunction; }

	protected:

		int m_freeTag; // tag generator, starts at 0 and increments

		hkArray<ProcessIdPair> m_name2creationFunction;

		mutable class hkCriticalSection* m_criticalSection;
};

#endif // HK_VISUALIZE_PROCESS_FACTORY_H

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
