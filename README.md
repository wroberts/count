count - UNIX line counting utilities
====================================

Copyright (c) 2014 Will Roberts \<wildwilhelm@gmail.com\>

Homepage: https://github.com/wroberts/count

This project is licensed under the terms of the MIT license (see
LICENSE.md).

Overview
--------

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
its lines in random order.  `shuf` in the
[GNU Coreutils](https://www.gnu.org/software/coreutils/) is faster and
more flexible.

Install
-------

From tarball:

    tar xf count-1.0.tar.gz
    cd count-1.0/
    ./configure
    make install

From github:

    autoreconf --install
    mkdir build
    cd build
    ../configure
    make install

Speed Test
----------

`count` is faster than `sort | uniq -c`, but can use much more memory:

    $ cat BIGFILE | wc
     1653677 21751482 75598346

    $ time (cat BIGFILE | sort | uniq -c > /dev/null)

    real   0m50.933s
    user   0m55.267s
    sys    0m0.347s

    $ time (cat BIGFILE | count > /dev/null)

    real   0m9.233s
    user   0m9.357s
    sys    0m0.453s

Awk Eqiuivalents
----------------

The `count` tools can be replicated with trivial `awk` scripts.
Usually, the compiled binaries are faster.

`count` is equivalent to, though faster than:

    awk '{c[$0]++} END {OFS="\t"; for (x in c) print c[x], x}' | sort -k2

`cat COUNT1 COUNT2 | sortalph` is equivalent to, though faster than:

    awk 'BEGIN{FS=OFS="\t"} {v=$1; $1=""; c[substr($0,2)]+=v} END {for (x in c) print c[x], x}' | sort -k2

`threshcount 2` is equivalent to, but slower than:

    `awk '{if (2 < $1) print $0}'`
