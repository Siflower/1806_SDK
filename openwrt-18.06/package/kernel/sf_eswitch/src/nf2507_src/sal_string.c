#include "sal_string.h"

l2sw_int8 *sal_strcat(l2sw_int8 *dst, const l2sw_int8 *src)
{
    l2sw_int8 *s_dst = dst;

    if((NULL == dst) ||(NULL == src))
    {
        return NULL;
    }
    
    while (*s_dst != 0) 
    {
        s_dst++;
    }
        
    while ((*s_dst++ = *src++) != 0)
    {
        ;
    }

    return dst;
}


l2sw_int32 sal_strlen(const l2sw_int8 *s)
{
    const l2sw_int8 *s_orig = s;

    if(NULL == s)
    {
        return -1;
    }

    while (*s != 0) 
	{
		s++;
    }

    return (s - s_orig);
}

l2sw_int8 *sal_strcpy(l2sw_int8 *dst, const l2sw_int8 *src)
{
    l2sw_int8 *dst_orig = dst;

    if((NULL == dst) ||(NULL == src))
    {
        return NULL;
    }

    while ((*dst++ = *src++) != 0)
    {
		;
    }
	
    return dst_orig;
}

l2sw_int8 *sal_strncpy(l2sw_int8 *dst, const l2sw_int8 *src, l2sw_uint32 length)
{
	l2sw_int32 i = 0;
  	l2sw_int8 *dst_orig = dst;

    if((NULL == dst) ||(NULL == src))
    {
        return NULL;
    }

    while (((*dst++ = *src++) != 0) && (i <= length)) 
  	{
    	++i;
  	}
    return dst_orig;
}


l2sw_int32 sal_strcmp(const l2sw_int8 *s1, const l2sw_int8 *s2)
{
    if((NULL == s1) ||(NULL == s2))
    {
        return NULL;
    }
        
    do 
	{
		if (*s1 < *s2) 
		{
	    	return -1;
		} 
		else if (*s1 > *s2) 
		{
	    	return 1;
		}
		s1++;
    } while (*s2++);

    return 0;
}

void *sal_memcpy(void *dst_void, const void *src_void, l2sw_uint32 len)
{
    l2sw_uint8 *dst = dst_void;
    const l2sw_uint8 *src = src_void;

    if((NULL == dst) ||(NULL == src))
    {
        return NULL;
    }

    while (len--) 
	{
		*dst++ = *src++;
    }

    return dst_void;
}

void *sal_memset(void *dst_void, l2sw_int32 val, l2sw_uint32 len)
{
    l2sw_uint8 *dst = dst_void;

    if(NULL == dst)
    {
        return NULL;
    }

    while (len--) 
	{
		*dst++ = (unsigned char) val;
    }

    return dst_void;
}

l2sw_int32 sal_memcmp(const void *m1_void, const void *m2_void, l2sw_uint32 len)
{
    const l2sw_uint8 *m1 = m1_void;
	const l2sw_uint8 *m2 = m2_void;

    if((NULL == m1_void) ||(NULL == m2_void))
    {
        return NULL;
    }

    while (len-- != 0) 
	{
		if (*m1 < *m2) 
		{
	    	return -1;
		} 
		else if (*m1 > *m2) 
		{
	    	return 1;
		}
		m1++;
		m2++;
    } 

    return 0;
}


