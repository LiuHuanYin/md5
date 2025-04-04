/*
 *  RFC 1321 compliant MD5 implementation
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
/*
 *  The MD5 algorithm was designed by Ron Rivest in 1991.
 *
 *  http://www.ietf.org/rfc/rfc1321.txt
 */

/* 基于如下文件创建的:
    https://github.com/Mbed-TLS/mbedtls/blob/v3.6.3/library/md5.c
*/

#include "md5.h"
#include <string.h>

/** This is a bug in the library */
#define MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED -0x006E
void mbedtls_md5_init(mbedtls_md5_context *ctx) {
    memset(ctx, 0, sizeof(mbedtls_md5_context));
}

void mbedtls_md5_free(mbedtls_md5_context *ctx) {
    if (ctx == NULL) {
        return;
    }

    memset(ctx, 0, sizeof(mbedtls_md5_context));
}

void mbedtls_md5_clone(mbedtls_md5_context *dst,
        const mbedtls_md5_context *src) {
    *dst = *src;
}

/*
  * MD5 context setup
  */
int mbedtls_md5_starts(mbedtls_md5_context *ctx) {
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;

    return 0;
}

int mbedtls_internal_md5_process(mbedtls_md5_context *ctx,
        const unsigned char data[64]) {
    struct {
        uint32_t X[16], A, B, C, D;
    } local;

    memcpy(local.X, data, 64);

#define S(x, n) \
    (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (32 - (n))))

#define P(a, b, c, d, k, s, t)                        \
    do {                                              \
        (a) += F((b), (c), (d)) + local.X[(k)] + (t); \
        (a) = S((a), (s)) + (b);                      \
    } while (0)

    local.A = ctx->state[0];
    local.B = ctx->state[1];
    local.C = ctx->state[2];
    local.D = ctx->state[3];

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

    P(local.A, local.B, local.C, local.D, 0, 7, 0xD76AA478);
    P(local.D, local.A, local.B, local.C, 1, 12, 0xE8C7B756);
    P(local.C, local.D, local.A, local.B, 2, 17, 0x242070DB);
    P(local.B, local.C, local.D, local.A, 3, 22, 0xC1BDCEEE);
    P(local.A, local.B, local.C, local.D, 4, 7, 0xF57C0FAF);
    P(local.D, local.A, local.B, local.C, 5, 12, 0x4787C62A);
    P(local.C, local.D, local.A, local.B, 6, 17, 0xA8304613);
    P(local.B, local.C, local.D, local.A, 7, 22, 0xFD469501);
    P(local.A, local.B, local.C, local.D, 8, 7, 0x698098D8);
    P(local.D, local.A, local.B, local.C, 9, 12, 0x8B44F7AF);
    P(local.C, local.D, local.A, local.B, 10, 17, 0xFFFF5BB1);
    P(local.B, local.C, local.D, local.A, 11, 22, 0x895CD7BE);
    P(local.A, local.B, local.C, local.D, 12, 7, 0x6B901122);
    P(local.D, local.A, local.B, local.C, 13, 12, 0xFD987193);
    P(local.C, local.D, local.A, local.B, 14, 17, 0xA679438E);
    P(local.B, local.C, local.D, local.A, 15, 22, 0x49B40821);

#undef F

#define F(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))

    P(local.A, local.B, local.C, local.D, 1, 5, 0xF61E2562);
    P(local.D, local.A, local.B, local.C, 6, 9, 0xC040B340);
    P(local.C, local.D, local.A, local.B, 11, 14, 0x265E5A51);
    P(local.B, local.C, local.D, local.A, 0, 20, 0xE9B6C7AA);
    P(local.A, local.B, local.C, local.D, 5, 5, 0xD62F105D);
    P(local.D, local.A, local.B, local.C, 10, 9, 0x02441453);
    P(local.C, local.D, local.A, local.B, 15, 14, 0xD8A1E681);
    P(local.B, local.C, local.D, local.A, 4, 20, 0xE7D3FBC8);
    P(local.A, local.B, local.C, local.D, 9, 5, 0x21E1CDE6);
    P(local.D, local.A, local.B, local.C, 14, 9, 0xC33707D6);
    P(local.C, local.D, local.A, local.B, 3, 14, 0xF4D50D87);
    P(local.B, local.C, local.D, local.A, 8, 20, 0x455A14ED);
    P(local.A, local.B, local.C, local.D, 13, 5, 0xA9E3E905);
    P(local.D, local.A, local.B, local.C, 2, 9, 0xFCEFA3F8);
    P(local.C, local.D, local.A, local.B, 7, 14, 0x676F02D9);
    P(local.B, local.C, local.D, local.A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))

    P(local.A, local.B, local.C, local.D, 5, 4, 0xFFFA3942);
    P(local.D, local.A, local.B, local.C, 8, 11, 0x8771F681);
    P(local.C, local.D, local.A, local.B, 11, 16, 0x6D9D6122);
    P(local.B, local.C, local.D, local.A, 14, 23, 0xFDE5380C);
    P(local.A, local.B, local.C, local.D, 1, 4, 0xA4BEEA44);
    P(local.D, local.A, local.B, local.C, 4, 11, 0x4BDECFA9);
    P(local.C, local.D, local.A, local.B, 7, 16, 0xF6BB4B60);
    P(local.B, local.C, local.D, local.A, 10, 23, 0xBEBFBC70);
    P(local.A, local.B, local.C, local.D, 13, 4, 0x289B7EC6);
    P(local.D, local.A, local.B, local.C, 0, 11, 0xEAA127FA);
    P(local.C, local.D, local.A, local.B, 3, 16, 0xD4EF3085);
    P(local.B, local.C, local.D, local.A, 6, 23, 0x04881D05);
    P(local.A, local.B, local.C, local.D, 9, 4, 0xD9D4D039);
    P(local.D, local.A, local.B, local.C, 12, 11, 0xE6DB99E5);
    P(local.C, local.D, local.A, local.B, 15, 16, 0x1FA27CF8);
    P(local.B, local.C, local.D, local.A, 2, 23, 0xC4AC5665);

