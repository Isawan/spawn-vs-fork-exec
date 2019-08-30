#include<spawn.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>
#include<time.h>
#include<assert.h>

extern char **environ;

char *allocate(size_t size) {
	char *buffer;
	buffer = malloc(size);
	memset(buffer, 1, size);
	return buffer;
}

int fork_and_wait(char *argv[]) {
	pid_t pid;
	int status;
	if ((pid = fork())) {
		wait(&status);
	} else {
		execvp(argv[0], argv);
	}
	return status;
}

int spawn_and_wait(char *argv[]) {
	pid_t pid;
	int status;
	posix_spawnattr_t attr;

	posix_spawnattr_init(&attr);
	
	posix_spawnp(
		&pid,    // Pointer to pid
		argv[0], // Path to executable
		NULL,    // handle file descriptors
		&attr,   // attribute configuration of child process
		argv,    // argv
		environ  // array of environment variables
	);
	wait(&status);
	return status;
}

long long timespec_diff(struct timespec start, struct timespec end) {
	long long sec_diff = 0;
	long long nsec_diff = 0;
	assert(start.tv_sec <= end.tv_sec);
	assert(start.tv_nsec <= end.tv_nsec);
	if (start.tv_sec < end.tv_sec) {
		sec_diff = end.tv_sec - start.tv_sec;
		nsec_diff = (1000000000 - start.tv_nsec) + end.tv_nsec;
	} else {
		nsec_diff = end.tv_nsec - start.tv_nsec;
	}
	return sec_diff*1000000000 + nsec_diff;
}

int main(int argc, char **argv) {
	char* buffer;
	struct timespec start_time;
	struct timespec end_time;
	long heap_size;


	if (argc < 3) {
		printf("expected arguments: %s <memory> [args ...]\n", argv[1]);
		exit(1);
	}

	heap_size = strtol(argv[1], NULL, 10);
	if (heap_size == 0) {
		printf("Invalid argument passed: %s", argv[1]);
		exit(1);
	}

	buffer = allocate((size_t) heap_size);

        clock_gettime(CLOCK_MONOTONIC, &start_time);

        #ifdef time_fork
	fork_and_wait(&argv[2]);
        #endif
	#ifdef time_spawn
	spawn_and_wait(&argv[2]);
	#endif
	
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	printf("%lld\n", timespec_diff(start_time, end_time));

	free(buffer);
}
