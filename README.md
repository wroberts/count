count - UNIX count utilities

Copyright (c) 2014 Will Roberts

This projected is licensed under the terms of the MIT license.

`count` works similarly to `sort fruit | uniq -c`.  The output is
tab-separated and in alphabetical order.

`addcount` sums two count files produced by `count`, assuming that the
files are sorted in alphabetical order.

`sortalph` takes count data as produced by `count` and sorts it
alphabetically; it can also be used to sum two (or more) count files
together (even if they're not in alphabetical order):

    `cat COUNT1 COUNT2 | sortalph`

`sortnum` is a script that calls `sort -nr`.
