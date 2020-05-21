/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_OSTREAM_LOGGER_H
#define HK_BASE_OSTREAM_LOGGER_H

#include <Common/Base/DebugUtil/Logger/hkLogger.h>

/// Logger which writes to an hkOstream
class hkOstreamLogger : public hkLogger
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Write to the given filename.
		hkOstreamLogger(const char* fname) : m_os(fname)
		{
			init();
		}
		
			/// Write to the given writer.
		hkOstreamLogger(hkStreamWriter* w) : m_os(w)
		{
			init();
		}

			/// Flushes buffers and all future output is directed to filename.
		void changeOutput(const char* filename)
		{
			flush();
			hkOstream tmp(filename);
			m_os.setStreamWriter(tmp.getStreamWriter());
		}

			/// Note that this will not produce output until a log message is received in this scope.
		virtual void pushScope(const char* s)
		{
			m_scopes.pushBack(s);
		}

		virtual void popScope()
		{
			if( m_printedScopeLevel == m_scopes.getSize() )
			{
				m_indent.popBack();
				m_indent.popBack();
				m_indent.back() = 0;
				if( m_scopes.back() )
				{
					m_os.printf("<  %s<<\n", m_indent.begin());
				}
				--m_printedScopeLevel;
			}
			m_scopes.popBack();
			if( m_buffered == false )
			{
				flush();
			}
		}

		virtual void setThreshold(int level)
		{
			m_logThreshold = level;
		}

		virtual void flush()
		{
			m_os.flush();
		}

	protected:

		virtual void _log( int level, const char* fmt, hk_va_list args )
		{
			if( m_logThreshold <= level )
			{
				for( /**/; m_printedScopeLevel < m_scopes.getSize(); ++m_printedScopeLevel )
				{
					if( m_scopes[m_printedScopeLevel] )
					{
						m_os.printf(">  %s>>%s\n", m_indent.begin(), m_scopes[m_printedScopeLevel]);
					}
					m_indent.back() = ' ';
					m_indent.pushBack(' ');
					m_indent.pushBack(0);
				}

				int sideIndex = hkMath::clamp<int>(level, 0, int(LOG_ERROR)) / 10;
				const char sidebar[][5] =
				{
					"   ",
					"   ",
					" . ",
					"-W-",
					"<E>"
				};
				m_os.printf("%s%s", sidebar[sideIndex], m_indent.begin());
				hkArray<char> buf; buf.setSize(256);
				hkString::vsnprintf(buf.begin(), buf.getSize(), fmt, args );
				m_os.printf("%s\n", buf.begin());
				if( m_buffered == false )
				{
					m_os.flush();
				}
			}
		}

			/// If enabled, don't flush after every _log call.
		void setBuffered(hkBool b)
		{
			m_buffered = b;
		}

	protected:

		void init()
		{
			m_indent.pushBack(0);
			m_printedScopeLevel = 0;
			m_logThreshold = 0;
			setBuffered(false);
		}

		hkOstream m_os;
		hkArray<char> m_indent;
		hkArray<const char*> m_scopes;
		int m_printedScopeLevel; // scopes labels only printed when there is a message in scope
		int m_logThreshold;
		hkBool m_buffered;
};

#endif //HK_BASE_OSTREAM_LOGGER_H

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
