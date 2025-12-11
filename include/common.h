#pragma once

#include <stdint.h>
#include <stdbool.h>

// -------------------------------------------------------------
// | Customized Types |
// -------------------------------------------------------------
typedef float f32;
typedef double f64;
typedef void* (dispose_fn) (void*);
typedef void* (handler_fn) (void*);

// -------------------------------------------------------------
// | NULL macro |
// -------------------------------------------------------------
// Use `nil` as the universal null/zero value.
#define nil (0)

// -------------------------------------------------------------
// | Ownership Annotations |
// -------------------------------------------------------------
// These are semantic markers - useful for documentation, not functionality.
#define copied
#define borrowed
#define owned
#define clone(x) (x)

// -------------------------------------------------------------
// | pointer / Reference helpers |
// -------------------------------------------------------------
/**
 * deref(ptr, type):
 *      Dereferences a pointer with the expected type.
 */
#define deref(ptr, type)                                            \
        (*((type *)(ptr)))

/**
 * ref(var, ptype):
 *      Get the address of a variable (pass-by-reference helper).
 */
#define ref(var, ptype)                                             \
        ((ptype)&(var))

/**
 * cast(obj, type):
 *      Explicitly cast object to type.
 */
#define cast(obj, type)                                             \
        ((type) (obj))


// -------------------------------------------------------------
// | Comparison Helpers |
// -------------------------------------------------------------
#define eq(o1, o2)                                                  \
        ((o1) == (o2))

#define neq(o1, o2)                                                 \
        ((o1) != (o2))

// -------------------------------------------------------------
// | Arithmetic Helpers |
// -------------------------------------------------------------
/**
 * CEIL_DIV(n, d):
 *      1. Rounds up `n` to the nearest multiple of `d`.
 *      2. Useful for alignment or chunked memory blocks.
 *
 * CEIL_DIV(10, 3) == 4
 */
#define CEIL_DIV(n, d)                                           \
        (((n) + (d) - 1) / (d))

/**
 * FLOOR_DIV(n, d):
 *      1. Round `n` down to the nearest multiple of `d`.
 *
 * FLOOR_DIV(10, 3) == 9
 */
#define FLOOR_DIV(n, d)                                             \
        ((n) / (d) * (d))

/**
 * ROUND_DIV(n, d):
 *      Round `n` to the nearest multiple of `d`.
 *
 * ROUND_DIV(10,3) == 9
 * ROUND_DIV(11,3) == 12
 * ROUND_DIV(13,3) == 12
 */
#define ROUND_DIV(n, d)                                                 \
        ((((n) + (d) / 2) / (d)) * (d))


/**
 * WATERMARK(used, total):
 *      1. Calculates the usage ratio as a floating-point value.
 *      2. Returns a value in [0.0, 1.0] when `used <= total`.
 */
#define WATERMARK(used, total)                                      \
        ((total > 0) ? (f64)(used) / (f64)(total) : (f64) 1.0)

#define WATERMARK_HIGH  (.7)
#define WATERMARK_LOW   (.4)

#define SHR32(x,n) ((uint32_t)(x) >> ((uint32_t)(n) & 31u))
#define SHL32(x,n) ((uint32_t)(x) << ((uint32_t)(n) & 31u))

#define shr32(x,n) ((uint32_t)(x) >> ((uint32_t)(n) & 31u))
#define shl32(x,n) ((uint32_t)(x) << ((uint32_t)(n) & 31u))

#define SHR64(x,n) ((uint64_t)(x) >> ((uint64_t)(n) & 63u))
#define SHL64(x,n) ((uint64_t)(x) << ((uint64_t)(n) & 63u))

#define shr64(x,n) ((uint64_t)(x) >> ((uint64_t)(n) & 63u))
#define shl64(x,n) ((uint64_t)(x) << ((uint64_t)(n) & 63u))

#define ROR32(x,n) ( (uint32_t)( ((uint32_t)(x) >> ((uint32_t)(n) & 31u)) |            \
                             ((uint32_t)(x) << ((-((uint32_t)(n) & 31u)) & 31u)) ) )

#define ROL32(x,n) ( (uint32_t)( ((uint32_t)(x) << ((uint32_t)(n) & 31u)) |            \
                             ((uint32_t)(x) >> ((-((uint32_t)(n) & 31u)) & 31u)) ) )

#define ROR64(x,n) ( (uint64_t)(( (uint64_t)(x) >> ((uint64_t)(n) & 63u) ) |           \
                            ( (uint64_t)(x) << ((-( (uint64_t)(n) & 63u)) & 63u) )) )

#define ROL64(x,n) ( (uint64_t)(( (uint64_t)(x) << ((uint64_t)(n) & 63u) ) |           \
                            ( (uint64_t)(x) >> ((-( (uint64_t)(n) & 63u)) & 63u) )) )

#define ror32(x,n) ( (uint32_t)( ((uint32_t)(x) >> ((uint32_t)(n) & 31u)) |            \
                             ((uint32_t)(x) << ((-((uint32_t)(n) & 31u)) & 31u)) ) )

#define rol32(x,n) ( (uint32_t)( ((uint32_t)(x) << ((uint32_t)(n) & 31u)) |            \
                             ((uint32_t)(x) >> ((-((uint32_t)(n) & 31u)) & 31u)) ) )

#define ror64(x,n) ( (uint64_t)(( (uint64_t)(x) >> ((uint64_t)(n) & 63u) ) |           \
                            ( (uint64_t)(x) << ((-( (uint64_t)(n) & 63u)) & 63u) )) )

#define rol64(x,n) ( (uint64_t)(( (uint64_t)(x) << ((uint64_t)(n) & 63u) ) |           \
                            ( (uint64_t)(x) >> ((-( (uint64_t)(n) & 63u)) & 63u) )) )

// -------------------------------------------------------------
// | Special ASCII Characters |
// -------------------------------------------------------------
#define BLANK           " "
#define TAB             "\t"
#define CR              "\r"
#define LF              "\n"
#define CRLF            "\r\n"
#define WHITESPACES     CRLF TAB BLANK

#define SPACE           ' '
#define HORIZONTAL_TAB  '\t'
#define CARRIAGE_RETURN '\r'
#define LINE_FEED       '\n'
