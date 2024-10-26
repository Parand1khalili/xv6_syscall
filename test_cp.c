#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/stat.h"
#include "kernel/proc.h"
#include "user/user.h"
struct child_processes;

char *states[] = {
    [UNUSED]   "unused",
    [USED]   "used",
    [SLEEPING]   "sleep",
    [RUNNABLE]   "runable",
    [RUNNING]   "run",
    [ZOMBIE]   "zombie"
};

int main() {
    int pid;
    for (int i = 0 ; i<4 ; i++){
        pid = fork();
        if(pid == 0){
            fork();
            fork();
            sleep(200);
            exit(0);
        }
    }
    sleep(5);
    struct child_processes cp;
    if(child_processes(&cp) == 0){
        printf("total children: %d \n", cp.count);
        printf("pid ppid state name\n");
        for(int i=0 ; i<cp.count ; i++){
            printf("%d %d %s %s \n",
            cp.processes[i].pid,
            cp.processes[i].ppid,
            states[cp.processes[i].state],
            cp.processes[i].name);
        }
    }else{
        printf("error calling system call");
    }
    exit(0);
}