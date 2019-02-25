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


int set_memory_cgroup(const char *user, pid_t pid, long memory_limit);
