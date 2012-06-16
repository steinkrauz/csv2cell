#!/bin/sh
DO_TEST=Y
gcc csv2cell.c cJSON.c -o csv2cell -lm
if [ "$DO_TEST" == "Y" ] ; then 
 echo
 ./csv2cell test_l.csv test.wdt
 diff -q test.wdt test_l.base
 case $? in
  2) echo "diff failed."
  ;;
  1) echo "Test failed!"
  ;;
  0) echo "Test passed!"
  ;;
 esac
fi

