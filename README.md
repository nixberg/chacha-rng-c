# ChaChaRng

[ChaCha](https://cr.yp.to/chacha/chacha-20080128.pdf)-based RNG for C.
[Compatible](https://github.com/nixberg/chacha-rng-compability-rs) with Rust’s [rand_chacha](https://crates.io/crates/rand_chacha).

Does not support big-endian platforms.

# Usage

```C
#include "chacha-rng.h"

ChaChaRng rng;
chacha8_rng_init_zero(&rng, 0); // ChaCha8, zero seed, stream: 0.

chacha_rng_next_u64(&rng); // 0xd6405f892fef003e
```

```C
uint32_t seed[8] = { 0 };
chacha20_rng_init(&rng, seed, 0); // ChaCha20, zero seed, stream: 0.

uint8_t array[4] = { 0 };
chacha_rng_fill_u8(&rng, array, 4); // { 0x76, 0xb8, 0xe0, 0xad }
```
