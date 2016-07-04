#!/usr/bin/env bash

# monitor forever and recursively
inotifywait \
    -mr \
    --format "%w%f" \
    -e close_write . | # notify on file write
        while read f; do
            filename=$(basename $f)
            extension="${filename##*.}" # extract file extension
            if [[ "cpp hpp" =~ $extension ]]; then # if it's a source or a header file
                make -j$(nproc)
            fi
        done
