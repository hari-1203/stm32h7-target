#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

/* Bare-metal / No OS */
#define SINGLE_THREADED
#define NO_FILESYSTEM
#define NO_MAIN_DRIVER
#define NO_WRITEV
#define WOLFCRYPT_ONLY
#define NO_TLS
#define NO_INLINE

/* STM32H7 specific */
// #define WOLFSSL_STM32H7
#define WOLFSSL_USER_IO

/* Small footprint for embedded */
#define WOLFSSL_SMALL_STACK
#define TFM_TIMING_RESISTANT
#define ECC_TIMING_RESISTANT

/* Disable what you don't need */
#define NO_MD4
#define NO_RC4
#define NO_DES
#define NO_DES3
#define NO_DSA
#define NO_RABBIT
#define NO_HC128
#define NO_PSK
#define NO_OLD_TLS

/* Enable what you need */
#define HAVE_AESGCM
#define WOLFSSL_SHA224
#define WOLFSSL_SHA384
#define WOLFSSL_SHA512

#define WOLFSSL_SHA3
#define WOLFSSL_SHAKE128
#define WOLFSSL_SHAKE256
/* ========== Post-Quantum Cryptography ========== */

#define WOLFSSL_HAVE_MLKEM
#define WOLFSSL_WC_MLKEM
#define WOLFSSL_MLKEM512

/* Enable PQC support flag */
#define HAVE_PQC

/* Required for PQC algorithms */
#define WC_NO_HARDEN

#endif /* USER_SETTINGS_H */
