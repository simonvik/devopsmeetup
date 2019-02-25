#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <linux/netlink.h>
#include <sys/capability.h>
#include <sys/types.h>
#include <seccomp.h>

#include <sys/mount.h>
/* execl, sethostname*/
#include <unistd.h>
#include <syscall.h>
#include <sched.h>
#include <sys/wait.h>
#include <stdarg.h>

int cgroup_add_task(const char *name, const char *section, pid_t pid);
int cgroup_set_memorylimit(const char *name, const char *section, int limit);
void log_die(const char *format, ...);
