#define _GNU_SOURCE

#include <sys/mount.h>
/* execl, sethostname*/
#include <unistd.h>
#include <string.h>
#include <syscall.h>
#include <sched.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "helpers.h"

#define STACKSIZE 1024*1024

/* https://linux.die.net/man/2/pivot_root */
static inline int pivot_root(const char *new_root, const char *put_old)
{
	return syscall(SYS_pivot_root, new_root, put_old);
}

void log_die(const char* format, ...){
    va_list args;
    va_start (args, format);
    printf (format, args);
    va_end (args);
    exit(1);
}

int mainfunc(void *args){
    int res = 0;

    /* https://linux.die.net/man/2/sethostname */
    res = sethostname("devopscontainer", strlen("devopscontainer"));
    if(res != 0)
        log_die("Failed to set hostname\n");

    /* https://linux.die.net/man/2/mount */
    res = mount("", "/", NULL, MS_REC | MS_PRIVATE, NULL);
    if(res != 0)
        log_die("Failed to remount / as private\n");
        
    res = mount("rootfs", "/mnt", "bind", MS_BIND, "");
    if(res != 0)
        log_die("Failed to mount rootfs\n");
        
    res = pivot_root("/mnt", "/mnt/.oldroot");
    if(res != 0)
        log_die("Failed to pivot root\n");
        
    res = mount("proc", "/proc", "proc", MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC, "");
    if(res != 0)
        log_die("Failed to mount /proc\n");

    res = umount2("/.oldroot", MNT_DETACH);   
    if(res != 0)
        log_die("Failed to unmount oldroot\n");

    chdir("/");

    /* https://linux.die.net/man/2/setresgid */
    res = setresgid(65534, 65534, 65534);
    if(res != 0)
        log_die("Failed to set group\n");

    /* https://linux.die.net/man/2/setresuid */
	res = setresuid(65534, 65534, 65534);
    if(res != 0)
        log_die("Failed to set user\n");


    /* https://linux.die.net/man/3/exec */
    execl("./exploit", "exploit", NULL);
    log_die("Should not be here\n");

    return 0;
}

int main(void)
{  
    pid_t child;
    int wstatus;
    /*
        https://linux.die.net/man/2/clone2:
        The child_stack argument specifies the location of the stack used by the child process. 
        Since the child and calling process may share memory, 
        it is not possible for the child process to execute in the same stack as the calling process.
    */
    char *stack = malloc(STACKSIZE);
    char *stackTop = stack + STACKSIZE;

    /* https://linux.die.net/man/2/unshare */
    int flags = CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS;

    /* https://linux.die.net/man/2/clone2 */
    child = clone(mainfunc, stackTop, flags | CLONE_VFORK | CLONE_PARENT_SETTID, NULL);

    /* https://linux.die.net/man/2/waitpid */
    waitpid(child, &wstatus, 0);
}
