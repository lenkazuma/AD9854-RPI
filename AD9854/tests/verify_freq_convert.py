#!/usr/bin/env python3
"""Host-side regression checks for AD9854 frequency tuning word math."""

from __future__ import annotations

CLK_SET = 12
REFCLK_HZ = 20_000_000
FREQ_MULT_ULONG = 1_172_812
FREQ_MULT_DOUBLE = 1_172_812.402961067
DEMO_FREQ_HZ = 70_000_000
HIGH_FREQ_HZ = 250_000_000
MASK32 = 0xFFFFFFFF


def fixed_word(freq_hz: int) -> int:
    return freq_hz * FREQ_MULT_ULONG


def legacy_broken_word(freq_hz: int) -> int:
    """Original 32-bit byte-wise multiply (overflows above ~200 MHz)."""
    temp = FREQ_MULT_ULONG
    parts = [
        freq_hz & 0xFF,
        (freq_hz >> 8) & 0xFF,
        (freq_hz >> 16) & 0xFF,
        (freq_hz >> 24) & 0xFF,
    ]
    freq_buf = (temp * parts[0]) & MASK32
    words = [0] * 6
    words[0] = freq_buf & 0xFF
    freq_buf = (freq_buf >> 8) & MASK32

    freq_buf = (freq_buf + temp * parts[1]) & MASK32
    words[1] = freq_buf & 0xFF
    freq_buf = (freq_buf >> 8) & MASK32

    freq_buf = (freq_buf + temp * parts[2]) & MASK32
    words[2] = freq_buf & 0xFF
    freq_buf = (freq_buf >> 8) & MASK32

    freq_buf = (freq_buf + temp * parts[3]) & MASK32
    words[3] = freq_buf & 0xFF
    freq_buf = (freq_buf >> 8) & MASK32
    words[4] = freq_buf & 0xFF
    words[5] = (freq_buf >> 8) & 0xFF
    return sum(words[i] << (8 * i) for i in range(6))


def legacy_double_split(freq_hz: float) -> int:
    """Original FreqDoubleConvert used 2^32-1 instead of 2^32."""
    scaled = freq_hz * FREQ_MULT_DOUBLE
    high16 = int(scaled / 4_294_967_295)
    low32 = int(scaled - high16 * 4_294_967_295)
    return (high16 << 32) | (low32 & MASK32)


def fixed_double_word(freq_hz: float) -> int:
    return int(freq_hz * FREQ_MULT_DOUBLE)


def main() -> int:
    demo = fixed_word(DEMO_FREQ_HZ)
    if demo != legacy_broken_word(DEMO_FREQ_HZ):
        print("WARN: demo frequency differs from legacy (truncation only)")

    high = fixed_word(HIGH_FREQ_HZ)
    broken_high = legacy_broken_word(HIGH_FREQ_HZ)
    if high == broken_high:
        print("FAIL: legacy algorithm should overflow at high frequency")
        print(f"  freq: {HIGH_FREQ_HZ}")
        print(f"  fixed:  0x{high:x}")
        print(f"  legacy: 0x{broken_high:x}")
        return 1

    double_fixed = fixed_double_word(DEMO_FREQ_HZ)
    double_broken = legacy_double_split(DEMO_FREQ_HZ)
    if double_fixed == double_broken:
        print("FAIL: double-precision split should use 2^32 divisor")
        return 1

    print(f"OK: {DEMO_FREQ_HZ} Hz -> 0x{demo:x}")
    print(f"OK: {HIGH_FREQ_HZ} Hz legacy overflow detected (0x{broken_high:x} vs 0x{high:x})")
    print(f"OK: double path divisor fix (delta {double_fixed - double_broken} LSB)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
