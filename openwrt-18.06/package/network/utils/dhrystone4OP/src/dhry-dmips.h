#define TIME 	2
#define REG 	register

#define g_print(fmt,arg...) printf(fmt,##arg)

#ifndef TIME
#define TIMES
#endif

#ifdef TIMES
#include <sys/types.h>
#include <sys/times.h>
#endif

#define Mic_secs_Per_Second   1000000.0

#ifdef NOSTRUCTASSIGN
#define structassign(d, s) 	memcpy(&(d), &(s), sizeof(d))
#else
#define structassign(d, s) 	d = s
#endif

#ifdef NOENUM
#define Ident_1 	0
#define Ident_2 	1
#define Ident_3 	2
#define Ident_4 	3
#define Ident_5 	4
  typedef int Enumeration;
#else
  typedef enum {Ident_1, Ident_2, Ident_3, Ident_4, Ident_5} 
		  Enumeration;
#endif

#include <stdio.h>
#include <string.h>
  
#define Null 	0

#define true 	1
#define false 	0
  
#define TEST_FORK_NUMBER 	1
  
typedef int 	One_Thirty;
typedef int 	One_Fifty;
typedef char 	Capital_Letter;
typedef int 	Boolean;
typedef char 	Str_30[31];
typedef int 	Arr_1_Dim[50];
typedef int 	Arr_2_Dim[50][50];

typedef struct record
{
    struct record 	*Ptr_Comp;
    Enumeration 	Discr;
    union{
      struct{
	  Enumeration 	Enum_Comp;
	  int 		Int_Comp;
	  char 		Str_Comp[31];
      }var_1;
      struct{
	  Enumeration 	E_Comp_2;
	  char 		Str_2_Comp[31];
      }var_2;
      struct{
	  char 		Ch_1_Comp;
	  char 		Ch_2_Comp;
      }var_3;
    }variant;
}Rec_Type, *Rec_Pointer;
extern int firetest;
extern void Proc_1();
extern void Proc_2();
extern void Proc_3();
extern void Proc_4();
extern void Proc_5();
extern void Proc_6();
extern void Proc_7();
extern void Proc_8();
extern float fork_pro(int n, int cpuNum);
extern void store_dmips(int pid, float dmipsval);
extern float read_dmips(int pid);

extern Enumeration Func_1();
extern Boolean Func_2(Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);
extern Boolean Func_3(Enumeration Enum_Par_Val);

