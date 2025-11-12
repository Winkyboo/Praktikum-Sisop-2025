#!/bin/bash

akses="access.log"

echo "Total req per IP:"
awk '{print $1}' "$akses" | sort | uniq -c | sort -nr

echo "Jumlah tiap status code:"
awk '{print $9}' "$akses" | sort | uniq -c | sort -nr
