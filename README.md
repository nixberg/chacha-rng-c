# ChaCha

Chacha implements a [ChaCha](https://cr.yp.to/chacha/chacha-20080128.pdf)-based
cryptographically secure pseudo-random number generator intended to be
compatible with Rust crate [rand_chacha](https://crates.io/crates/rand_chacha).

# Usage

```C
#include "chacha.h"

ChaChaRng rng;
chacha8_zero(&rng, 0); // ChaCha8, zero seed, stream: 0.

chacha_u64(&rng); // 0xd6405f892fef003e
```

```C
uint32_t seed[8] = { 0 };
chacha20_init(&rng, seed, 0); // ChaCha20, zero seed, stream: 0.

uint8_t array[4] = { 0 };
chacha_fill_u8(&rng, array, 4); // { 0x76, 0xb8, 0xe0, 0xad }
```
