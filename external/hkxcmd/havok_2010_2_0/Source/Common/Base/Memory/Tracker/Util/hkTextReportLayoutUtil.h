/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_TEXT_REPORT_LAYOUT_UTIL_H
#define HKBASE_TEXT_REPORT_LAYOUT_UTIL_H

#include <Common/Base/Container/SubString/hkSubString.h>

class hkTextReportLayoutUtil
{
    public:

		//+hk.MemoryTracker(ignore=true)

		enum Align
		{
			ALIGN_LEFT = 'l',
			ALIGN_RIGHT = 'r',
			ALIGN_CENTER = 'c',
			ALIGN_DEFAULT = 'd', 
		};

		struct Column
		{
			int m_start;
			int m_width;
		};

			/// Columns are always output where m_start is greater
		static void HK_CALL calcColumns(const int* colWidths, int numCols, int colSpacing, int startOffset, hkArray<Column>& columns);
			/// Write underlines
		static void HK_CALL writeCharColumns(const hkArray<Column>& columns, char c, hkOstream& stream);
			/// Write all of the columns
		static void HK_CALL writeColumns(const hkArray<Column>& columns, const char* align, const hkSubString* text, hkOstream& stream); 
			/// 
		static void HK_CALL writeColumns(const hkArray<Column>& columns, const char* align, const char*const* text, hkOstream& stream);
			/// Write columns - columns are delimited by 'delimiter' character
		static void HK_CALL writeColumns(const hkArray<Column>& columns, const char* align, const char* texts, char delimiter, hkOstream& stream);

			/// Write a character
		static void HK_CALL writeRepeatChar(char c, int size, hkOstream& stream);
			/// Write aligned text
		static void HK_CALL writeAlignedText(Align align, const hkSubString& textIn, int colSize, hkOstream& stream);

			/// Returns true if valid align char
		static hkBool HK_CALL isValidAlignChar(char c);

			/// Calculate column widths
		static void HK_CALL calcColumnWidths(const char* texts, char delimiter, int numCols, const int* colWidths, int* colWidthsOut);

			/// Calculate delimited text
		static void HK_CALL calcDelimitedText(const char* texts, char delimiter, hkArray<hkSubString>& subStrings);
			/// Calculate delimited text - forcing there to be the amount specified
		static void HK_CALL calcDelimitedText(const char* texts, char delimiter, int numCols, hkArray<hkSubString>& subStrings);

};

#endif // HKBASE_SCAN_REPORT_UTIL_H

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
