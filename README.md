count - UNIX line counting utilities

Copyright (c) 2014 Will Roberts <wildwilhelm@gmail.com>

Homepage: https://github.com/wroberts/count

This project is licensed under the terms of the MIT license.

`count` works similarly to `sort fruit | uniq -c`.  The output is
tab-separated and in alphabetical order.

`addcount` sums two count files produced by `count`, assuming that the
files are sorted in alphabetical order.

`sortalph` takes count data as produced by `count` and sorts it
alphabetically; it can also be used to sum two (or more) count files
together (even if they're not in alphabetical order):

    `cat COUNT1 COUNT2 | sortalph`

`sortnum` is a script that calls `sort -nr`.

`threshcount` reads a count file as produced by `count` and outputs
only those lines whose counts are greater than the given threshold
argument.

`shuffle` is a short Python script which reads in a file and outputs
its lines in random order.
