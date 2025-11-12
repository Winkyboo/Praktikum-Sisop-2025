#!/bin/bash

USERNAME=$1
DOWNLOAD_DIR=~/cloud_storage/downloads/$USERNAME
LOG_FILE=~/cloud_storage/cloud_log.txt
TIME_STAMP=$(date "+%H-%M_%d-%m-%Y")

mkdir -p "$DOWNLOAD_DIR"
echo "Downloading gambar untuk $USERNAME..."
wget -q "https://source.unsplash.com/random?nature" -O "$DOWNLOAD_DIR/$TIME_STAMP.jpg"
echo "$(date '+%Y/%m/%d %H:%M:%S') DOWNLOAD: INFO Downloading gambar untuk $USERNAME" >>"$LOG_FILE"
echo "Gambar berhasil di download!"
