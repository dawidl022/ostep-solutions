# Chapter 5: Process API - Homework (Simulation)

> 1. Run `./fork.py -s 10` and see which actions are taken. Can you predict what
> the process tree looks like at each step? Use the -c flag to check your
> answers. Try some different random seeds (-s) or add more actions (-a) to get
> the hang of it.

```txt
                           Process Tree:
                               a

Action: a forks b              a
                               └── b

Action: a forks c              a
                               ├── b
                               └── c
Action: c EXITS                a
                               └── b

Action: a forks d              a
                               ├── b
                               └── d
Action: a forks e
                               a
                               ├── b
                               ├── d
                               └── e
```

> One control the simulator gives you is the fork percentage, controlled by the
> -f flag. The higher it is, the more likely the next action is a fork; the
> lower it is, the more likely the action is an exit. Run the simulator with a
> large number of actions (e.g., -a 100) and vary the fork percentage from 0.1
> to 0.9. What do you think the resulting final process trees will look like as
> the percentage changes?

With a large fork percentage the tree will likely have many levels, since the
forked processes will fork themselves again. On the other hand, a low fork
percentage will likely result in a tree with few levels, as the child processes
are more likely to exit than to fork themselves.

After having ran the simulator with the `-c` flag, I realised it's not only the number of levels that increase with the fork percentage, but also in general the total number of nodes in the tree.

> 3. Now, switch the output by using the -t flag (e.g., run ./fork.py
> -t). Given a set of process trees, can you tell which actions were
> taken?

```txt
                           Process Tree:
                               a

a forks b                      a
                               └── b

b exits                        a

a forks c                      a
                               └── c

c forks d                      a
                               └── c
                                   └── d

c exits                        a
                               └── d
```

> 4. One interesting thing to note is what happens when a child exits; what
> happens to its children in the process tree? To study this, let’s create a
> specific example: ./fork.py -A a+b,b+c,c+d,c+e,c-. This example has process
> ’a’ create ’b’, which in turn creates ’c’, which then creates ’d’ and ’e’.
> However, then, ’c’ exits. What do you think the process tree should like after
> the exit? What if you use the -R flag? Learn more about what happens to
> orphaned processes on your own to add more context.

When a process exits, it's children are orphaned by the the root process, in
this example, orphan processes d and e become the children of the root process
"a".

When using the `-R` flag "reparent to local parent" is enabled. I.e. When a
process exits, it's children are orphaned by the the parent process. So in this
example, when c exits, processes d and e will end up as a direct children of
process b.

> 5. One last flag to explore is the -F flag, which skips intermediate steps and
> only asks to fill in the final process tree. Run ./fork.py -F and see if you
> can write down the final tree by looking at the series of actions generated.
> Use different random seeds to try this a few times.

```txt
Action: a forks b
Action: b forks c
Action: a forks d
Action: c forks e
Action: e EXITS

                        Final Process Tree?

                        a
                        ├── b
                        │   └── c
                        └── d
```

> 6. Finally, use both -t and -F together. This shows the final process tree,
> but then asks you to fill in the actions that took place. By looking at the
> tree, can you determine the exact actions that took place? In which cases can
> you tell? In which can’t you tell? Try some different random seeds to delve
> into this question.

```txt
Action?
Action?
Action?
Action?
Action?

                        Final Process Tree:
                               a
                               └── d
```

It is not possible to tell what exact action led to the creation of this process
tree. a might have forked d, or some other process may have forked "d" and
orphaned the process. And we have no clue about any other processes, such as b
or c.


```
Action?
Action?
Action?
Action?
Action?

                        Final Process Tree:
                               a
                               └── b
                                   └── c
                                       └── d
                                           └── e
                                               └── f
```

In this example, there is only 1 possibility, since there are 6 process in the
final tree and 5 actions were taken. As such all of the actions must have been
fork actions, that led to the creation of the final tree:

```txt
a forks b
b forks c
c forks d
d forks e
e forks f
```

So in general, if all actions were forks, then the number of processes in the
final tree is 1 more than the number of actions, and we can tell by looking at
the final tree what actions were taken, assuming the processes get created in
alphabetical sequence (i.e. b always gets created before c). In all other cases
it is not possible to tell the exact set of actions taken.

Another example like this:

```txt

a forks b
b forks c
a forks d
d forks e
b forks f

                        Final Process Tree:
                               a
                               ├── b
                               │   ├── c
                               │   └── f
                               └── d
                                   └── e
```
