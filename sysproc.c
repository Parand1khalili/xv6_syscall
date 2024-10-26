#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
struct report_traps;
struct treport;
struct _internal_report_list;

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//child_proccess
uint64
sys_child_processes(void){
uint64 user_cpinfo; //user space ppointer
struct child_processes cpinfo; // kernel side
argaddr(0, &user_cpinfo);
struct proc *p = myproc();
int err = child_processes(&cpinfo);
if (copyout(p->pagetable, user_cpinfo, (char*)&cpinfo, sizeof(cpinfo)) < 0){
  return -1;
}
return err;
}

//trap reports
uint64
sys_report_traps(void){
uint64 addr;
struct report_traps traps;
argaddr(0, &addr);
report_traps(&traps);
    // Copy reports to user space
    if (copyout(myproc()->pagetable, (uint64)addr, (char *)&traps, sizeof(struct report_traps)) < 0) {
        return -1;
    }
    return 0;
}

