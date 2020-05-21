/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_VDB_COMMAND_WRITER_H
#define HKBASE_VDB_COMMAND_WRITER_H

#include <Common/Base/System/Io/Writer/hkStreamWriter.h>

/// Allows large amounts of data to be sent to the visual debugger by splitting
/// it into separate commands.
/// The commands are given a maximum size, and their first internal byte
/// indicates whether they are the last of a sequence representing one big
/// chunk of data.
class hkVdbCommandWriter : public hkStreamWriter
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

			/// Construct a VDB command writer which issues commands to the writer.
			/// \param command the header command (see hkVisualDebuggerProtocol).
			/// \param size the maximum size of the commands.
		hkVdbCommandWriter( hkStreamWriter* writer, hkUint8 command, hkUint32 size = 2048 );

			/// Flushing this stream will send the final command.
		virtual void flush();
		
			/// The destructor calls flush.
		~hkVdbCommandWriter();

		// hkStreamWriter interface.
		virtual hkBool isOk() const;
		virtual int write(const void* buf, int nbytes);

	protected:
			/// Write the command to the underlying m_writer stream.
		void writeCommand( hkBool final );

	protected:
		hkArray<char>::Debug m_buffer;
		hkStreamWriter* m_writer;
		int m_writePos;
		hkUint8 m_command;
};

#endif // HKBASE_VDB_COMMAND_WRITER_H

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
