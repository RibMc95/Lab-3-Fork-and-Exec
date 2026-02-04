# Lab-3-Fork-and-Exec

## Completed Challenges

###  Challenge 1 - Single fork() lifecycle

- **Program:** `ch1_single_fork.c`
- **Description:** Fork once, child prints PID/PPID, sleeps 2 sec, exits with code 7. Parent waits and reports exit status.
- **Acceptance:** Parent prints "child X exited with status 7" ✓

###  Challenge 2 - Multiple children in a loop

- **Program:** `ch2_multi_child.c`
- **Description:** Parent creates N children (from CLI arg), each exits with status = index+1. Parent waits for all and reports exit codes.
- **Acceptance:** Parent collects N children and reports each WEXITSTATUS ✓

###  Challenge 3 - execlp() to run ls -la

- **Program:** `ch3_exec_ls.c`
- **Description:** Fork; child uses `execlp("ls", "ls", "-la", NULL)` to list directory; parent waits.
- **Acceptance:** Screen shows `ls -la` output; parent prints final message ✓

###  Challenge 4 - Exec an internal helper program

- **Programs:** `worker.c`, `ch4_exec_worker.c`
- **Description:** Create worker binary that prints args and environment variable MYVAR. Parent forks and execs worker with custom environment.
- **Acceptance:** Worker prints args and MYVAR=hello ✓

###  Challenge 5 - Show execv vs execl

- **Program:** `ch5_exec_examples.c`
- **Description:** Two child forks: Child A uses `execl()`, Child B uses `execv()` to run same command.
- **Acceptance:** Both children print "one two" ✓

###  Challenge 6 - Parent-to-child pipe (producer/consumer)

- **Program:** `ch6_pipe_sum.c`
- **Description:** Parent writes numbers 1-10 to pipe; child reads and sums them.
- **Acceptance:** Child prints "Sum = 55" ✓

###  Challenge 7 - Build a pipeline (ls | grep pattern)

- **Program:** `ch7_pipeline.c`
- **Description:** Implement `ls | grep <pattern>` manually with two forks and one pipe. Pattern from command line.
- **Acceptance:** Output matches `ls | grep pattern` ✓

###  Challenge 8 - waitpid() with WNOHANG

- **Program:** `ch8_wait_nonblock.c`
- **Description:** Spawn 3 children sleeping 1, 2, 3 seconds. Parent polls with `waitpid(-1, &status, WNOHANG)` non-blocking.
- **Acceptance:** Parent prints children completion in ascending order (1s, 2s, 3s) ✓

###  Challenge 9 - Create & observe a zombie process

- **Program:** `ch9_zombie.c`
- **Description:** Fork child that exits immediately; parent sleeps 10 seconds before calling wait(). Demonstrates zombie state.
- **Acceptance:** Child visible as `<defunct>` in `ps aux` during sleep window ✓

###  Challenge 10 - Parallel worker pool (limit concurrency)

- **Program:** `ch10_pool.c`
- **Description:** Spawn up to M worker children at once. As children finish, spawn new ones for remaining tasks. Uses waitpid() to detect finished children.
- **Acceptance:** Processes all tasks, respects concurrency limit (max M children at once) ✓

## Demo Programs

- `fork_demo.c` - Basic fork() example
- `exec_demo.c` - Fork + execv() example
- `pipe_demo.c` - Parent-child pipe communication
- `pipeline_demo.c` - Two-process pipeline with pipe
- `zombie_demo.c` - Demonstrates zombie state with configurable delay

## Build & Run

```bash
cd os-lab/build
cmake ..
cmake --build .

# Run any challenge
./ch1_single_fork
./ch2_multi_child 7
./ch3_exec_ls
./ch4_exec_worker
./ch5_exec_examples
./ch6_pipe_sum
./ch7_pipeline "\.c$"
./ch8_wait_nonblock
./ch9_zombie
./ch10_pool 3 file1 file2 file3 file4 file5
```

## Known Issues

- None identified. All challenges complete and working as expected.
