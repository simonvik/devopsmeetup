#include "helpers.h"

#define ELEMENTSOF(x)  (sizeof(x) / sizeof((x)[0]))

void log_die(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	printf(format, args);
	va_end(args);
	exit(1);
}

static FILE *cgroup_file_open(const char *name, const char *section, const char *file)
{
	FILE *f;
	char buf[255];
	int len = sizeof(buf);
	char cg_path[] = "/sys/fs/cgroup/";

	snprintf(buf, len, "%s/%s/containers/%s/%s", cg_path, section, name, file);

	f = fopen(buf, "r+");
	if (f != NULL)
		return f;

	snprintf(buf, len, "%s/%s", cg_path, section);
	mkdir(buf, 0755);

	snprintf(buf, len, "%s/%s/containers", cg_path, section);
	mkdir(buf, 0755);

	snprintf(buf, len, "%s/%s/containers/%s", cg_path, section, name);
	mkdir(buf, 0755);

	snprintf(buf, len, "%s/%s/containers/%s/%s", cg_path, section, name, file);
	f = fopen(buf, "r+");

	if (f == NULL) {
		// Do something
	}
	return f;
}

int cgroup_set_memorylimit(const char *name, const char *section, int limit)
{
	FILE *file;
	file = cgroup_file_open(name, section, "memory.limit_in_bytes");

	if (file == NULL) {
		// Do something
		return -1;
	}

	fprintf(file, "%d\n", limit);
	fclose(file);

	return 0;
}

int cgroup_add_task(const char *name, const char *section, pid_t pid)
{

	FILE *file;

	file = cgroup_file_open(name, section, "tasks");

	if (file == NULL) {
		// Do something
		return -1;
	}

	fprintf(file, "%d\n", pid);
	fclose(file);

	return 0;
}
