# Makefile for testing libavl

# The main thing we'll build:
PROGRAMS	= avl

# Need to link in libavl:
LDLIBS		= -lavl

# On my system, I've been using pqiv as an image viewer lately... it's
# certainly not the only avaliable choice, though.  You're encouraged to
# set this to whatever other image viewer you have that you like, if you
# don't like and/or have pqiv.
IMGVIEW		= pqiv -i

# generated files to remove:
EXTRAREMOVE	= avl.dot avl-abc.dot avl.png

# the display dependency is optional, but I like to see what it did:
default: test display

# most of the power of this makefile is in here:
include ../../../.makebase

display: avl.png avl-abc.svg
	${IMGVIEW} avl.png avl-abc.svg

# note that we have no target for avl.dot -- we rely on the fact that
# test is part of the default target to get it made.  We do this because
# we don't want to re-generate it if avl was run by hand with
# a different set of arguments.
avl.png: avl.dot
	dot -Tpng -o $@ $<

# To generate avl-abc.dot, we temporarily move any existing avl.dot out
# of the way, make a new avl.dot, move that to avl-abc.dot, and then
# move the original back so other things can be built from it as needed.
avl-abc.dot: avl
	mv -f avl.dot .avl-hold.dot
	./avl a b c
	mv avl.dot avl-abc.dot
	mv -f .avl-hold.dot avl.dot

avl-abc.svg: avl-abc.dot Makefile
	dot -Tsvg -o $@ $<
