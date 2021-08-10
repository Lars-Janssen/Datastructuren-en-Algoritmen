#!/bin/bash

declare -r COLORS_SUPPORTED=0
export LIBC_FATAL_STDERR_=2
export MAX_OUTPUT_SIZE=90000
export _ASAN_OK=0 # Have we seen a valgrind error
export _ASAN_ERR
export _ASAN=false
export ASAN_POINTS=-1 # points to add if no valgrind errors were found.
export _TIMEOUT_TIME=6 # The max time a command may run
export LC_NUMERIC="en_US.UTF-8"
export _DONE_SETUP="false"
export _CUR_CAT
export _BAILED=false

_errcho() {
    (>&2 echo "$@")
}

exit_trap() {
    if [[ -n "$_CUR_CAT" ]]; then
        output_cat_grade
    fi
}

trap exit_trap EXIT

# Grant the given number of points. The number should be the first and only
# argument.
grant() {
    ok
}

header() {
    printf -- '\n----------------------------------------------------'
    printf -- '------------------------\n'
    printf "%-70s\n" "$*"
    printf -- '----------------------------------------------------'
    printf -- '------------------------\n'
}

color() {
    if [[ $COLORS_SUPPORTED -eq 1 ]]; then
        printf "\033[1;%sm%s\033[0m" "$1" "${*:2}"
    else
        shift
        printf "%s" "$*"
    fi
}

ok() {
    color 32 "PASS"
    echo
}

fail() {
    color 31 "FAIL"
}

with_timeout () {
    local ret err

    if "$_ASAN"; then
        err="$(mktemp)"

        export ASAN_OPTIONS='exitcode=200:log_path=stderr:detect_leaks=true'
        timeout -k "$_TIMEOUT_TIME" "$_TIMEOUT_TIME" "$@" 2> "$err" >/dev/null;

        if [[ "$?" -eq 200 ]]; then
            _ASAN_OK=1
            echo "Error at command $*" >> "$_ASAN_ERR"
            cat "$err" >> "$_ASAN_ERR"
        fi

        rm "$err"
        export ASAN_OPTIONS='exitcode=200:log_path=stderr:detect_leaks=false'
    fi

    timeout -k "$_TIMEOUT_TIME" "$_TIMEOUT_TIME" "$@" | head -c $MAX_OUTPUT_SIZE
    ret="${PIPESTATUS[0]}"

    if [[ "$ret" -eq 200 ]]; then
        _ASAN_OK=1
    fi

    return "$ret"
}

check_error() {
    local extra="$1" outfile ret
    # shift

    if "$_BAILED"; then
        return
    fi

    outfile="$(mktemp)"

    with_timeout "$@" > "$outfile" 2>&1
    ret="$?"
    if [[ "$ret" -ne 0 ]] && [[ "$ret" -ne 124 ]] && [[ "$ret" -ne 200 ]]; then
        [[ "$extra" != "0" ]] && grant "$extra"
    else
        fail
        echo "Command: $*"
        echo 'Error output:'
        cat "$outfile"
        echo
    fi
    rm "$outfile"
    return "$ret"
}

# The command is not eval'd so you can only pass things that you can pass
# behind timeout.
# For a successful test the command should return 0 and its
# output should be equal to the reference output.
# The return value of this function is the same as of the test command.
check_equal() {
    local todiff="$1" diffout errout outfile ret
    shift

    diffout="$(mktemp)"
    errout="$(mktemp)"
    outfile="$(mktemp)"

    with_timeout "$@" > "$outfile" 2> "$errout"

    ret="$?"

    if [[ "$ret" -eq 0 ]]; then
        diff -y "$outfile" "$todiff" > "$diffout"
        ret="$?"
        if [[ "$ret" -eq 0 ]]; then
            ok
        else
            fail
            echo "Command: $*"
            echo "Differences (left is your output, right is the reference):"
            cat "$diffout"
            echo
            echo "Output on stderr:"
            cat "$errout"
        fi
    elif [[ "$ret" -gt 100 ]]; then
        fail
        echo "Command: $*"
        echo "There was a timeout or asan error. Output:"
        cat "$outfile"
        echo
        echo "Output on stderr:"
        cat "$errout"
    else
        echo
        fail
        echo "Command: $*"
        echo "You're command exit with an error code, not diffing. Output:"
        cat "$outfile"
        echo
        echo "Output on stderr:"
        cat "$errout"
    fi

    rm "$outfile"
    rm "$diffout"
    rm "$errout"
    return "$ret"
}

check_equal_err() {
    local todiff="$1" diffout errout outfile ret
    shift

    diffout="$(mktemp)"
    errout="$(mktemp)"
    outfile="$(mktemp)"

    with_timeout "$@" > "$outfile" 2> "$errout"

    ret="$?"

    if [[ "$ret" -eq 0 ]]; then
        diff -y "$errout" "$todiff" > "$diffout"
        ret="$?"
        if [[ "$ret" -eq 0 ]]; then
            ok
        else
            fail
            echo "Command: $*"
            echo "Differences (left is your output, right is the reference):"
            cat "$diffout"
            echo
            echo "Output on stderr:"
            cat "$errout"
        fi
    elif [[ "$ret" -gt 100 ]]; then
        fail
        echo "Command: $*"
        echo "There was a timeout or asan error. Output:"
        cat "$outfile"
        echo
        echo "Output on stderr:"
        cat "$errout"
    else
        echo
        fail
        echo "Command: $*"
        echo "You're command exit with an error code, not diffing. Output:"
        cat "$outfile"
        echo
        echo "Output on stderr:"
        cat "$errout"
    fi

    rm "$outfile"
    rm "$diffout"
    rm "$errout"
    return "$ret"
}


# Begin asan testing.
start_asan() {
    export ASAN_OPTIONS='exitcode=200:log_path=stderr:detect_leaks=false'
    _ASAN_ERR="$(mktemp)"
    export _ASAN_ERR
    export _ASAN=true
    export _ASAN_OK=0
    return 0
}

# Finalize the valgrind tests. The return code is 0 if no valgrind test failed
# and 1 otherwise.
finalize_asan() {
    local ret=2

    if ! "$_BAILED"; then
        if [[ "$_ASAN_OK" -eq 0 ]]; then
            grant "$ASAN_POINTS"
            ret=0
        else
            not_grant "$ASAN_POINTS"
            echo 'Asan output:'
            cat "$_ASAN_ERR"
            ret=1
        fi
    fi

    export _ASAN=false
    if [[ -n "$_ASAN_ERR" ]]; then
        rm "$_ASAN_ERR"
    fi
    unset _ASAN_ERR

    return "$ret"
}
