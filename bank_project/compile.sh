#!/usr/bin/env bash

echo "compiling ..." && \
make -s && \
echo "creating submission archive ..." && \
make -s submission
