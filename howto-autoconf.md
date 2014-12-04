How to set up with autoconf
===========================

A lot of this is taken from
https://www.lrde.epita.fr/~adl/dl/autotools.pdf .

Set up autotools from git (always in the / directory):

    autoreconf --install

Update autotools automatically (always in the / directory):

    autoreconf --install

Test a build:

    mkdir build
    cd build
    ../configure
    make

Create a distribution:

    mkdir build
    cd build
    ../configure
    make distcheck
    make distclean

Install locally:

    mkdir build
    cd build
    ../configure --prefix=$HOME/.local
    make install
    make distclean
