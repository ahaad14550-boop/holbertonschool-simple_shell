#!/bin/bash
# Test suite for tasks 6, 9, 10, 11, 12, 13
# Author: Ahad Alqahtani <ahaad.14550@gmail.com>

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

echo "========== Task 6: env =========="
result=$(echo "env" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "PATH")" "1" "env prints PATH"
check "$(echo "$result" | grep -c "HOME")" "1" "env prints HOME"
check "$(echo "$result" | grep -c "USER")" "1" "env prints USER"
check "$(echo "$result" | grep -c "=")" "$(env | grep -c "=")" "env format KEY=VALUE"
echo "env" | $SHELL 2>&1
check "$?" "0" "env exit status 0"

echo "========== Task 9: custom getline =========="
result=$(echo "echo Hello" | $SHELL 2>&1)
check "$result" "Hello" "reads single line"
result=$(printf "echo Hello\necho World\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "Hello")" "1" "reads multiple lines"
check "$(echo "$result" | grep -c "World")" "1" "reads second line"
result=$(echo "" | $SHELL 2>&1)
check "$?" "0" "handles empty input"
result=$(printf "echo A\n\necho B\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "A")" "1" "handles empty line between commands"

echo "========== Task 10: no strtok =========="
result=$(echo "echo Hello" | $SHELL 2>&1)
check "$result" "Hello" "single argument"
result=$(echo "echo Hello World" | $SHELL 2>&1)
check "$result" "Hello World" "multiple arguments"
result=$(echo "   echo Hello" | $SHELL 2>&1)
check "$result" "Hello" "leading spaces"
result=$(echo "" | $SHELL 2>&1)
check "$?" "0" "empty input"

echo "========== Task 11: exit with status =========="
echo "exit" | $SHELL 2>&1
check "$?" "0" "exit no args returns 0"
echo "exit 0" | $SHELL 2>&1
check "$?" "0" "exit 0"
echo "exit 98" | $SHELL 2>&1
check "$?" "98" "exit 98"
echo "exit 1" | $SHELL 2>&1
check "$?" "1" "exit 1"
echo "exit 255" | $SHELL 2>&1
check "$?" "255" "exit 255"

echo "========== Task 12: Ctrl+C =========="
$SHELL <<'EOF' &
sleep 10
EOF
PID=$!
sleep 0.5
kill -INT $PID 2>/dev/null
sleep 0.5
kill -0 $PID 2>/dev/null
check "$?" "0" "shell stays alive after Ctrl+C"
kill $PID 2>/dev/null
result=$(printf "" | $SHELL 2>&1)
check "$?" "0" "shell exits normally after signal test"

echo "========== Task 13: setenv and unsetenv =========="
result=$(printf "setenv TEST hello\nenv\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "TEST=hello")" "1" "setenv creates new variable"
result=$(printf "setenv HOME /tmp\nenv\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "HOME=/tmp")" "1" "setenv modifies existing variable"
result=$(printf "setenv\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "setenv")" "1" "setenv no args prints error"
result=$(printf "setenv TEST hello\nunsetenv TEST\nenv\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "TEST=hello")" "0" "unsetenv removes variable"
result=$(printf "unsetenv\n" | $SHELL 2>&1)
check "$(echo "$result" | grep -c "unsetenv")" "1" "unsetenv no args prints error"
result=$(printf "unsetenv NONEXISTENT\n" | $SHELL 2>&1)
check "$?" "0" "unsetenv nonexistent variable"

echo "========================================="
echo "PASSED: $PASS"
echo "FAILED: $FAIL"
echo "TOTAL:  $((PASS + FAIL))"
