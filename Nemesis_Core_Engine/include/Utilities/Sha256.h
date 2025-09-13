#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>

namespace nemesis
{
    struct SHA256
    {
    private:
        static constexpr std::array<uint32_t, 64> K
            = {0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u,
               0xab1c5ed5u, 0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu,
               0x9bdc06a7u, 0xc19bf174u, 0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu,
               0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau, 0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
               0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u, 0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu,
               0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u, 0xa2bfe8a1u, 0xa81a664bu,
               0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u, 0x19a4c116u,
               0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
               0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u,
               0xc67178f2u};

        std::array<uint32_t, 8> state{};
        uint64_t bitlen = 0;
        std::array<uint8_t, 64> buffer{};
        size_t buffer_len = 0;

    public:
        using Digest = std::array<uint8_t, 32>;

        SHA256()
        {
            reset();
        }

        void update(const uint8_t* data, size_t len)
        {
            for (size_t i = 0; i < len; ++i)
            {
                buffer[buffer_len++] = data[i];

                if (buffer_len == 64)
                {
                    transform(buffer.data());
                    bitlen += 512;
                    buffer_len = 0;
                }
            }
        }

        void update(std::string_view sv)
        {
            update(reinterpret_cast<const uint8_t*>(sv.data()), sv.size());
        }

        Digest digest()
        {
            Digest out{};
            finalize(out);
            return out;
        }

        std::string hexdigest()
        {
            Digest d = digest();
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');

            for (uint8_t b : d)
            {
                oss << std::setw(2) << static_cast<int>(b);
            }

            return oss.str();
        }

        static Digest hash(const uint8_t* data, size_t len)
        {
            SHA256 ctx;
            ctx.update(data, len);
            return ctx.digest();
        }

        static Digest hash(std::string_view sv)
        {
            return hash(reinterpret_cast<const uint8_t*>(sv.data()), sv.size());
        }

        static std::string hex(std::string_view sv)
        {
            SHA256 ctx;
            ctx.update(sv);
            return ctx.hexdigest();
        }

        static std::string uuid(std::string_view sv)
        {
            std::string hxd, hx = hex(sv);
            std::ostringstream oss;

            const size_t groups[] = {8, 4, 4, 4, 12, 32};
            size_t pos            = 0;

            for (size_t i = 0; i < sizeof(groups) / sizeof(groups[0]); ++i)
            {
                oss << hx.substr(pos, groups[i]);
                pos += groups[i];

                if (i != (sizeof(groups) / sizeof(groups[0]) - 1))
                {
                    oss << "-";
                }
            }

            return oss.str();
        }

        void reset()
        {
            state      = {0x6a09e667u,
                          0xbb67ae85u,
                          0x3c6ef372u,
                          0xa54ff53au,
                          0x510e527fu,
                          0x9b05688cu,
                          0x1f83d9abu,
                          0x5be0cd19u};
            bitlen     = 0;
            buffer_len = 0;
            std::memset(buffer.data(), 0, buffer.size());
        }

    private:
        static inline uint32_t rotr(uint32_t x, uint32_t n)
        {
            return (x >> n) | (x << (32 - n));
        }

        static inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
        {
            return (x & y) ^ (~x & z);
        }

        static inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
        {
            return (x & y) ^ (x & z) ^ (y & z);
        }

        static inline uint32_t big_sigma0(uint32_t x)
        {
            return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
        }

        static inline uint32_t big_sigma1(uint32_t x)
        {
            return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
        }

        static inline uint32_t small_sigma0(uint32_t x)
        {
            return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
        }

        static inline uint32_t small_sigma1(uint32_t x)
        {
            return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
        }

        static inline uint32_t load_be32(const uint8_t* p)
        {
            return (static_cast<uint32_t>(p[0]) << 24) | (static_cast<uint32_t>(p[1]) << 16)
                   | (static_cast<uint32_t>(p[2]) << 8) | (static_cast<uint32_t>(p[3]));
        }

        static inline void store_be32(uint8_t* p, uint32_t v)
        {
            p[0] = static_cast<uint8_t>(v >> 24);
            p[1] = static_cast<uint8_t>(v >> 16);
            p[2] = static_cast<uint8_t>(v >> 8);
            p[3] = static_cast<uint8_t>(v);
        }

        static inline void store_be64(uint8_t* p, uint64_t v)
        {
            for (int i = 7; i >= 0; --i)
            {
                p[7 - i] = static_cast<uint8_t>(v >> (i * 8));
            }
        }

        void transform(const uint8_t* chunk)
        {
            uint32_t w[64];

            for (int i = 0; i < 16; ++i)
            {
                w[i] = load_be32(chunk + i * 4);
            }

            for (int i = 16; i < 64; ++i)
            {
                w[i] = small_sigma1(w[i - 2]) + w[i - 7] + small_sigma0(w[i - 15]) + w[i - 16];
            }

            uint32_t a = state[0];
            uint32_t b = state[1];
            uint32_t c = state[2];
            uint32_t d = state[3];
            uint32_t e = state[4];
            uint32_t f = state[5];
            uint32_t g = state[6];
            uint32_t h = state[7];

            for (int i = 0; i < 64; ++i)
            {
                uint32_t t1 = h + big_sigma1(e) + ch(e, f, g) + K[i] + w[i];
                uint32_t t2 = big_sigma0(a) + maj(a, b, c);
                h           = g;
                g           = f;
                f           = e;
                e           = d + t1;
                d           = c;
                c           = b;
                b           = a;
                a           = t1 + t2;
            }

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
            state[4] += e;
            state[5] += f;
            state[6] += g;
            state[7] += h;
        }

        void finalize(Digest& out)
        {
            uint64_t total_bits = bitlen + static_cast<uint64_t>(buffer_len) * 8ull;
            buffer[buffer_len++] = 0x80;

            if (buffer_len > 56)
            {
                while (buffer_len < 64)
                {
                    buffer[buffer_len++] = 0;
                }

                transform(buffer.data());
                buffer_len = 0;
            }

            while (buffer_len < 56)
            {
                buffer[buffer_len++] = 0;
            }

            store_be64(buffer.data() + 56, total_bits);
            transform(buffer.data());

            for (int i = 0; i < 8; ++i)
            {
                store_be32(out.data() + i * 4, state[i]);
            }

            reset();
        }
    };
}
