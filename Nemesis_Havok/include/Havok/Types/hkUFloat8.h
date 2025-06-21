#pragma once

#include <array>
#include <cmath>

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkUFloat8 : nemesis::hkVariant
    {
#define hkUFloat8_eps 0.01f
#define hkUFloat8_maxValue 1000000.0f

    private:
        enum
        {
            MAX_VALUE = 256,

            // Constants used in compressing / decompressing the values table
            ENCODED_EXPONENT_BITS = 5,
            ENCODED_MANTISSA_BITS = (16 - ENCODED_EXPONENT_BITS),
            ENCODED_EXPONENT_BIAS = 119,
            ENCODED_EXPONENT_MASK = (1 << ENCODED_EXPONENT_BITS) - 1,
            ENCODED_MANTISSA_MASK = (1 << ENCODED_MANTISSA_BITS) - 1,

            FLOAT_MANTISSA_BITS = 23,
            FLOAT_MANTISSA_MASK = (1 << FLOAT_MANTISSA_BITS) - 1,

            ENCODE_MANTISSA_SHIFT = (FLOAT_MANTISSA_BITS - ENCODED_MANTISSA_BITS),
            DECODE_EXPONENT_BIAS  = (ENCODED_EXPONENT_BIAS << (FLOAT_MANTISSA_BITS - ENCODE_MANTISSA_SHIFT)),
        };

        unsigned char m_value;

    public:
        hkUFloat8() = default;
        hkUFloat8(const float f);
        hkUFloat8(const double d);

        nemesis::hkUFloat8& operator=(const double& dv);
        nemesis::hkUFloat8& operator=(const float& fv);
        nemesis::hkUFloat8& operator=(const unsigned char& b);

        bool IsZero() const noexcept;
        void SetZero() noexcept;

        bool IsMax() const noexcept;
        void SetMax() noexcept;

        bool operator==(const hkUFloat8& other) const noexcept;

        void SetArithmeticMean(const hkUFloat8& a, const hkUFloat8& b);

        float AsFloat() const;
        unsigned char AsByte() const;

    private:
        static float DecodeFloat(const unsigned short i)
        {
            const int intExpo = (i + hkUFloat8::DECODE_EXPONENT_BIAS) << hkUFloat8::ENCODE_MANTISSA_SHIFT;

            union
            {
                const float* f;
                const int* i;
            } f2i;

            f2i.i = &intExpo;
            return i ? *f2i.f : 0.0f;
        }

        static unsigned short GetEncodedFloat(unsigned char index)
        {
            if (index == 0) return 0;

            float target = hkUFloat8_eps * std::powf(hkUFloat8_maxValue / hkUFloat8_eps, (index - 1) / 254.0f);

            union
            {
                const float* f;
                const int* i;
            } f2i;

            f2i.f = &target;

            unsigned short encoded
                = static_cast<unsigned short>((*f2i.i >> ENCODE_MANTISSA_SHIFT) - DECODE_EXPONENT_BIAS);
            return encoded;
        }

        static constexpr float AbsFloat(const float& f)
        {
            return f < 0 ? -f : f;
        }

    public:
        static constexpr std::array<float, 256> FloatLookupTable{
            0,         0.00999832, 0.0107498, 0.0115585, 0.0124283, 0.0133629, 0.01437,   0.0154495,
            0.0166092, 0.0178604,  0.0192032, 0.0206451, 0.0222015, 0.0238724, 0.0256653, 0.0275955,
            0.0296783, 0.0319061,  0.0343018, 0.0368805, 0.0396576, 0.0426483, 0.0458527, 0.0493011,
            0.053009,  0.0569916,  0.0612793, 0.0658875, 0.0708313, 0.0761719, 0.0819092, 0.0880737,
            0.094696,  0.101807,   0.109467,  0.117706,  0.126526,  0.136047,  0.146301,  0.157288,
            0.169128,  0.181885,   0.195557,  0.210266,  0.226074,  0.243103,  0.261353,  0.281006,
            0.302124,  0.324829,   0.349365,  0.37561,   0.403809,  0.434204,  0.466919,  0.501953,
            0.539795,  0.580322,   0.624023,  0.670898,  0.721436,  0.775635,  0.833984,  0.896729,
            0.964355,  1.03662,    1.11475,   1.19873,   1.28857,   1.38574,   1.48975,   1.60205,
            1.72266,   1.85205,    1.99121,   2.14062,   2.30176,   2.47559,   2.66113,   2.86133,
            3.07715,   3.30859,    3.55762,   3.8252,    4.11133,   4.42188,   4.75391,   5.11133,
            5.49609,   5.91016,    6.35547,   6.83203,   7.3457,    7.89844,   8.49219,   9.13281,
            9.82031,   10.5586,    11.3516,   12.207,    13.125,    14.1094,   15.1719,   16.3125,
            17.5391,   18.8594,    20.2734,   21.8047,   23.4453,   25.2031,   27.1016,   29.1406,
            31.3359,   33.6875,    36.2188,   38.9531,   41.875,    45.0312,   48.4062,   52.0625,
            55.9688,   60.1875,    64.6875,   69.5625,   74.8125,   80.4375,   86.4688,   93,
            100,       107.5,      115.594,   124.281,   133.625,   143.688,   154.5,     166.125,
            178.625,   192.062,    206.5,     222,       238.75,    256.625,   276,       296.75,
            319,       343,        368.875,   396.625,   426.5,     458.5,     493,       530,
            570,       612.75,     659,       708.5,     761.75,    819,       880.75,    947,
            1018.25,   1094.5,     1177,      1265.5,    1361,      1463,      1573,      1691.5,
            1819,      1955.5,     2102,      2261,      2431,      2614,      2810,      3022,
            3249,      3493,       3756,      4039,      4342,      4668,      5020,      5398,
            5804,      6240,       6710,      7214,      7758,      8340,      8968,      9640,
            10368,     11148,      11984,     12888,     13856,     14900,     16020,     17224,
            18520,     19912,      21408,     23024,     24752,     26616,     28616,     30768,
            33088,     35568,      38240,     41120,     44224,     47536,     51120,     54960,
            59104,     63552,      68320,     73472,     78976,     84928,     91328,     98176,
            105568,    113504,     122048,    131200,    141120,    151680,    163136,    175424,
            188608,    202752,     218048,    234432,    252096,    270976,    291328,    313344,
            336896,    362240,     389504,    418816,    450304,    484096,    520576,    559616,
            601856,    647168,     695808,    748032,    804352,    864768,    929792,    999936};

        static constexpr unsigned char GetClosestIndex(const float& val)
        {
            if (val <= 0.0f) return 0;

            if (val >= hkUFloat8_maxValue) return 255;

            int bestIndex  = 1;
            float bestDiff = AbsFloat(val - FloatLookupTable[1]);

            for (int i = 2; i < 256; ++i)
            {
                float diff = AbsFloat(val - FloatLookupTable[i]);

                if (diff < bestDiff)
                {
                    bestDiff  = diff;
                    bestIndex = i;
                }
            }

            return static_cast<unsigned char>(bestIndex);
        }
    };

    constexpr std::array<float, 256> nemesis::hkUFloat8::FloatLookupTable;
}
