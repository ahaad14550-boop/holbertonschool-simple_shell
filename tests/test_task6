#!/bin/bash
# Task 6: env built-in
SHELL=./hsh
PASS=0
FAIL=0

check() {
    if [ "$1" = "$2" ]; then
        echo "PASS: $3"
        PASS=$((PASS + 1))
    else
        echo "FAIL: $3"
        echo "  GOT:      $1"
        echo "  EXPECTED: $2"
        FAIL=$((FAIL + 1))
    fi
}

result=$(echo "env" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "PATH")" "1" "env prints PATH"
check "$(echo "$result" | grep -c "HOME")" "1" "env prints HOME"
check "$(echo "$result" | grep -c "USER")" "1" "env prints USER"
check "$(echo "$result" | grep -c "=")" "$(env | grep -c "=")" "env format KEY=VALUE"
echo "env" | $SHELL 2>&1
check "$?" "0" "env exit status 0"

echo "PASSED: $PASS | FAILED: $FAIL"
