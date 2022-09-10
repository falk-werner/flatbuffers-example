#!/bin/bash

which flatc > /dev/null
if [[ "$?" != "0" ]]; then
    echo "error: flatc not found: please install flatbuffers" >&2
    exit 1
fi

flatc --python todolist.fbs