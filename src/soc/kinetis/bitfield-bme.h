static inline int
bf_mask_to_width(uint32_t mask)
{
        int width = 0;

        mask += 1;
        while (mask) {
                width++;
                mask >>= 1;
        }
        return (width);
}

#define bf_set(loc, name, val)                                          \
        do {                                                            \
        	uint32_t _bitpos = name ## _SHIFT;                      \
                uint32_t _bitmask = name ## _MASK;                      \
                                                                        \
                if (__builtin_constant_p(val) &&                        \
                    bf_single_bit_p(_bitpos, _bitmask) &&               \
                    bme_op_possible(loc)) {                             \
                        if (val)                                        \
                                bme_setbit(loc, _bitpos);               \
                        else                                            \
                                bme_clearbit(loc, _bitpos);             \
                } else if (__builtin_constant_p(val) &&                 \
                           bme_bf_possible(loc)) {                      \
                        bme_bf_set(loc, _bitpos, bf_mask_to_width(_bitmask >> _bitpos), val); \
                } else {                                                \
                        typeof(&loc) locp = &loc;                       \
                        uint32_t _locval = *locp;                       \
                        *locp = bf_set1(_locval, _bitpos, _bitmask, val); \
                }                                                       \
        } while (0)

static inline int
bf_single_bit_p(int bitpos, uint32_t bitmask)
{
        return (__builtin_constant_p(bitmask) &&
                __builtin_constant_p(bitpos) &&
                bitmask == (1 << bitpos));
}

static inline uint32_t
bf_set1(uint32_t locval, int bitpos, uint32_t bitmask, uint32_t val)
{
        if (bf_single_bit_p(bitpos, bitmask)) {
                /* single bit op */
                if (val)
                        return (locval | bitmask);
                else
                        return (locval & ~bitmask);
        }
        return ((locval & ~bitmask) | (val << bitpos));
}


#define bf_get(loc, name)                                               \
        ({                                                              \
        	uint32_t _bitpos = name ## _SHIFT;                      \
                uint32_t _bitmask = name ## _MASK;                      \
                uint32_t _res;                                          \
                                                                        \
                if (bme_bf_possible(loc))                               \
                        _res = bme_bf_get(loc, _bitpos, bf_mask_to_width(_bitmask >> _bitpos)); \
                else                                                    \
                        _res = (((loc) & (name ## _MASK)) >> (name ## _SHIFT)); \
                _res;                                                   \
        })


#define bf_or(loc, val)                         \
        do {                                    \
                if (bme_op_possible(loc))       \
                        bme_or(loc, val);       \
                else                            \
                        loc |= val;             \
        } while (0)

#define bf_and(loc, val)                        \
        do {                                    \
                if (bme_op_possible(loc))       \
                        bme_and(loc, val);      \
                else                            \
                        loc &= val;             \
        } while (0)

#define bf_xor(loc, val)                        \
        do {                                    \
                if (bme_op_possible(loc))       \
                        bme_xor(loc, val);      \
                else                            \
                        loc ^= val;             \
        } while (0)
