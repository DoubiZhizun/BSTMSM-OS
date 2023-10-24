#pragma once
#include <stdlib.h>
#include "BigInt.h"

template <uint32_t L, uint32_t K>
class Zp{
    public:
        Zp(const uint32_t _m[L], const uint32_t _u[L]) {
            assert(L > 0 & K > 0);
            assert(K + 2 <= L * 32);
            assert(_m[0] % 2 == 1);
            copy<L>(_m, m);
            copy<L>(_u, u);
            sub<L>(_m, 1, ms1);
            sub<L>(_m, 2, ms2);
        }
        void madd(const uint32_t a[L], const uint32_t b[L], uint32_t s[L]) const {
            add<L>(a, b, s);
            uint32_t temp[L];
            if (!sub<L>(s, m, temp)) copy<L>(temp, s);
        }
        void msub(const uint32_t a[L], const uint32_t b[L], uint32_t s[L]) const {
            if (!sub<L>(a, b, s)) return;
            add<L>(s, m, s);
        }
        void mmul(const uint32_t a[L], const uint32_t b[L], uint32_t p[L]) const {
            uint32_t c[L * 2];
            uint32_t d[L * 2];
            uint32_t e[L * 2];
            uint32_t f[L * 2];
            uint32_t g[L];
            uint32_t temp[L];
            uint32_t temp2[L];
            mul<L>(a, b, c);
            shiftRight<L * 2, (K - 1) % 32>(c, d);
            mul<L>(d + (K - 1) / 32, u, e);
            shiftRight<L * 2, (K + 1) % 32>(e, f);
            mull<L>(f + (K + 1) / 32, m, g);
            sub<L>(c, g, p);
            if (sub<L>(p, m, temp)) return;
            if (sub<L>(temp, m, temp2)) copy<L>(temp, p);
            else copy<L>(temp2, p);
        }
        template<uint32_t L2>
        void mpow(const uint32_t a[L], const uint32_t e[L2], uint32_t p[L]) const {
            p[0] = 1;
            for (uint32_t i = 1; i < L; i++) {
                p[i] = 0;
            }
            int i, j;
            for (i = L2 - 1; i >= 0; i--) {
                for (j = 31; j >= 0; j--) {
                    if (e[i] & (1 << j)) {
                        goto jmp1;
                    }
                }
            }
            for (; i >= 0; i--) {
                for (j = 31; j >= 0; j--) {
                    mmul(p, p, p);
                    if (e[i] & (1 << j)) {
                        jmp1: mmul(p, a, p);
                    }
                }
            }
        }
        void minv(const uint32_t a[L], uint32_t inv[L]) const {
            mpow<L>(a, ms2, inv);
        }
        void random(uint32_t a[L]) const {
            int i;
            for (i = L * 4 - 1; i >= 0; i--) {
                ((uint8_t *)a)[i] = rand() % ((int)((uint8_t *)ms1)[i] + 1);
                if(((uint8_t *)a)[i] != ((uint8_t *)ms1)[i]) break;
            }
            for (i--; i >= 0; i--) {
                ((uint8_t *)a)[i] = rand() & 0xff;
            }
        }
    private:
        uint32_t m[L];
        uint32_t ms1[L];
        uint32_t ms2[L];
        uint32_t u[L];
};

uint32_t _m[12] = {
    0x00000001, 0x8508c000, 0x30000000, 0x170b5d44,
    0xba094800, 0x1ef3622f, 0x00f5138f, 0x1a22d9f3,
    0x6ca1493b, 0xc63b05c0, 0x17c510ea, 0x01ae3a46,
};
uint32_t _u[12] = {
    0x5e4daffc, 0x1f9fd58c, 0x89c42a59, 0xd0ed6877,
    0xd85a6d02, 0x6af2d488, 0x6776b1a0, 0x3bbad0de,
    0x582ef4f7, 0x976c3ca0, 0x0cc4060e, 0x0261508d,
};

Zp<12, 377> zp(_m, _u);
