#ifndef __STDIO_H__
#define __STDIO_H__

#include <io.h>
#include <stddef.h> /* for size_t */

#define NOFLOAT

#define ZEROPAD 1               // Pad with zero
#define SIGN    2               // Unsigned/signed long
#define PLUS    4               // Show plus
#define SPACE   8               // Space if plus
#define LEFT    16              // Left justified
#define SPECIAL 32              // 0x
#define LARGE   64              // Use 'ABCDEF' instead of 'abcdef'

#define LOG_BUF_LEN_MAX 256

typedef int            acpi_native_int;

#define   _AUPBND        (sizeof (acpi_native_int) - 1)
#define   _ADNBND        (sizeof (acpi_native_int) - 1)

#define _bnd(X, bnd)     (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)    (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)       (void) 0
#define va_start(ap, A)  (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))
#define is_digit(c) ((c) >= '0' && (c) <= '9')


extern unsigned int strlen(char *s);

#endif //__STDIO_H__
