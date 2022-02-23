# Some experiments written in C

## Measure the overhead of mutexes without contention

Preshing's [Always Use a Lightweight Mutex](https://preshing.com/20111124/always-use-a-lightweight-mutex/) indicates that
it is a misconception a mutex always hurt performance. As per this article, for modern implementations of mutexes, performance
penalties mostly come from lock contention. Therefore, using mutex is low-cost in the case of no other threads competing
for it, as suggested by the average time of acquiring and then releasing a mutex. However these numbers came from microbenchmarks
10 years ago (I am writing this in 2022), I am curious about whether there are improvements in mutex's implementaions. So I
wrote some C code ([lock_overhead.c](#lock_overhead.c)).

My settings are as follow:

- `Ubuntu 20.04.4 LTS` with the `5.13 Linux kernel`
- `gcc 9.3`
- `glibc 2.31`
- compile with `gcc -O2 -o lock_overhead lock_overhead.c -lpthread` 

The lock/unlock pair was done `1,000,000,000` times in `4.22 seconds`, `4.2 ns` for each. Incidently, I guess the overhead under
contention results from context switches.
