#include "../common/helpers.h"
#define STACKSIZE 1024*1024

int mainfunc(void *args)
{

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
	int flags = 0;

	/* https://linux.die.net/man/2/clone2 */
	child = clone(mainfunc, stackTop, flags | CLONE_VFORK | CLONE_PARENT_SETTID, NULL);

	if (child == -1)
		log_die("Failed to clone");

	/* https://linux.die.net/man/2/waitpid */
	waitpid(child, &wstatus, 0);
}
