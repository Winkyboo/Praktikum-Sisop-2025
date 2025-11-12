#!/bin/bash

LOG_FILE=~/cloud_storage/cloud_log.txt
USERNAME=$(awk '/LOGIN: INFO/ {user=$6} END {print user}' "$LOG_FILE")

if [ -n "$USERNAME" ]; then
echo "Mendownload gambar untuk $USERNAME..."
~/cloud_storage/download.sh "$USERNAME"

echo "Mengarchive gambar untuk $USERNAME..."
~/cloud_storage/archive.sh "$USERNAME"
else
echo "Tidak ada user lain yang sedang login."
fi
