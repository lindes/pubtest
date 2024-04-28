/* avl.c - build an AVL tree of our arguments
 * (Use the argument as a key, and argc for that key as the value.)
 *
 * Copyright 2024 by David Lindes.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <avl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* a simple key/value struct with an integer key */
typedef struct {
  char *key;
  int value;
} int_kv;

/* simple wrapper so I can have one-line fatal errors */
void die(char *msg)
{
  perror(msg);
  exit(1);
}

/* ikv - allocate and set up a new int_kv data item with given
 * key and value */
int_kv *ikv(char *k, int v)
{
  int_kv *kv;

  /* we leave it to the caller to do error reporting on this: */
  if(!(kv = calloc(1, sizeof(int_kv))))
    return NULL;

  /* only duplicate key if we got one coming in: */
  if(k)
    kv->key = strdup(k);
  else
    kv->key = NULL;

  kv->value = v;

  /* and indeed, here too, we check to make sure strdup worked, though
   * if it didn't, we free what we had already successfully allocated
   * first, since our caller won't have a way to do so: */
  if(k && !(kv->key))
  {
    free(kv);
    return NULL;
  }

  return kv;
}

/* comparison function for the avl tree */
int ikv_compare(int_kv *a, int_kv *b)
{
  return strcmp(a->key, b->key);
}

/* free function for data items of the avl tree */
void ikv_free(int_kv *ikv)
{
  if(ikv == NULL)
    return;
  /* print out a little debugging info as we go... just to see what
   * order things free in, out of morbid curiosity. */
  printf("Freeing item for node \"%s\" (%d)\n", ikv->key, ikv->value);
  if(ikv->key)
    free(ikv->key);
  free(ikv);
}

/* recursively walk the nodes of a tree, adding to a graphvis "dot" file
 * (created below): */
void dot_walk_node(avl_node_t *node, FILE *dotfile)
{
  int_kv *i, *j;

  if(!node) return;

  i = node->item;

  /* specify the label for this particular node (with both key and
   * value, though the node identifier in dot is the value, since that's
   * guaranteed (as just a non-negative integer) to be a valid name for
   * a node: */
  fprintf(dotfile, "  %d [label=\"%s\\n(%d)\"];\n",
          i->value, i->key, i->value);

  /* now draw edges for left and right nodes, and descend: */

  if(node->left)
  {
    j = node->left->item;
    fprintf(dotfile, "  %d -> %d [label=\"left\"; color=blue; "
            "fontcolor=blue];\n", i->value, j->value);
    dot_walk_node(node->left, dotfile);
  }

  if(node->right)
  {
    j = node->right->item;
    fprintf(dotfile, "  %d -> %d [label=\"right\"; color=red; "
            "fontcolor=red];\n", i->value, j->value);
    dot_walk_node(node->right, dotfile);
  }

  /* don't walk the next and previous edges (they'll be walked by the
   * left and right pointers, instead), just label them, and in
   * different colors: */

  if(node->next)
  {
    j = node->next->item;
    fprintf(dotfile, "  %d -> %d [label=\"next\"; color=magenta; "
            "fontcolor=magenta];\n", i->value, j->value);
  }

  if(node->prev)
  {
    j = node->prev->item;
    fprintf(dotfile, "  %d -> %d [label=\"prev\"; color=cyan; "
            "fontcolor=cyan];\n", i->value, j->value);
  }

}

/* walk the tree, creating a (graphviz) dot file for it: */
void dot_walk(avl_tree_t *tree)
{
  FILE *dotfile;
  int_kv *item;

  if(!tree)
    return;

  if(!(dotfile = fopen("avl.dot", "w")))
    die("fopen avl.dot");

  fprintf(dotfile, "digraph {\n");

  if(tree->top)
  {
    item = tree->top->item;
    fprintf(dotfile, "  tree -> %d [label=\"top\"; color=green; "
            "fontcolor=green];\n", item->value);

    dot_walk_node(tree->top, dotfile);

    /* do these after the recursive walk, because otherwise they influence the
     * left-right ordering of nodes, which I want to be based purely on the
     * left and right pointers: */
    item = tree->head->item;
    fprintf(dotfile, "  tree -> %d [label=\"head\"; color=darkcyan; "
            "fontcolor=darkcyan; constraint=false];\n", item->value);
    item = tree->tail->item;
    fprintf(dotfile, "  tree -> %d [label=\"tail\"; color=darkmagenta; "
            "fontcolor=darkmagenta; constraint=false];\n", item->value);
  }
  else
  {
    fprintf(dotfile, "  tree -> NULL [label=\"top\"; color=green; "
            "fontcolor=green];\n");
    fprintf(dotfile, "  NULL [color=red; fontcolor=red]\n");
  }

  /* the end of the digraph: */
  fprintf(dotfile, "}\n");

  /* this is exceedingly unlikely to fail, assuming a non-networked
   * filesystem, but... it _can_ fail, so, let's check it.  We're
   * consciously skipping checks on fprintf in hopes that this will let
   * us know if those failed??  Not sure if that's true -- I don't know
   * how to get any of these calls to fail in practice.  :D */
  if(fclose(dotfile) == EOF)
    die("fclose failed");
}

