#!/usr/bin/env python

# cwtest.py - test code for compute_week{,-minimal}.py

import time, sys

# Note that this test code is expected only to run on a full Python, not
# on CircuitPython.  Also, we pass in the function, since that's
# otherwise not readily accessable to us in an imported file.
def test_iso_week(iso_week):
  successes = failures = 0
  # For an arbitrary bunch of years:
  for year in range(1970, 2070):
    eoy = time.mktime((year,12,31,0,0,0,0,0,0))
    # Computing days before and after the end of the year:
    for offset in range(-10, 10):
      lt = time.localtime(eoy + offset*86400)
      system_iso = time.strftime("%G-W%V", lt)
      our_iso = iso_week(eoy+offset*86400)
      if system_iso != our_iso:
        failures += 1
        error_date = time.strftime("%Y-%m-%d", lt)
        print(f"ERROR for following date: {error_date};")
        print(f"  system says {system_iso}, our code says {our_iso}")
      else:
        successes += 1
  print(f"Testing found {successes} successes, {failures} failures.")
  if failures > 0:
    sys.exit(1)

if __name__ == '__main__':
  print("Sorry, cwtest is meant to be imported.")
  sys.exit(1)
