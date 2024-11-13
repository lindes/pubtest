#!/usr/bin/env python

# compute_week-minimal.py - stripped down version of compute_week.py
# Just copy/paste the one function (and the import, if you don't have it
# already) into your circuitpython code.  No further comments or
# anything here, to facilitate compactness; we just copy lines 11
# through 21 (and 9 if needed) below; leave 22 onward out of it:

import time

def iso_week(when=time.time(), as_string=True):
  def p(y):     return int(y + int(y/4) - int(y/100) + int(y/400)) % 7
  def weeks(y): return 52 + (1 if p(y) == 4 or p(y-1) == 3 else 0)
  def woy(y, w):
    if w < 1:           return (y-1, weeks(y-1))
    elif w > weeks(y):  return (y+1, 1)
    else:               return (y, w)
  lt = time.localtime(when)
  y, w = woy(lt.tm_year, int((10+lt.tm_yday-(lt.tm_wday+1))/7))
  if as_string:         return f"{y}-W{w:02}"
  return (y, w)


# Note: the test code is expected only to run on a full Python, not on
# CircuitPython; still, I want to be able to test edits to this, so
# including it here:
if __name__ == '__main__':
  from cwtest import test_iso_week
  test_iso_week(iso_week)
