#!/bin/bash

# count the lines in a.txt (winners of FIFA World Cup)
cat a.txt | ./count > a-cnt.txt
# count the lines in b.txt (runners-up of FIFA World Cup)
cat b.txt | ./count > b-cnt.txt
# reverse the lines of b-cnt.txt and save this as c-cnt.txt
tail -r b-cnt.txt > c-cnt.txt

# for testing, e.g.:
# ./addcount - b-cnt.txt
# ./addcount a-cnt.txt b-cnt.txt
# ./addcount a-cnt.txt c-cnt.txt