#include "dhry-dmips.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
//#include <gio/gio.h>

Rec_Pointer  	Ptr_Glob, Next_Ptr_Glob;
int 		Int_Glob;
Boolean 	Bool_Glob;
char 		Ch_1_Glob, Ch_2_Glob;
int 		Arr_1_Glob[50];
int		Arr_2_Glob[50][50];

#ifndef REG
Boolean Reg = false;
#define REG
#else
Boolean Reg = true;
#endif

#ifdef TIMES
struct tms time_info;
//extern int times();
extern clock_t times (struct tms *__buffer) ;
#define Too_Small_Time 	120
#endif

#ifdef TIME
extern long time();
#ifndef Too_Small_Time	
#define Too_Small_Time 	2
#endif
#endif

double 	Begin_Time, End_Time, User_Time;
float 	Microseconds, Dhrystones_Per_Second;


float fork_pro(int n, int cpuNum)
{
		struct timeval begin_tim,end_tim;
		struct timezone tz;
		One_Fifty	Int_1_Loc;
		REG One_Fifty	Int_2_Loc;
		One_Fifty	Int_3_Loc;
		REG char 		Ch_Index;
		Enumeration 	Enum_Loc;
		Str_30 		Str_1_Loc;
		Str_30 		Str_2_Loc;
		REG int 		Run_Index;
		REG int 		Number_Of_Runs;

		Next_Ptr_Glob = (Rec_Pointer)malloc(sizeof(Rec_Type));
		Ptr_Glob = (Rec_Pointer)malloc(sizeof(Rec_Type));

		Ptr_Glob->Ptr_Comp 			= Next_Ptr_Glob;
		Ptr_Glob->Discr			= Ident_1;
		Ptr_Glob->variant.var_1.Enum_Comp 	= Ident_3;
		Ptr_Glob->variant.var_1.Int_Comp 	= 40;

		strcpy(Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING");
		strcpy(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

		Arr_2_Glob[8][7] = 10;

		Number_Of_Runs = n * 1000000;
		if(firetest){
			g_print("Excution starts, firetest nerver end!!(cpuNum=%d,pid=%d)\n", cpuNum,(int)getpid());
		}else{
			g_print("Excution starts, %d runs through Dhrystone(cpuNum=%d,pid=%d)\n", Number_Of_Runs, cpuNum,(int)getpid());
		}
		gettimeofday(&begin_tim, &tz);

		for(Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
		{
				Proc_5();
				Proc_4();
				Int_1_Loc = 2;
				Int_2_Loc = 3;
				strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
				Enum_Loc = Ident_2;
				Bool_Glob = !Func_2(Str_1_Loc, Str_2_Loc);

				while(Int_1_Loc < Int_2_Loc)
				{
						Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
						Proc_7(Int_1_Loc, Int_2_Loc, &Int_3_Loc);
						Int_1_Loc += 1;
				}

				Proc_8(Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
				Proc_1(Ptr_Glob);

				for(Ch_Index='A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
				{
						if(Enum_Loc == Func_1(Ch_Index, 'C'))
						{
								Proc_6(Ident_1, &Enum_Loc);
								strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");

								Int_2_Loc = Run_Index;
								Int_Glob = Run_Index;
						}
				}

				Int_2_Loc = Int_2_Loc * Int_1_Loc;
				Int_1_Loc = Int_2_Loc / Int_3_Loc;
				Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;

				Proc_2(&Int_1_Loc);
				if(firetest == 1){
					Run_Index--;
				}
		}

		gettimeofday(&end_tim, &tz);
		g_print("Execute ends(cpuNum = %d,pid=%d)\n",cpuNum,(int)getpid());

		Begin_Time = begin_tim.tv_sec * Mic_secs_Per_Second + begin_tim.tv_usec;
		End_Time = end_tim.tv_sec * Mic_secs_Per_Second + end_tim.tv_usec;

		User_Time = End_Time - Begin_Time;
		if(User_Time < Too_Small_Time)
		{
				Microseconds = 0.0f;
				g_print( "Measured time too small to obtain meaningful results(cpuNum = %d)\n", cpuNum);
				g_print("Please increase number of runs\n");
				g_print("\n");
		}
		else
		{
				Dhrystones_Per_Second = (float)Number_Of_Runs * (float)Mic_secs_Per_Second / (float)User_Time;
				Microseconds = Dhrystones_Per_Second / 1757.0;
				//g_print("*** Dhrystones Per Second:      ");
				//g_print("%6.1f (cpuNum = %d)\n", Dhrystones_Per_Second, cpuNum);
				//g_print( "Dhrystones Per Second: %6.1f(cpuNum=%d)\n", Dhrystones_Per_Second, cpuNum);
				//g_print("*** DMIPS:      ");
				//g_print("%6.2f (cpuNum=%d)\n", Microseconds, cpuNum);
				//g_print( "DMIPS: %6.2f (cpuNum = %d)\n", Microseconds, cpuNum);
		}

		g_print("==========================================\n");
		g_print("\n");

		return Microseconds;

}

void store_dmips(int pid, float dmipsval)
{
		char filename[32] = {0};
		char dmipsstr[16] = {0};
		FILE *fp = NULL;

		sprintf(filename, "/tmp/dmips_proc_%d.tmp", pid);
		fp = fopen(filename, "w+");
		if(NULL == fp)
		{
				g_print("Failed to open file %s\n", filename);
				return;
		}

		sprintf(dmipsstr, "%.2f", dmipsval);
		fputs(dmipsstr,fp);
		fclose(fp);
		g_print("save dmips %s,pid:%d\n",dmipsstr,pid);
		return;
}

float read_dmips(int pid)
{
		char filename[32] = {0};
		float value = 0.0f;
		FILE *fp = NULL;

		sprintf(filename, "/tmp/dmips_proc_%d.tmp", pid);
		fp = fopen(filename, "r");
		if(NULL == fp)
		{
				g_print("Failed to open file %s\n", filename);
				return value;
		}

		if(fscanf(fp, "%f", &value) != 1)
		{
				g_print("Failed to read dmips value from %s\n", filename);
		}

		fclose(fp);

		if(remove(filename) != 0)
		{
				g_print("Failed to remove tmp file %s\n", filename);
		}
		g_print("read dmips:%.2f,pid:%d\n",value,pid); 
		return value;
}


void Proc_1(Ptr_Val_Par)
		register Rec_Pointer Ptr_Val_Par;
{
		REG Rec_Pointer Next_Recorder = Ptr_Val_Par->Ptr_Comp;
		structassign(*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
		Ptr_Val_Par->variant.var_1.Int_Comp = 5;
		Next_Recorder->variant.var_1.Int_Comp = Ptr_Val_Par->variant.var_1.Int_Comp;
		Next_Recorder->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;

		Proc_3(&Next_Recorder->Ptr_Comp);
		if(Next_Recorder->Discr == Ident_1)
		{
				Next_Recorder->variant.var_1.Int_Comp = 6;
				Proc_6(Ptr_Val_Par->variant.var_1.Enum_Comp, &Next_Recorder->variant.var_1.Enum_Comp);
				Next_Recorder->Ptr_Comp = Ptr_Glob->Ptr_Comp;
				Proc_7(Next_Recorder->variant.var_1.Int_Comp, 10, &Next_Recorder->variant.var_1.Int_Comp);
		}
		else
		{
				structassign(*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
		}

}


void Proc_2(Int_Par_Ref)
		One_Fifty *Int_Par_Ref;
{
		One_Fifty Int_Loc;
		Enumeration Enum_Loc;
		Int_Loc = *Int_Par_Ref + 10;
		do
				if(Ch_1_Glob == 'A')
				{
						Int_Loc-=1;
						*Int_Par_Ref = Int_Loc - Int_Glob;
						Enum_Loc = Ident_1;
				}
		while(Enum_Loc != Ident_1);
}


void Proc_3(Ptr_Ref_Par)
		Rec_Pointer *Ptr_Ref_Par;
{
		if(Ptr_Glob != Null)
				*Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
		Proc_7(10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
}

void Proc_4()
{
		Boolean Bool_Loc;
		Bool_Loc = Ch_1_Glob == 'A';
		Bool_Glob = Bool_Loc | Bool_Glob;
		Ch_2_Glob = 'B';
}


void Proc_5()
{
		Ch_1_Glob = 'A';
		Bool_Glob = false;
}

#ifdef NOSTRUCTASSIGN
memcpy(d, s, l)
		register char *d;
		register char *s;
		register int l;
{
		while(l--)  *d++ = *s++;
}
#endif


