#!/bin/bash
log_file="$HOME/cloud_storage/cloud_log.txt"
users_file="$HOME/cloud_storage/users.txt"

log_message() {
echo "$(date '+%Y/%m/%d %H:%M:%S') $1" >> "$log_file"
}

if [[ "$1" == "logout" ]]; then
last_login=$(grep "LOGIN: INFO" "$log_file" | tail -n1)
if [[ -n "$last_login" ]]; then
active_usr=$(echo "$last_login" | awk '{print $6}')
log_message "LOGOUT: INFO User $active_usr keluar"
echo "Logout berhasil!"
else
echo "Tidak ada sesi aktif."
fi
exit 0
fi

last_event=$(grep -E "(LOGIN: INFO|LOGOUT: INFO)" "$log_file" | tail -n1)
if [[ "$last_event" == *"LOGIN: INFO"* ]]; then
active_usr=$(echo "$last_login" | awk '{print $6}')
echo "User lain sedang masuk: $active_usr. Sabar ya kids"
exit 1
fi

read -p "Masukkan username: " username
read -s -p "Masukkan password: " password
echo "\n"

if grep -qi "^$username:" "$users_file"; then
stored_password=$(grep -i "^$username:" "$users_file" | cut -d: -f2)
if [[ "$password" == "$stored_password" ]]; then
log_message "LOGIN: INFO User $username masuk"
echo "Login berhasil!"
else
log_message "LOGIN: ERROR Gagal masuk untuk User $username"
echo "Password salah."
exit 1
fi
else
log_message "LOGIN: ERROR Gagal masuk untuk User $username"
echo "Username tidak ditemukan."
exit 1
fi