/* our main program -- allocate a tree, stuff argv (not including
 * argv[0]) into it (it'll thus get turned into alphabetical order),
 * print it out, make a dotfile from it, and test searching for items in
 * it: */
int main(int argc, char *argv[])
{
  avl_tree_t    *tree;
  int_kv        *item, searchitem;
  avl_node_t    *node;

  /* allocate our tree.  We print out the return value before checking
   * it, because this is just informational output for a test program
   * anyway. */
  tree = avl_alloc_tree((avl_compare_t)ikv_compare, (avl_freeitem_t)ikv_free);
  printf("Got tree %p\n", (void*)tree);
  if(!tree)
    die("avl_alloc_tree");

  /* now we loop over arguments, creating items and then nodes for each: */
  for(int i = 1; i < argc; ++i)
  {
    /* allocate an item to go into the tree, and on success, insert it: */
    item = ikv(argv[i], i);
    if(item)
      node = avl_insert(tree, item);
    else
      die("ikv alloc");

    /* we can fail under two main circumstances: a duplicate key, which
     * we report on but otherwise ignore, or some other error, which we
     * treat as fatal */
    if(!node)
    {
      if(errno == EEXIST)
      {
        printf("WARNING: not inserting duplicate key '%s', "
               "value %d is thus lost.\n", item->key, item->value);
        free(item);
      }
      else
        die("avl_insert");
    }

    printf("Got node for %*s\"%s\" (%2d): %p\n",
      /* %*s and these arguments gives us a variable-width padding
       * string, to allow for the quotes to stick close to the %s
       * argument, while still having right justification of all the
       * keys, for easier readability.  10 is the maximum we're
       * expecting our key length to be: */
      (int)(10-strlen(argv[i])), "",
      argv[i], i, (void*)node);
  }

  /* the avl library gives us two ways to go through the tree -- as
   * a tree, and as an ordered linked-list.  Here we're testing out the
   * latter: */
  printf("Walking tree as linked-list:\n");
  for(node = tree->head; node; node = node->next)
  {
    item = node->item;
    printf("  %10s: %3d\n", item->key, item->value);
  }

  /* we then do a recursive descent down into the tree as a tree,
   * creating a graphviz dot file for it: */
  printf("Walking tree from root, top-first, creating avl.dot.\n");
  dot_walk(tree);

  /* then we test out searching -- note that we _do_ include argv[0]
   * here, as a way to demonstrate a failed search: */
  printf("Now searching for arguments in tree...\n");
  for(int i = 0; i < argc; ++i)
  {
    searchitem.key = argv[i]; /* key is ignored */
    node = avl_search(tree, &searchitem);
    if(node)
    {
      item = node->item;
      printf("  For \"%s\", got node %p (%s, %d)\n",
             argv[i], (void*)node, item->key, item->value);
    }
    else
      printf("  For \"%s\", failed to find node (got %p)\n",
             argv[i], (void*)node);
  }

  /* one other thing to try is deleting something... we'll try deleting
   * the last searched-for item: */
  printf("Attempting deletion of '%s' from tree with %d nodes...\n",
         searchitem.key, avl_count(tree));
  avl_delete(tree, &searchitem);
  printf("     After deletion, tree now has %d nodes.\n",
         avl_count(tree));

  /* and finally, we clean up: */
  printf("Cleaning up!\n");
  avl_free_tree(tree);
  printf("Freed tree.\n");
}
