#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <dlfcn.h>

#define PLUGINS_FOLDER_NAME "plugins"

#define MAX_PLUGINS_COUNT 64

static char PLUGINS[MAX_PLUGINS_COUNT][FILENAME_MAX + 1];
static int PLUGINS_COUNT = 0;

static int init_plugins() {

	int status;
	DIR * dir;
	struct dirent * de;
	struct stat st;

	dir = opendir(PLUGINS_FOLDER_NAME);

	if(dir == NULL) {
		return 1;
	}

	while(1) {
		de = readdir(dir);
		if(de == NULL) {
			break;
		}
		if(PLUGINS_COUNT >= MAX_PLUGINS_COUNT) {
			break;
		}
		snprintf(PLUGINS[PLUGINS_COUNT], FILENAME_MAX, "%s/%s", PLUGINS_FOLDER_NAME, de->d_name);
		status = stat(PLUGINS[PLUGINS_COUNT], &st);
		if(S_ISREG(st.st_mode)) {
			PLUGINS_COUNT++;
		}
	}

	closedir(dir);

	return 0;
}

static void set_status(int * status, int value) {
	if(status != NULL) {
		(*status) = value;
	}
}

/**
 * - str - not null, expression
 * - status - can be null, will be set to 0 if ok
 */
static double calc(const char * plugin_name, const char * str, int * status) {

	void * plib;
	double (*pc) (const char * str, int * status);
	double result;

	plib = dlopen(plugin_name, RTLD_NOW);

	if(plib == NULL) {
		fprintf(stderr, "Error loading plugin %s\n", plugin_name);
		set_status(status, 1);
		return 0;
	}

	pc = dlsym(plib, "calc");

	if(pc == NULL) {
		fprintf(stderr, "Error searching calc in plugin %s\n", plugin_name);
		set_status(status, 1);
		dlclose(plib);
		return 0;
	}

	result = pc(str, status);

	dlclose(plib);

	return result;
}

int main(int argc, char **argv) {

	int pno;
	int i;
	int status;
	double result;

	if(argc < 2) {
		fprintf(stderr, "Expression required\n");
		return 1;
	}

	status = init_plugins();

	if(status != 0) {
		fprintf(stderr, "Error reading plugins list\n");
		return 1;
	}
	else {
		printf("Available plugins (%d):\n", PLUGINS_COUNT);
		for(i=0; i<PLUGINS_COUNT; ++i) {
			printf("%i] %s\n", i, PLUGINS[i]);
		}
	}

	printf("Enter plugin number: ");
	scanf("%d", &pno);

	if(pno < 0 || pno >= PLUGINS_COUNT) {
		fprintf(stderr, "Bad plugin number\n");
		return 1;
	}

	result = calc(PLUGINS[pno], argv[1], &status);

	if(status == 0) {
		printf("Result: %f\n", result);
	}
	else {
		printf("Result: ERROR\n");
	}

	return 0;
}
