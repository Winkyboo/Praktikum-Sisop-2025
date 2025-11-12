# Laporan Resmi Praktikum Modul 1 
Kami dari kelas Sistem Operasi D, kelompok D13 yang beranggotakan:
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241089 | Rhea Debora Sianturi |
| 5025241096 | Willy Marcelius |
| 5025241113 | Rafa Huga Nirando |
| 5025241124 | Melvan Hapianan Allo Ponglabba |

Berikut hasil pengerjaan Praktikum Modul 1 Sistem Operasi:

### 1️⃣Task 1 (Top Global New Jeans)
Setelah comeback besar-besaran, NewJeans memecahkan berbagai rekor di chart musik global. Oleh karena itu, muncul lah niat Minji untuk menganalisis performa album mereka selama 2024. Manajer Minji memberikan kepada Minji data streaming dari platform “N” melalui link newjeans_analysis.zip. Data tersebut mencakup data streamer dan detail album NewJeans. Sebagai penggemar NewJeans dan juga seorang suhu dalam dunia Linux, kamu ditantang oleh Minji untuk memenuhi rasa ingin tahunya. Minji ingin kamu untuk menganalisis data yang diberikan oleh manajernya menggunakan shell scripting dan awk. Sebagai Bunnies yang baik, tentu kamu tidak bisa menolak tantangan ini. Adapun analisis yang diminta oleh Minji adalah sebagai berikut:

Soal:

a. Minji penasaran dengan nama-nama user yang mendengarkan lagunya pada platform “N”. Berhubung Minji sangat menyukai angka 2 dan ia sangat membenci orang yang membuat username menggunakan underscore (_), bantulah Minji untuk mendapatkan username yang sesuai dengan kriterianya (terdapat angka 2 dan tidak terdapat underscore) dan tampilkan jumlahnya. Pastikan username yang ditampilkan berurutan berdasarkan ASCII Table (dari kecil ke besar).

Input:
```bash
awk -F, 'NR>1 && $2 ~/2/ && $2 !~ /_/ {print $2}' DataStreamer.csv | sort
awk -F, 'NR>1 && $2 ~/2/ && $2 !~ /_/ {count++} END {print "\nJumlah username yang sesuai:", count}' DataStreamer.csv
```
Output:

