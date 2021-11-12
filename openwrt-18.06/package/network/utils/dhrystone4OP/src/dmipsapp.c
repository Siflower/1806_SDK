/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/time.h>

//#include <gio/gio.h>

#include "dhry-dmips.h"

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <sched.h>
#include <sys/wait.h>

void show_cpuinfo()
{
	char line[256]={0};
	FILE *fp = NULL;
	fp = fopen("/proc/cpuinfo","r");
	if(!fp){
		printf("open /proc/cpuinfo err!\n");
		return;
	}
printf("===================/proc/cpuinfo==============");
    while(fgets(line,sizeof(line),fp)){
        printf("%s",line);
    }
	fclose(fp);
	printf("===================/proc/cpuinfo end==============");
	return;
}

int firetest = 0;
int main(int argc, char *argv[])
{
		pid_t fpid, pr;
		int forkNum, count;
		cpu_set_t mask;
		int n = 20;
		int num = TEST_FORK_NUMBER;
		int ret = 0;
		float dmipsval = 0.0f;
		int status, returncode;

		if(argc == 2 && strcmp(argv[1],"--firetest")==0){
			firetest = 1;
		}
		g_print("Dhrystone BenchMark, Version 2.1(Language:C)\n");
		show_cpuinfo();
		num = sysconf(_SC_NPROCESSORS_ONLN);
		if(firetest){
			g_print("system has %d processor(s),I will fork %d process,then firetest nerver stop! untill ctrl+c\n", num, num);
			g_print("DMIPSApp firetest Running\n");
		}else{
			g_print("system has %d processor(s),I will fork %d process,then adding together dmips,get total dmips!\n", num, num);
			g_print("DMIPSApp Running\n");
		}
		g_print("=====================================================\n");

		for(forkNum = 0;forkNum < (num - 1); forkNum++)
		{
				fpid = fork();
				if(fpid < 0)
				{
						g_print("!!!failed to fork(cpuid=%d)\n", forkNum);
						return 1;
				}
				else if(fpid > 0)
				{
						g_print("Parent process forked process %d(cpuid=%d)\n", fpid, forkNum);
						continue;
				}
				else
				{
						CPU_ZERO(&mask);
						CPU_SET(forkNum, &mask);
						if(sched_setaffinity(0, sizeof(mask), &mask) == -1)
						{
								g_print("WARNING: Could not set CPU Affinity(cpuid=%d,pid=%d)\n", forkNum,(int)getpid());
								return 2;
						}

						dmipsval = fork_pro(n, forkNum);
						if(dmipsval <= 0.0f)
						{
								g_print("WARNING:Failed to calculate DMIPS(cpuid=%d,pid=%d)\n", forkNum,(int)getpid());
								return 3;
						}

						store_dmips((int)getpid(), dmipsval);
						//g_print("CPU ID:%d,dmips value=%.2f\n",forkNum,dmipsval);
						return 0;
				}
		}

		CPU_ZERO(&mask);
		CPU_SET(forkNum, &mask);
		if(sched_setaffinity(0, sizeof(mask), &mask) == -1)
		{
				g_print("WARNING: Could not set CPU Affinity(cpuid=%d)\n", forkNum);
		}
		else
		{
				dmipsval = fork_pro(n, forkNum);
				if(dmipsval <= 0.0f)
				{
						g_print("WARNING:Failed to calculate DMIPS(cpuid=%d)\n", forkNum);
				}
				store_dmips((int)getpid(), dmipsval);
				//g_print("CPU ID:%d,dmips value=%.2f\n",forkNum,dmipsval);
		}

		if(num > 1)
		{
				count = num - 1;
				while(count > 0)
				{
						pr = wait(&status);
						if(WIFEXITED(status))
						{
								returncode = WEXITSTATUS(status);
								if(returncode != 0)
								{
										g_print("forked process %d returned err code %d\n", pr, returncode);
								}
								else
								{
										dmipsval += read_dmips((int)pr);
								}
						}
						else
						{
								g_print("forked process %d exited abnormally!\n", pr);
						}
						count--;
				}

		}

		g_print("\n");
		g_print("Total DMIPS:%6.2f\n", dmipsval);


		return 0;
}

