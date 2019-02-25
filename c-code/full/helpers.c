#include "helpers.h"

static FILE *cgroup_file_open(const char *user, const char *section, const char *file)
{
	FILE *f;
    char buf[255] = { 0 };
    int len = sizeof(buf);
	char cg_path[] = "/sys/fs/cgroup/";

	snprintf(buf, len, "%s/%s/containers/%s/%s", cg_path, section, user, file);

	f = fopen(buf, "r+");
	if (f != NULL)
		return f;

	snprintf(buf, len, "%s/%s", cg_path, section);
	mkdir(buf, 0755);

	snprintf(buf, len, "%s/%s/containers", cg_path, section);
	mkdir(buf, 0755);

	snprintf(buf, len, "%s/%s/containers/%s", cg_path, section, user);
	mkdir(buf, 0755);

	snprintf(buf, len, "%s/%s/containers/%s/%s", cg_path, section, user, file);
	f = fopen(buf, "r+");

	if (f == NULL) {
		// Do something
	}
	return f;
}

int add_task_to_cgroup(const char *user, const char *section, pid_t pid)
{

	FILE *file;

	file = cgroup_file_open(user, section, "tasks");

	if (file == NULL) {
		// Do something
        return -1;
	}

	fprintf(file, "%d\n", pid);
	fclose(file);

	return 0;
}
