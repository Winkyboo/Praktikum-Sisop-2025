Kami dari kelas Sistem Operasi D, kelompok D13 yang beranggotakan:
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241089 | Rhea Debora Sianturi |
| 5025241096 | Willy Marcelius |
| 5025241113 | Rafa Huga Nirando |
| 5025241124 | Melvan Hapianan Allo Ponglabba |

Berikut hasil laporan praktikum Sistem Operasi modul 4

# Praktikum Modul 4 _(Module 4 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - FUSecure](/task-1/)

- [Task 2 - LawakFS++](/task-2/)

- [Task 3 - Drama Troll](/task-3/)

- [Task 4 - LilHabOS](/task-4/)

# Task 1 (FUSecure)

Yuadi adalah seorang developer brilian yang sedang mengerjakan proyek top-secret. Sayangnya, temannya Irwandi yang terlalu penasaran memiliki kebiasaan buruk menyalin jawaban praktikum sistem operasi Yuadi tanpa izin setiap kali deadline mendekat. Meskipun sudah diperingatkan berkali-kali dan bahkan ditegur oleh asisten dosen karena sering ketahuan plagiat, Irwandi tetap tidak bisa menahan diri untuk mengintip dan menyalin jawaban praktikum Yuadi. Merasa kesal karena prestasinya terus-menerus dicuri dan integritasnya dipertaruhkan, Yuadi yang merupakan ahli keamanan data memutuskan untuk mengambil tindakan sendiri dengan membuat FUSecure, sebuah file system berbasis FUSE yang menerapkan akses read-only dan membatasi visibilitas file berdasarkan permission user.

## Deskripsi Tugas

Setelah frustrasi dengan kebiasaan plagiat Irwandi yang merugikan prestasi akademiknya, Yuadi memutuskan untuk merancang sistem keamanan yang sophisticated. Dia akan membuat sistem file khusus yang memungkinkan mereka berdua berbagi materi umum, namun tetap menjaga kerahasiaan jawaban praktikum masing-masing.

### a. Setup Direktori dan Pembuatan User

Langkah pertama dalam rencana Yuadi adalah mempersiapkan infrastruktur dasar sistem keamanannya.

1. Buat sebuah "source directory" di sistem Anda (contoh: `/home/shared_files`). Ini akan menjadi tempat penyimpanan utama semua file.

2. Di dalam source directory ini, buat 3 subdirektori: `public`, `private_yuadi`, `private_irwandi`. Buat 2 Linux users: `yuadi` dan `irwandi`. Anda dapat memilih password mereka.
   | User | Private Folder |
   | ------- | --------------- |
   | yuadi | private_yuadi |
   | irwandi | private_irwandi |

Yuadi dengan bijak merancang struktur ini: folder `public` untuk berbagi materi kuliah dan referensi yang boleh diakses siapa saja, sementara setiap orang memiliki folder private untuk menyimpan jawaban praktikum mereka masing-masing.

### b. Akses Mount Point

Selanjutnya, Yuadi ingin memastikan sistem filenya mudah diakses namun tetap terkontrol.

FUSE mount point Anda (contoh: `/mnt/secure_fs`) harus menampilkan konten dari `source directory` secara langsung. Jadi, jika Anda menjalankan `ls /mnt/secure_fs`, Anda akan melihat `public/`, `private_yuadi/`, dan `private_irwandi/`.

### c. Read-Only untuk Semua User

Yuadi sangat kesal dengan kebiasaan Irwandi yang suka mengubah atau bahkan menghapus file jawaban setelah menyalinnya untuk menghilangkan jejak plagiat. Untuk mencegah hal ini, dia memutuskan untuk membuat seluruh sistem menjadi read-only.

1. Jadikan seluruh FUSE mount point **read-only untuk semua user**.
2. Ini berarti tidak ada user (termasuk `root`) yang dapat membuat, memodifikasi, atau menghapus file atau folder apapun di dalam `/mnt/secure_fs`. Command seperti `mkdir`, `rmdir`, `touch`, `rm`, `cp`, `mv` harus gagal semua.

"Sekarang Irwandi tidak bisa lagi menghapus jejak plagiatnya atau mengubah file jawabanku," pikir Yuadi puas.

### d. Akses Public Folder

Meski ingin melindungi jawaban praktikumnya, Yuadi tetap ingin berbagi materi kuliah dan referensi dengan Irwandi dan teman-teman lainnya.

Setiap user (termasuk `yuadi`, `irwandi`, atau lainnya) harus dapat **membaca** konten dari file apapun di dalam folder `public`. Misalnya, `cat /mnt/secure_fs/public/materi_kuliah.txt` harus berfungsi untuk `yuadi` dan `irwandi`.

### e. Akses Private Folder yang Terbatas

Inilah bagian paling penting dari rencana Yuadi - memastikan jawaban praktikum benar-benar terlindungi dari plagiat.

1. File di dalam `private_yuadi` **hanya dapat dibaca oleh user `yuadi`**. Jika `irwandi` mencoba membaca file jawaban praktikum di `private_yuadi`, harus gagal (contoh: permission denied).
2. Demikian pula, file di dalam `private_irwandi` **hanya dapat dibaca oleh user `irwandi`**. Jika `yuadi` mencoba membaca file di `private_irwandi`, harus gagal.

"Akhirnya," senyum Yuadi, "Irwandi tidak bisa lagi menyalin jawaban praktikumku, tapi dia tetap bisa mengakses materi kuliah yang memang kubuat untuk dibagi."

## Contoh Skenario

Setelah sistem selesai, beginilah cara kerja FUSecure dalam kehidupan akademik sehari-hari:

