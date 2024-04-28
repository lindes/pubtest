# AVL test code

The code in this directory is meant as a simple example of using the
[libavl](https://adtinfo.org/) C library to impliment a simple key/value
in-memory store that can give O(log(n)) search, insert, and deletion
times.

Note: to install this library on Ubuntu, one might do `sudo apt install
libavl-dev`.  Installation on other systems is left as an exercise to
the reader.

This can be used as one way to implement an
[associative array](https://en.wikipedia.org/wiki/Associative_array),
which is basically what I've done here.

For example, running this command as `./avl a b c` would generate an
[AVL tree](https://en.wikipedia.org/wiki/AVL_tree) with key/value pairs
based on the arguments I passed (`a`, `b`, and `c`) and their indexes
within the [argv](https://en.wikipedia.org/wiki/argv) vector.  It then
tests out searches (also based on argv), deletions, and also generates
a `dot` file (more below) to represent the structure of the tree (note:
this is done before deleting anything).  The output of running it this
way would be like the following (pointer values (`0x61b...`) may differ,
of course):

```
Got tree 0x61bf8b6312a0
Got node for          "a" ( 1): 0x61bf8b632320
Got node for          "b" ( 2): 0x61bf8b6323a0
Got node for          "c" ( 3): 0x61bf8b632420
Walking tree as linked-list:
           a:   1
           b:   2
           c:   3
Walking tree from root, top-first, creating avl.dot.
Now searching for arguments in tree...
  For "./avl", failed to find node (got (nil))
  For "a", got node 0x61bf8b632320 (a, 1)
  For "b", got node 0x61bf8b6323a0 (b, 2)
  For "c", got node 0x61bf8b632420 (c, 3)
Attempting deletion of 'c' from tree with 3 nodes...
Freeing item for node "c" (3)
     After deletion, tree now has 2 nodes.
Cleaning up!
Freeing item for node "a" (1)
Freeing item for node "b" (2)
Freed tree.
```

Additionally, it would generate an `avl.dot` file which can be converted
with [Graphviz](https://graphviz.org/)'s `dot` program to generate
a visual representation of the tree like the following:

![example graphviz output for arguments: a b c](avl-abc.svg?raw=true)

May this proof of concept be useful!
