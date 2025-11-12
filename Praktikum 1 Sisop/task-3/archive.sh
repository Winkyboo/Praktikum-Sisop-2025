#!/bin/bash

USERNAME=$1
ARCHIVE_DIR=~/cloud_storage/archives/$USERNAME
mkdir -p "$ARCHIVE_DIR"

DOWNLOAD_DIR=~/cloud_storage/downloads/$USERNAME
LOG_FILE=~/cloud_storage/cloud_log.txt
TIME_STAMP=$(date "+%H-%d-%m-%Y")

echo "Mengarsipkan gambar untuk $USERNAME..."
zip -r "$ARCHIVE_DIR/archive_$TIME_STAMP.zip" "$DOWNLOAD_DIR"/*
rm -rf "$DOWNLOAD_DIR"/*
echo "$(date '+%Y/%m/%d %H:%M:%S') ARCHIVE: INFO Archive dibuat untuk $USERNAME" >>"$LOG_FILE"
echo "Archive berhasil dibuat!"
