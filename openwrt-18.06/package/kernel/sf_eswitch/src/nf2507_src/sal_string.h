/** @file
  * @brief 定义SAL字符串操作接口
  * @author
  * @copyright
  * @date 2017/07/17
  * @version 0.1
  * @note
  * 定义SAL字符串操作接口，包括字符串长度/字符串拷贝/字符串比较等
  * $LastChangedDate$
  * $LastChangedRevision$
  * $LastChangedBy$
  */


#ifndef __SAL_STRING_H__
#define __SAL_STRING_H__

#include "l2sw_types.h"

l2sw_int8 *sal_strcat(l2sw_int8 *dst, const l2sw_int8 *src);
l2sw_int32 sal_strlen(const l2sw_int8 *s);
l2sw_int8 *sal_strcpy(l2sw_int8 *dst, const l2sw_int8 *src);
l2sw_int8 *sal_strncpy(l2sw_int8 *dst, const l2sw_int8 *src, l2sw_uint32 length);
l2sw_int32 sal_strcmp(const l2sw_int8 *s1, const l2sw_int8 *s2);
void *sal_memcpy(void *dst_void, const void *src_void, l2sw_uint32 len);
void *sal_memset(void *dst_void, l2sw_int32 val, l2sw_uint32 len);



l2sw_int32 sal_memcmp(const void *m1_void, const void *m2_void, l2sw_uint32 len);
l2sw_int8 *sal_strstr(const l2sw_int8 *s1, const l2sw_int8 *s2);
l2sw_int32 sal_ctoi(const l2sw_int8 *s, l2sw_int8 **end);

//#define MEMSET(dst_void, val,len) sal_memset(dst_void, val,len)
#define MEMSET(dst_void, val,len) memset(dst_void, val,len)
//#define MEMCPY(dst_void, src_void, len) sal_memcpy(dst_void, src_void, len)
#define MEMCPY(dst_void, src_void, len) memcpy(dst_void, src_void, len)

#endif /*__SAL_STRING_H__*/
