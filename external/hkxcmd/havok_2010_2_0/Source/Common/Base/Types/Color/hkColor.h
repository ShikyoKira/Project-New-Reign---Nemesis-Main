/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_COLOR
#define HK_VISUALIZE_COLOR


extern const class hkClass hkColorClass;

/// Colors are represented as unsigned ints [0xAARRGGBB] though there are helper
/// functions to create them from floats (and chars)
class hkColor
{
	HK_DECLARE_REFLECTION();
	
public:
			/// Creates a color using the specified char values.
		static int HK_CALL rgbFromChars(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xff);
		
			/// Creates a color using the specified floating-point values interpreted as RGB colors in [0,1].
		static int HK_CALL rgbFromFloats(const hkReal red, const hkReal green, const hkReal blue, const hkReal alpha = 1.0f);
	
			/// Creates a color using the specified floating-point values interpreted as HSV colors in [0,1]
		static int HK_CALL rgbFromHSV(const hkReal h, const hkReal s, const hkReal v, const hkReal alpha = 1.0f);

			/// Creates a random color
		static int HK_CALL getRandomColor();

			/// Creates a random color using the given hkPseudoRandomGenerator
		static int HK_CALL getRandomColor( class hkPseudoRandomGenerator& rand );

			/// Get the i'th color from s_colorTable with optional alpha.
		static int HK_CALL getPaletteColor(int i, int alpha=0xff);

			/// Gets the Alpha component of a color.
		inline static char HK_CALL getAlphaAsChar(int color) { return char((unsigned(color) >> 24) & 0xFF);	}
		inline static hkReal HK_CALL getAlphaAsFloat(int color) { return ((unsigned char)getAlphaAsChar(color)) * (1.0f / 255.0f); }

			/// Gets the Red component of a color.
		inline static char HK_CALL getRedAsChar(int color) { return char((unsigned(color) >> 16) & 0xFF); }
		inline static hkReal HK_CALL getRedAsFloat(int color) { return ((unsigned char)getRedAsChar(color)) * (1.0f / 255.0f); }

			/// Gets the Green component of a color.
		inline static char HK_CALL getGreenAsChar(int color) { return char((unsigned(color) >> 8) & 0xFF); }
		inline static hkReal HK_CALL getGreenAsFloat(int color) { return ((unsigned char)getGreenAsChar(color)) * (1.0f / 255.0f); }

			/// Gets the Blue component of a color.
		inline static char HK_CALL getBlueAsChar(int color) { return char(unsigned(color) & 0xFF); }
		inline static hkReal HK_CALL getBlueAsFloat(int color) { return ((unsigned char)getBlueAsChar(color)) * (1.0f / 255.0f); }

			/// Combine two colors. Uses the identity (a+b)/2 == (a^b)>>1 + (a&b).
		inline static int HK_CALL average(int a, int b) {return int( unsigned((a^b)&0xfefefefe) >> 1 ) + (a&b); }

			/// Halve the alpha.
		inline static int HK_CALL semiTransparent(int color, int n=1)
		{
			return (int(unsigned(color)>>n) & 0xFF000000) | (color&0x00ffffff);
		}

			/// Make the color darker.
		inline static int HK_CALL darken(int col, int n=1)
		{
			int masklo = (((1<<n) - 1) * 0x00010101); // Build a mask for the lower n bits of each of the bytes
			int maskLoAndAlpha = masklo | 0xff000000;
			int colShifted = ( col & ~maskLoAndAlpha ) >> n; // Clear alpha, Mask out the lower bits of each component and shift
 			return (col & 0xff000000) | colShifted; // Restore alpha
		}

			/// Make the color lighter.
		inline static int HK_CALL lighten(int col, int n=1)
		{
			int lobits = (( (1<<   n ) - 1) * 0x00010101); // populate new low bits with 1s
			int maskhi = (( (1<<(8-n)) - 1) * 0x00010101); // mask of bits shifted out
			return (col & ~maskhi) // preserve alpha and high bits of each component. consider lighten(col=0xff808080,n=1)
				| ((col&maskhi) << n) // shift bits in each component upward, mask ensures components don't interfere
				| lobits; // fill new low bits with 1s
		}

