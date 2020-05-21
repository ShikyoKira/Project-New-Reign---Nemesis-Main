/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>

// LZ77/LZSS-style compression. Data is compressed into a sequence of runs of literal bytes and backreferences 
// into the output stream. Literals and backreferences are not encoded specially, just written out. This gives
// a worse compression ratio than, say, LZMA, but keeps decompression very fast.
// 
// Formats:
// {0      :3, litcount:5} followed by "litcount+1" literal bytes (1 - 32)
// {backlen:3, offhigh :5} {offlow:8} is a reference to "backlen+2" bytes at offset "offhigh||offlow + 1"
// {7      :3, offhigh :5} {offlow:8} {backlen:8} is a reference to "backlen+9" bytes at offset "offhigh||offlow + 1"

namespace hkBufferCompression
{
	enum {LITCOUNT_BITS = 5};
	enum {MAX_LITERAL_LEN = (1<<LITCOUNT_BITS)};
	enum {MAX_MATCH_OFFSET = (1<<(8 + LITCOUNT_BITS))};
	enum {MAX_SHORT_MATCH_LEN = (1<<(8-LITCOUNT_BITS)) - 2 + 2};
	enum {MAX_LONG_MATCH_LEN = (1<<8) - 1 + 9};

	enum {DICTSIZE = 1<<14 };

	class CompressedOutput
	{
		public:

			CompressedOutput(hkUchar* out) : m_out(out), m_out_start(out), m_litchars(0)
			{
			}

			unsigned bytesWritten()
			{
				return static_cast<unsigned>(m_out - m_out_start);
			}

			void endLiteralRun()
			{
				if (m_litchars > 0)
				{
					// We are ending a run of literals
					// We need to seek back in the output stream and update the count
					// in the byte that began the run of literals.

					HK_ASSERT(0x69a7fd0a, m_litchars <= MAX_LITERAL_LEN);
					m_out[-(m_litchars+1)] |= (m_litchars - 1);
					m_litchars = 0;
				}
			}

			void writeLiteral(char c)
			{
				if (m_litchars == 0)
				{
					// We are beginning a run of literals
					// We write a zero byte, the exact count will be filled in later.
					*m_out++ = 0;
				}
				*m_out++ = c;

				m_litchars++;

				if (m_litchars == MAX_LITERAL_LEN)
				{
					endLiteralRun();
				}
			}

			void writeBackref(int matchlen, int offset)
			{
				endLiteralRun();
				HK_ASSERT(0x5ea3a449, matchlen <= MAX_LONG_MATCH_LEN && offset <= MAX_MATCH_OFFSET);

				int o = offset - 1;
				if (matchlen <= MAX_SHORT_MATCH_LEN)
				{
					// short backref format
					HK_ASSERT(0x769cb317, matchlen - 2 < 7 && matchlen - 2 > 0);
					*m_out++ = static_cast<hkUchar>(((matchlen - 2) << LITCOUNT_BITS) | (o >> 8));
					*m_out++ = static_cast<hkUchar>(o & 0xff);
				}
				else
				{
					// long backref format
					HK_ASSERT(0x1a45b4e5, matchlen - 9 <= 256);
					*m_out++ = static_cast<hkUchar>((7 << LITCOUNT_BITS) | (o >> 8));
					*m_out++ = static_cast<hkUchar>(o & 0xff);
					*m_out++ = static_cast<hkUchar>(matchlen - 9);
				}
			}

		private:
			hkUchar* m_out;
			hkUchar* m_out_start;
			// litchars is the number of literals already written to the current run.
			int m_litchars;
	};


		// memcpy(), but takes pointers as reference parameters and updates them
	static HK_FORCE_INLINE void copyout(hkUchar*& dst, const hkUchar*& src, int n)
	{
		while (n--) *dst++ = *src++;
	}
}

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
