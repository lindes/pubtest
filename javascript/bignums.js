#!/usr/bin/env node

// bignums.js -- find the biggest number javascript can deal with.

// note: this is probably misleading!  See, e.g.:
// https://stackoverflow.com/questions/4557509/javascript-summing-large-integers
// https://stackoverflow.com/questions/10631494/json-parse-parses-converts-big-numbers-incorrectly
// https://stackoverflow.com/questions/17531062/javascript-not-parsing-large-number-in-json-correctly

// I may or may not bother to do future work to find the edge cases
// systematically.  It's "well understood" where they are, in theory, so
// it's not necessarily a priority.  On the other hand, it could be fun
// to figure out how to find them empirically, so we'll see.  :)

function main() {
    ver = process.version;
    console.log(`The biggest integer your javascript (${ver}) can manage:`, findBiggestNumber());
}

// not sure if this script's code would ever be used for anything other
// than just running it, so just run it:
main()


// core algorithm:
// 1. find the biggest valid "1 followed by zeroes" number
// 2. grow it by upping those zeroes as far as we can
// 3. return it as a string.
function findBiggestNumber() {
    arr = biggestBase();
    growIt(arr);
    return arr.join('')
}

// a limited notion of validity for this purpose: is a number not
// considered by javascript to have reached infinity:
function valid(arr) {
    return parseInt(arr.join('')) !== Infinity;
}

// the biggest number composed of 1 followed by all zeroes that still
// parses as a valid integer.
function biggestBase() {
    // start with that 1
    arr = [1];
    // and an index one place past that (javascript will thankfully
    // auto-grow the array for us):
    idx = 1;
    // keep adding zeroes to the end until we burst validity:
    while(valid(arr)) { arr[idx++] = 0; }
    // then drop back one digit, because we went too far:
    arr.pop()
    // and finally, return what we got -- as an array of single-digit
    // numbers:
    return arr;
}

// and now, we take that 1-and-zeroes number, and increment the zeroes
// to be as big a digit as they can be, in each place.  This could
// probably be done in some faster way with a binary search, but I was
// feeling too lazy to sort out how to do that when we can't just
// subtract a number if we grow too big... so, I just search the space;
// it's not that big, really.
function growIt(arr) {
    // for each number in the array:
    for(idx = 0; idx < arr.length; ++idx) {
	// grow it until it overflows (note: it may do this by getting
	// to "10", i.e. going from a 1-digit number to a 2-digit
	// number, thus increasing the length of the overall number.)
	while(valid(arr)) { ++arr[idx]; }
	// then shrink it back down by one: (and in the grew-to-10 case,
	// it'll shrink back to a 9 here, and the number of digits will
	// be restored.)
	--arr[idx]
    }
}
