#include "../common/helpers.h"
#define STACKSIZE 1024*1024

static inline int pivot_root(const char *new_root, const char *put_old)
{
	return syscall(SYS_pivot_root, new_root, put_old);
}

int mainfunc(void *args)
{
	int ret = 0;

	ret = cgroup_set_memorylimit("devopsc", "memory", 1024 * 1024 * 1024);
	if (ret != 0)
		log_die("Failed to set memory limit for cgroup\n");

	ret = cgroup_add_task("devopsc", "memory", getpid());
	if (ret != 0)
		log_die("Failed to add task to cgroup\n");

	ret = sethostname("devopscontainer", strlen("devopscontainer"));
	if (ret != 0)
		log_die("Failed to set hostname\n");

	ret = mount("", "/", NULL, MS_REC | MS_PRIVATE, NULL);
	if (ret != 0)
		log_die("Failed to remount / as private\n");

	ret = mount("rootfs", "/mnt", "bind", MS_BIND, "");
	if (ret != 0)
		log_die("Failed to mount rootfs\n");

	ret = pivot_root("/mnt", "/mnt/.oldroot");
	if (ret != 0)
		log_die("Failed to pivot root\n");

	ret = mount("proc", "/proc", "proc", MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC, "");
	if (ret != 0)
		log_die("Failed to mount /proc\n");

	ret = umount2("/.oldroot", MNT_DETACH);
	if (ret != 0)
		log_die("Failed to unmount oldroot\n");

	chdir("/");

    ret = setresgid(65534, 65534, 65534);
    if(ret != 0)
        log_die("Failed to set group\n");

	ret = setresuid(65534, 65534, 65534);
    if(ret != 0)
        log_die("Failed to set user\n");


	execl("/usr/bin/php", "php", "index.php", NULL);
	log_die("Should not be here\n");

	return 0;
}

int main(void)
{
	pid_t child;
	int wstatus;
	/* https://linux.die.net/man/2/clone2 */
	char *stack = malloc(STACKSIZE);
	char *stackTop = stack + STACKSIZE;

	/* https://linux.die.net/man/2/unshare */
	int flags = CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS;

	/* https://linux.die.net/man/2/clone2 */
	child = clone(mainfunc, stackTop, flags | CLONE_VFORK | CLONE_PARENT_SETTID, NULL);

	if (child == -1)
		log_die("Failed to clone");

	/* https://linux.die.net/man/2/waitpid */
	waitpid(child, &wstatus, 0);
}