#undef F

#define F(x, y, z) ((y) ^ ((x) | ~(z)))

    P(local.A, local.B, local.C, local.D, 0, 6, 0xF4292244);
    P(local.D, local.A, local.B, local.C, 7, 10, 0x432AFF97);
    P(local.C, local.D, local.A, local.B, 14, 15, 0xAB9423A7);
    P(local.B, local.C, local.D, local.A, 5, 21, 0xFC93A039);
    P(local.A, local.B, local.C, local.D, 12, 6, 0x655B59C3);
    P(local.D, local.A, local.B, local.C, 3, 10, 0x8F0CCC92);
    P(local.C, local.D, local.A, local.B, 10, 15, 0xFFEFF47D);
    P(local.B, local.C, local.D, local.A, 1, 21, 0x85845DD1);
    P(local.A, local.B, local.C, local.D, 8, 6, 0x6FA87E4F);
    P(local.D, local.A, local.B, local.C, 15, 10, 0xFE2CE6E0);
    P(local.C, local.D, local.A, local.B, 6, 15, 0xA3014314);
    P(local.B, local.C, local.D, local.A, 13, 21, 0x4E0811A1);
    P(local.A, local.B, local.C, local.D, 4, 6, 0xF7537E82);
    P(local.D, local.A, local.B, local.C, 11, 10, 0xBD3AF235);
    P(local.C, local.D, local.A, local.B, 2, 15, 0x2AD7D2BB);
    P(local.B, local.C, local.D, local.A, 9, 21, 0xEB86D391);

#undef F

    ctx->state[0] += local.A;
    ctx->state[1] += local.B;
    ctx->state[2] += local.C;
    ctx->state[3] += local.D;

    /* Zeroise variables to clear sensitive data from memory. */
    memset(&local, 0, sizeof(local));

    return 0;
}

/*
  * MD5 process buffer
  */
int mbedtls_md5_update(mbedtls_md5_context *ctx,
        const unsigned char *input,
        size_t ilen) {
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t fill;
    uint32_t left;

    if (ilen == 0) {
        return 0;
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (uint32_t) ilen) {
        ctx->total[1]++;
    }

    if (left && ilen >= fill) {
        memcpy((void *) (ctx->buffer + left), input, fill);
        if ((ret = mbedtls_internal_md5_process(ctx, ctx->buffer)) != 0) {
            return ret;
        }

        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        if ((ret = mbedtls_internal_md5_process(ctx, input)) != 0) {
            return ret;
        }

        input += 64;
        ilen -= 64;
    }

    if (ilen > 0) {
        memcpy((void *) (ctx->buffer + left), input, ilen);
    }

    return 0;
}

/*
  * MD5 final digest
  */
int mbedtls_md5_finish(mbedtls_md5_context *ctx,
        unsigned char output[16]) {
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    uint32_t used;
    uint32_t high, low;

    /*
      * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
      */
    used = ctx->total[0] & 0x3F;

    ctx->buffer[used++] = 0x80;

    if (used <= 56) {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 56 - used);
    } else {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 64 - used);

        if ((ret = mbedtls_internal_md5_process(ctx, ctx->buffer)) != 0) {
            goto exit;
        }

        memset(ctx->buffer, 0, 56);
    }

    /*
      * Add message length
      */
    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low = (ctx->total[0] << 3);

    memcpy((void *) (ctx->buffer + 56), &low, 4);
    memcpy((void *) (ctx->buffer + 60), &high, 4);

    if ((ret = mbedtls_internal_md5_process(ctx, ctx->buffer)) != 0) {
        goto exit;
    }

    /*
      * Output final state
      */
    memcpy(output, ctx->state, 16);

    ret = 0;

exit:
    mbedtls_md5_free(ctx);
    return ret;
}

/*
  * output = MD5( input buffer )
  */
int mbedtls_md5(const unsigned char *input,
        size_t ilen,
        unsigned char output[16]) {
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_md5_context ctx;

    mbedtls_md5_init(&ctx);

    if ((ret = mbedtls_md5_starts(&ctx)) != 0) {
        goto exit;
    }

    if ((ret = mbedtls_md5_update(&ctx, input, ilen)) != 0) {
        goto exit;
    }

    if ((ret = mbedtls_md5_finish(&ctx, output)) != 0) {
        goto exit;
    }

exit:
    mbedtls_md5_free(&ctx);

    return ret;
}
