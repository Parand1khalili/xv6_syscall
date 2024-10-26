#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/stat.h"
#include "kernel/proc.h"
#include "user/user.h"
struct treport;
struct report_traps;

void trap_builder(){
    int *ptr = (int *)0x0;
    *ptr = 42; // This will cause a segmentation fault
}

int main(){
    int pid = fork();
    if(pid < 0) {
        // Fork failed
        printf("Fork failed\n");
        exit(1);
    }

    if(pid == 0){
        // Child process
        trap_builder();
        exit(0);
    }
    else{
        // Parent process
        wait(0);
        
        struct report_traps rt;
        if(report_traps(&rt) < 0){
            printf("report_traps failed\n");
            exit(1);
        }

        printf("Total traps: %d\n", rt.count);
        for(int i = 0; i < rt.count; i++){
            struct treport *r = &rt.reports[i];
            printf("PID: %d, Process Name: %s, Cause: %ld, EPC: %ld, STVAL: %ld\n",
                   r->pid, r->pname, r->scause, r->sepc, r->stval);
        }
        exit(0);
    }

}