	public:

		// Colors taken from http://www.keller.com/rgb.html
		enum ExtendedColors
		{
			MAROON = 0xFF800000,
			DARKRED = 0xFF8B0000,
			RED = 0xFFFF0000,
			LIGHTPINK = 0xFFFFB6C1,
			CRIMSON = 0xFFDC143C,
			PALEVIOLETRED = 0xFFDB7093,
			HOTPINK = 0xFFFF69B4,
			DEEPPINK = 0xFFFF1493,
			MEDIUMVIOLETRED = 0xFFC71585,
			PURPLE = 0xFF800080,
			DARKMAGENTA = 0xFF8B008B,
			ORCHID = 0xFFDA70D6,
			THISTLE = 0xFFD8BFD8,
			PLUM = 0xFFDDA0DD,
			VIOLET = 0xFFEE82EE,
			FUCHSIA = 0xFFFF00FF,
			MAGENTA = 0xFFFF00FF,
			MEDIUMORCHID = 0xFFBA55D3,
			DARKVIOLET = 0xFF9400D3,
			DARKORCHID = 0xFF9932CC,
			BLUEVIOLET = 0xFF8A2BE2,
			INDIGO = 0xFF4B0082,
			MEDIUMPURPLE = 0xFF9370DB,
			SLATEBLUE = 0xFF6A5ACD,
			MEDIUMSLATEBLUE = 0xFF7B68EE,
			DARKBLUE = 0xFF00008B,
			MEDIUMBLUE = 0xFF0000CD,
			BLUE = 0xFF0000FF,
			NAVY = 0xFF000080,
			MIDNIGHTBLUE = 0xFF191970,
			DARKSLATEBLUE = 0xFF483D8B,
			ROYALBLUE = 0xFF4169E1,
			CORNFLOWERBLUE = 0xFF6495ED,
			LIGHTSTEELBLUE = 0xFFB0C4DE,
			ALICEBLUE = 0xFFF0F8FF,
			GHOSTWHITE = 0xFFF8F8FF,
			LAVENDER = 0xFFE6E6FA,
			DODGERBLUE = 0xFF1E90FF,
			STEELBLUE = 0xFF4682B4,
			DEEPSKYBLUE = 0xFF00BFFF,
			SLATEGRAY = 0xFF708090,
			LIGHTSLATEGRAY = 0xFF778899,
			LIGHTSKYBLUE = 0xFF87CEFA,
			SKYBLUE = 0xFF87CEEB,
			LIGHTBLUE = 0xFFADD8E6,
			TEAL = 0xFF008080,
			DARKCYAN = 0xFF008B8B,
			DARKTURQUOISE = 0xFF00CED1,
			CYAN = 0xFF00FFFF,
			MEDIUMTURQUOISE = 0xFF48D1CC,
			CADETBLUE = 0xFF5F9EA0,
			PALETURQUOISE = 0xFFAFEEEE,
			LIGHTCYAN = 0xFFE0FFFF,
			AZURE = 0xFFF0FFFF,
			LIGHTSEAGREEN = 0xFF20B2AA,
			TURQUOISE = 0xFF40E0D0,
			POWDERBLUE = 0xFFB0E0E6,
			DARKSLATEGRAY = 0xFF2F4F4F,
			AQUAMARINE = 0xFF7FFFD4,
			MEDIUMSPRINGGREEN = 0xFF00FA9A,
			MEDIUMAQUAMARINE = 0xFF66CDAA,
			SPRINGGREEN = 0xFF00FF7F,
			MEDIUMSEAGREEN = 0xFF3CB371,
			SEAGREEN = 0xFF2E8B57,
			LIMEGREEN = 0xFF32CD32,
			DARKGREEN = 0xFF006400,
			GREEN = 0xFF008000,
			LIME = 0xFF00FF00,
			FORESTGREEN = 0xFF228B22,
			DARKSEAGREEN = 0xFF8FBC8F,
			LIGHTGREEN = 0xFF90EE90,
			PALEGREEN = 0xFF98FB98,
			MINTCREAM = 0xFFF5FFFA,
			HONEYDEW = 0xFFF0FFF0,
			CHARTREUSE = 0xFF7FFF00,
			LAWNGREEN = 0xFF7CFC00,
			OLIVEDRAB = 0xFF6B8E23,
			DARKOLIVEGREEN = 0xFF556B2F,
			YELLOWGREEN = 0xFF9ACD32,
			GREENYELLOW = 0xFFADFF2F,
			BEIGE = 0xFFF5F5DC,
			LINEN = 0xFFFAF0E6,
			LIGHTGOLDENRODYELLOW = 0xFFFAFAD2,
			OLIVE = 0xFF808000,
			YELLOW = 0xFFFFFF00,
			LIGHTYELLOW = 0xFFFFFFE0,
			IVORY = 0xFFFFFFF0,
			DARKKHAKI = 0xFFBDB76B,
			KHAKI = 0xFFF0E68C,
			PALEGOLDENROD = 0xFFEEE8AA,
			WHEAT = 0xFFF5DEB3,
			GOLD = 0xFFFFD700,
			LEMONCHIFFON = 0xFFFFFACD,
			PAPAYAWHIP = 0xFFFFEFD5,
			DARKGOLDENROD = 0xFFB8860B,
			GOLDENROD = 0xFFDAA520,
			ANTIQUEWHITE = 0xFFFAEBD7,
			CORNSILK = 0xFFFFF8DC,
			OLDLACE = 0xFFFDF5E6,
			MOCCASIN = 0xFFFFE4B5,
			NAVAJOWHITE = 0xFFFFDEAD,
			ORANGE = 0xFFFFA500,
			BISQUE = 0xFFFFE4C4,
			TAN = 0xFFD2B48C,
			DARKORANGE = 0xFFFF8C00,
			BURLYWOOD = 0xFFDEB887,
			SADDLEBROWN = 0xFF8B4513,
			SANDYBROWN = 0xFFF4A460,
			BLANCHEDALMOND = 0xFFFFEBCD,
			LAVENDERBLUSH = 0xFFFFF0F5,
			SEASHELL = 0xFFFFF5EE,
			FLORALWHITE = 0xFFFFFAF0,
			SNOW = 0xFFFFFAFA,
			PERU = 0xFFCD853F,
			PEACHPUFF = 0xFFFFDAB9,
			CHOCOLATE = 0xFFD2691E,
			SIENNA = 0xFFA0522D,
			LIGHTSALMON = 0xFFFFA07A,
			CORAL = 0xFFFF7F50,
			DARKSALMON = 0xFFE9967A,
			MISTYROSE = 0xFFFFE4E1,
			ORANGERED = 0xFFFF4500,
			SALMON = 0xFFFA8072,
			TOMATO = 0xFFFF6347,
			ROSYBROWN = 0xFFBC8F8F,
			PINK = 0xFFFFC0CB,
			INDIANRED = 0xFFCD5C5C,
			LIGHTCORAL = 0xFFF08080,
			BROWN = 0xFFA52A2A,
			FIREBRICK = 0xFFB22222,
			BLACK = 0xFF000000,
			DIMGRAY = 0xFF696969,
			GRAY = 0xFF808080,
			DARKGRAY = 0xFFA9A9A9,
			SILVER = 0xFFC0C0C0,
			LIGHTGREY = 0xFFD3D3D3,
			GAINSBORO = 0xFFDCDCDC,
			WHITESMOKE = 0xFFF5F5F5,
			WHITE = 0xFFFFFFFF,
			GREY		= 0xff888888,
			GREY25 = 0xff404040,
			GREY50 = 0xff808080,
			GREY75 = 0xffc0c0c0,
		};

			// A simple color table - arrange in batches of 8 colors light to dark
		static int s_colorTable[32];
};

#endif // HK_VISUALIZE_COLOR

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