- **User `yuadi` login:**

  - `cat /mnt/secure_fs/public/materi_algoritma.txt` (Berhasil - materi kuliah bisa dibaca)
  - `cat /mnt/secure_fs/private_yuadi/jawaban_praktikum1.c` (Berhasil - jawaban praktikumnya sendiri)
  - `cat /mnt/secure_fs/private_irwandi/tugas_sisop.c` (Gagal dengan "Permission denied" - tidak bisa baca jawaban praktikum Irwandi)
  - `touch /mnt/secure_fs/public/new_file.txt` (Gagal dengan "Permission denied" - sistem read-only)

- **User `irwandi` login:**
  - `cat /mnt/secure_fs/public/materi_algoritma.txt` (Berhasil - materi kuliah bisa dibaca)
  - `cat /mnt/secure_fs/private_irwandi/tugas_sisop.c` (Berhasil - jawaban praktikumnya sendiri)
  - `cat /mnt/secure_fs/private_yuadi/jawaban_praktikum1.c` (Gagal dengan "Permission denied" - tidak bisa baca jawaban praktikum Yuadi)
  - `mkdir /mnt/secure_fs/new_folder` (Gagal dengan "Permission denied" - sistem read-only)

Dengan sistem ini, kedua mahasiswa akhirnya bisa belajar dengan tenang. Yuadi bisa menyimpan jawaban praktikumnya tanpa khawatir diplagiat Irwandi, sementara Irwandi terpaksa harus mengerjakan tugasnya sendiri namun masih bisa mengakses materi kuliah yang dibagikan Yuadi di folder public.

## Catatan

- Anda dapat memilih nama apapun untuk FUSE mount point Anda.
- Ingat untuk menggunakan argument `-o allow_other` saat mounting FUSE file system Anda agar user lain dapat mengaksesnya.
- Fokus pada implementasi operasi FUSE yang berkaitan dengan **membaca** dan **menolak** operasi write/modify. Anda perlu memeriksa **UID** dari user yang mengakses di dalam operasi FUSE Anda untuk menerapkan pembatasan private folder.

---

## Source Code Reference

```c
#define FUSE_USE_VERSION 31

#include <fuse3/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>

static const char *source_dir = "/home/shared_files";

char *get_username(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) return NULL;
    return pw->pw_name;
}

static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", source_dir, path);
}
static int fs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    (void) fi;
    char fpath[PATH_MAX];
    fullpath(fpath, path);
    int res = lstat(fpath, stbuf);
    if(res == -1) return -errno;
    return 0;
}

static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    (void) offset;
    (void) fi;
    (void) flags;
    char fpath[PATH_MAX];
    fullpath(fpath, path);
    DIR *dp;
    struct dirent *de;
    dp = opendir(fpath);
    if (dp == NULL) return -errno;
    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        filler(buf, de->d_name, &st, 0, 0);
    }
    closedir(dp);
    return 0;
}

static int fs_open(const char *path, struct fuse_file_info *fi){
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return -EACCES;

    uid_t uid = fuse_get_context()->uid;
    char *user = get_username(uid);

    if (strncmp(path, "/private_yuadi/", 15) == 0 && strcmp(user, "yuadi") != 0)
        return -EACCES;

    if (strncmp(path, "/private_irwandi/", 17) == 0 && strcmp(user, "irwandi") != 0)
        return -EACCES;

    int fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    close(fd);
    return 0;
}

static int fs_read(const char *path, char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi) {
    (void) fi;
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    uid_t uid = fuse_get_context()->uid;
    char *user = get_username(uid);

    if (strncmp(path, "/private_yuadi/", 15) == 0 && strcmp(user, "yuadi") != 0) return -EACCES;

    if (strncmp(path, "/private_irwandi/", 17) == 0 && strcmp(user, "irwandi") != 0) return -EACCES;

    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;
    int res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);
    return res;
}

static int fs_mkdir(const char *path, mode_t mode){ return -EROFS; }
static int fs_rmdir(const char *path){ return -EROFS; }
static int fs_create(const char *path, mode_t mode, struct fuse_file_info *fi){ return -EROFS; }
static int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) { return -EROFS; }
static int fs_unlink(const char *path){ return -EROFS; }
static int fs_rename(const char *from, const char *to, unsigned int flags){ return -EROFS; }

static struct fuse_operations fs_oper = {
    .getattr = fs_getattr,
    .readdir = fs_readdir,
    .open    = fs_open,
    .read    = fs_read,
    .mkdir   = fs_mkdir,
    .rmdir   = fs_rmdir,
    .create  = fs_create,
    .write   = fs_write,
    .unlink  = fs_unlink,
    .rename  = fs_rename,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &fs_oper, NULL);
}
```
## Struktur Sistem

### Struktur Direktori Sumber

Direktori dasar berisi:

```
/home/shared_files/
├── public/
├── private_yuadi/
└── private_irwandi/
```

* `public/`: dapat diakses oleh semua user
* `private_yuadi/`: hanya dapat diakses oleh user `yuadi`
* `private_irwandi/`: hanya dapat diakses oleh user `irwandi`

### User yang Terlibat

* `yuadi` — user yang ingin melindungi jawaban praktikumnya
* `irwandi` — user yang dapat mengakses materi publik dan folder privat miliknya sendiri

## Aturan Akses yang Diterapkan

| Folder             | Dapat Diakses oleh | Hak Akses |
| ------------------ | ------------------ | --------- |
| public/            | Semua user         | Read-only |
| private\_yuadi/    | Hanya `yuadi`      | Read-only |
| private\_irwandi/  | Hanya `irwandi`    | Read-only |
| Seluruh filesystem | Semua orang        | Read-only |

Tidak ada user, bahkan root, yang dapat membuat, menulis, mengganti nama, atau menghapus file atau folder di dalam mount point.

## Cara Kerja

FUSecure menimpa beberapa operasi FUSE yang dipilih:

* `getattr`, `readdir`, `open`, `read`: memperbolehkan akses baca
* `write`, `mkdir`, `rmdir`, `create`, `unlink`, `rename`: semua mengembalikan `EROFS` (read-only error)

