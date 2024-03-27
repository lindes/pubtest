// dir.c -- say which direction the stack grows, with extra information.

// inspired by "Low Level Learning"'s video "a strange but powerful
// interview question", at https://www.youtube.com/watch?v=V2h_hJ5MSpY

#include <stdio.h>

size_t tester(int depth, size_t *first)
{
    /* the value of i is never really used, only its address: */
    int i = 0;

    /* if depth is zero, we're done -- return our last position: */
    if(depth == 0)
    {
	return (size_t)&i;
    }
    /* if called with positive depth, set `first`, then flip the sign of
     * depth before we continue recursing */
    else if(depth > 0)
    {
	*first = (size_t)&i;
	depth = -depth;
    }
    /* either way (depth is positive or negative), if we haven't
     * returned because depth=0, recurse, shrinking (making less
     * negative, so we get to zero) the depth flag. */
    return tester(depth + 1, first);
}

int main(int argc, char *argv[])
{
    /* a and b are for two calls to tester (only one of which will
     * recurse), f is for the position value of the first (outermost)
     * call in the recursion chain of the second call (since we can only
     * easily return one thing, and we're returning the innermost value.
     */
    size_t a, b, f;
    int delta;

    /* for a call that won't recurse, we don't need to pass f: */
    a = tester(0, NULL);

    /* then we call again in a way that will recurse, wherein we use f: */
    b = tester(1, &f);

    /* this is the true test of the stack growth, based on a recursive
     * call to tester: */
    if(f > b)
    {
	/* compute positive delta since we have size_t, which is
	 * unsigned, and then just display negative: */
	delta = f - b;
	printf("Stack grows downwards (0x%lx -> 0x%lx, delta=-%d bytes).\n", f, b, delta);
    }
    else
    {
	delta = b - f;
	printf("Stack grows upwards (0x%lx -> 0x%lx, delta=%d bytes).\n", f, b, delta);
    }

    /* but for kicks, we also check to see if two consecutive
     * (non-recursive relative to each other) calls get the same first
     * stack value (they probably will, since they're just called from
     * main the same way, but, who knows?  I figure it could be
     * interesting to check, so here we are): */
    if(a == f)
    {
	printf("Stack positions are consistent across consecutive calls.\n");
    }
    else
    {
	printf("First call got 0x%lx, second call first iteration got 0x%lx\n", a, f);
    }

    return 0;
}
