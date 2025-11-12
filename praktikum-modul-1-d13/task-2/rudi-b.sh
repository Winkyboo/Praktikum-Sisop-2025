#!/bin/bash

LOG="access.log"
CSV="peminjaman_computer.csv"

read -p "masukkan tanggal (Format MM/DD/YYYY):" tanggal
read -p "masukkan IP Address (Format 192.168.1.x):" ip

IFS="/" read -r mm dd yyyy <<< "$tanggal"
cari_tgl="$dd/$mm/$yyyy"
IFS="." read -r a b c komputer <<< "$ip"

nm_pengguna=$(awk -F '.' -v tgl="$cari_tgl"  -v komputer="$komputer" '$1 == tgl && $2 komputer {print$3}' "$CSV")

if [ -z "$nm_pengguna" ]; then
echo "Data yang kamu cari tidak ada."
exit 1
fi

mkdir -p backup/

wkt_sekarang=$(date +"%H%M%S")
nm_file="backup/$(nm_pengguna)_${mm}${dd}${yyyy}_$(wkt_sekarang).log"

awk -v ip="$ip" -v tgl="$dd/$mm/$yyyy"'
$1 == ip && $4 ~ tgl
{
split($0, arr, "\"")
print substr($4, 2) ": " arr[2] " - " $NF
}' "$LOG" > "$nm_file"

echo "Pengguna saat itu adalah $nm_pengguna"
echo "Log Aktivitas $nm_pengguna telah disimpan di $nm_file"
