/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_REMOTE_PROCESS_H
#define HK_VISUALIZE_REMOTE_PROCESS_H

#include <Common/Visualize/Serialize/hkDisplaySerializeOStream.h>

class hkDisplaySerializeIStream;
class hkDebugDisplayHandler;
class hkProcessHandler;

	///
class hkProcess 
{
	public:

			// A type used to identify a process.  Currently this is only used to 
			// identify a "class" of viewers that are associated with a product.
		enum ProcessType
		{			
			HK_PROCESS_TYPE_UNKNOWN = -1,
			HK_PROCESS_TYPE_BEHAVIOR = 1,
		};

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkProcess);

			///
		hkProcess( hkBool selectable ) 
			: m_selectable(selectable) {}

			/// Virtual destructor for base class.
		virtual ~hkProcess() {}

			///
		virtual int getProcessTag() = 0;

			/// Gets a type identification for the viewer.
		virtual ProcessType getType() const { return HK_PROCESS_TYPE_UNKNOWN; }
		
		//
		// General state and settings
		//
		hkBool m_selectable; // can it be selected by the client or is just always on on the server
	
		//
		// The streams and interfaces the process can use to communicate.
		//
		hkDisplaySerializeIStream* m_inStream; 
		hkDisplaySerializeOStream* m_outStream; 
		hkDebugDisplayHandler* m_displayHandler;
		hkProcessHandler* m_processHandler;

		//
		// Command Interface (How it processes incoming data)
		//

			/// Get the commands this handler understands so
			/// that this handler can be mapped correctly and process only
			/// called when appropriate
		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands ) { commands = HK_NULL; numCommands = 0; }
		
			/// given a input stream and a command (that has already been parsed from
			/// the stream), consume the data.
		virtual void consumeCommand( hkUint8 command ) { }
			
			/// We can say, for any current process we have, that it is
			/// ok if its output stream is still ok ( a lot don't use input, but all use output)
		virtual bool isOk() { return m_outStream && m_outStream->isOk(); }

			/// Returns true if the process is a local process in the demo framework, false otherwise.
		bool isLocalViewer() const { return m_outStream == HK_NULL && m_inStream == HK_NULL; }

		//
		// Process step interface
		//
		virtual void init() { } // after the process handler has created the process, it sets up the stream ptrs etc, then calls this init func			

			/// step (a lot don't bother to impl this as driven by callbacks)
		virtual void step( hkReal frameTimeInMs  ) { }

};

#endif // HK_VISUALIZE_REMOTE_PROCESS_H

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
