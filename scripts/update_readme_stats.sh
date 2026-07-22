#!/usr/bin/env sh
set -eu

README="README.md"
STATS_FILE="$(mktemp)"
OUTPUT_FILE="$(mktemp)"

code_files="$(find src -type f \( -name '*.c' -o -name '*.h' \) | sort)"
code_file_count="$(printf '%s\n' "$code_files" | sed '/^$/d' | wc -l | tr -d ' ')"
code_lines="$(wc -l $code_files | awk 'END { print $1 }')"
project_lines="$(wc -l $code_files makefile | awk 'END { print $1 }')"

{
    printf '%s\n' '<!-- CODEBASE-STATS:START -->'
    printf '%s\n\n' 'Generated with `make stats`.'
    printf '%s\n' '| Metric | Count |'
    printf '%s\n' '| --- | ---: |'
    printf '| C source/header files | %s |\n' "$code_file_count"
    printf '| C source/header lines | %s |\n' "$code_lines"
    printf '| Lines including makefile | %s |\n\n' "$project_lines"
    printf '%s\n' '| File | Lines |'
    printf '%s\n' '| --- | ---: |'

    for file in $code_files makefile; do
        lines="$(wc -l < "$file" | tr -d ' ')"
        printf '| `%s` | %s |\n' "$file" "$lines"
    done

    printf '%s\n' '<!-- CODEBASE-STATS:END -->'
} > "$STATS_FILE"

awk -v stats_file="$STATS_FILE" '
BEGIN {
    while ((getline line < stats_file) > 0) {
        stats = stats line ORS
    }
}
/<!-- CODEBASE-STATS:START -->/ {
    printf "%s", stats
    in_stats = 1
    next
}
/<!-- CODEBASE-STATS:END -->/ {
    in_stats = 0
    next
}
!in_stats {
    print
}
' "$README" > "$OUTPUT_FILE"

mv "$OUTPUT_FILE" "$README"
rm -f "$STATS_FILE"