![image](https://github.com/user-attachments/assets/eaf3c3c1-2a23-4fe2-8d22-913aa6bea4c9)
![image](https://github.com/user-attachments/assets/e73f1e80-bb3c-46c9-9c7a-44a146528b31)
Penjelasan:
- ``awk`` merupakan salah satu command dalam linux yang digunakan untuk memanipulasi data dan pemrosesan pola. Ini mencari satu atau beberapa file untuk melihat apakah file tersebut berisi baris yang cocok dengan pola yang ditentukan
- ``-F,`` merupakan suatu command dalam ``awk`` sebagai seperator atau pemisah antar kolom dalam suatu file. Pada file csv, pemisah antar kolomnya adalah ","
- ``'NR>1`` merupakah sebuah pola yang digunakan untuk melakukan scanning pada file setelah baris yang pertama. Pada kondisi ini, kita tidak melakukan scanning di baris pertama file yang merupakan sebuah header file
- ``&& $2 ~/2/ && $2 !~ /_/`` merupakan sebuah pola untuk mencari baris yang mengandung nomor "2" dan tidak mengandung underscore "_" pada kolom kedua file
- ``{print $2}'`` merupakan suatu perintah untuk mencetak isi kolom kedua dari suatu baris yang sebelumnya telah difilter sesuai kebutuhan
- ``DataStreamer.csv`` merupakan nama file yang dieksekusi oleh command ``awk``
- ``sort`` merupakan suatu command dalam linux yang digunakan untuk mengurutkan output dari file secara ASCII
- ``{count}`` merupakan suatu perintah untuk menghitung jumlah output yang ditampilkan dari hasil penfilteran tadi
- ``END {print "\n Jumlah username yang sesuai:", count}'``"END" sendiri merupakan command dalam awk untuk mengeksekusi tindakan yang ditentukan dalam braces dalam END sebelum keluar. Dalam kasus ini, output akan menampilkan teks dalam "print" dan juga hasil akhir variabel count


b. Ternyata, Minji tidak hanya menyukai angka 2, tetapi semua angka. Ia ingin mengetahui lagu apa yang paling banyak di-streaming oleh user yang memiliki username berangka. Beri tahu Minji nama lagu tersebut dan jumlah usernya. Jika jumlah user tersebut tidak mencapai 24, maka berikan pesan kepada Minji “Maaf, Minji, total streamingnya tidak sesuai harapan :(“. Jika jumlah user mencapai atau melebihi 24, berikan pesan kepada Minji “Keren, Minji! Kamu hebat <3!”.

Input:
```bash
awk -F, 'NR>1 && $2 ~ /[0-9]/ {count[$3]++}
END{
countmax=0;
songmax="";
for(song in count){
if(count[song]>countmax){
countmax=count[song];
songmax=song;
}
}
print "Lagu yang paling banyak di-streaming oleh user yang memiliki username berangka: ", songmax;
print "Jumlah usernya: ", countmax;
if(countmax>24){
print "Keren, Minji! Kamu hebat <3";
}
else{
print "Maaf, Minji, total streamingnya tidak sesuai harapan :(";
}
}
' DataStreamer.csv
```

Output:
![image](https://github.com/user-attachments/assets/9613a480-fd65-4f4d-a8ee-5c412b881750)

Penjelasan:
- ``awk`` merupakan salah satu command dalam linux yang digunakan untuk memanipulasi data dan pemrosesan pola. Ini mencari satu atau beberapa file untuk melihat apakah file tersebut berisi baris yang cocok dengan pola yang ditentukan
- ``-F,`` merupakan suatu command dalam ``awk`` sebagai seperator atau pemisah antar kolom dalam suatu file. Pada file csv, pemisah antar kolomnya adalah ","
- ``NR>1 && $2 ~ /[0-9]/ {count[$3]++}`` merupakan sebuah pola untuk mencari baris yang mengandung angka (0-9) pada kolom kedua dari file, lalu menghitung berapa kali nilai unik di kolom ketiga muncul, tetapi hanya jika kolom kedua mengandung angka
- ``END{
countmax=0;
songmax="";
for(song in count){
if(count[song]>countmax){
countmax=count[song];
songmax=song;
}
}`` "END" sendiri merupakan command dalam awk untuk mengeksekusi tindakan yang ditentukan dalam braces dalam END sebelum keluar. Dalam kasus ini, ada 2 variabel yang dibuat yakni countmax (untuk mendapatkan jumlah total streaming user pada lagu yang terbanyak) dan songmax (judul lagu yang distreaming paling banyak), Setelah melakukan iterasi pada setiap elemen dalam array count, dilakukan pengecekan menggunakan pernyataan "if(count[song] > countmax)", di mana jika jumlah streaming dari suatu lagu "(count[song])" lebih besar dari "countmax" yang tersimpan sebelumnya, maka nilai "countmax" akan diperbarui dengan jumlah streaming lagu tersebut. Selain itu, "songmax" juga diperbarui dengan nama lagu yang memiliki jumlah streaming terbanyak sejauh ini. Dengan demikian, setelah iterasi selesai, "songmax" akan berisi judul lagu dengan jumlah streaming terbanyak, dan "countmax" akan berisi jumlah streaming tertinggi yang telah ditemukan dalam dataset
- `` print "Lagu yang paling banyak di-streaming oleh user yang memiliki username berangka: ", songmax;
print "Jumlah usernya: ", countmax;`` Sintaks bagian ini adalah bagian untuk menampilkan hasil dari lagu yang paling banyak distreaming yang disimpan di variabel songmax dan juga jumlah user yang melakukan streaming di lagu itu yang tersimpan di variabel countmax
- ``if(countmax>24){
print "Keren, Minji! Kamu hebat <3";
}
else{
print "Maaf, Minji, total streamingnya tidak sesuai harapan :(";
}
}
'`` Pada bagian ini, diberikan percabangan untuk mengeksekusi blok kode yang berbeda berdasarkan suatu kondisi. Dalam kasus ini, percabangan if else digunakan untuk mengeksekusi blok kode yang bergantung pada jumlah "countmax", seperti yang diminta di soal, apabila jumlah streaming lebih dari 24 maka akan menampilkan output "Keren, Minji! Kamu hebat <3", namun apabila jumlah streaming kurang dari sama dengan 24 maka akan menampilkan output "Maaf, Minji, total streamingnya tidak sesuai harapan :("
- ``DataStreamer.csv`` merupakan nama file yang dieksekusi oleh command ``awk``


c. Tiba-tiba Minji lupa, di album mana lagu yang paling banyak di-streaming tersebut berada. Carikan Minji nama album dari lagu yang paling banyak di-streaming di platform tersebut, beserta tahun rilisnya!

Input:
```bash
awk -F, '$2 == "Cookie" {print "Lagu : ", $2, "\nAlbum: ", $1, "\nTahun rilis: ", $3}' AlbumDetails.csv
```

Output:
![image](https://github.com/user-attachments/assets/93afcdfc-d1e2-413e-b102-25c1c49fefff)
Penjelasan:
- ``awk`` merupakan salah satu command dalam linux yang digunakan untuk memanipulasi data dan pemrosesan pola. Ini mencari satu atau beberapa file untuk melihat apakah file tersebut berisi baris yang cocok dengan pola yang ditentukan
- ``-F,`` merupakan suatu command dalam ``awk`` sebagai seperator atau pemisah antar kolom dalam suatu file. Pada file csv, pemisah antar kolomnya adalah ","
- ``'$2 == "Cookie" {print "Lagu : ", $2, "\nAlbum: ", $1, "\nTahun rilis: ", $3}'`` merupakan sebuah pola untuk mencari baris yang sesuai dengan kata "Cookie" pada kolom kedua dari file, lalu menampilkan output tersebut, setelah itu menampilkan output dari kolom pertama yakni nama album dari lagu "Cookie", dan yang terakhir menampilkan output dari kolom ketiga yakni tahun rilis dari lagu "Cookie", dimana tiap output dari tiap kolomnya ditampilkan di baris yang berbeda karena "\n" yang merupakan new line character 
- ``AlbummDetails.csv`` merupakan nama file yang dieksekusi oleh command ``awk``
- (Dikarenakan ini merupakan lanjutan soal yang b, jadi kita bisa menganggap bahwa kita sudah tahu lagu yang paling banyak distreaming yakni lagu "Cookie", sehingga di soal c ini kita tinggal menampilkan lagu "Cookie" saja beserta dengan album dan tahun rilisnya)

d. Minji ingin mengajukan ke agensi untuk membuat campaign iklan supaya lagu-lagu NewJeans lebih dikenal, tapi Minji bingung mau optimasi iklan untuk device apa. Berikut tugas kamu untuk membantu Minji menyusun proposal iklan sebelum diberikan ke CEO ADOR:

    Hitung berapa banyak pengguna yang menggunakan setiap jenis device (Tablet, Smart TV, Desktop, Mobile) untuk streaming lagu NewJeans.

    Hitung total durasi streaming (detik) untuk setiap device.

    Tentukan:
        Device paling populer yang punya user terbanyak.
        Device menang durasi yang memiliki total streaming tertinggi.
        Device ter-“loyal” berdasarkan rasio (total durasi / user) tertinggi.

Input:
```bash
tr -d '\r' < DataStreamer.csv | awk -F',' '
NR>1 {DeviceCount[$7]++; DeviceStream[$7] += $4}
END {
    maxDevice = "";
    maxCount = 0;
    maxStream = "";
    streamCount = 0;
    maxloyal = 0;
    loyal = 0;
    loyaldevice = "";

    for (Device in DeviceCount) {
        loyal = DeviceStream[Device] / DeviceCount[Device];
        if (maxloyal < loyal) {
            maxloyal = loyal;
            loyaldevice = Device;
        }
        if (DeviceCount[Device] > maxCount) {
            maxCount = DeviceCount[Device];
            maxDevice = Device;
        }
        if (DeviceStream[Device] > streamCount) {
            maxStream = Device;
            streamCount = DeviceStream[Device];
        }
        print "Device Name:", Device, "|Users:", DeviceCount[Device], "|Stream Time:", DeviceStream[Device];
    }

    print "Most used device by user count is", maxDevice, "with", maxCount, "users.";
    print "Most used device by stream time is", maxStream, "with", streamCount, "[Time Unit]";
    print "And the most loyal device is", loyaldevice, "with an average stream time of", maxloyal, "[Time Unit] per device";
}'
```
 
Output:
![image](https://github.com/user-attachments/assets/ee6d8357-e8cd-4531-8c60-0f516e16cc4d)
Penjelasan:
- ``tr -d '\r' < DataStreamer.csv`` berfungsi untuk menghapus semua karakter "\r" dari file ``DataSreamer.csv``, karena karakter \r tidak bisa dibaca dengan benar oleh linux dan dapat menyenbabkan error
- ``| awk`` meneruskan (pipe) file ``DataSreamer.csv`` yang sudah dibersihkan kedalam command ``awk`` untuk kita proses lebih lanjut
- ``awk -F',' ' NR>1 {DeviceCount[$7]++; DeviceStream[$7] += $4}`` menetapkan tanda koma (,) sebagai pembatas kolom dengan ``-F','``, lalu menyuruh command awk untuk melewati baris pertama (karena isinya judul kolom) dengan ``NR>1``, serta membuat 2 array, dimana array ``DeviceCount[$7]++`` berisi variable count yang terus ditambahi, untuk setiap data unik di kolom 7 (yang isinya tipe device) untuk menghitung jumlah user tiap device, dan ``DeviceStream[$7] += $4`` yang berguna untuk menambahkan variable dari kolom ke 4 (yang isinya streamtime tiap user) untuk setiap deivce unik dari kolom 7 untuk menghitung streamtime tiap device.
- ``END {..}`` berguna untuk memproses data sekali lagi sebelum output dikeluarkan, dimana didalam command ``END`` ini kita mengaplikasikan algoritma sederhana (for looping) untuk mencari device yang memiliki nilai paling tinggi dalam 3 kategori, yaitu
  1. ``maxDevice`` untuk device dengan user paling banyak, dan ``maxCount`` untuk jumlah user ``maxDevice``
  2. ``masStream`` untuk device dengan streamtime paling tinggi , dan ``streamCount`` untuk streamtime ``maxStream``
  3. ``loyaldevice`` untuk device dengan loyalitas paling tinggi, dan ``maxloyal`` untuk loyalitas ``loyaldevice``
- ``for (Device in DeviceCount) { ...  print "Device Name:", Device, "|Users:", DeviceCount[Device], "|Stream Time:", DeviceStream[Device]; ...)`` untuk mengeluarkan output statistik dari tiap device yang ada (Nama device, Jumlah User device, dna juga Stream time tiap device)
- Terakhir, setelah for loop selesai menemukan device tertinggi di tiap kategori, kita mengeluarkan spesifikasi dari device tertinggi di masing masing kategori.



### 2️⃣Task 2 (Liburan Bersama Rudi)
Mengisi waktu liburan, Rudi membuat sebuah website untuk personal brandingnya yang sementara berjalan di local pada komputer laboratorium. Rudi yang baru belajar kebingungan karena sering muncul error yang tidak ia pahami. Untuk itu dia meminta ketiga temannya Andi, Budi, dan Caca untuk melakukan pengecekan secara berkala pada website Rudi. Dari pengecekan secara berkala, Rudi mendapatkan sebuah file access.log yang berisi catatan akses dari ketiga temannya. Namun, Rudi masih tidak paham cara untuk membaca data pada file tersebut sehingga meminta bantuanmu untuk mencari data yang dibutuhkan Rudi.

Soal:

a. Karena melihat ada IP dan Status Code pada file access.log. Rudi meminta praktikan untuk menampilkan total request yang dibuat oleh setiap IP dan menampilkan jumlah dari setiap status code.

Input:
```bash
#!/bin/bash

akses="access.log"

echo "Total req per IP:"
awk '{print $1}' "$akses" | sort | uniq -c | sort -nr

echo "Jumlah tiap status code:"
awk '{print $9}' "$akses" | sort | uniq -c | sort -nr
```
Output:
![image](https://github.com/user-attachments/assets/8877c3da-aa57-4076-b9bf-cd754d13ea59)
Penjelasan:
-``akses="access.log"`` untuk menyimpan file log dalam variabel akses
-``echo "Total req per IP:"
awk '{print $1}' "$akses" | sort | uniq -c | sort -nr`` mencetak dan menghitung jumlah total request tiap IP, ``awk '{print$1}'`` untuk mengambil kolom pertama (IP Address), ``sort`` mengurutkan IP Address agar IP yang sama jadi berdekatan, ``uniq -c`` menghitung munculnya tiap IP Adresss, ``sort -nr`` mengurutkan hasil dari yang terbesar ke yang terkecil.
-``echo "Jumlah tiap status code:"
awk '{print $9}' "$akses" | sort | uniq -c | sort -nr``, untuk mencetak dan menghitung jumlah tiap status code, hampir sama dengan yang sebelumnya, hanya berbeda di ``{print$9}`` yang merupakan status code

b. Karena banyaknya status code error, Rudi ingin tahu siapa yang menemukan error tersebut. Setelah melihat-lihat, ternyata IP komputer selalu sama. Dengan bantuan peminjaman_komputer.csv, Rudi meminta kamu untuk membuat sebuah program bash yang akan menerima inputan tanggal dan IP serta menampilkan siapa pengguna dan membuat file backup log aktivitas, dengan format berikut:

Tanggal (format: MM/DD/YYYY)

IP Address (format: 192.168.1.X, karena menggunakan jaringan lokal, di mana X adalah nomor komputer)

Setelah pengecekan, program akan memberikan message pengguna dan log aktivitas dengan format berikut:

Pengguna saat itu adalah [Nama Pengguna Komputer]
Log Aktivitas [Nama Pengguna Komputer]
atau jika data tidak ditemukan:

Data yang kamu cari tidak ada
File akan disimpan pada directory “/backup/[Nama file]”, dengan format nama file sebagai berikut

[Nama Pengguna Komputer]_[Tanggal Dipilih (MMDDYYY)]_[Jam saat ini (HHMMSS)].log
Format isi log

[dd/mm/yyyy:hh:mm:ss]: Method - Endpoint - Status Code

Input:
```bash
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
```
Output:
![image](https://github.com/user-attachments/assets/434f6e08-55ee-4705-bcef-89c305c46ff4)
Penjelasan:
-``



c. Rudi ingin memberikan hadiah kepada temannya yang sudah membantu. Namun karena dana yang terbatas, Rudi hanya akan memberikan hadiah kepada teman yang berhasil menemukan server error dengan Status Code 500 terbanyak. Bantu Rudi untuk menemukan siapa dari ketiga temannya yang berhak mendapat hadiah dan tampilkan jumlah Status Code 500 yang ditemukan

Input:
```bash
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
```
Output:

### 3️⃣Task 3 (Ignatius Si Cloud Engineer)
Ignatius ingin mengembangkan sebuah sistem Cloud Storage Otomatis untuk mengelola penyimpanan file secara terintegrasi setelah pengguna berhasil login. Sistem ini harus menyediakan mekanisme register dan login untuk memastikan keamanan akses. Serta secara otomatis mengunduh gambar dari internet ketika pengguna berhasil login, tentunya layanan Cloud Storage juga memberikan otomasi backup gambar dalam format terkompresi dengan nama yang disesuaikan berdasarkan waktu pembuatan.

Soal:

a. Login dan Register Sistem harus memastikan hanya pengguna terdaftar yang dapat mengakses layanan. Proses registrasi dilakukan melalui script register.sh dan data pengguna yang berhasil didaftarkan disimpan di /cloud_storage/users.txt. Proses login dilakukan melalui script login.sh dan semua aktivitas login atau register dicatat dalam file cloud_log.txt.

Data pengguna disimpan dalam: /cloud_storage/users.txt

Kriteria Password:

Minimal 8 karakter.
Mengandung setidaknya satu huruf kapital, satu angka, dan satu karakter spesial (misalnya: @, #, $, dll.).
Password tidak boleh sama dengan username.
Tidak boleh mengandung kata "cloud" atau "storage".
Pencatatan Log : Semua aktivitas dicatat dalam cloud_log.txt dengan format: YY/MM/DD hh:mm:ss MESSAGE Contoh pesan log:

Jika pengguna mencoba register dengan username yang sudah ada:

YY/MM/DD hh:mm:ss REGISTER: ERROR User already exists
Jika pengguna mencoba register tetapi tidak sesuai dengan kriteria password:

YY/MM/DD hh:mm:ss REGISTER: ERROR {Penyebab Error}
Jika register berhasil:

YY/MM/DD hh:mm:ss REGISTER: INFO User registered successfully
Jika login gagal:

YY/MM/DD hh:mm:ss LOGIN: ERROR Failed login attempt on user {USERNAME}
Jika login berhasil:

YY/MM/DD hh:mm:ss LOGIN: INFO User {USERNAME} logged in
Jika logout berhasil:

YY/MM/DD hh:mm:ss LOGOUT: INFO User {USERNAME} logged out
Catatan: Sistem hanya mengizinkan satu pengguna login pada satu waktu. Jika sudah ada pengguna aktif berdasarkan log, login dari pengguna lain tidak diproses sampai sesi sebelumnya berakhir (User yang sedang login melakukan logout).

### File Register.sh
```bash
#!/bin/bash

# Mendefinisikan tempat penyimpanan user dan log, supaya tiap kali dipanggil tidak perlu panjang"
USER_FILE=~/cloud_storage/users.txt
LOG_FILE=~/cloud_storage/cloud_log.txt

# Memulai proses login dengan mendapatkan input dari user, dengan command read
# -p untuk menambah placeholder, dan -s pada password supaya password tidak terlihat (rahasia hehe)
# Menambah echo agar pindah ke baris baru
read -p "Masukkan username: " username
read -s -p "Masukkan password: " password
echo ""

# Membuat variable baru, dimana username dan password diubah menjadi lowercase (pake tr), untuk mempermudah pemrosesan data nantinya
username_lower=$(echo "$username" | tr '[:upper:]' '[:lower:]')
password_lower=$(echo "$password" | tr '[:upper:]' '[:lower:]')

# Membuat case case kesalahan pada proses register serta output" nya sesusai urutan kepentingan
# username sudah ada -> panjang password kurang dari 8 -> password tidak mengandung huruf besar, atau angka, atau karakter spesial -> password sama dengan username -> password mengadung kata cloud atau storage

# 1. Mengecek jika username yang diinput sudah ada di dalam file user.txt, dengan grep -i supaya case insensitive (Ricard sama dengan ricard), dan -q supaya output tidak dikeluarkan
# Jika kondisi terpenuhi, catat tanggal dan waktunya, masukkan ke log, output ke terminal, dan keluar dari program
if grep -i -q "^$username_lower:" "$USER_FILE"; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Username sudah ada" >>"$LOG_FILE"
echo "Error: Username sudah ada"
exit 1
fi

# 2. Mengecek jika panjang password < 8
# ${#password} = panjang password 
# -lt 8 = kurang dari 8
# Jika kondisi terpenuhi, catat tanggal dan waktunya, masukkan ke log, output ke terminal, dan keluar dari program
if [[ ${#password} -lt 8 ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password kurang dari 8 karakter" >>"$LOG_FILE"
echo "Error: Password harus minimal 8 karakter"
exit 1
fi

# 3. Mengecek jika password tidak memiliki semua berikut: (huruf besar, angka, dan karakter spesial)
# "$password" =~ [A-Z] , "$password" =~ [0-9], "$password" =~ [[:punct:]] mengecek apakah password punya karakter A-Z, 0-9, dan setidaknya 1 karakter spesial
# ! berarti tidak, jadi mengecek jika password tidak mengandung elemen tersebut
# || berarti atau
# Jika kondisi terpenuhi, catat tanggal dan waktunya, masukkan ke log, output ke terminal, dan keluar dari program
if ! [[ "$password" =~ [A-Z] ]] || ! [[ "$password" =~ [0-9] ]] || ! [[ "$password" =~ [[:punct:]] ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password harus mengandung huruf besar, angka, dan karakter spesial" >>"$LOG_FILE"
echo "Error: Password harus mengandung huruf besar, angka, dan karakter spesial"
exit 1
fi

# 4. Mengecek jika password sama dengan username
# == operator untuk mengecek kesamaan persis (comparison)
# Jika kondisi terpenuhi, catat tanggal dan waktunya, masukkan ke log, output ke terminal, dan keluar dari program
if [[ "$password" == "$username" ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password tidak boleh sama dengan username" >>"$LOG_FILE"
echo "Error: Password tidak boleh sama dengan username"
exit 1
fi

# 5. Mengecek jika versi lowercase password mengandung kata "cloud" ataupun "storage"
# =~ operator mengecek pola, tidak perlu sama persis
# || berarti atau
# Jika kondisi terpenuhi, catat tanggal dan waktunya, masukkan ke log, output ke terminal, dan keluar dari program
if [[ "$password_lower" =~ "cloud" ]] || [[ "$password_lower" =~ "storage" ]]; then
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Password tidak boleh mengandung kata 'cloud' atau 'storage'" >>"$LOG_FILE"
echo "Error: Password tidak boleh mengandung kata 'cloud' atau 'storage'"
exit 1
fi

# Jika semua case error sudah dilalui, maka register berhasil
# Masukkan username dan password yang baru diinput kedalam users.txt (via user file)
# Lalu catat tanggal dan waktunya, masukkan ke log, output ke terminal, dan program selesai (Yeay)
echo "$username:$password" >>"$USER_FILE"
echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: INFO Pengguna $username berhasil terdaftar" >>"$LOG_FILE"
echo "Pendaftaran berhasil!"
```
### Output ``Register.sh``
1. Username sudah ada<br>
![Screenshot 2025-03-28 190933](https://github.com/user-attachments/assets/1a04bfac-c2e4-45e6-a8c7-e67ee98e774b)
2. Password kurang dari 8 [input = apalah]<br>
![image_2025-03-28_191331743](https://github.com/user-attachments/assets/6c33a3b5-83a8-4b7b-a622-123b72c776db)
3. Password tidak memiliki semua ketentuan berikut (Huruf kapital, angka, karakter spesial) [input = Apalahhh]<br>
![image](https://github.com/user-attachments/assets/f1d070ba-44c8-4d61-b755-04e651fba353)
4. Password sama dengan username [input = William1?]<br>
![image](https://github.com/user-attachments/assets/67e56668-277b-4e62-955c-9a7eb1e36ef8)
5. Password mengandung kata "cloud" atau "storage" [input = Cloud!0g]<br>
![image](https://github.com/user-attachments/assets/596b5822-61ee-48d9-a415-4594ce5dd43d)
6. Register Berhasil [input = Semoga100!]<br>
![image](https://github.com/user-attachments/assets/7fb72422-0988-4bb6-8ccd-92ceb3ef2152)
7. Output ``Register.sh`` di file ``cloud_log.txt``<br> (Log downloadnya diabaikan saja hehe lupa logout)
![image](https://github.com/user-attachments/assets/b530afe7-9638-4e37-bba7-becada941694)

### File ``Login.sh``
```bash
#!/bin/bash

# Mendefinisikan tempat penyimpanan user dan log, supaya tiap kali dipanggil tidak perlu panjang"
log_file="$HOME/cloud_storage/cloud_log.txt"
users_file="$HOME/cloud_storage/users.txt"

# Membuat fungsi supaya mempersingkat masukin data ke cloud_log, supaya tanggal dan jam auto ada
log_message() {
echo "$(date '+%Y/%m/%d %H:%M:%S') $1" >> "$log_file"
}

# Mengecek last event yang terjadi (apakah event terakhir login atau logout) melalui cloud_log menggunakan grep -E, dan disimpan dalam variable last event
# grep berguna mencari baris yang ada kata" LOGIN: INFO atau LOGOUT: INFO
# -E supaya bisa memakai kondisi atau (|)
# tail -n1 supaya yang diambil adalah dari baris paling baru (paling bawah)
last_event=$(grep -E "(LOGIN: INFO|LOGOUT: INFO)" "$log_file" | tail -n1)

# Jika saat manggil fungsi login.sh lalu diberi argumen "logout", maka
if [[ "$1" == "logout" ]]; then

# Mengecek last event, jika last event adalah event logout juga, berarti tidak ada user yang aktif
# dan output ke terminal, lalu keluar dari program
if [[ "$last_event" == *"LOGOUT: INFO"* || -z "$last_event" ]]; then
echo "Tidak ada sesi aktif."
exit 1
fi
# Jika last event bukan logout, pasti last eventnya login, sehingga user aktif akan di logout kan
# Pakai fungsi log message untuk tanggal dan waktu, lalu tambahkan detail logout user, masukkan ke cloud log, dan output ke terminal
# active_usr berguna untuk mendapatkan nama username yang berhasil keluar, menggunakan awk, di kolom ke 6, karena format output login dan logout adalah
# 25/03/21 18:59:02 LOGIN/LOGOUT: INFO User [nama user] masuk/keluar, nama user ada di kolom ke 6 (Spasi sebagai separator kolom default dari awk)
# Lalu keluar dari program
active_usr=$(echo "$last_event" | awk '{print $6}')
log_message "LOGOUT: INFO User $active_usr keluar"
echo "Logout berhasil!"
exit 0
fi

# Jika last event adalah login DAN login.sh dipanggil tanpa argumen logout, maka sedang ada user lain yang sedang login, 
# sehingga user yang ingin login harus ditolak
# active_usr mendapatkan nama username memakai awk, sama seperti sebelumnya
# output ke terminal, user siapa ayng sedang login, lalu exit program
if [[ "$last_event" == *"LOGIN: INFO"* ]]; then
active_usr=$(echo "$last_event" | awk '{print $6}')
echo "User lain sedang masuk: $active_usr. Sabar ya kids"
exit 1
fi

# Jika login.sh dipanggil tanpa argumen, dan tidak ada user yang sedang login
# Baca input dari user menggunakan read
# -p untuk menaruh placeholder, -s supaya password tidak terlihat (rahasia)
read -p "Masukkan username: " username
read -s -p "Masukkan password: " password
echo ""

# Cek jika username yang dimasukkan ada didalam file users.txt dengan grep
# -q untuk quiet mode dan output tidak dikeluarkan, -i supaya case insensitive
# ^ pada ^$username untuk memastikan kata yang cocok adanya diawal baris (Format di users.txt [username]:[password])
if grep -qi "^$username:" "$users_file"; then

# Jika username valid, maka ambil password yang berada di kolom ke 2 file users.txt
# grep -i "^$username:" "$users_file" supaya berada di baris yang usernamenya sesuai
# cut -d: membuat colon (:) sebagai pemisah kolom, dan -f2 untuk mengambil dari kolom yang kedua, lalu dimasukkan dalam varaible stored password
stored_password=$(grep -i "^$username:" "$users_file" | cut -d: -f2)

# Jika password yang di input sama dengan password yang tersimpan, maka login berhasil
# Output ke terminal, dan juga echo(tulis) ke dalam file cloud_log.txt
if [[ "$password" == "$stored_password" ]]; then
log_message "LOGIN: INFO User $username masuk"
echo "Login berhasil!"
# Jika password salah, maka output ke terminal dan cloud log bahwa password salah
else
log_message "LOGIN: ERROR Gagal masuk untuk User $username"
echo "Password salah."
exit 1
fi
# Jika username tidak ditemukan di file users.txt, maka output ke terminal dan juga ke cloud log bahwa username tidak valid 
else
log_message "LOGIN: ERROR Gagal masuk untuk User $username"
echo "Username tidak ditemukan."
exit 1
fi
```

### Output File Login.sh
1. Logout saat tidak ada user aktif<br>
![image](https://github.com/user-attachments/assets/16d2a698-300d-4f08-8c33-8fbd97f9c3e9)
2. Logout saat user aktif<br>
![image](https://github.com/user-attachments/assets/636ae2e0-7ab7-40f8-bb57-854c2617ed82)
3. Login saat ada user lain yang aktif<br>
![image](https://github.com/user-attachments/assets/a11931d4-8b0e-4b4b-a070-e6402c57c11e)
4. Login, tapi password salah<br>
![image](https://github.com/user-attachments/assets/fe4f0a5e-8b9e-472e-af68-1b68c389caa3)
5. Login, tapi username tidak ditemukan<br>
![image](https://github.com/user-attachments/assets/aeb8a34d-9669-4017-b07c-24f258247080)
6. Login berhasil<br>
![image](https://github.com/user-attachments/assets/7981f01f-7a2a-42dd-b942-a30e0e3c45e5)
7. Output ``Login.sh`` di ``cloud_log.txt`` <br>
![image](https://github.com/user-attachments/assets/7defb672-ec41-49b2-bfe8-c5a3f9ab765a)

b. Pengunduhan Gambar Otomatis Sistem akan secara berkala memeriksa file log secara berkala untuk mendeteksi apakah ada satu pengguna yang sedang login memanfaatkan script automation.sh. Jika terdeteksi, sistem akan menjalankan proses pengunduhan gambar secara otomatis pada script download.sh.

Pengecekan Status Login:

Sistem mengecek file log (cloud_log.txt) setiap 2 menit untuk memastikan ada satu pengguna yang login.
Jika kondisi terpenuhi, proses download gambar dimulai.
Jika pengguna pengguna logout, proses download dihentikan.
Catatan: Memanfaatkan penggunaan cronjob untuk melakukan otomasi pengecekan status login.

Penyimpanan Gambar:

Download gambar dari Google Images setiap 10 menit bertema alam.
Setiap pengguna memiliki folder penyimpanan gambar sendiri di: /cloud_storage/downloads/{USERNAME}/
Gambar yang diunduh harus dinamai dengan format: HH-MM_DD-MM-YYYY.zip (Contoh: 14-20_12-03-2025.zip menunjukkan arsip dibuat pada pukul 14:20 tanggal 12 Maret 2025.)

Penjelasan:

### File Automation.sh
```bash
#!/bin/bash

# Mendefinisikan path log file supaya tidak perlu panjang" 
LOG_FILE=~/cloud_storage/cloud_log.txt

# Mencari last event yang terjadi, apakah login atau logout? dan disimpan dalam variable last_event, sama seperti di login.sh 
last_event=$(grep -E "(LOGIN: INFO|LOGOUT: INFO)" "$log_file" | tail -n1)

# JIka event terakhir adalah logout ATAU tidak ada event login/logout (-z "$last_event"), maka automation tidak perlu dijalankan, dan program berhenti
if [[ "$last_event" == *"LOGOUT: INFO"* || -z "$last_event" ]]; then
echo "Tidak ada user lain yang sedang login."
exit 1
fi

# Jika last event bukan logout, pasti login, dan kode dibawah dieksekusi
# Mencari username dari user terakhir yang login, dan menyimpannya dalam variabel username
USERNAME=$(awk '/LOGIN: INFO/ {user=$6} END {print user}' "$LOG_FILE")
# Jika variable username tidak kosong, maka output ke terminal, dan panggil download.sh dengan argumen username yang sedang aktif
if [ -n "$USERNAME" ]; then
echo "Mendownload gambar untuk $USERNAME..."
~/cloud_storage/download.sh "$USERNAME"
fi
```

### File Download.sh
```bash
#!/bin/bash

# Mendefinsikan directory tempat download, supaya ada directory khusus bagi tiap user, dan mendefinsikan tempat cloud lod
# Membuat variable username sebagai kolom pertama dari input saat memanggil download.sh oleh automation.sh tadi
# Membuat variable time stamp dengan command date untuk mempermudah pembuatan nama file gambar
USERNAME=$1
DOWNLOAD_DIR=~/cloud_storage/downloads/$USERNAME
LOG_FILE=~/cloud_storage/cloud_log.txt
TIME_STAMP=$(date "+%H-%M_%d-%m-%Y")

# Membuat direktori baru yang namanya dari variable download dir tadi
# -p supaya menjamin direktori akan otomatis dibuat jika belum ada
mkdir -p "$DOWNLOAD_DIR"

# Output ke terminal bahwa download dimulai untuk user yg aktif
echo "Downloading gambar untuk $USERNAME..."

# Menggunakan wget untuk mendownload file dari internet
# -q supaya mode quiet, dan mencegah output aneh"
# menggunakan link unsplash yang memunculkan image random kategori nature
# -O "$DOWNLOAD_DIR/$TIME_STAMP.jpg"\ untuk menspesifikasikan dimana image di save, yaitu di address dari variable download dir
# dan file diberi nama timestamp.jpg
wget -q "https://source.unsplash.com/random?nature" -O "$DOWNLOAD_DIR/$TIME_STAMP.jpg"\

# Output ke cloud log, dengan waktu menggunakan date
echo "$(date '+%Y/%m/%d %H:%M:%S') DOWNLOAD: INFO Downloading gambar untuk $USERNAME" >>"$LOG_FILE"
# Outpit jika gampar berhasil di download
echo "Gambar berhasil di download!"
```

### File Cron 
File Cron yang digunakan untuk mengeksekusi automation.sh setiap 2 menit (saat ada user yang login)
![image](https://github.com/user-attachments/assets/5c8bd70f-fca8-48be-b072-200c37ff3649)
Dengan */2 berarti ``Automation.sh`` akan di run setiap 2 menit, dan begitu juga ``Download.sh`` yang dipegang oleh ``Automation.sh``, dan * * * * sisanya berarti akan di run setiap saat, setiap 2 menit tadi


### Output ``Download.sh`` di ``cloud_log.txt``<br>
![image](https://github.com/user-attachments/assets/1645dfbe-9b43-48e3-bf01-1ea91c946c3d)

### Gambar yang sudah di download di directory downloads/user <br>
![image](https://github.com/user-attachments/assets/7ed29041-3d63-4c30-8cdd-0b233e2dfd40)

c. Pengarsipan Gambar Untuk menjaga kerapihan penyimpanan, setiap gambar yang telah diunduh akan dikumpulkan dan diarsipkan ke dalam file ZIP secara otomatis setiap 2 jam menggunakan script archive.sh. Setiap pengguna memiliki folder arsip sendiri.

Frekuensi Pengarsipan: Sistem mengarsipkan gambar setiap 2 jam.
Setiap pengguna memiliki folder arsip masing-masing di: /cloud_storage/archives/USERNAME/
Format nama file zip: archive_HH-DD-MM-YYYY.zip (Contoh: archive_12-03-2025.zip menunjukkan arsip dibuat pada tanggal 12 Maret 2025.)

Penjelasan:

### File Archive.sh
```bash
#!/bin/bash

# Mendefinisikan log file, directory archive dan download, dan menyimpan timestamp saat ini
LOG_FILE=~/cloud_storage/cloud_log.txt
ARCHIVE_DIR=~/cloud_storage/archives
DOWNLOAD_DIR=~/cloud_storage/downloads
TIME_STAMP=$(date "+%H-%d-%m-%Y")

# Looping untuk melalui semua username yang ada di directory downloads
for USERNAME in $(ls "$DOWNLOAD_DIR"); do

# Menyimpan archive dan download directory dari tiap user unik
USER_ARCHIVE_DIR="$ARCHIVE_DIR/$USERNAME"
USER_DOWNLOAD_DIR="$DOWNLOAD_DIR/$USERNAME"

# Mengecek jika directory download dari seorang user ada isinya (tidak kosong)
# Jika tidak kosong, maka membuat directory archive baru untuk user tersebut
# ls- A untuk melihat seluruh isi direktori download seorang user, yang nantinya di pass ke $()
# 2>/dev/null untuk mengabaikan error message
# -n = cek jika non-empty
if [ -n "$(ls -A "$USER_DOWNLOAD_DIR" 2>/dev/null)" ]; then
mkdir -p "$USER_ARCHIVE_DIR"

# Output pesan ke terminal, dan meng compress file Download user tadi ke file .zip yang judulnya sesuai timestamp dan berada di directory unik milik user
echo "Mengarsipkan gambar untuk $USERNAME..."
zip -r "$USER_ARCHIVE_DIR/archive_$TIME_STAMP.zip" "$USER_DOWNLOAD_DIR"/*
# Menghapus semua image yang sekarang ada di directory user
rm -rf "$USER_DOWNLOAD_DIR"/*


# Output ke dalam log file bahwa archive berhasil dibuat untuk user, lalu lanjut ke iterasi selanjutnya
echo "$(date '+%Y/%m/%d %H:%M:%S') ARCHIVE: INFO Archive dibuat untuk $USERNAME" >>"$LOG_FILE"
echo "Archive berhasil dibuat untuk $USERNAME!"
fi
done
```


### 4️⃣Task 4 (Proxy Terbaik di New Eridu)
Soal:
Belle dan Wise adalah Proxy dengan nama Phaethon di Kota New Eridu yang menyamar sebagai warga biasa dengan membuka Toko Video di Sixth Street. Suatu hari, Wise meminta Belle untuk memantau setiap server yang berjalan di PC mereka karena biaya listrik bulanan yang tinggi. Karena Belle terlalu sibuk mengelola Toko Video, ia meminta bantuan kalian (Proxy yang Hebat) untuk membuat program monitoring ini.

Buatlah program untuk memantau sumber daya pada setiap server. Program ini hanya perlu memantau:

Penggunaan RAM menggunakan perintah free -m.
Ukuran suatu direktori menggunakan perintah du -sh <target_path>.
Uptime menggunakan perintah uptime dan ambil bagian yang menunjukkan waktu berjalan.
Load average menggunakan perintah cat /proc/loadavg dan ambil tiga nilai pertama (1, 5, dan 15 menit).
Catat semua metrics yang diperoleh dari hasil free -m. Untuk hasil du -sh <target_path>, catat ukuran dari path direktori tersebut. Direktori yang akan dipantau adalah /home/{user}/.

### Script minute5_log.sh
```bash
#!/bin/bash

# Menyimpan waktu eksekusi script ke dalam log cron
echo "Script minute5_log.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics" # Menentukan direktori penyimpanan log
mkdir -p "$LOG_DIR" # Membuat direktori penyimpanan log apabila belum ada

CURRENT_TIME=$(date +"%Y%m%d%H%M%S") # Mendapatkan timestamp saat ini dalam format {YmdHms}

# Menentukan nama file log berdasarkan timestamp
LOG_FILE="$LOG_DIR/metrics_$CURRENT_TIME.log" 

# Mendapatkan informasi penggunaan RAM dalam satuan MB dengan perintah free -m
RAM_INFO=$(free -m | grep Mem)
mem_total=$(echo $RAM_INFO | awk '{print $2}')
mem_used=$(echo $RAM_INFO | awk '{print $3}')
mem_free=$(echo $RAM_INFO | awk '{print $4}')
mem_shared=$(echo $RAM_INFO | awk '{print $5}')
mem_buff=$(echo $RAM_INFO | awk '{print $6}')
mem_available=$(echo $RAM_INFO | awk '{print $7}')

# Mendapatkan informasi penggunaan swap dalam satuan MB dengan perintah free -m
SWAP_INFO=$(free -m | grep Swap)
swap_total=$(echo $SWAP_INFO |awk '{print $2}')
swap_used=$(echo $SWAP_INFO |awk '{print $3}')
swap_free=$(echo $SWAP_INFO |awk '{print $4}')

DIR_SIZE=$(du -sh /home/$USER | awk '{print $1}') # Menghitung ukuran direktori /home/$USER

# Menampilkan header 
echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" > $LOG_FILE 

# Menampilkan data aktual yang dikumpulkan ke dalam file log	
echo "$mem_total,$mem_used,$mem_free,$mem_shared,$mem_buff,$mem_available,$swap_total,$swap_used,$swap_free,/home/$USER/,$DIR_SIZE" >> $LOG_FILE

# Mengatur permission agar hanya bisa dibaca pemilik
chmod 600 $LOG_FILE  
```
### Output log
![image](https://github.com/user-attachments/assets/899ce3f3-fae1-48d1-b309-686a05376aa1)

### Script agg_5min_to_hour.sh
```bash
#!/bin/bash

# Menyimpan waktu eksekusi script ke dalam log cron
echo "Script agg_5min_to_hour.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics"  # Menentukan direktori penyimpanan log

# Mendapatkan timestamp saat ini dalam format {YmdH}
CURRENT_HOUR=$(date +"%Y%m%d%H")  

# Nama file untuk log agregasi per jam
AGG_LOG_FILE="$LOG_DIR/metrics_agg_$CURRENT_HOUR.log"  

# Mengambil semua file log dari 5 menit terakhir dalam jam ini
FILES=$(ls $LOG_DIR/metrics_${CURRENT_HOUR}*.log 2>/dev/null)

# Jika tidak ada file log ditemukan, keluar dari script dengan kode error 1
if [[ -z "$FILES" ]]; then
    exit 1
fi

# Mengambil header dari salah satu file log sebagai referensi untuk hasil agregasi
HEADER=$(head -n 1 $(ls $LOG_DIR/metrics_${CURRENT_HOUR}*.log | head -n 1))

# Proses agregasi (MAX, MIN, AVG) menggunakan awk
awk -F, '
NR==1 { next }
{
    for (i=1; i<=NF; i++) {
        if (NR==2) { min[i]=$i; max[i]=$i; sum[i]=0 }
        if ($i+0 == $i) {
            if ($i < min[i]) min[i] = $i;
            if ($i > max[i]) max[i] = $i;
            sum[i] += $i;
        }
    }
    count++;
}
END {
    print "'"$HEADER"'" > "'"$AGG_LOG_FILE"'"
    printf "MIN," > "'"$AGG_LOG_FILE"'"
    for (i=1; i<=NF; i++) printf "%s,", min[i] > "'"$AGG_LOG_FILE"'"
    printf "\nMAX," > "'"$AGG_LOG_FILE"'"
    for (i=1; i<=NF; i++) printf "%s,", max[i] > "'"$AGG_LOG_FILE"'"
    printf "\nAVG," > "'"$AGG_LOG_FILE"'"
    for (i=1; i<=NF; i++) printf "%.2f,", sum[i]/count > "'"$AGG_LOG_FILE"'"
    printf "\n" > "'"$AGG_LOG_FILE"'"
}' $LOG_DIR/metrics_${CURRENT_HOUR}*.log

# Mengatur permission agar hanya bisa dibaca pemilik
chmod 600 $AGG_LOG_FILE
```
### Output log
![image](https://github.com/user-attachments/assets/e7b7a1a0-c088-48f3-94ba-1410839d11da)

### Script uptime_monitor.sh
```bash
#!/bin/bash

# Menyimpan waktu eksekusi script ke dalam log cron
echo "Script uptime_monitor.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics"  # Menentukan direktori penyimpanan log

CURRENT_HOUR=$(date +"%Y%m%d%H")  # Mendapatkan timestamp saat ini dalam format {YmdH}

# Nama file log uptime berdasarkan jam saat ini
UPTIME_LOG_FILE="$LOG_DIR/uptime_$CURRENT_HOUR.log"

# Mendapatkan waktu saat ini dalam format HH:MM:SS
UPTIME=$(date +"%H:%M:%S")  

# Mengambil data load average dari file /proc/loadavg
# Output dari /proc/loadavg berisi lima angka, tetapi kita hanya mengambil tiga angka pertama:
#   - Load average dalam 1 menit terakhir
#   - Load average dalam 5 menit terakhir
#   - Load average dalam 15 menit terakhir
LOAD_AVG=$(awk '{print $1","$2","$3}' /proc/loadavg)

# Menuliskan header kolom ke dalam file log
echo "uptime,load_avg_1min,load_avg_5min,load_avg_15min" > $UPTIME_LOG_FILE

# Menuliskan data uptime dan load average ke dalam file log
echo "$UPTIME,$LOAD_AVG" >> "$UPTIME_LOG_FILE"

# Mengatur permission agar hanya bisa dibaca pemilik
chmod 600 "$UPTIME_LOG_FILE"
```
### Output log
![image](https://github.com/user-attachments/assets/aa32d791-035c-404c-82bc-49c709ffe276)

### Script cleanup_log.sh
```bash
#!/bin/bash

# Menyimpan waktu eksekusi script ke dalam log cron
echo "Script cleanup_log.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics"  # Menentukan direktori penyimpanan log

# Mencari dan menghapus file log agregasi yang lebih lama dari 12 jam (720 menit)
find $LOG_DIR -type f -name "metrics_agg_*.log" -mmin +720 -exec rm -f {} \;

# - $LOG_DIR        : Direktori tempat mencari file
# - -type f         : Hanya mencari file (bukan folder)
# - -name "metrics_agg_*.log" : Hanya mencari file yang namanya diawali dengan "metrics_agg_" dan berekstensi .log
# - -mmin +720      : Hanya memilih file yang lebih tua dari 720 menit (12 jam)
# - -exec rm -f {} \; : Menghapus setiap file yang ditemukan tanpa konfirmasi
```
### Output log
![image](https://github.com/user-attachments/assets/7f88d975-26bd-4421-a598-1487767ea018)
(File log lama benar telah terhapus)

### Konfigurasi cron
```bash
*/5 * * * * /home/willy/sisop/praktikum1/minute5_log.sh

0 * * * * /home/willy/sisop/praktikum1/agg_5min_to_hour.sh

0 * * * * /home/willy/sisop/praktikum1/uptime_monitor.sh

0 0 * * * /home/willy/sisop/praktikum1/cleanup_log.sh
```

Penjelasan:
- ``*/5 * * * * /home/willy/sisop/praktikum1/minute5_log.sh`` Script akan dijalankan tiap 5 menit sekali dan berlaku untuk setiap jam, setiap hari, setiap bulan, dan setiap hari dalam seminggu
- ``0 * * * * /home/willy/sisop/praktikum1/agg_5min_to_hour.sh`` Script akan dijalankan tiap jam dan berlaku untuk setiap jam, setiap hari, setiap bulan, dan setiap hari dalam seminggu
- ``0 * * * * /home/willy/sisop/praktikum1/uptime_monitor.sh`` Script akan dijalankan tiap jam dan berlaku untuk setiap jam, setiap hari, setiap bulan, dan setiap hari dalam seminggu
- ``0 0 * * * /home/willy/sisop/praktikum1/cleanup_log.sh`` Script akan dijalankan tengah malam (00.00) setiap hari dan berlaku untuk setiap hari, setiap bulan, dan setiap hari dalam seminggu
