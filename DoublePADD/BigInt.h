#pragma once
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

template<uint32_t L>
bool add(const uint32_t a[L], const uint32_t b[L], uint32_t s[L]) {
    assert(L > 0);
    uint64_t carry = 0;
    for (uint32_t i = 0; i < L; i++) {
        carry += (uint64_t)a[i] + (uint64_t)b[i];
        s[i] = (uint32_t)carry;
        carry >>= 32;
    }
    return carry;
}

template<uint32_t L>
bool add(const uint32_t a[L], const uint32_t b, uint32_t s[L]) {
    assert(L > 0);
    uint64_t carry = (uint64_t)a[0] + (uint64_t)b;
    s[0] = (uint32_t)carry;
    carry >>= 32;
    for (uint32_t i = 1; i < L; i++) {
        carry += (uint64_t)a[i];
        s[i] = (uint32_t)carry;
        carry >>= 32;
    }
    return carry;
}

template<uint32_t L>
bool sub(const uint32_t a[L], const uint32_t b[L], uint32_t s[L]) {
    assert(L > 0);
    int64_t carry = 0;
    for (uint32_t i = 0; i < L; i++) {
        carry += (uint64_t)a[i] - (uint64_t)b[i];
        s[i] = (uint32_t)carry;
        carry >>= 32;
    }
    return carry;
}

template<uint32_t L>
bool sub(const uint32_t a[L], const uint32_t b, uint32_t s[L]) {
    assert(L > 0);
    int64_t carry = (uint64_t)a[0] - (uint64_t)b;
    s[0] = (uint32_t)carry;
    carry >>= 32;
    for (uint32_t i = 1; i < L; i++) {
        carry += (uint64_t)a[i];
        s[i] = (uint32_t)carry;
        carry >>= 32;
    }
    return carry;
}

template<uint32_t L>
void mul(const uint32_t a[L], const uint32_t b[L], uint32_t p[L * 2]) {
    assert(L > 0);
    for (uint32_t i = 0; i < L; i++) {
        p[i] = 0;
    }
    uint64_t temp;
    for (uint32_t i = 0; i < L; i++) {
        temp = 0;
        for (uint32_t j = 0; j < L; j++) {
            temp += (uint64_t)a[i] * (uint64_t)b[j] + p[i + j];
            p[i + j] = (uint32_t)temp;
            temp >>= 32;
        }
        p[L + i] = (uint32_t)temp;
    }
}

template<uint32_t L>
void mull(const uint32_t a[L], const uint32_t b[L], uint32_t p[L]) {
    assert(L > 0);
    for (uint32_t i = 0; i < L; i++) {
        p[i] = 0;
    }
    uint64_t temp;
    for (uint32_t i = 0; i < L; i++) {
        temp = 0;
        for (uint32_t j = 0; j < L - i; j++) {
            temp += (uint64_t)a[i] * (uint64_t)b[j] + p[i + j];
            p[i + j] = (uint32_t)temp;
            temp >>= 32;
        }
    }
}

template<uint32_t L, uint32_t S>
void shiftRight(const uint32_t a[L], uint32_t o[L]) {
    assert(L > 0);
    assert(S >= 0 && S <= 32);
    for (uint32_t i = 0; i < L - 1; i++) {
        o[i] = (uint32_t)(*(uint64_t *)&a[i] >> S);
    }
    o[L - 1] = a[L - 1] >> S;
}

template<uint32_t L>
void copy(const uint32_t a[L], uint32_t o[L]) {
    for (uint32_t i = 0; i < L; i++) {
        o[i] = a[i];
    }
}

void print(const uint32_t *a, uint32_t L) {
    for (uint32_t i = 0;  i < L; i++) {
        printf("%08x", a[L - 1 - i]);
    }
}