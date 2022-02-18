#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* for size_t */

#define portBYTE_ALIGNMENT			8


#if portBYTE_ALIGNMENT == 32
	#define portBYTE_ALIGNMENT_MASK ( 0x001f )
#endif

#if portBYTE_ALIGNMENT == 16
	#define portBYTE_ALIGNMENT_MASK ( 0x000f )
#endif

#if portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#if portBYTE_ALIGNMENT == 4
	#define portBYTE_ALIGNMENT_MASK	( 0x0003 )
#endif

#if portBYTE_ALIGNMENT == 2
	#define portBYTE_ALIGNMENT_MASK	( 0x0001 )
#endif

#if portBYTE_ALIGNMENT == 1
	#define portBYTE_ALIGNMENT_MASK	( 0x0000 )
#endif

#ifndef portBYTE_ALIGNMENT_MASK
	#error "Invalid portBYTE_ALIGNMENT definition"
#endif

typedef long BaseType_t;


/* Used by heap_5.c. */
typedef struct HeapRegion
{
    u8 *pucStartAddress;
    size_t xSizeInBytes;
} HeapRegion_t;


#define configASSERT( x ) if( ( x ) == 0  ) ;//vAssertCalled( __FILE__, __LINE__ )

#ifndef mtCOVERAGE_TEST_MARKER
	#define mtCOVERAGE_TEST_MARKER()
#endif

#ifndef traceMALLOC
    #define traceMALLOC( pvAddress, uiSize )
#endif

#ifndef traceFREE
    #define traceFREE( pvAddress, uiSize )
#endif


#define configTOTAL_HEAP_SIZE					( ( size_t ) (32 * 1024) )

extern void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions );
extern void * malloc( size_t xWantedSize );
extern void free( void *pv );

#endif //__HEAP_H__
