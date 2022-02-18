#if defined(UART) || defined(UART_LOG)
#include <uart.h>
#endif

#ifdef CLIB
#include <stdio.h>
#include <stdarg.h>
#endif

struct stat {
    unsigned int    st_dev;
    unsigned int    st_ino;
    unsigned int    st_mode;
    unsigned int    st_nlink;
    unsigned int    st_uid;
    unsigned int    st_gid;
    unsigned int    st_rdev;
    long        st_size;
    unsigned long   st_atime;
    unsigned long   st_mtime;
    unsigned long   st_ctime;
    unsigned int    st_blksize;
    unsigned int    st_blocks;
    unsigned int    st_flags;
    unsigned int    st_gen;
};

/*******************************************************************
*	clib stubs funtion _open_r           	                       *
*	reentrant version of open			                           *
********************************************************************/
int _open_r(void *reent,const char *file, int flags, int mode)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _close_r           	                       *
*	reentrant version of close			                           *
********************************************************************/
int _close_r(void *reent, int fd)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _lseek_r           	                       *
*	reentrant version of lseek			                           *
********************************************************************/
off_t _lseek_r(void *reent,int fd, off_t pos, int whence)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _read_r           	                       *
*	reentrant version of read,we redirect to uart0 for input msg   *
********************************************************************/
long _read_r(void *reent,int fd, void *buf, size_t cnt)
{
	#if defined(UART) || defined(UART_LOG)
	uart_rx_debug_msg((char *)buf,(unsigned int)cnt);
	return cnt;
	#else
	return -1;
	#endif
}

/*******************************************************************
*	clib stubs funtion _write_r           	                       *
*	reentrant version of write,we redirect to uart0 for output msg *
********************************************************************/
long _write_r(void *reent,int fd, const void *buf, size_t cnt)
{
	#if defined(UART) || defined(UART_LOG)
	uart_tx_debug_msg((char *)buf,(unsigned int)cnt);
	return cnt;
	#else
	return -1;
	#endif
}

/*******************************************************************
*	clib stubs funtion _fork_r           	                       *
*	reentrant version of fork			                           *
********************************************************************/
int _fork_r(void *reent)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _wait_r           	                       *
*	reentrant version of wait			                           *
********************************************************************/
int _wait_r(void *reent, int *status)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _fstat_r           	                       *
*	reentrant version of fstat			                           *
********************************************************************/
int _fstat_r(void *reent, int fd, struct stat *pstat)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _link_r           	                       *
*	reentrant version of link			                           *
********************************************************************/
int _link_r(void *reent,const char *old, const char *new)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion isatty           	                       *
*	reentrant version of isatty			                           *
********************************************************************/
int isatty(int desc)
{
    return 1;
}


/*******************************************************************
*	clib stubs funtion _unlink_r           	                       *
*	reentrant version of unlink			                           *
********************************************************************/
int _unlink_r(void *reent, const char *file)
{
	return -1;
}

/*******************************************************************
*	clib stubs funtion _sbrk_r           	                       *
*	reentrant version of sbrk,use for malloc functio               *
********************************************************************/
char *heap_end = 0;
#define HEAD_SIZE 0x20000
extern unsigned long _heap_start;

char *_sbrk_r(void *reent, size_t incr)
{
    static char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = (caddr_t)&_heap_start;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > (caddr_t)(&_heap_start + HEAD_SIZE)) {
        return (caddr_t)0;
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
}

