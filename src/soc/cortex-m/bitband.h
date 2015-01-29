static inline uintptr_t
bitband_addr(uintptr_t addr, uint32_t bitpos)
{
        uintptr_t offset = addr & 0xfffff;
        return (addr | 0x02000000 | (offset * 32) | (bitpos * 4));
}

static inline void
bitband_set(uintptr_t addr, uint32_t bitpos, uint32_t value)
{
        volatile uint32_t *p = (void *)bitband_addr(addr, bitpos);
        *p = value;
}

static inline uint32_t
bitband_get(uintptr_t addr, uint32_t bitpos)
{
        volatile uint32_t *p = (void *)bitband_addr(addr, bitpos);
        return (*p);
}
