#ifndef chacha_rng_h
#define chacha_rng_h

#include <stdint.h>

typedef struct {
    uint32_t state[16];
    uint32_t working_state[16];
    size_t rounds;
    size_t index;
} ChaChaRng;

void chacha8_rng_init(ChaChaRng *rng, const uint32_t seed[8], uint64_t stream);

void chacha8_rng_init_zero(ChaChaRng *rng, uint64_t stream);

void chacha12_rng_init(ChaChaRng *rng, const uint32_t seed[8], uint64_t stream);

void chacha12_rng_init_zero(ChaChaRng *rng, uint64_t stream);

void chacha20_rng_init(ChaChaRng *rng, const uint32_t seed[8], uint64_t stream);

void chacha20_rng_init_zero(ChaChaRng *rng, uint64_t stream);

uint8_t chacha_rng_next_u8(ChaChaRng *rng);

uint16_t chacha_rng_next_u16(ChaChaRng *rng);

uint32_t chacha_rng_next_u32(ChaChaRng *rng);

uint64_t chacha_rng_next_u64(ChaChaRng *rng);

void chacha_rng_fill_u8(ChaChaRng *rng, uint8_t *array, size_t count);

void chacha_rng_fill_u16(ChaChaRng *rng, uint16_t *array, size_t count);

void chacha_rng_fill_u32(ChaChaRng *rng, uint32_t *array, size_t count);

void chacha_rng_fill_u64(ChaChaRng *rng, uint64_t *array, size_t count);

#endif /* chacha_rng_h */
