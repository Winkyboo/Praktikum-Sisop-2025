#!/bin/bash

LOG="access.log"

declare -A jml_error
jml_errror["andi"]=0
jml_error["budi"]=0
jml_error["caca"]=0

while read -r line;
do status_code=$(echo "$line" | awk '{print $NF}')

if [[ "$status_code" == "500" ]]; then
ip=$(echo "$line" | awk '{print $1}')
case "$ip" in
"192.168.1.1") jml_error["budi"]=$((jml_error["budi"] + 1)) ;;
"192.168.1.2") jml_error["andi"]=$((jml_error["andi"] + 1)) ;;
"192.168.1.3") jml_error["caca"]=$((jml_error["caca"] + 1)) ;;
esac
fi
done < "$LOG"

trbnyk=""
error_maks=0

for nm in "$!jml_error[@]}"; do
	if (( jml_error["$nm"] > error_maks )); then
maks_error=${jml_error["$nm"]}
trbnyk=$nm
fi
done

if [[ $error_maks -gt 0 ]]; then
echho "status code paling banyak $error_maks oleh $trbnyk."
else
echo "gak ada status code 500"
fi