ID user diperiksa menggunakan:

```c
uid_t uid = fuse_get_context()->uid;
```

Akses ke folder privat dibatasi dengan mencocokkan UID user terhadap awalan nama direktori.

## Instruksi Setup

### 1. Instalasi Dependensi

```bash
sudo apt install libfuse3-dev build-essential
```

### 2. Membuat User dan Direktori

```bash
sudo useradd -m yuadi
sudo useradd -m irwandi
sudo passwd yuadi
sudo passwd irwandi

sudo mkdir -p /home/shared_files/public /home/shared_files/private_yuadi /home/shared_files/private_irwandi
sudo chown -R root:root /home/shared_files
```

### 3. Kompilasi Program

```bash
gcc fusecure.c -o fusecure `pkg-config fuse3 --cflags --libs`
```

### 4. Mount Filesystem

Jalankan sebagai user target:

```bash
su - yuadi
./fusecure /mnt/secure_fs -f -o allow_other
```

Pastikan `/mnt/secure_fs` sudah ada dan dimiliki oleh user yang menjalankan `fusecure`.

## Contoh Eksekusi

### Akses oleh `yuadi`

```bash
cat /mnt/secure_fs/public/materi.txt            # Berhasil
cat /mnt/secure_fs/private_yuadi/answer.c       # Berhasil
cat /mnt/secure_fs/private_irwandi/tugas.c      # Ditolak (Permission Denied)
rm /mnt/secure_fs/public/materi.txt             # Ditolak (Permission Denied)
```
Dokumentasi Akses Yuadi:
![Screenshot 2025-06-22 214136](https://github.com/user-attachments/assets/1bc49d62-4aff-4fa0-b30b-705576502866)

### Akses oleh `irwandi`

```bash
cat /mnt/secure_fs/public/materi.txt            # Berhasil
cat /mnt/secure_fs/private_irwandi/tugas.c      # Berhasil
cat /mnt/secure_fs/private_yuadi/answer.c       # Ditolak (Permission Denied)
mkdir /mnt/secure_fs/testfolder                 # Ditolak (Permission Denied)
```
Dokumentasi Akses Irwandi:
![Screenshot 2025-06-22 214007](https://github.com/user-attachments/assets/1a7efbfa-4c19-4611-84d3-d205510867f5)

## Daftar File

* `fusecure.c` : Source code utama dalam bahasa C
* `README.md` : Dokumentasi ini

## Catatan

* Selalu jalankan program FUSE sebagai user yang sesuai (`yuadi` atau `irwandi`)
* Mount point harus dimiliki oleh user yang menjalankan `fusecure`
* Jangan hanya menggunakan `su` untuk debugging — FUSE memeriksa UID sebenarnya, bukan shell user


# Task 2 (LawakFS++ - A Cursed Filesystem with Censorship and Strict Access Policies)

Teja adalah seorang penggemar sepak bola yang sangat bersemangat. Namun, akhir-akhir ini, tim kesayangannya selalu tampil kurang memuaskan di setiap pertandingan. Kekalahan demi kekalahan membuat Teja muak dan kesal. "Tim lawak!" begitu umpatnya setiap kali timnya gagal meraih kemenangan. Kekecewaan Teja yang mendalam ini menginspirasi sebuah ide: bagaimana jika ada sebuah filesystem yang bisa menyensor hal-hal "lawak" di dunia ini?

Untuk mengatasi hal tersebut, kami membuat filesystem terkutuk bernama **LawakFS++** yang mengimplementasikan kebijakan akses yang ketat, filtering konten dinamis, dan kontrol akses berbasis waktu untuk file tertentu. Filesystem ini dirancang sebagai read-only dan akan menerapkan perilaku khusus untuk akses file, termasuk logging dan manajemen konfigurasi.

- Kamu boleh memilih direktori sumber dan mount point apa pun untuk filesystem kamu.

- Kamu **wajib** mengimplementasikan setidaknya fungsi-fungsi berikut dalam struct `fuse_operations` kamu:

  - `getattr`
  - `readdir`
  - `read`
  - `open`
  - `access`

- Kamu diperbolehkan menyertakan fungsi tambahan seperti `init`, `destroy`, atau `readlink` jika diperlukan untuk implementasi kamu.

- **LawakFS++ harus benar-benar read-only.** Setiap percobaan untuk melakukan operasi tulis dalam FUSE mountpoint harus **gagal** dan mengembalikan error `EROFS` (Read-Only File System).

- System call berikut, dan perintah yang bergantung padanya, harus diblokir secara eksplisit:

  - `write()`
  - `truncate()`
  - `create()`
  - `unlink()`
  - `mkdir()`
  - `rmdir()`
  - `rename()`

> **Catatan:** Ketika pengguna mencoba menggunakan perintah seperti `touch`, `rm`, `mv`, atau perintah lain yang melakukan operasi tulis, mereka harus menerima error "Permission denied" atau "Read-only file system" yang jelas.

### a. Ekstensi File Tersembunyi

Setelah beberapa hari menggunakan filesystem biasa, Teja menyadari bahwa ekstensi file selalu membuat orang-orang bisa mengetahui jenis file dengan mudah. "Ini terlalu mudah ditebak!" pikirnya. Dia ingin membuat sistem yang lebih misterius, di mana orang harus benar-benar membuka file untuk mengetahui isinya.

Semua file yang ditampilkan dalam FUSE mountpoint harus **ekstensinya disembunyikan**.

- **Contoh:** Jika file asli adalah `document.pdf`, perintah `ls` di dalam direktori FUSE hanya menampilkan `document`.
- **Perilaku:** Meskipun ekstensi disembunyikan, mengakses file (misalnya, `cat /mnt/your_mountpoint/document`) harus dipetakan dengan benar ke path dan nama aslinya (misalnya, `source_dir/document.pdf`).

### b. Akses Berbasis Waktu untuk File Secret

Suatu hari, Teja menemukan koleksi foto-foto memalukan dari masa SMA-nya yang tersimpan dalam folder bernama "secret". Dia tidak ingin orang lain bisa mengakses file-file tersebut kapan saja, terutama saat dia sedang tidur atau tidak ada di rumah. "File rahasia hanya boleh dibuka saat jam kerja!" putusnya dengan tegas.

File yang nama dasarnya adalah **`secret`** (misalnya, `secret.txt`, `secret.zip`) hanya dapat diakses **antara pukul 08:00 (8 pagi) dan 18:00 (6 sore) waktu sistem**.

- **Pembatasan:** Di luar rentang waktu yang ditentukan, setiap percobaan untuk membuka, membaca, atau bahkan melakukan list file `secret` harus menghasilkan error `ENOENT` (No such file or directory).
- **Petunjuk:** Kamu perlu mengimplementasikan kontrol akses berbasis waktu ini dalam operasi FUSE `access()` dan/atau `getattr()` kamu.

### c. Filtering Konten Dinamis

Kekesalan Teja terhadap hal-hal "lawak" semakin memuncak ketika dia membaca artikel online yang penuh dengan kata-kata yang membuatnya kesal. Tidak hanya itu, gambar-gambar yang dia lihat juga sering kali tidak sesuai dengan ekspektasinya. "Semua konten yang masuk ke sistem saya harus difilter dulu!" serunya sambil mengepalkan tangan.

Ketika sebuah file dibuka dan dibaca, isinya harus **secara dinamis difilter atau diubah** berdasarkan tipe file yang terdeteksi:

| Tipe File      | Perlakuan                                                                                 |
| :------------- | :---------------------------------------------------------------------------------------- |
| **File Teks**  | Semua kata yang dianggap lawak (case-insensitive) harus diganti dengan kata `"lawak"`.    |
| **File Biner** | Konten biner mentah harus ditampilkan dalam **encoding Base64** alih-alih bentuk aslinya. |

> **Catatan:** Daftar "kata-kata lawak" untuk filtering file teks akan didefinisikan secara eksternal, seperti yang ditentukan dalam persyaratan **e. Konfigurasi**.

### d. Logging Akses

Sebagai seorang yang paranoid, Teja merasa perlu untuk mencatat setiap aktivitas yang terjadi di filesystemnya. "Siapa tahu ada yang mencoba mengakses file-file penting saya tanpa izin," gumamnya sambil menyiapkan sistem logging. Dia ingin setiap gerakan tercatat dengan detail, lengkap dengan waktu dan identitas pelakunya.

Semua operasi akses file yang dilakukan dalam LawakFS++ harus **dicatat** ke file yang terletak di **`/var/log/lawakfs.log`**.

Setiap entri log harus mematuhi format berikut:

```
[YYYY-MM-DD HH:MM:SS] [UID] [ACTION] [PATH]
```

Di mana:

- **`YYYY-MM-DD HH:MM:SS`**: Timestamp operasi.
- **`UID`**: User ID pengguna yang melakukan aksi.
- **`ACTION`**: Jenis operasi FUSE (misalnya, `READ`, `ACCESS`, `GETATTR`, `OPEN`, `READDIR`).
- **`PATH`**: Path ke file atau direktori dalam FUSE mountpoint (misalnya, `/secret`, `/images/photo.jpg`).

> **Persyaratan:** Kamu **hanya diwajibkan** untuk mencatat operasi `read` dan `access` yang berhasil. Logging operasi lain (misalnya, write yang gagal) bersifat opsional.

### e. Konfigurasi

Setelah menggunakan filesystemnya beberapa minggu, Teja menyadari bahwa kebutuhannya berubah-ubah. Kadang dia ingin menambah kata-kata baru ke daftar filter, kadang dia ingin mengubah jam akses file secret, atau bahkan mengubah nama file secret itu sendiri. "Saya tidak mau repot-repot kompilasi ulang setiap kali ingin mengubah pengaturan!" keluhnya. Akhirnya dia memutuskan untuk membuat sistem konfigurasi eksternal yang fleksibel.

Untuk memastikan fleksibilitas, parameter-parameter berikut **tidak boleh di-hardcode** dalam source code `lawak.c` kamu. Sebaliknya, mereka harus dapat dikonfigurasi melalui file konfigurasi eksternal (misalnya, `lawak.conf`):

- **Nama file dasar** dari file 'secret' (misalnya, `secret`).
- **Waktu mulai** untuk mengakses file 'secret'.
- **Waktu berakhir** untuk mengakses file 'secret'.
- **Daftar kata-kata yang dipisahkan koma** yang akan difilter dari file teks.

**Contoh konten `lawak.conf`:**

```
FILTER_WORDS=ducati,ferrari,mu,chelsea,prx,onic,sisop
SECRET_FILE_BASENAME=secret
ACCESS_START=08:00
ACCESS_END=18:00
```

FUSE kamu harus membaca dan mem-parse file konfigurasi ini saat inisialisasi.

### Ringkasan Perilaku yang Diharapkan

Untuk memastikan kejelasan, berikut adalah tabel konsolidasi perilaku yang diharapkan untuk skenario tertentu:

| Skenario                                                              | Perilaku yang Diharapkan                                                                         |
| :-------------------------------------------------------------------- | :----------------------------------------------------------------------------------------------- |
| Mengakses file di luar waktu yang diizinkan (misalnya, file `secret`) | Mengembalikan `ENOENT` (No such file or directory)                                               |
| Membaca file biner                                                    | Konten harus dioutput dalam **encoding Base64**                                                  |
| Membaca file teks                                                     | Kata-kata yang difilter harus diganti dengan `"lawak"`                                           |
| Melakukan list file di direktori mana pun                                  | Semua ekstensi file harus disembunyikan                                                          |
| Mencoba menulis, membuat, atau mengganti nama file/direktori          | Mengembalikan `EROFS` (Read-Only File System)                                                    |
| Logging operasi file                                                  | Entri baru harus ditambahkan ke `/var/log/lawakfs.log` untuk setiap operasi `read` dan `access`. |

### Contoh Perilaku

```bash
$ ls /mnt/lawak/
secret   image   readme

$ cat /mnt/lawak/secret
cat: /mnt/lawak/secret: No such file or directory
# (Output ini diharapkan jika diakses di luar 08:00-18:00)

$ cat /mnt/lawak/image
<string base64 dari konten gambar>

$ cat /mnt/lawak/readme
"Ini adalah filesystem lawak."
# (Kata "sisop" asli diganti dengan "lawak")

$ sudo tail /var/log/lawakfs.log
[2025-06-10 14:01:22] [1000] [READ] /readme
[2025-06-10 14:01:24] [1000] [ACCESS] /secret
```

---

Cara pengerjaan
1. Membuat file c baru dengan memasukkan operasi FUSE untuk 
    .read       = xmp_read,
    .open       = xmp_open,
    .access     = xmp_access,
    .write      = xmp_write,
    .create     = xmp_create,
    .truncate   = xmp_truncate,
    .unlink     = xmp_unlink,
    .mkdir      = xmp_mkdir,
    .rmdir      = xmp_rmdir,
    .rename     = xmp_rename,
    .readdir    = xmp_readdir,
    .getattr    = xmp_getattr,
     (diambil dari struct)
  2. memblokir operasi open, access, write, create, truncate, unlink, mkdir, rmdir, rename, dengan return -EROFS, untuk memastikan file hanya read-only, sehingga jika menggunakan perintah yang diblokir akan tidak dijalankan
  3. membuat fungsi log_action untuk membuat log dengan , dimana setiap aksi akan disimpan dalam file lawakfs.log di path "/var/log/lawakfs/log" yang mendata setiap aksi didalam operasi FUSE
  4. membuat fungsi  trim, untuk menghapus string yang diterima, yaitu eksistensi file
  5. membuat fungsi load_config untuk mengakses file lawak.conf untuk mengambil kata-kata yang difilter, base file, dan waktu
  6. membuat fungsi bool is_secret_file_blocked_now untuk mencari waktu akses dan menentukan apakah file secret bisa diakses atau todak
  7. membuat fungsi fullpath untuk mengambil path sebuah file dalam bentuk string
  8. membuat fungsi  find_fullname_by_stripped untuk membaca nama file yang sudah dipotong
  9. membuat fungsi is_text_file untuk menentukan apakah jenis file adalah sebuah file txt
  10. membuat fungsi replace_keywords untuk mengganti kata yang difilter menjadi "lawak"
  11. membuat fungsi *base64_encode untuk mengganti fie image menjadi base 64
  12. mengedit xmp_read agar , mengecek file teks atau bukan, jika iya, mencari kata yg difilter dan mengganti dengan "lawak" sesuai fungsi replace_keywords
  13. mengedit xmp_getattr untuk mengembalikan nama file yang sudah dipotong dengan fungsi find_fullname_by_stripped dan trim


Screenshot
a. Ekstensi File Tersembunyi
![image](https://github.com/user-attachments/assets/31b214cc-268c-465c-8950-c7474b0a00aa)

b. Akses Berbasis Waktu untuk File Secret
![image](https://github.com/user-attachments/assets/47033ee8-7bc8-4beb-a348-d6661fec780e)

c. Filtering Konten Dinamis
![image](https://github.com/user-attachments/assets/3b03b303-ec9b-4c58-b418-989c9ba6cdb3)

d. Logging Akses
![image](https://github.com/user-attachments/assets/b2902009-9a66-44fa-912e-eb95c435f07b)

# Task 3 (**Drama Troll**)

Dalam sebuah perusahaan penuh drama fandom, seorang karyawan bernama DainTontas telah menyinggung komunitas Gensh _ n, Z _ Z, dan Wut \* ering secara bersamaan. Akibatnya, dua rekan kerjanya, SunnyBolt dan Ryeku, merancang sebuah troll dengan gaya khas: membuat filesystem jebakan menggunakan FUSE.

Mereka membutuhkan bantuanmu, ahli Sistem Operasi, untuk menciptakan filesystem kustom yang bisa mengecoh DainTontas, langsung dari terminal yang dia cintai.

## **a. Pembuatan User**

Buat 3 user di sistem sebagai berikut yang merepresentasikan aktor-aktor yang terlibat dalam _trolling_ kali ini, yaitu:

- DainTontas
- SunnyBolt
- Ryeku

Gunakan `useradd` dan `passwd` untuk membuat akun lokal. User ini akan digunakan untuk mengakses filesystem FUSE yang kamu buat.

Cara Pengerjaan:
```bash
sudo useradd -m DainTontas
sudo passwd DainTontas

sudo useradd -m SunnyBolt
sudo passwd SunnyBolt

sudo useradd -m Ryeku
sudo passwd Ryeku
```

## **b. Jebakan Troll**

Untuk menjebak DainTontas, kamu harus menciptakan sebuah filesystem FUSE yang dipasang di `/mnt/troll`. Di dalamnya, hanya akan ada dua file yang tampak sederhana:

- `very_spicy_info.txt` - umpan utama.
- `upload.txt` - tempat DainTontas akan "secara tidak sadar" memicu jebakan.

## **c. Jebakan Troll (Berlanjut)**

Setelah membuat dua file tersebut, SunnyBolt dan Ryeku memintamu untuk membuat jebakan yang telah dirancang oleh mereka. SunnyBolt dan Ryeku yang merupakan kolega lama DainTontas dan tahu akan hal-hal memalukan yang dia pernah lakukan, akan menaruh aib-aib tersebut pada file `very_spicy_info.txt`. Apabila dibuka dengan menggunakan command `cat` oleh user selain DainTontas, akan memberikan output seperti berikut:

```
DainTontas' personal secret!!.txt
```

Untuk mengecoh DainTontas, apabila DainTontas membuka `very_spicy_info.txt`, isinya akan berupa seperti berikut:

```
Very spicy internal developer information: leaked roadmap.docx
```

Screenshot:
- user DainTontas
  ![image](https://github.com/user-attachments/assets/da7f8beb-c08b-444f-b8b8-aa232e80bfa8)
- user Ryeku
  ![image](https://github.com/user-attachments/assets/abc7ca46-12a7-445b-ba46-4e62cfc91055)
- user SunnyBolt
  ![image](https://github.com/user-attachments/assets/0a55e90f-ff68-47f4-8abd-3cad7cb1d545)


## **d. Trap**

Suatu hari, DainTontas mengecek PC dia dan menemukan bahwa ada sebuah file baru, `very_spicy_info.txt`. Dia kemudian membukanya dan menemukan sebuah leak roadmap. Karena dia ingin menarik perhatian, dia akan mengupload file tersebut ke sosial media `Xitter` dimana _followernya_ dapat melihat dokumen yang bocor tersebut. Karena dia pernah melakukan hal ini sebelumnya, dia punya script bernama `upload.txt` yang dimana ketika dia melakukan `echo` berisi `upload` pada file tersebut, dia akan otomatis mengupload sebuah file ke akun `Xitter` miliknya.

Namun ini semua adalah rencana SunnyBolt dan Ryeku. Atas bantuanmu, mereka telah mengubah isi dari `upload.txt`. Apabila DainTontas melakukan upload dengan metode `echo` miliknya, maka dia dinyatakan telah "membocorkan" info leak tersebut. Setelah DainTontas membocorkan file tersebut, semua file berekstensi `.txt` akan selalu memunculkan sebuah ASCII Art dari kalimat

```
Fell for it again reward
```

Setelah seharian penuh mencoba untuk membetulkan PC dia, dia baru sadar bahwa file `very_spicy_info.txt` yang dia sebarkan melalui script uploadnya tersebut teryata berisikan aib-aib dia. Dia pun ditegur oleh SunnyBolt dan Ryeku, dan akhirnya dia pun tobat.

- user DainTontas
  ![image](https://github.com/user-attachments/assets/7cd45d41-d5e7-4148-8cc9-0c7399b1ff77)
- user Ryeku
  ![image](https://github.com/user-attachments/assets/23074f88-5ff1-474a-a762-a2b2efa9e485)
- user SunnyBolt
  ![image](https://github.com/user-attachments/assets/d38e705c-c95a-43f7-9495-3fbe266efb23)

## **NOTES**

- `upload.txt` hanyalah berupa file kosong yang dipakai untuk trigger _behavior change_ dari `cat` milik DainTontas. Deskripsi di soal hanyalah sebagai _storytelling_ belaka.
- Trigger dari Trap akan bersifat `persist` bahkan ketika FUSE telah di-unmount ataupun ketika mesin dimatikan/direstart

---

Kode :
```bash
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *hello_path = "/hello.txt";
static const char *hello_content = "Hello, World!\n";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755; 
        stbuf->st_nlink = 2; 
    } 
    else if (strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_content); 
    } 
    else res = -ENOENT; 

    return res;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi){
    (void) offset;
    (void) fi;

    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    
    filler(buf, "hello.txt", NULL, 0);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;

    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    len = strlen(hello_content);

    if (offset >= len)
        return 0;

    if (offset + size > len)
        size = len - offset;

    memcpy(buf, hello_content + offset, size);

    return size; 
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read    = xmp_read,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
Penjelasan kode:
- 
Penjelasan:
```markdown
# Penjelasan Kode FUSE Sederhana

Kode ini mengimplementasikan sebuah filesystem virtual yang sangat dasar menggunakan FUSE (Filesystem in Userspace). Filesystem ini, ketika dijalankan, akan membuat sebuah "drive" virtual yang berisi:
1.  Sebuah direktori root (`/`).
2.  Sebuah file bernama `hello.txt` yang bersifat *read-only* (hanya bisa dibaca).

Jika file `hello.txt` dibaca (misalnya dengan perintah `cat`), isinya akan selalu "Hello, World!".

## Analisis Kode per Bagian

Berikut adalah penjelasan sintaks dan fungsi dari setiap bagian kode.

### 1. Header dan Variabel Global

```c
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *hello_path = "/hello.txt";
static const char *hello_content = "Hello, World!\n";
```

*   **Sintaks & Penjelasan:**
    *   `#define FUSE_USE_VERSION 28`: Sebuah *preprocessor directive* yang wajib ada untuk memberitahu kompiler bahwa kita menggunakan API FUSE versi 28.
    *   `#include <...> `: Menyertakan *header files* (pustaka) yang berisi fungsi-fungsi yang kita butuhkan.
        *   `fuse.h`: Pustaka utama untuk semua fungsi dan struktur data FUSE.
        *   `string.h`: Untuk fungsi manipulasi string seperti `strcmp()` (membandingkan string) dan `strlen()` (panjang string).
        *   `errno.h`: Untuk definisi kode-kode error standar, seperti `ENOENT` (No Such File or Directory).
    *   `static const char *...`: Mendeklarasikan dua variabel global sebagai pointer ke string yang konstan (`const`).
        *   `hello_path`: Menyimpan path lengkap dari file virtual kita.
        *   `hello_content`: Menyimpan konten dari file tersebut.

### 2. Implementasi `xmp_getattr` (Mendapatkan Atribut File)

Fungsi ini dipanggil oleh sistem operasi setiap kali ia butuh metadata atau atribut dari sebuah file/direktori (misalnya saat menjalankan `ls -l`).

```c
static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755; 
        stbuf->st_nlink = 2; 
    } 
    else if (strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_content); 
    } 
    else res = -ENOENT; 

    return res;
}
```

*   **Sintaks & Penjelasan:**
    *   `memset(stbuf, 0, ...)`: Mengisi struktur `stbuf` dengan nol untuk membersihkan data sisa.
    *   `if (strcmp(path, "/") == 0)`: Jika path yang diminta adalah direktori root (`/`):
        *   `stbuf->st_mode = S_IFDIR | 0755;`: Atur modenya sebagai **direktori** (`S_IFDIR`) dengan izin akses `rwxr-xr-x` (`0755`).
        *   `stbuf->st_nlink = 2;`: Direktori biasanya punya 2 link (`.` dan `..`).
    *   `else if (strcmp(path, hello_path) == 0)`: Jika path yang diminta adalah `/hello.txt`:
        *   `stbuf->st_mode = S_IFREG | 0444;`: Atur modenya sebagai **file biasa** (`S_IFREG`) dengan izin akses `r--r--r--` (`0444`), yang artinya *read-only* untuk semua user.
        *   `stbuf->st_nlink = 1;`: File biasanya punya 1 link.
        *   `stbuf->st_size = strlen(hello_content);`: Atur ukurannya sesuai panjang konten "Hello, World!\n".
    *   `else res = -ENOENT;`: Jika path yang diminta tidak ada, kembalikan error `ENOENT`.

### 3. Implementasi `xmp_readdir` (Membaca Isi Direktori)

Fungsi ini dipanggil saat kita mencoba melihat isi sebuah direktori (misalnya dengan `ls`).

```c
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, ...){
    // ...
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, "hello.txt", NULL, 0);
    // ...
}
```

*   **Sintaks & Penjelasan:**
    *   `filler`: Ini adalah sebuah *function pointer* yang disediakan FUSE. Tugas kita adalah memanggil fungsi `filler` ini untuk setiap entri yang ada di dalam direktori.
    *   `filler(buf, ".", ...)`: Menambahkan entri `.` (direktori saat ini) ke dalam daftar.
    *   `filler(buf, "..", ...)`: Menambahkan entri `..` (direktori induk) ke dalam daftar.
    *   `filler(buf, "hello.txt", ...)`: Menambahkan nama file `hello.txt` ke dalam daftar.

### 4. Implementasi `xmp_read` (Membaca Isi File)

Fungsi ini dipanggil saat sebuah program (seperti `cat`) mencoba membaca isi dari file `/hello.txt`.

```c
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, ...){
    // ...
    len = strlen(hello_content);

    if (offset >= len) return 0;

    if (offset + size > len) size = len - offset;

    memcpy(buf, hello_content + offset, size);

    return size; 
}
```
*   **Sintaks & Penjelasan:**
    *   `buf`, `size`, `offset`: Parameter standar. `buf` adalah buffer tujuan, `size` adalah jumlah byte yang diminta, dan `offset` adalah posisi awal membaca.
    *   `if (offset >= len) return 0;`: Jika posisi awal (`offset`) sudah melebihi akhir konten, hentikan pembacaan (kembalikan 0 byte).
    *   `if (offset + size > len) size = len - offset;`: Jika permintaan baca (`size`) akan melebihi akhir konten, sesuaikan `size` agar hanya membaca sampai akhir.
    *   `memcpy(buf, hello_content + offset, size);`: **Inti dari fungsi ini**. Menyalin data dari `hello_content` (dimulai dari posisi `offset`) sebanyak `size` byte ke dalam buffer `buf`.
    *   `return size;`: Mengembalikan jumlah byte yang berhasil disalin.

### 5. Penggabungan Operasi dan Fungsi `main`

Bagian ini "mendaftarkan" semua fungsi yang telah kita buat ke FUSE dan memulai filesystem.

```c
static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read    = xmp_read,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```

*   **Sintaks & Penjelasan:**
    *   `static struct fuse_operations xmp_oper = { ... };`: Membuat sebuah struct yang berfungsi sebagai "peta". Ini memberitahu FUSE fungsi mana yang harus dipanggil untuk setiap operasi filesystem.
        *   `.getattr = xmp_getattr` berarti: "Jika ada permintaan `getattr`, panggil fungsi `xmp_getattr`".
    *   `int main(...)`: Titik masuk utama program.
    *   `umask(0);`: Mengatur *user file-creation mode mask* ke 0. Ini memastikan izin akses yang kita set di `getattr` (seperti `0755` dan `0444`) tidak dibatasi oleh sistem.
    *   `return fuse_main(...)`: **Fungsi Kunci**. Memulai *main loop* FUSE, yang akan mengambil alih program, memproses argumen baris perintah (seperti direktori tujuan mount), dan mulai meneruskan permintaan dari kernel ke fungsi-fungsi yang kita daftarkan di `xmp_oper`.


# NO 4

## Struktur Direktori

task-4/
├── bin/ # File biner hasil kompilasi
├── include/ # Header file
│ ├── kernel.h
│ ├── std_lib.h
│ └── std_type.h
├── src/ # Sumber kode utama
│ ├── bootloader.asm
│ ├── kernel.asm
│ ├── kernel.c
│ └── std_lib.c
├── bochsrc.txt # Konfigurasi Bochs
└── makefile # Automasi kompilasi dan run

yaml
Copy
Edit

---

## A. Implementasi Fungsi `printString`, `readString`, dan `clearScreen`

### `printString`

```c
void printString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        interrupt(0x10, 0x0E00 + str[i], 0, 0, 0);
        i++;
    }
}
Mencetak string ke layar karakter demi karakter.

Menggunakan interrupt BIOS int 0x10 dengan AH = 0x0E (mode TTY output).

readString
c
Copy
Edit
void readString(char* buf) {
    int i = 0;
    char c;
    while (1) {
        c = interrupt(0x16, 0x0000, 0, 0, 0) & 0xFF;
        if (c == 0x0D) {
            buf[i] = '\0';
            break;
        } else if (c == 0x08 && i > 0) {
            i--;
            printString("\b \b");
        } else {
            buf[i++] = c;
            interrupt(0x10, 0x0E00 + c, 0, 0, 0);
        }
    }
}
Membaca input keyboard hingga Enter ditekan.

Menangani backspace secara manual.

Simpan karakter ke buffer, echo ke layar.

clearScreen
c
Copy
Edit
void clearScreen() {
    interrupt(0x10, 0x0600, 0, 0x184F, 0x07);
    interrupt(0x10, 0x0200, 0, 0, 0);
}
Membersihkan layar dengan int 0x10 AH=0x06.

Mengatur kursor ke posisi (0,0) dengan AH=0x02.

B. Implementasi Fungsi-fungsi Dasar di std_lib.c
1. div dan mod
c
Copy
Edit
int div(int a, int b) {
    int count = 0;
    while (a >= b) {
        a -= b;
        count++;
    }
    return count;
}

int mod(int a, int b) {
    while (a >= b) {
        a -= b;
    }
    return a;
}
Operasi pembagian dan modulo manual tanpa operator / dan %.

2. memcpy
c
Copy
Edit
void memcpy(byte* src, byte* dst, unsigned int size) {
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}
Menyalin isi memori byte per byte.

3. strlen
c
Copy
Edit
unsigned int strlen(char* str) {
    int i = 0;
    while (str[i] != '\0') i++;
    return i;
}
Menghitung panjang string hingga null terminator.

4. strcmp
c
Copy
Edit
bool strcmp(char* str1, char* str2) {
    int i = 0;
    while (str1[i] && str2[i]) {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}
Membandingkan dua string apakah identik.

5. strcpy
c
Copy
Edit
void strcpy(char* src, char* dst) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}
Menyalin isi string dari sumber ke tujuan.

6. clear
c
Copy
Edit
void clear(byte* buf, unsigned int size) {
    for (int i = 0; i < size; i++) buf[i] = 0;
}
Mengisi buffer dengan nol (mirip memset(buf, 0, size)).

7. itoa
c
Copy
Edit
void itoa(int num, char* str) {
    int i = 0, isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    while (num != 0) {
        str[i++] = mod(num, 10) + '0';
        num = div(num, 10);
    }
    if (isNegative) str[i++] = '-';
    str[i] = '\0';

    for (int s = 0, e = i - 1; s < e; s++, e--) {
        char temp = str[s];
        str[s] = str[e];
        str[e] = temp;
    }
}
Mengubah integer ke string dengan membalik digit hasil pembagian.

C. Implementasi Perintah echo
c
Copy
Edit
void handleEcho(char* cmd, char* pipeCmd) {
    if (strncmp(cmd, "echo ", 5) == 0) {
        char* arg = cmd + 5;
        if (pipeCmd == NULL) {
            printString(arg); printString("\n");
        } else if (strncmp(pipeCmd, "grep ", 5) == 0) {
            handleGrep(arg, pipeCmd + 5);
        } else if (strncmp(pipeCmd, "wc", 2) == 0) {
            handleWC(arg);
        } else {
            printString("Perintah setelah pipe tidak dikenali\n");
        }
    } else {
        printString("Perintah tidak dikenali\n");
    }
}
Mencetak string ke layar jika tidak ada pipe.

Jika ada pipe, hasilnya diteruskan ke grep atau wc.

D. Implementasi grep dan wc
handleGrep
c
Copy
Edit
void handleGrep(char* input, char* pattern) {
    int len = strlen(input), plen = strlen(pattern);
    for (int i = 0; i <= len - plen; i++) {
        int match = 1;
        for (int j = 0; j < plen; j++) {
            if (input[i + j] != pattern[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            printString(input); printString("\n");
            return;
        }
    }
    printString("NULL\n");
}
Melakukan pencocokan substring manual.

Jika cocok, string ditampilkan. Jika tidak, cetak NULL.

handleWC
c
Copy
Edit
void handleWC(char* input) {
    if (!input || strlen(input) == 0) {
        printString("0 0 0\n");
        return;
    }

    int lines = 1, words = 0, chars = 0, inWord = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        chars++;
        if (input[i] == '\n') lines++;
        if (input[i] != ' ' && inWord == 0) {
            words++;
            inWord = 1;
        } else if (input[i] == ' ') {
            inWord = 0;
        }
    }

    char buf[16];
    itoa(lines, buf); printString(buf); printString(" ");
    itoa(words, buf); printString(buf); printString(" ");
    itoa(chars, buf); printString(buf); printString("\n");
}
Menghitung jumlah karakter, kata, dan baris secara manual.

Memanfaatkan itoa untuk konversi ke string.

E. Makefile dan Proses Build
Kompilasi Otomatis (makefile)
Contoh target:

make
Copy
Edit
build: prepare bootloader stdlib kernel link

prepare:
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader:
	nasm -f bin src/bootloader.asm -o bin/bootloader.bin

stdlib:
	bcc -ansi -c -o bin/std_lib.o src/std_lib.c

kernel:
	bcc -ansi -c -o bin/kernel.o src/kernel.c
	nasm -f as86 src/kernel.asm -o bin/kernel_asm.o

link:
	ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel_asm.o bin/std_lib.o
	dd if=bin/bootloader.bin of=bin/floppy.img conv=notrunc
	dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 conv=notrunc

run:
	bochs -f bochsrc.txt
Menyusun pipeline build dari bootloader hingga kernel.

File image akhir berada di bin/floppy.img.

\
