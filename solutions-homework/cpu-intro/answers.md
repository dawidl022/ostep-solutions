# Chapter 4: The Abstraction: The Process - Homework

> 1. Run `process-run.py` with the following flags: `-l 5:100,5:100`. What
> should the CPU utilization be (e.g., the percent of time the CPU is in use?)
> Why do you know this?

CPU utilization should be 100%. Neither process makes any I/O requests, so
neither process will block. Instead each process will run on the CPU till
completion.

> 2. Now run with these flags: `./process-run.py -l 4:100,1:0`. These flags
> specify one process with 4 instructions (all to use the CPU), and one that
> simply issues an I/O and waits for it to be done. How long does it take to
> complete both processes?

6 CPU ticks + the I/O wait time. 4 CPU for the first process, 1 I/O initiation
time waiting for the I/O to complete and 1 I/O completion for the second
process. The processes run sequentially, so the second process only initiates
the I/O operation once the first process completes.

> 3. Switch the order of the processes: `-l 1:0,4:100`. What happens now? Does
> switching the order matter? Why?

Now the time to complete both processes is 6 CPU ticks + min(the I/O wait time -
4, 0).

Switching the order matters, because the `PROCESS_SWITCH_BEHAVIOR` is by default
set to `SWITCH_ON_IO`, meaning that when the first processes blocks waiting for
I/O, the second process has the chance to run CPU instructions to completion.

In the previous example, there were no more processes to run after the process
that blocked for I/O, so the CPU was just idle.

> 4. We’ll now explore some of the other flags. One important flag is `-S`,
> which determines how the system reacts when a process issues an I/O. With the
> flag set to `SWITCH_ON_END`, the system will NOT switch to another process
> while one is doing I/O, instead waiting until the process is completely
> finished. What happens when you run the following two processes (`-l 1:0,4:100
> -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?

Now we get the exact same results in terms of total time to run both processes
as when we ran the CPU-bound process before the I/O-build process. Essentially,
we have eliminated the opportunity to use concurrency when one process blocks
for I/O.

> 5. Now, run the same processes, but with the switching behavior set to switch
> to another process whenever one is `WAITING` for I/O (`-l 1:0,4:100 -c -S
> SWITCH_ON_IO`).What happens now?

Since `SWITCH_ON_IO` is the default option for the switching strategy, we get
the same results as expressed in the answer to question 3.

> 6. One other important behavior is what to do when an I/O completes. With `-I
> IO RUN LATER`, when an I/O completes, the process that issued it is not
> necessarily run right away; rather, whatever was running at the time keeps
> running. What happens when you run this combination of processes? (Run
> `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c
> -p`) Are system resources being effectively utilized?

Upon completion of I/O, the process that was waiting is put at the end of the
queue of processes to run. This means that the first process only gets to issue
its first I/O request before the other 3 processes get to run to completion.

This is not the optimal usage of system resources while the CPU-bound processes
are running, I/O is idle for the most part, and while the I/O bound process
is running the CPU is idle for the most part.

> 7. Now run the same processes, but with `-I IO_RUN_IMMEDIATE` set, which
> immediately runs the process that issued the I/O. How does this behavior
> differ? Why might running a process that just completed an I/O again be a good
> idea?

Now the I/O requests are nicely interleaved with the execution of CPU-bound
processes.

A process that issued an I/O request is likely to issue another I/O request,
hence it would be efficient to let the process make the I/O request as soon as
it can, so that while it's blocked, other processes can run, and conversely,
while other processes are using the CPU, an I/O operation can be talking place.

> 8. Now run with some randomly generated processes: -s 1 -l 3:50,3:50 or -s 2
> -l 3:50,3:50 or -s 3 -l 3:50,3:50. See if you can predict how the trace will
> turn out. What happens when you use the flag -I IO_RUN_IMMEDIATE vs. -I
> IO_RUN_LATER? What happens when you use -S SWITCH_ON_IO vs. -S SWITCH_ON_END?

```txt
python3 process-run.py -s 1 -l 3:50,3:50

Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  io
  io_done
  io
  io_done

Process 1
  cpu
  cpu
  cpu
```

My prediction with `IO_RUN_LATER` and `SWITCH_ON_IO`:

```
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1             
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7        BLOCKED          DONE                           1
  8*   RUN:io_done          DONE             1          
  9         RUN:io          DONE             1          
 10        BLOCKED          DONE                           1
 11        BLOCKED          DONE                           1
 12        BLOCKED          DONE                           1
 13        BLOCKED          DONE                           1
 14        BLOCKED          DONE                           1
 15*   RUN:io_done          DONE             1              
```

Changing to `IO_RUN_IMMEDIATE` will have no effect in this example, since there
are no more processes that could be running concurrently on the I/O-bounded
processes's second I/O request.

My prediction with `IO_RUN_LATER` and `SWITCH_ON_END`:
```
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1             
  3        BLOCKED         READY                           1
  4        BLOCKED         READY                           1
  5        BLOCKED         READY                           1
  6        BLOCKED         READY                           1
  7        BLOCKED         READY                           1
  8*   RUN:io_done         READY             1          
  9         RUN:io         READY             1          
 10        BLOCKED         READY                           1
 11        BLOCKED         READY                           1
 12        BLOCKED         READY                           1
 13        BLOCKED         READY                           1
 14        BLOCKED         READY                           1
 15*   RUN:io_done         READY             1              
 16          DONE        RUN:cpu             1              
 17          DONE        RUN:cpu             1              
 18          DONE        RUN:cpu             1              
```

In general `IO_RUN_LATER` has no effect if `SWITCH_ON_END` is set, and behaves
the same as `SWITCH_ON_END` with `IO_RUN_IMMEDIATE`.
