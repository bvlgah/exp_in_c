#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static int str_to_ul(const char *name, const char *s, unsigned long *res) {
    int errnum;

    *res = strtoul(s, NULL, 10);
    errnum = errno;

    if (errnum != 0) {
        fprintf(
            stderr,
            "%s: cannot convert iterations \"%s\" to an integer: %s\n",
            name,
            s,
            strerror(errnum));
        return -1;
    }

    return 0;
}

static int get_boot_time(const char *name, struct timespec *tp) {
    int errnum;

    if (clock_gettime(CLOCK_BOOTTIME, tp)) {
        errnum = errno;

        fprintf(
            stderr,
            "%s: cannot get clock time: %s\n",
            name,
            strerror(errnum));

        return -1;
    }

    return 0;
}

static double time_diff(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec)
        + (double) (end->tv_nsec - start->tv_nsec) / 1000000000.0;
}

int main (int argc, char *argv[]) {
    unsigned long i;
    unsigned long iter;
    const char *name;
    struct timespec start;
    struct timespec end;
    double time_duration;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    name = argc > 0 ? argv[0] : "lock_overhead";

    if (argc != 2) {
        fprintf(
            stderr,
            "Usage: %s iterations\n",
            name);
        goto error;
    }

    if (str_to_ul(name, argv[1], &iter)) {
        goto error;
    }

    if (get_boot_time(name, &start)) {
        goto error;
    }

    for (i = 0; i < iter; ++i) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }

    if (get_boot_time(name, &end)) {
        goto error;
    }

    time_duration = time_diff(&start, &end);

    printf("It takes %f seconds to iterate %lu times\n", time_duration, iter);

    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;

error:
    pthread_mutex_destroy(&mutex);
    return EXIT_FAILURE;
}

