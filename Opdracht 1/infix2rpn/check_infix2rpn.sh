#!/usr/bin/env bash

get_expr() {
    local to_convert="$1" ignore_whitespace="$2" got ret

    got="$($PROG "$to_convert")"
    ret="$?"

    if [[ -n "$ignore_whitespace" ]]; then
        got="$(echo "$got" | sed -e 's/\s\+/ /g' | sed -e 's/^\s\+//g' | sed -e 's/\s\+$//g' )"
        :;
    fi

    echo "$got"
    return "$ret"
}
export -f get_expr

source check_lib.sh

export PROG=./infix2rpn
header "Simple exprs" "checking simple expressions"
check_equal /dev/stdin bash -c "get_expr '3+3' true" <<< '3 3 +'
check_equal /dev/stdin bash -c "get_expr '3+4-5' true" <<< '3 4 + 5 -'
check_equal /dev/stdin bash -c "get_expr '3+3+3+3+3+3' true" <<< '3 3 + 3 + 3 + 3 + 3 +'
check_equal /dev/stdin bash -c "get_expr '3/8' true" <<< '3 8 /'
check_equal /dev/stdin bash -c "get_expr '6*7' true" <<< '6 7 *'
check_equal /dev/stdin bash -c "get_expr '6*7/3*5' true" <<< '6 7 * 3 / 5 *'

header "Hard exprs" "Checking difficult expressions"
check_equal /dev/stdin bash -c "get_expr '1+2*3' true" <<< '1 2 3 * +'
check_equal /dev/stdin bash -c "get_expr '1+7*5/4+6*3' true" <<< '1 7 5 * 4 / + 6 3 * +'
check_equal /dev/stdin bash -c "get_expr '100*10/20' true" <<< '100 10 * 20 /'
check_equal /dev/stdin bash -c "get_expr '1236+722*890/3-675' true" <<< '1236 722 890 * 3 / + 675 -'

header "Tilde" "Checking if tilde operator works"
check_equal /dev/stdin bash -c "get_expr '~2' true" <<< '2 ~'
check_equal /dev/stdin bash -c "get_expr '~2^3' true" <<< '2 ~ 3 ^'

header "Stats" "Checking stack stats"
check_equal_err /dev/stdin bash -c "get_expr '2 + 2' true" <<< 'stats 1 1 1'
check_equal_err /dev/stdin bash -c "get_expr '1+2+3+4+5' true" <<< 'stats 4 4 1'

header "Invalid characters" "Checking if invalid characters are handled."
check_error ./infix2rpn 'ab+cd'
check_error ./infix2rpn '3%5@4'

exit 0  # return succes to makefile
