#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="bytes.txt"
echo "" > log.txt
touch grep.txt
touch s21_grep.txt

for var in -e -i -v -c -l -n -h -s -o
do

          TEST1="$TEST_FILE $var"
          #echo "$TEST1"
          ./s21_grep $var A bytes.txt > s21_grep.txt
          grep $var A bytes.txt > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi

done

for var1 in -i -v -n -h
do
  for var2 in e v c l n h s i
  do
          TEST1="$TEST_FILE $var1$var2"
          #echo "$TEST1"
          grep $var1$var2 A bytes.txt bytes2.txt > grep.txt
          ./s21_grep $var1$var2 A bytes.txt bytes2.txt > s21_grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
	      echo $TEST1
          fi

  done
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

function check() {
    if [ "$COUNTER_FAIL" = "0" ]
    then
        out=0;
    else
	out=1;
    fi
    return $out;
}

check
