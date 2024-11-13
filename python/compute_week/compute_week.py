#!/usr/bin/env python

# compute_week - compute the ISO 8601 "week year" and week number
# (See https://en.wikipedia.org/wiki/ISO_week_date for general
# information about what this is all about.)

# This is being written with the intention of having it be possible to
# use in a bit of CircuitPython code, since CircuitPython doesn't have
# (as of this writing, anyway) strftime, where I could just use this:
#    time.strftime("%G-W%V", when)
# So, trying to mimic that in code that'll work on CircuitPython.

import time

def iso_week(when=time.time(), as_string=True):
  """
determine the ISO week number, including week-based year.  Returns as
a string equivalent to strftime("%G-W%V") on systems that support that.
Two examples, from 2024:
  - 2024-01-01 (January 1st) is in week 2024-W01 (as a casual observer
    might expect).
  - 2024-12-31 (December 31st) is in week 2025-W01 (i.e. it's considered
    to be part of week 1 of the following year).

See https://en.wikipedia.org/wiki/ISO_week_date for the theory on all this.

parameters:
  when - a time in unix epoch format (seconds since midnight, January 1st, 1970, UTC)
  as_string - when true (the default), returns a string such as
    "2024-W01" (to indicate week 1 of year 2024, as defined in the ISO
    standard mentioned above); otherwise, returns a tuple, e.g. (2024,
    1) for the same date.
"""

  # Helper functions that do the work under the covers:

  # Next two functions based on this section:
  # https://en.wikipedia.org/wiki/ISO_week_date#Weeks_per_year

  # "returns the day of the week of 31 December" (for a given year)
  # (Note that this is zero-based, but 1 is still Monday, as in the rest
  # of ISO stuff -- so, 0 is Sunday here, except we only check to see if
  # it's 3 or 4 so it doesn't matter.)
  def p(y):
    return int(y + int(y/4) - int(y/100) + int(y/400)) % 7

  def weeks(y):
    p1 = p(y)
    p2 = p(y-1)

    if (p1 == 4) or (p2 == 3):
      return 53
    else:
      return 52

  # ref https://en.wikipedia.org/wiki/ISO_week_date#Calculating_the_week_number_from_an_ordinal_date
  def woy(y, w):
    if w < 1:
      return (y-1, weeks(y-1)) # it's part of last year
    elif w > weeks(y):
      return (y+1, 1) # it's part of next year
    else:
      return (y, w)


  ### With helper functions out of the way, here's the code for iso_week:

  # convert to localtime mode:
  lt = time.localtime(when)

  # The actual (Gregorian) year:
  y1 = lt.tm_year

  doy = lt.tm_yday
  # add 1 because ISO uses 1-7, tm_wday uses 0-6:
  dow = lt.tm_wday + 1

  # our initial approximation of the week number:
  w1 = int((10+doy-dow)/7)

  # use the Gregorian year and approximate week number to figure out the
  # actual ISO values:
  y, w = woy(y1, w1)

  # finally, the iso_week interface, giving a string or a tuple as
  # described above:
  if as_string:
    return f"{y}-W{w:02}"
  # else return the tuple:
  return (y, w)


# Test code used for both compute_week.py and compute_week-minimal.py,
# so just import it:
if __name__ == '__main__':
  from cwtest import test_iso_week
  test_iso_week(iso_week)
