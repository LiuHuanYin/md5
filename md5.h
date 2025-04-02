/**
 * \file md5.h
 *
 * \brief MD5 message digest algorithm (hash function)
 *
 * \warning   MD5 is considered a weak message digest and its use constitutes a
 *            security risk. We recommend considering stronger message
 *            digests instead.
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

/* 基于如下文件创建的:
    https://github.com/Mbed-TLS/mbedtls/blob/v3.6.3/include/mbedtls/md5.h
 */
#ifndef MBEDTLS_MD5_H
#define MBEDTLS_MD5_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Regular implementation
//

/**
 * \brief          MD5 context structure
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
typedef struct mbedtls_md5_context {
    uint32_t(total)[2]; /*!< number of bytes processed  */
    uint32_t(state)[4]; /*!< intermediate digest state  */
    unsigned char(buffer)[64]; /*!< data block being processed */
} mbedtls_md5_context;


/**
 * \brief          Initialize MD5 context
 *
 * \param ctx      MD5 context to be initialized
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md5_init(mbedtls_md5_context *ctx);

/**
 * \brief          Clear MD5 context
 *
 * \param ctx      MD5 context to be cleared
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md5_free(mbedtls_md5_context *ctx);

/**
 * \brief          Clone (the state of) an MD5 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_md5_clone(mbedtls_md5_context *dst,
        const mbedtls_md5_context *src);

/**
 * \brief          MD5 context setup
 *
 * \param ctx      context to be initialized
 *
 * \return         0 if successful
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_md5_starts(mbedtls_md5_context *ctx);

/**
 * \brief          MD5 process buffer
 *
 * \param ctx      MD5 context
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 *
 * \return         0 if successful
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_md5_update(mbedtls_md5_context *ctx,
        const unsigned char *input,
        size_t ilen);

/**
 * \brief          MD5 final digest
 *
 * \param ctx      MD5 context
 * \param output   MD5 checksum result
 *
 * \return         0 if successful
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_md5_finish(mbedtls_md5_context *ctx,
        unsigned char output[16]);

/**
 * \brief          MD5 process data block (internal use only)
 *
 * \param ctx      MD5 context
 * \param data     buffer holding one block of data
 *
 * \return         0 if successful
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_internal_md5_process(mbedtls_md5_context *ctx,
        const unsigned char data[64]);

/**
 * \brief          Output = MD5( input buffer )
 *
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 * \param output   MD5 checksum result
 *
 * \return         0 if successful
 *
 * \warning        MD5 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_md5(const unsigned char *input,
        size_t ilen,
        unsigned char output[16]);


#ifdef __cplusplus
}
#endif

#endif /* mbedtls_md5.h */
