#include <stddef.h>
#include <assert.h>

#include "chacha-rng.h"

static inline uint32_t u32_to_le(uint32_t value) {
    return value;
}

static inline uint32_t u32_from_le(uint32_t value) {
    return value;
}

static inline uint64_t u64_to_le(uint64_t value) {
    return value;
}

static void chacha_rng_init(ChaChaRng *rng, size_t rounds, const uint32_t seed[8], uint64_t stream) {
    stream = u64_to_le(stream);

    rng->state[ 0] = u32_to_le(0x61707865);
    rng->state[ 1] = u32_to_le(0x3320646e);
    rng->state[ 2] = u32_to_le(0x79622d32);
    rng->state[ 3] = u32_to_le(0x6b206574);

    rng->state[ 4] = u32_to_le(seed[0]);
    rng->state[ 5] = u32_to_le(seed[1]);
    rng->state[ 6] = u32_to_le(seed[2]);
    rng->state[ 7] = u32_to_le(seed[3]);
    rng->state[ 8] = u32_to_le(seed[4]);
    rng->state[ 9] = u32_to_le(seed[5]);
    rng->state[10] = u32_to_le(seed[6]);
    rng->state[11] = u32_to_le(seed[7]);

    rng->state[12] = 0;
    rng->state[13] = 0;
    rng->state[14] = stream;
    rng->state[15] = stream >> 32;

    rng->rounds = rounds;

    rng->index = 0;
}

void chacha8_rng_init(ChaChaRng *rng, const uint32_t seed[8], uint64_t stream) {
    chacha_rng_init(rng, 8, seed, stream);
}

void chacha8_rng_init_zero(ChaChaRng *rng, uint64_t stream) {
    uint32_t seed[8] = { 0 };
    chacha_rng_init(rng, 8, seed, stream);
}

void chacha12_rng_init(ChaChaRng *rng, const uint32_t seed[8], uint64_t stream) {
    chacha_rng_init(rng, 12, seed, stream);
}

void chacha12_rng_init_zero(ChaChaRng *rng, uint64_t stream) {
    uint32_t seed[8] = { 0 };
    chacha_rng_init(rng, 12, seed, stream);
}

void chacha20_rng_init(ChaChaRng *rng, const uint32_t seed[8], uint64_t stream) {
    chacha_rng_init(rng, 20, seed, stream);
}

void chacha20_rng_init_zero(ChaChaRng *rng, uint64_t stream) {
    uint32_t seed[8] = { 0 };
    chacha_rng_init(rng, 20, seed, stream);
}

uint8_t chacha_rng_next_u8(ChaChaRng *rng) {
    return chacha_rng_next_u32(rng);
}

void chacha_rng_fill_u8(ChaChaRng *rng, uint8_t *array, size_t count) {
    size_t tail_count = count % 4;

    for (size_t i = 0; i < (count - tail_count); i += 4) {
        uint32_t word = chacha_rng_next_u32(rng);
        array[i + 0] = word;
        array[i + 1] = word >> 8;
        array[i + 2] = word >> 16;
        array[i + 3] = word >> 24;
    }

    if (tail_count > 0) {
        uint32_t word = chacha_rng_next_u32(rng);
        for (size_t i = tail_count; i > 0; i--) {
            array[count - i] = word;
            word >>= 8;
        }
    }
}

uint16_t chacha_rng_next_u16(ChaChaRng *rng) {
    return chacha_rng_next_u32(rng);
}

void chacha_rng_fill_u16(ChaChaRng *rng, uint16_t *array, size_t count) {
    size_t tail_count = count % 2;

    for (size_t i = 0; i < (count - tail_count); i += 2) {
        uint32_t word = chacha_rng_next_u32(rng);
        array[i + 0] = word;
        array[i + 1] = word >> 16;
    }

    if (tail_count > 0) {
        uint32_t word = chacha_rng_next_u32(rng);
        array[count - 1] = word;
    }
}

static void double_round(uint32_t state[16]);

static inline void increment_counter(ChaChaRng *rng) {
    rng->state[12] = u32_to_le(u32_from_le(rng->state[12]) + 1);
    if (rng->state[12] == 0) {
        rng->state[13] = u32_to_le(u32_from_le(rng->state[13]) + 1);
        assert(rng->state[13] != 0);
    }
}

uint32_t chacha_rng_next_u32(ChaChaRng *rng) {
    if (rng->index % 16 == 0) {
        for (size_t i = 0; i < 16; i++) {
            rng->working_state[i] = rng->state[i];
        }

        for (size_t i = 0; i < rng->rounds; i += 2) {
            double_round(rng->working_state);
        }

        for (size_t i = 0; i < 16; i++) {
            rng->working_state[i] += rng->state[i];
        }

        increment_counter(rng);
    }

    uint32_t result = rng->working_state[rng->index % 16];

    rng->index += 1;

    return u32_from_le(result);
}

void chacha_rng_fill_u32(ChaChaRng *rng, uint32_t *array, size_t count) {
    for (size_t i = 0; i < count; i++) {
        array[i] = chacha_rng_next_u32(rng);
    }
}

uint64_t chacha_rng_next_u64(ChaChaRng *rng) {
    uint64_t lo = chacha_rng_next_u32(rng);
    uint64_t hi = chacha_rng_next_u32(rng);
    return (hi << 32) | lo;
}

void chacha_rng_fill_u64(ChaChaRng *rng, uint64_t *array, size_t count) {
    for (size_t i = 0; i < count; i++) {
        array[i] = chacha_rng_next_u64(rng);
    }
}

float chacha_rng_next_f32(ChaChaRng *rng) {
    return (chacha_rng_next_u32(rng) >> 8) * 0x1p-24f;
}

void chacha_rng_fill_f32(ChaChaRng *rng, float *array, size_t count) {
    for (size_t i = 0; i < count; i++) {
        array[i] = chacha_rng_next_f32(rng);
    }
}

double chacha_rng_next_f64(ChaChaRng *rng) {
    return (chacha_rng_next_u64(rng) >> 11) * 0x1p-53;
}

void chacha_rng_fill_f64(ChaChaRng *rng, double *array, size_t count) {
    for (size_t i = 0; i < count; i++) {
        array[i] = chacha_rng_next_f64(rng);
    }
}

static inline uint32_t rotated_left(uint32_t value, uint32_t count) {
    return (value << count) | (value >> (32 - count));
}

#define QUARTER_ROUND(a, b, c, d) \
    state[a] += state[b]; state[d] = rotated_left(state[d] ^ state[a], 16); \
    state[c] += state[d]; state[b] = rotated_left(state[b] ^ state[c], 12); \
    state[a] += state[b]; state[d] = rotated_left(state[d] ^ state[a],  8); \
    state[c] += state[d]; state[b] = rotated_left(state[b] ^ state[c],  7);

static inline void double_round(uint32_t state[16]) {
    QUARTER_ROUND(0, 4,  8, 12)
    QUARTER_ROUND(1, 5,  9, 13)
    QUARTER_ROUND(2, 6, 10, 14)
    QUARTER_ROUND(3, 7, 11, 15)

    QUARTER_ROUND(0, 5, 10, 15)
    QUARTER_ROUND(1, 6, 11, 12)
    QUARTER_ROUND(2, 7,  8, 13)
    QUARTER_ROUND(3, 4,  9, 14)
}
