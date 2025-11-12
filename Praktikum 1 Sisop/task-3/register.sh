#!/bin/bash
USER_FILE=~/cloud_storage/users.txt
LOG_FILE=~/cloud_storage/cloud_log.txt

read -p "Masukkan username: " username
read -s -p "Masukkan password: " password
echo ""
username_lower=$(echo "$username" | tr '[:upper:]' '[:lower:]')
password_lower=$(echo "$password" | tr '[:upper:]' '[:lower:]')

if grep -i -q "^$username_lower:" "$USER_FILE"; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Username sudah ada" >>"$LOG_FILE"
echo "Error: Username sudah ada"
exit 1
fi
if [[ ${#password} -lt 8 ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password kurang dari 8 karakter" >>"$LOG_FILE"
echo "Error: Password harus minimal 8 karakter"
exit 1
fi
if ! [[ "$password" =~ [A-Z] ]] || ! [[ "$password" =~ [0-9] ]] || ! [[ "$password" =~ [[:punct:]] ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password harus mengandung huruf besar, angka, dan karakter spesial" >>"$LOG_FILE"
echo "Error: Password harus mengandung huruf besar, angka, dan karakter spesial"
exit 1
fi
if [[ "$password" == "$username" ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password tidak boleh sama dengan username" >>"$LOG_FILE"
echo "Error: Password tidak boleh sama dengan username"
exit 1
fi
if [[ "$password_lower" =~ "cloud" ]] || [[ "$password_lower" =~ "storage" ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password tidak boleh mengandung kata 'cloud' atau 'storage'" >>"$LOG_FILE"
echo "Error: Password tidak boleh mengandung kata 'cloud' atau 'storage'"
exit 1
fi
echo "$username:$password" >>"$USER_FILE"
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: INFO Pengguna $username berhasil terdaftar" >>"$LOG_FILE"
echo "Pendaftaran berhasil!"
