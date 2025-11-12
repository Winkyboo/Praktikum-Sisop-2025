Kami dari kelas Sistem Operasi D, kelompok D13 yang beranggotakan:
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241089 | Rhea Debora Sianturi |
| 5025241096 | Willy Marcelius |
| 5025241113 | Rafa Huga Nirando |
| 5025241124 | Melvan Hapianan Allo Ponglabba |

# Praktikum Modul 2 _(Module 2 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Trabowo & Peddy Movie Night](/task-1/)

- [Task 2 - Organize and Analyze Anthony's Favorite Films](/task-2/)

- [Task 3 - Cella’s Manhwa](/task-3/)
- [Task 4 - Pipip's Load Balancer](/task-4/)

### Laporan Resmi Praktikum Modul 2 _(Module 2 Lab Work Report)_

## 1️⃣Task 1 (Prabowo & Peddy Movie Night)
Trabowo dan sahabatnya, Peddy, sedang menikmati malam minggu di rumah sambil mencari film seru untuk ditonton. Mereka menemukan sebuah file ZIP yang berisi gambar-gambar poster film yang sangat menarik. File tersebut dapat diunduh dari **[Google Drive](https://drive.google.com/file/d/1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B/view?usp=sharing)**. Karena penasaran dengan film-film tersebut, mereka memutuskan untuk membuat sistem otomatis guna mengelola semua file tersebut secara terstruktur dan efisien. Berikut adalah tugas yang harus dikerjakan untuk mewujudkan sistem tersebut:

### **a. Ekstraksi File ZIP**

Trabowo langsung mendownload file ZIP tersebut dan menyimpannya di penyimpanan lokal komputernya. Namun, karena file tersebut dalam bentuk ZIP, Trabowo perlu melakukan **unzip** agar dapat melihat daftar film-film seru yang ada di dalamnya.

### **b. Pemilihan Film Secara Acak**

Setelah berhasil melakukan unzip, Trabowo iseng melakukan pemilihan secara acak/random pada gambar-gambar film tersebut untuk menentukan film pertama yang akan dia tonton malam ini.

**Format Output:**

```
Film for Trabowo & Peddy: ‘<no_namafilm_genre.jpg>’
```

### **c. Memilah Film Berdasarkan Genre**

Karena Trabowo sangat perfeksionis dan ingin semuanya tertata rapi, dia memutuskan untuk mengorganisir film-film tersebut berdasarkan genre. Dia membuat 3 direktori utama di dalam folder `~/film`, yaitu:

- **FilmHorror**
- **FilmAnimasi**
- **FilmDrama**

Setelah itu, dia mulai memindahkan gambar-gambar film ke dalam folder yang sesuai dengan genrenya. Tetapi Trabowo terlalu tua untuk melakukannya sendiri, sehingga ia meminta bantuan Peddy untuk memindahkannya. Mereka membagi tugas secara efisien dengan mengerjakannya secara bersamaan (overlapping) dan membaginya sama banyak. Trabowo akan mengerjakan dari awal, sementara Peddy dari akhir. Misalnya, jika ada 10 gambar, Trabowo akan memulai dari gambar pertama, gambar kedua, dst dan Peddy akan memulai dari gambar kesepuluh, gambar kesembilan, dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task

Contoh format log :

```
[15-04-2025 13:44:59] Peddy: 50_toystory_animasi.jpg telah dipindahkan ke FilmAnimasi
```

Setelah memindahkan semua film, Trabowo dan Peddy juga perlu menghitung jumlah film dalam setiap kategori dan menuliskannya dalam file **`total.txt`**. Format dari file tersebut adalah:

```
Jumlah film horror: <jumlahfilm>
Jumlah film animasi: <jumlahfilm>
Jumlah film drama: <jumlahfilm>
Genre dengan jumlah film terbanyak: <namagenre>
```

### **d. Pengarsipan Film**

Setelah semua film tertata dengan rapi dan dikelompokkan dalam direktori masing-masing berdasarkan genre, Trabowo ingin mengarsipkan ketiga direktori tersebut ke dalam format **ZIP** agar tidak memakan terlalu banyak ruang di komputernya.

---
KODE:
```bash
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define UNZIP_DIR "/home/ubuntu/film_list"
#define FILM_DIR "/home/ubuntu"
#define ZIP_OUTPUT_DIR "/home/ubuntu"
#define RECAP_FILE "/home/ubuntu/recap.txt"
#define TOTAL_FILE "/home/ubuntu/total.txt"

typedef struct {
    char **files;
    int start;
    int end;
    const char *person;
} ThreadArgs;

void *move_files(void *args_void) {
    ThreadArgs *args = (ThreadArgs *)args_void;
    char temp_log[4096] = "";  // Buffer to store this thread's log
    for (int i = args->start; i < args->end; i++) {
        char *filename = strrchr(args->files[i], '/');
        if (!filename) continue;
        filename++;

        char target_path[512];
        const char *genre_dir = NULL;
        if (strstr(filename, "Horror") || strstr(filename, "horror")) genre_dir = "FilmHorror";
        else if (strstr(filename, "Animasi") || strstr(filename, "animasi")) genre_dir = "FilmAnimasi";
        else if (strstr(filename, "Drama") || strstr(filename, "drama")) genre_dir = "FilmDrama";
        else continue;

        sprintf(target_path, "%s/%s/%s", FILM_DIR, genre_dir, filename);
        rename(args->files[i], target_path);

        char log_line[512];
        snprintf(log_line, sizeof(log_line), "[%s] %s telah dipindahkan ke %s\n", args->person, filename, genre_dir);
        strcat(temp_log, log_line);
    }

    // After loop, write buffer to shared file (brief locking here is OK)
    FILE *recap = fopen(RECAP_FILE, "a");
    if (recap) {
        fputs(temp_log, recap);
       fclose(recap);
    }

    pthread_exit(NULL);
}
void collect_jpg_files_recursive(const char *dir_path, char **files, int *count) {
    DIR *dir = opendir(dir_path);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        stat(path, &st);

        if (S_ISDIR(st.st_mode)) {
            collect_jpg_files_recursive(path, files, count);  // Recursive call for subdirectory
        } else if (S_ISREG(st.st_mode) && strstr(entry->d_name, ".jpg")) {
            files[*count] = strdup(path);  // Store full path
            (*count)++;
        }
    }

    closedir(dir);
}
void download_file(const char *url, const char *output) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("wget", "wget", "-O", output, url, NULL);
        perror("execlp wget failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void unzip_file(const char *zipfile, const char *output_dir) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("unzip", "unzip", "-o", zipfile, "-d", output_dir, NULL);
        perror("execlp unzip failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

char* pick_random_unzipped_film(const char *unzipped_dir) {
    static char result[512];
    char *films[1000];
    int count = 0;

    // Recursively search for .jpg files
    void search_dir(const char *dir_path) {
        DIR *dir = opendir(dir_path);
        if (!dir) return;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

            struct stat st;
            stat(path, &st);

            if (S_ISDIR(st.st_mode)) {
                search_dir(path);  // Recurse
            } else if (S_ISREG(st.st_mode) && strstr(entry->d_name, ".jpg")) {
                films[count++] = strdup(path);  // Save full path
            }
        }

        closedir(dir);
    }

    search_dir(unzipped_dir);

    if (count == 0) {
        printf("No films found in unzipped directory.\n");
        return NULL;
    }

    int film_idx = rand() % count;
   char *filename = strrchr(films[film_idx], '/');
    snprintf(result, sizeof(result), "%s", filename ? filename + 1 : films[film_idx]);

    for (int i = 0; i < count; i++) free(films[i]);
    return result;
}


int count_jpg_files(const char *dirpath) {
    int count = 0;
    DIR *d = opendir(dirpath);
    struct dirent *entry;
    if (!d) return 0;
    while ((entry = readdir(d)) != NULL) {
        if (strstr(entry->d_name, ".jpg")) count++;
    }
    closedir(d);
    return count;
}

void zip_folder(const char *folder_path, const char *zip_output) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("zip", "zip", "-r", zip_output, folder_path, NULL);
        perror("execlp zip failed");
       exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void remove_folder(const char *folder_path) {
    DIR *d = opendir(folder_path);
    if (!d) return;
    struct dirent *entry;
    char filepath[512];
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        snprintf(filepath, sizeof(filepath), "%s/%s", folder_path, entry->d_name);
        struct stat st;
        stat(filepath, &st);
        if (S_ISDIR(st.st_mode)) {
            remove_folder(filepath);
        } else {
            unlink(filepath);
        }
    }
    closedir(d);
   rmdir(folder_path);
}

int main() {
    mkdir(UNZIP_DIR, 0755);
    mkdir(FILM_DIR, 0755);
    mkdir(ZIP_OUTPUT_DIR, 0755);
    mkdir(FILM_DIR "/FilmHorror", 0755);
    mkdir(FILM_DIR "/FilmAnimasi", 0755);
    mkdir(FILM_DIR "/FilmDrama", 0755);

    // Clean recap and total file
    FILE *f = fopen(RECAP_FILE, "w"); if (f) fclose(f);
    f = fopen(TOTAL_FILE, "w"); if (f) fclose(f);
    printf("a. Ekstrasi File ZIP");
    // Download & unzip
    download_file("https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B", "film_list.zip");
    unzip_file("film_list.zip", UNZIP_DIR);

   printf("b. Pemilihan Film secara Acak");    
    srand(time(NULL));
    char *random_film1 = pick_random_unzipped_film(UNZIP_DIR);
    if (random_film1) {
        printf("Film for Trabowo & Peddy: '%s'\n", random_film1);
    }


    // Read unzipped JPGs
     char *files[1000];
    int count = 0;
    collect_jpg_files_recursive(UNZIP_DIR, files, &count);
    if (count == 0) {
        fprintf(stderr, "Tidak ada file .jpg ditemukan dalam direktori unzip.\n");
        return 1;
    }
printf("c. Memilah Film Berdasarkan Genre");     
int mid = count / 2;

pthread_t worker_tid1, worker_tid2;
ThreadArgs trabowo_args = { files, 0, mid, "Trabowo" };
ThreadArgs peddy_args = { files, mid, count, "Peddy" };

int iret1 = pthread_create(&worker_tid1, NULL, move_files, (void*)&trabowo_args);
if (iret1) {
    fprintf(stderr, "Error - pthread_create() for Trabowo: %d\n", iret1);
    exit(EXIT_FAILURE);
}
int iret2 = pthread_create(&worker_tid2, NULL, move_files, (void*)&peddy_args);
if (iret2) {
    fprintf(stderr, "Error - pthread_create() for Peddy: %d\n", iret2);
    exit(EXIT_FAILURE);
}

pthread_join(worker_tid1, NULL);
pthread_join(worker_tid2, NULL);

    int count_horror = count_jpg_files(FILM_DIR "/FilmHorror");
    int count_animasi = count_jpg_files(FILM_DIR "/FilmAnimasi");
    int count_drama = count_jpg_files(FILM_DIR "/FilmDrama");

    const char *genre = "horror";
    int max = count_horror;
    if (count_animasi > max) { max = count_animasi; genre = "animasi"; }
    if (count_drama > max) { max = count_drama; genre = "drama"; }

    FILE *fp = fopen(TOTAL_FILE, "w");
    if (fp) {
        fprintf(fp, "Jumlah film horror: %d\n", count_horror);
        fprintf(fp, "Jumlah film animasi: %d\n", count_animasi);
        fprintf(fp, "Jumlah film drama: %d\n", count_drama);
        fprintf(fp, "Genre dengan jumlah film terbanyak: %s\n", genre);
        fclose(fp);
    }
printf("d. Archiving Movies"); 
    zip_folder(FILM_DIR "/FilmHorror", ZIP_OUTPUT_DIR "/FilmHorror.zip");
    zip_folder(FILM_DIR "/FilmAnimasi", ZIP_OUTPUT_DIR "/FilmAnimasi.zip");
    zip_folder(FILM_DIR "/FilmDrama", ZIP_OUTPUT_DIR "/FilmDrama.zip");

    remove_folder(FILM_DIR "/FilmHorror");
    remove_folder(FILM_DIR "/FilmAnimasi");
    remove_folder(FILM_DIR "/FilmDrama");

    for (int i = 0; i < count; i++) free(files[i]);

    return 0;
}
```
Cara pengerjaan
- Mendownload dan Unzip Link:

Kode mulai dengan mengunduh file ZIP dari URL yang diberikan menggunakan perintah wget, kemudian mengekstrak file ZIP tersebut ke direktori tertentu (UNZIP_DIR) menggunakan perintah unzip. File-file yang diunduh dan diekstrak adalah film yang akan diproses lebih lanjut.

- Memilih Film Random:

Setelah file diekstrak, kode memilih sebuah film secara acak dari direktori yang telah diekstrak. Film ini akan menjadi film yang akan diproses oleh dua thread yang berjalan secara bersamaan. Film tersebut dipilih dengan cara mengumpulkan semua file .jpg dalam direktori, kemudian memilihnya secara acak.

- Memilah Berdasarkan Genre, Mendapat File Recap dan Total:

Kode kemudian mengurutkan file berdasarkan genre (Horror, Animasi, Drama). Dua thread, yaitu "Trabowo" dan "Peddy", masing-masing menangani setengah file dan memindahkannya ke direktori yang sesuai (FilmHorror, FilmAnimasi, dan FilmDrama). Selama proses pemindahan, log aktivitas ditulis ke file recap.txt, yang mencatat film yang dipindahkan oleh masing-masing thread.

Kode juga menghitung jumlah film untuk setiap genre dan mencatatnya dalam file total.txt, serta menentukan genre dengan jumlah film terbanyak.

- Menyimpan Hasil Akhir dalam Zip:

Setelah pemilahan selesai, setiap genre (Horror, Animasi, Drama) di-zip ke dalam file terpisah. File ZIP ini disimpan di direktori ZIP_OUTPUT_DIR. Setelah itu, folder untuk setiap genre yang berisi file-film tersebut dihapus untuk membersihkan ruang. Kode memastikan hasil akhir berupa file ZIP yang berisi film-film yang telah dipilah berdasarkan genre.

- Hasil di home
![image](https://github.com/user-attachments/assets/dd90e5de-3b6a-4f02-b21f-df1b82721f99)

- hasil file film_list.zip
![image](https://github.com/user-attachments/assets/68843856-f9a0-4011-aae0-1786e950eb8c)

- hasil file FilmHorror.zip
![image](https://github.com/user-attachments/assets/c846d71f-4fe0-46fd-b4c4-ffd10b89e6e2)

- hasil file FilmAnimasi.zip
![image](https://github.com/user-attachments/assets/bfed9d7d-d419-434b-a799-75a9fde6dd6f)

- hasil file FilmDrama.zip
![image](https://github.com/user-attachments/assets/489bc61d-6a5c-4ec1-8655-45d4064ef00c)

- hasil file total.txt
![image](https://github.com/user-attachments/assets/475c46a4-b59a-4284-a576-84662ae59215)

- hasil file recap.txt
![image](https://github.com/user-attachments/assets/60f35c6c-3724-46f9-9974-87b30bcae9d1)

Kendala selama pengerjaan : Tidak bisa menggunakan system sehingga kode sebelum revisi perlu diubah. 

Revisi : Kode selama demo tidak berhasil karena mengakses directory yang salah karena pengaturan file path yang rancau, kode sekarang berhasil memrosesnya dengan file path yang lebih sederhana, hal yang sama juga terjadi dalam pemilahan, sehingga isi file setelah pemilahan kosong karena tidak berhasil memroses isi file film_list.zip
Catatan, memastikan thread berjalan selang seling

## 2️⃣Task 2 (Organize and Analyze Anthony's Favorite Films)

Soal:
Anthony sedang asyik menonton film favoritnya dari Netflix, namun seiring berjalannya waktu, koleksi filmnya semakin menumpuk. Ia pun memutuskan untuk membuat sistem agar film-film favoritnya bisa lebih terorganisir dan mudah diakses. Anthony ingin melakukan beberapa hal dengan lebih efisien dan serba otomatis.

> Film-film yang dimaksud adalah film-film yang ada di dalam file ZIP yang bisa diunduh dari **[Google Drive](https://drive.google.com/file/d/12GWsZbSH858h2HExP3x4DfWZB1jLdV-J/view?usp=drive_link)**.

Berikut adalah serangkaian tugas yang Anthony ingin capai untuk membuat pengalaman menonton filmnya jadi lebih menyenangkan:

### **a. One Click and Done!**

Pernahkah kamu merasa malas untuk mengelola file ZIP yang penuh dengan data film? Anthony merasa hal yang sama, jadi dia ingin semuanya serba instan dengan hanya satu perintah. Dengan satu perintah saja, Anthony bisa:

- Mendownload file ZIP yang berisi data film-film Netflix favoritnya.
- Mengekstrak file ZIP tersebut ke dalam folder yang sudah terorganisir.
- Menghapus file ZIP yang sudah tidak diperlukan lagi, supaya tidak memenuhi penyimpanan.

Buatlah skrip yang akan mengotomatiskan proses ini sehingga Anthony hanya perlu menjalankan satu perintah untuk mengunduh, mengekstrak, dan menghapus file ZIP.

### **b. Sorting Like a Pro**

Koleksi film Anthony semakin banyak dan dia mulai bingung mencari cara yang cepat untuk mengelompokkannya. Nah, Anthony ingin mengelompokkan film-filmnya dengan dua cara yang sangat mudah:

1. Berdasarkan huruf pertama dari judul film.
2. Berdasarkan tahun rilis (release year).

Namun, karena Anthony sudah mempelajari **multiprocessing**, dia ingin mengelompokkan kedua kategori ini secara paralel untuk menghemat waktu.

**Struktur Output:**

- **Berdasarkan Huruf Pertama Judul Film:**

  - Folder: `judul/`
  - Setiap file dinamai dengan huruf abjad atau angka, seperti `A.txt`, `B.txt`, atau `1.txt`.
  - Jika judul film tidak dimulai dengan huruf atau angka, film tersebut disimpan di file `#.txt`.

- **Berdasarkan Tahun Rilis:**
  - Folder: `tahun/`
  - Setiap file dinamai sesuai tahun rilis film, seperti `1999.txt`, `2021.txt`, dst.

Format penulisan dalam setiap file :

```
Judul Film - Tahun Rilis - Sutradara
```

Setiap proses yang berjalan akan mencatat aktivitasnya ke dalam satu file bernama **`log.txt`** dengan format:

```
[jam:menit:detik] Proses mengelompokkan berdasarkan [Abjad/Tahun]: sedang mengelompokkan untuk film [judul_film]
```

**Contoh Log:**

```
[14:23:45] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Avengers: Infinity War
[14:23:46] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film Kung Fu Panda
```

### **c. The Ultimate Movie Report**

Sebagai penggemar film yang juga suka menganalisis, Anthony ingin mengetahui statistik lebih mendalam tentang film-film yang dia koleksi. Misalnya, dia ingin tahu berapa banyak film yang dirilis **sebelum tahun 2000** dan **setelah tahun 2000**.

Agar laporan tersebut mudah dibaca, Anthony ingin hasilnya disimpan dalam file **`report_ddmmyyyy.txt`**.

**Format Output dalam Laporan:**

```
i. Negara: <nama_negara>
Film sebelum 2000: <jumlah>
Film setelah 2000: <jumlah>

...
i+n. Negara: <nama_negara>
Film sebelum 2000: <jumlah>
Film setelah 2000: <jumlah>
```

Agar penggunaannya semakin mudah, Anthony ingin bisa menjalankan semua proses di atas melalui sebuah antarmuka terminal interaktif dengan pilihan menu seperti berikut:
1. Download File
2. Mengelompokkan Film
3. Membuat Report

Catatan:
- Dilarang menggunakan `system`
- Harap menggunakan thread dalam pengerjaan soal C

---

# Kode Lengkap
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

typedef struct country_report {
    int before_2000;
    int after_2000;
    char country[100];
} country_report;

country_report country_reports[100];
int country_count = 0;

int execute_command(char *const argv[]){
    pid_t pid = fork();
    if (pid < 0){
        perror("Fork failed");
        return -1;
    } else if (pid == 0){
        // Child process
        execvp(argv[0], argv);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else{
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void download() {
    FILE *log = fopen("log.txt", "w");
    if (log) fclose(log);
    printf("Men-download file ZIP...\n");
    char *download_args[] = {
        "wget", 
        "https://docs.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J", 
        "-O", 
        "netflixData.zip", 
        NULL
    };
    
    if (execute_command(download_args) != 0){
        fprintf(stderr, "File gagal didownload!\n");
        return;
    }
    printf("Meng-ekstrak file ZIP...\n");
    char *unzip_args[] = {"unzip", "-j", "-o", "netflixData.zip", NULL};
    
    if (execute_command(unzip_args) != 0){
        fprintf(stderr, "File ZIP gagal diekstrak\n");
        return;
    }
    
    printf("Removing the ZIP file...\n");
    char *delete_args[] = {"rm", "netflixData.zip", NULL};
    if (execute_command(delete_args) != 0) fprintf(stderr, "File ZIP gagal dihapus\n");
    printf("File berhasil didownload!\n");
}

void parse_csv_line(char *line, char *columns[]) {
    int in_quotes = 0, col_idx = 0, temp_idx = 0;
    char temp[1000];
    char *current = line;

    while (*current != '\0') {
        if (*current == '"') {
            in_quotes = !in_quotes;
            temp[temp_idx++] = *current;
        } else if (*current == ',' && !in_quotes) {
            temp[temp_idx] = '\0';
            columns[col_idx++] = strdup(temp);
            temp_idx = 0;
        } else {
            temp[temp_idx++] = *current;
        }
        current++;
    }
    temp[temp_idx] = '\0';
    columns[col_idx++] = strdup(temp);
}

void buat_log(char *category, char *title) {
    FILE *log_file = fopen("log.txt", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }
    
    char time_str[9];
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
    
    fprintf(log_file, "[%s] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",
            time_str, category, title);
    fclose(log_file);
}


void sorting(char *csv_path, char *type) {
    int status;
    pid_t mkdir_pid = fork();
    if (mkdir_pid == 0) {
        char *mkdir_args[] = {"mkdir", "-p", type, NULL};
        execvp("mkdir", mkdir_args);
    } else {
        wait(&status);
    }

    FILE *csv_file = fopen(csv_path, "r");
    char line[1000];
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        line[strcspn(line, "\n")] = '\0';
        char *columns[4];
        parse_csv_line(line, columns);

        char *title = columns[0];
        char *director = columns[1];
        char *year = columns[3];
        char filename[50];

        if (strcmp(type, "judul") == 0) {
            buat_log("Abjad", title);
            char first_char = title[0];
            snprintf(filename, sizeof(filename), 
                    isalnum(first_char) ? "judul/%c.txt" : "judul/#.txt", first_char);
        } 
        else if (strcmp(type, "tahun") == 0) {
            buat_log("Tahun", title);
            snprintf(filename, sizeof(filename), "tahun/%s.txt", year);
        }

        FILE *output = fopen(filename, "a");
        if (output) {
            fprintf(output, "%s - %s - %s\n", title, year, director);
            fclose(output);
        }
    }
    fclose(csv_file);
}

void *process_country_data(void *arg) {
    char *csv_path = (char *)arg;
    FILE *csv_file = fopen(csv_path, "r");
    char line[1000];
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        line[strcspn(line, "\n")] = '\0';
        char *columns[4];
        parse_csv_line(line, columns);

        char *country = columns[2];
        int year = atoi(columns[3]);
        int found = 0;

        for (int i = 0; i < country_count; i++) {
            if (strcmp(country_reports[i].country, country) == 0) {
                year < 2000 ? country_reports[i].before_2000++ : country_reports[i].after_2000++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(country_reports[country_count].country, country);
            if (year < 2000) {
                country_reports[country_count].before_2000 = 1;
                country_reports[country_count].after_2000 = 0;
            } else {
                country_reports[country_count].before_2000 = 0;
                country_reports[country_count].after_2000 = 1;
            }
            country_count++;
        }
    }
    fclose(csv_file);
    return NULL;
}

void buat_report(char *csv_path) {
    pthread_t report_thread;
    pthread_create(&report_thread, NULL, process_country_data, csv_path);
    pthread_join(report_thread, NULL);

    char report_name[100];
    time_t raw_time = time(NULL);
    struct tm *tm_info = localtime(&raw_time);
    strftime(report_name, sizeof(report_name), "report_%d%m%Y.txt", tm_info);
    
    FILE *report_file = fopen(report_name, "w");
    for (int i = 0; i < country_count; i++) {
        fprintf(report_file, "%d. Negara: %s\n", i+1, country_reports[i].country);
        fprintf(report_file, "Film sebelum 2000: %d\n", country_reports[i].before_2000);
        fprintf(report_file, "Film setelah 2000: %d\n\n", country_reports[i].after_2000);
    }
    fclose(report_file);
}

int main() {
    int choice;
    char *data_path = "netflixData.csv";

    do {
        printf("\n1. Download File\n");
        printf("2. Mengelompokkan Film\n");
        printf("3. Membuat Report\n");
        printf("4. Keluar\n");
        printf("Pilihan : ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                download();
                break;
            case 2:
                pid_t judul_pid = fork();
                if (judul_pid == 0) {
                    sorting(data_path, "judul");
                    exit(0);
                }

                pid_t tahun_pid = fork();
                if (tahun_pid == 0) {
                    sorting(data_path, "tahun");
                    exit(0);
                }
                waitpid(judul_pid, NULL, 0);
                waitpid(tahun_pid, NULL, 0);
                break;
            case 3:
                printf("Membuat country reports...");
                buat_report(data_path);
                break;
            case 4:
                printf("Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }while (choice != 4);

    return 0;
}
```
# Kode untuk soal 2a (Mendownload file ZIP, mengekstrak file ZIP, menghapus file ZIP yang sudah tidak diperlukan)
```bash
int execute_command(char *const argv[]){
    pid_t pid = fork();
    if (pid < 0){
        perror("Fork failed");
        return -1;
    } else if (pid == 0){
        // Child process
        execvp(argv[0], argv);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else{
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void download() {
    FILE *log = fopen("log.txt", "w");
    if (log) fclose(log);
    printf("Men-download file ZIP...\n");
    char *download_args[] = {
        "wget", 
        "https://docs.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J", 
        "-O", 
        "netflixData.zip", 
        NULL
    };
    
    if (execute_command(download_args) != 0){
        fprintf(stderr, "File gagal didownload!\n");
        return;
    }
    printf("Meng-ekstrak file ZIP...\n");
    char *unzip_args[] = {"unzip", "-j", "-o", "netflixData.zip", NULL};
    
    if (execute_command(unzip_args) != 0){
        fprintf(stderr, "File ZIP gagal diekstrak\n");
        return;
    }
    
    printf("Removing the ZIP file...\n");
    char *delete_args[] = {"rm", "netflixData.zip", NULL};
    if (execute_command(delete_args) != 0) fprintf(stderr, "File ZIP gagal dihapus\n");
    printf("File berhasil didownload!\n");
}
```
PENJELASAN:

~~ Fungsi execute_command:
```bash
int execute_command(char *const argv[]){
    pid_t pid = fork();
    if (pid < 0){
        perror("Fork failed");
        return -1;
    } else if (pid == 0){
        // Child process
        execvp(argv[0], argv);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else{
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}
```
- fork(): Membuat proses baru (child process). Jika gagal, mengembalikan error
- Child process (pid == 0): Menjalankan perintah yang diberikan melalui execvp(). Fungsi ini menggantikan proses child dengan program baru sesuai argumen argv
- Jika execvp() gagal, akan muncul error dan program keluar dari proses child
- Parent process: Menunggu proses child selesai dengan command waitpid()
- Mengembalikan status exit dari proses child apabila berhasil dan -1 apabila gagal

Fungsi ini menggunakan child and parent process yang berguna untuk menjalankan perintah shell (seperti wget, unzip, rm) secara terpisah dan menunggu hingga selesai.

~~ Fungsi download:
```bash
void download() {
    FILE *log = fopen("log.txt", "w");
    if (log) fclose(log);
    printf("Men-download file ZIP...\n");
    char *download_args[] = {
        "wget", 
        "https://docs.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J", 
        "-O", 
        "netflixData.zip", 
        NULL
    };
    
    if (execute_command(download_args) != 0){
        fprintf(stderr, "File gagal didownload!\n");
        return;
    }
    printf("Meng-ekstrak file ZIP...\n");
    char *unzip_args[] = {"unzip", "-j", "-o", "netflixData.zip", NULL};
    
    if (execute_command(unzip_args) != 0){
        fprintf(stderr, "File ZIP gagal diekstrak\n");
        return;
    }
    
    printf("Removing the ZIP file...\n");
    char *delete_args[] = {"rm", "netflixData.zip", NULL};
    if (execute_command(delete_args) != 0) fprintf(stderr, "File ZIP gagal dihapus\n");
    printf("File berhasil didownload!\n");
}
```
- Membuka log.txt dengan mode write ("w") untuk mengosongkan file log (jika ada), lalu langsung ditutup
- Menampilkan pesan ke layar apabila proses download dimulai
- Menyiapkan argumen untuk menjalankan perintah wget yang mengunduh file ZIP dari URL Google Drive dan menyimpannya sebagai netflixData.zip
- Apabila proses berhasil, program akan menampilkan pesan bahwa proses ekstraksi file ZIP akan dilakukan
- Menyiapkan argumen untuk menjalankan perintah unzip dengan opsi -j untuk ekstrak tanpa membuat folder (junk paths) dan -o untuk overwrite file tanpa konfirmasi
- Memanggil execute_command untuk mengekstrak file ZIP. Jika gagal, menampilkan pesan error dan keluar fungsi
- Jika berhasil, menampilkan pesan bahwa file ZIP akan dihapus
- Menyiapkan argumen untuk menjalankan perintah rm untuk menghapus file ZIP
- Memanggil execute_command untuk menghapus file ZIP. Jika gagal, menampilkan pesan error
- Menampilkan pesan bahwa file berhasil didownload dan proses selesai

Hasil dari soal 2a:
![image](https://github.com/user-attachments/assets/f592d805-30dc-4e65-9e12-0246fc519a1c)

# Kode untuk soal 2b (Mengelompokkan file dan membuat file log)
```bash
void parse_csv_line(char *line, char *columns[]) {
    int in_quotes = 0, col_idx = 0, temp_idx = 0;
    char temp[1000];
    char *current = line;

    while (*current != '\0') {
        if (*current == '"') {
            in_quotes = !in_quotes;
            temp[temp_idx++] = *current;
        } else if (*current == ',' && !in_quotes) {
            temp[temp_idx] = '\0';
            columns[col_idx++] = strdup(temp);
            temp_idx = 0;
        } else {
            temp[temp_idx++] = *current;
        }
        current++;
    }
    temp[temp_idx] = '\0';
    columns[col_idx++] = strdup(temp);
}

void buat_log(char *category, char *title) {
    FILE *log_file = fopen("log.txt", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }
    
    char time_str[9];
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
    
    fprintf(log_file, "[%s] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",
            time_str, category, title);
    fclose(log_file);
}


void sorting(char *csv_path, char *type) {
    int status;
    pid_t mkdir_pid = fork();
    if (mkdir_pid == 0) {
        char *mkdir_args[] = {"mkdir", "-p", type, NULL};
        execvp("mkdir", mkdir_args);
    } else {
        wait(&status);
    }

    FILE *csv_file = fopen(csv_path, "r");
    char line[1000];
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        line[strcspn(line, "\n")] = '\0';
        char *columns[4];
        parse_csv_line(line, columns);

        char *title = columns[0];
        char *director = columns[1];
        char *year = columns[3];
        char filename[50];

        if (strcmp(type, "judul") == 0) {
            buat_log("Abjad", title);
            char first_char = title[0];
            snprintf(filename, sizeof(filename), 
                    isalnum(first_char) ? "judul/%c.txt" : "judul/#.txt", first_char);
        } 
        else if (strcmp(type, "tahun") == 0) {
            buat_log("Tahun", title);
            snprintf(filename, sizeof(filename), "tahun/%s.txt", year);
        }

        FILE *output = fopen(filename, "a");
        if (output) {
            fprintf(output, "%s - %s - %s\n", title, year, director);
            fclose(output);
        }
    }
    fclose(csv_file);
}
```
PENJELASAN:

~~ Fungsi parse_csv_line
```bash
void parse_csv_line(char *line, char *columns[]) {
    int in_quotes = 0, col_idx = 0, temp_idx = 0;
    char temp[1000];
    char *current = line;

    while (*current != '\0') {
        if (*current == '"') {
            in_quotes = !in_quotes;
            temp[temp_idx++] = *current;
        } else if (*current == ',' && !in_quotes) {
            temp[temp_idx] = '\0';
            columns[col_idx++] = strdup(temp);
            temp_idx = 0;
        } else {
            temp[temp_idx++] = *current;
        }
        current++;
    }
    temp[temp_idx] = '\0';
    columns[col_idx++] = strdup(temp);
}
```
- Fungsi ini memecah satu baris CSV menjadi kolom-kolomnya, memperhatikan tanda kutip agar koma di dalam kutip tidak dianggap sebagai pemisah
- Variabel in_quotes menandai apakah saat ini sedang di dalam tanda kutip
- Setiap kali menemukan koma di luar kutip, substring yang sudah terbaca disalin ke columns menggunakan strdup
- Outputnya akan menampilkan columns berisi string untuk tiap kolom (misal: judul, sutradara, negara, tahun)

~~ Fungsi buat_log
```bash
void buat_log(char *category, char *title) {
    FILE *log_file = fopen("log.txt", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }
    
    char time_str[9];
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
    
    fprintf(log_file, "[%s] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",
            time_str, category, title);
    fclose(log_file);
}
```
- Fungsi ini menulis log aktivitas ke file log.txt dengan format waktu saat log dicatat
- category adalah kategori pengelompokan (misal "Abjad" atau "Tahun")
- title adalah judul film yang sedang diproses
- Membuka file dalam mode append ("a") sehingga log baru ditambahkan ke akhir file
- Format waktu menggunakan jam:menit:detik

~~ Fungsi sorting
```bash
void sorting(char *csv_path, char *type) {
    int status;
    pid_t mkdir_pid = fork();
    if (mkdir_pid == 0) {
        char *mkdir_args[] = {"mkdir", "-p", type, NULL};
        execvp("mkdir", mkdir_args);
    } else {
        wait(&status);
    }

    FILE *csv_file = fopen(csv_path, "r");
    char line[1000];
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        line[strcspn(line, "\n")] = '\0';
        char *columns[4];
        parse_csv_line(line, columns);

        char *title = columns[0];
        char *director = columns[1];
        char *year = columns[3];
        char filename[50];

        if (strcmp(type, "judul") == 0) {
            buat_log("Abjad", title);
            char first_char = title[0];
            snprintf(filename, sizeof(filename), 
                    isalnum(first_char) ? "judul/%c.txt" : "judul/#.txt", first_char);
        } 
        else if (strcmp(type, "tahun") == 0) {
            buat_log("Tahun", title);
            snprintf(filename, sizeof(filename), "tahun/%s.txt", year);
        }

        FILE *output = fopen(filename, "a");
        if (output) {
            fprintf(output, "%s - %s - %s\n", title, year, director);
            fclose(output);
        }
    }
    fclose(csv_file);
}
```
- Membuat folder : Fungsi memulai dengan fork() dan execvp() untuk menjalankan perintah shell mkdir -p <type> agar folder judul/ atau tahun/ dibuat jika belum ada.Proses induk menunggu proses anak selesai dengan wait()
- Membuka file CSV : File CSV dibuka untuk dibaca, dan baris pertama (header) dilewati dengan fgets()
- Membaca setiap baris data film : Loop membaca setiap baris, menghilangkan newline, dan memecah kolom menggunakan parse_csv_line
- Menentukan nama file output menjadi dua type : "tahun" dan "judul"
- Jika type adalah "judul", maka log aktivitas dibuat dengan kategori "Abjad", ambil karakter pertama judul film (first_char), jika karakter tersebut huruf atau angka (isalnum), file output adalah judul/<char>.txt, misal judul/A.txt. Jika bukan huruf/angka, file output adalah judul/#.txt
- Jika type adalah "tahun", maka log aktivitas dibuat dengan kategori "Tahun", file output adalah tahun/<tahun>.txt, misal tahun/1999.txt
- Membuka file output dan menulis data : file output dibuka dalam mode append ("a"), lalu menulis data film dalam format: judul - tahun - sutradara, Setelah itu file ditutup
- Proses ini dijalankan berulang kali untuk setiap baris dalam file CSV

Hasil dari soal 2b:
- file log.txt
![image](https://github.com/user-attachments/assets/043e9c7b-3fa0-4d94-9858-859f17aecfb1)
- sortingan dengan type "judul"
![image](https://github.com/user-attachments/assets/f1089489-7a9a-4cab-8288-1bfedc5cbd51)
- sortingan dengan type "tahun"
![image](https://github.com/user-attachments/assets/ad4c9ca7-e008-4628-adb2-aa526f4da19d)

# Kode untuk soal 2C (Laporan film tiap negara dan menu menjalankan semua proses di atas melalui sebuah antarmuka terminal interaktif)
```bash
typedef struct country_report {
    int before_2000;
    int after_2000;
    char country[100];
} country_report;

country_report country_reports[100];
int country_count = 0;

void *process_country_data(void *arg) {
    char *csv_path = (char *)arg;
    FILE *csv_file = fopen(csv_path, "r");
    char line[1000];
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        line[strcspn(line, "\n")] = '\0';
        char *columns[4];
        parse_csv_line(line, columns);

        char *country = columns[2];
        int year = atoi(columns[3]);
        int found = 0;

        for (int i = 0; i < country_count; i++) {
            if (strcmp(country_reports[i].country, country) == 0) {
                year < 2000 ? country_reports[i].before_2000++ : country_reports[i].after_2000++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(country_reports[country_count].country, country);
            if (year < 2000) {
                country_reports[country_count].before_2000 = 1;
                country_reports[country_count].after_2000 = 0;
            } else {
                country_reports[country_count].before_2000 = 0;
                country_reports[country_count].after_2000 = 1;
            }
            country_count++;
        }
    }
    fclose(csv_file);
    return NULL;
}

void buat_report(char *csv_path) {
    pthread_t report_thread;
    pthread_create(&report_thread, NULL, process_country_data, csv_path);
    pthread_join(report_thread, NULL);

    char report_name[100];
    time_t raw_time = time(NULL);
    struct tm *tm_info = localtime(&raw_time);
    strftime(report_name, sizeof(report_name), "report_%d%m%Y.txt", tm_info);
    
    FILE *report_file = fopen(report_name, "w");
    for (int i = 0; i < country_count; i++) {
        fprintf(report_file, "%d. Negara: %s\n", i+1, country_reports[i].country);
        fprintf(report_file, "Film sebelum 2000: %d\n", country_reports[i].before_2000);
        fprintf(report_file, "Film setelah 2000: %d\n\n", country_reports[i].after_2000);
    }
    fclose(report_file);
}

int main() {
    int choice;
    char *data_path = "netflixData.csv";

    do {
        printf("\n1. Download File\n");
        printf("2. Mengelompokkan Film\n");
        printf("3. Membuat Report\n");
        printf("4. Keluar\n");
        printf("Pilihan : ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                download();
                break;
            case 2:
                pid_t judul_pid = fork();
                if (judul_pid == 0) {
                    sorting(data_path, "judul");
                    exit(0);
                }

                pid_t tahun_pid = fork();
                if (tahun_pid == 0) {
                    sorting(data_path, "tahun");
                    exit(0);
                }
                waitpid(judul_pid, NULL, 0);
                waitpid(tahun_pid, NULL, 0);
                break;
            case 3:
                printf("Membuat country reports...");
                buat_report(data_path);
                break;
            case 4:
                printf("Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }while (choice != 4);

    return 0;
}
```
PENJELASAN:

~~ Struct ountry_report dan variabel global
```bash
typedef struct country_report {
    int before_2000;
    int after_2000;
    char country[100];
} country_report;

country_report country_reports[100];
int country_count = 0;
```
- country_report menyimpan jumlah film sebelum dan setelah tahun 2000 untuk suatu negara
- country_reports adalah array global yang menyimpan data per negara
- country_count menyimpan jumlah negara yang sudah diproses

~~ Fungsi *process_country_data
```bash
void *process_country_data(void *arg) {
    char *csv_path = (char *)arg;
    FILE *csv_file = fopen(csv_path, "r");
    char line[1000];
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        line[strcspn(line, "\n")] = '\0';
        char *columns[4];
        parse_csv_line(line, columns);

        char *country = columns[2];
        int year = atoi(columns[3]);
        int found = 0;

        for (int i = 0; i < country_count; i++) {
            if (strcmp(country_reports[i].country, country) == 0) {
                year < 2000 ? country_reports[i].before_2000++ : country_reports[i].after_2000++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(country_reports[country_count].country, country);
            if (year < 2000) {
                country_reports[country_count].before_2000 = 1;
                country_reports[country_count].after_2000 = 0;
            } else {
                country_reports[country_count].before_2000 = 0;
                country_reports[country_count].after_2000 = 1;
            }
            country_count++;
        }
    }
    fclose(csv_file);
    return NULL;
}
```
- Fungsi ini dijalankan oleh thread
- Membaca file CSV baris per baris, dengan skip bagian header
- Memecah baris menjadi kolom dengan parse_csv_line di function yang dicantumkan sebelumnya
- Mengambil nama negara dan tahun rilis film
- Mencari apakah negara sudah ada di country_reports
- Jika sudah, update hitungan film sebelum atau sesudah tahun 2000
- Jika belum, tambahkan data negara baru dengan inisialisasi hitungan
- Mengembalikan NULL value saat selesai

~~ Fungsi buat_report
```bash
void buat_report(char *csv_path) {
    pthread_t report_thread;
    pthread_create(&report_thread, NULL, process_country_data, csv_path);
    pthread_join(report_thread, NULL);

    char report_name[100];
    time_t raw_time = time(NULL);
    struct tm *tm_info = localtime(&raw_time);
    strftime(report_name, sizeof(report_name), "report_%d%m%Y.txt", tm_info);
    
    FILE *report_file = fopen(report_name, "w");
    for (int i = 0; i < country_count; i++) {
        fprintf(report_file, "%d. Negara: %s\n", i+1, country_reports[i].country);
        fprintf(report_file, "Film sebelum 2000: %d\n", country_reports[i].before_2000);
        fprintf(report_file, "Film setelah 2000: %d\n\n", country_reports[i].after_2000);
    }
    fclose(report_file);
}
```
- Membuat thread baru yang menjalankan process_country_data untuk memproses data dari file CSV
- pthread_join menunggu thread selesai agar data sudah lengkap
- Membuat nama file laporan dengan format report_ddmmyyyy.txt berdasarkan tanggal saat ini
- Membuka file laporan dan menulis data statistik per negara sesuai format yang diminta
- Menutup file laporan

~~ Fungsi main
```bash
int main() {
    int choice;
    char *data_path = "netflixData.csv";

    do {
        printf("\n1. Download File\n");
        printf("2. Mengelompokkan Film\n");
        printf("3. Membuat Report\n");
        printf("4. Keluar\n");
        printf("Pilihan : ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                download();
                break;
            case 2:
                pid_t judul_pid = fork();
                if (judul_pid == 0) {
                    sorting(data_path, "judul");
                    exit(0);
                }

                pid_t tahun_pid = fork();
                if (tahun_pid == 0) {
                    sorting(data_path, "tahun");
                    exit(0);
                }
                waitpid(judul_pid, NULL, 0);
                waitpid(tahun_pid, NULL, 0);
                break;
            case 3:
                printf("Membuat country reports...");
                buat_report(data_path);
                break;
            case 4:
                printf("Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }while (choice != 4);

    return 0;
}
```
- Menampilkan menu interaktif di terminal dengan memanfaatkan do while dan switch - case
- Apabila user menginput "1" maka, program akan menampilkan hasil sesuai pilihan 1: Memanggil fungsi download() untuk mengunduh dan ekstrak data
- Apabila user menginput "2" maka, program akan menampilkan hasil sesuai pilihan 2: Membuat dua proses anak (fork) untuk menjalankan pengelompokan film berdasarkan judul dan tahun secara paralel (menggunakan fungsi sorting)
- Apabila user menginput "3" maka, program akan menampilkan hasil sesuai pilihan 3: Membuat laporan statistik negara menggunakan thread (buat_report)
- Apabila user menginput "4" maka, program akan menampilkan hasil sesuai pilihan 4: Keluar dari program
- Jika input tidak valid, menampilkan pesan error
- Loop terus berjalan sampai pengguna memilih keluar

Hasil dari soal 2c:
- File report
![image](https://github.com/user-attachments/assets/02d30858-6f43-462e-ad28-4e6843a4444a)
- Menu Interaktif
![image](https://github.com/user-attachments/assets/e5036d25-6342-4ecb-85f3-c6731ce83261)


## 3️⃣Task 3 (Cella’s Manhwa)

# *Cella’s Manhwa*

Cella, si ratu scroll Facebook, tiba-tiba terinspirasi untuk mengumpulkan informasi dan foto dari berbagai *manhwa favoritnya*. Namun, kemampuan ngoding Cella masih cetek, jadi dia butuh bantuanmu untuk membuatkan skrip otomatis agar semua berjalan mulus. Tugasmu adalah membantu Cella mengolah data manhwa dan heroine-nya.

Berikut adalah daftar manhwa bergenre shoujo/josei yang paling disukai Cella:

|    No     |      Manhwa      |
| :--------: | :------------: |
| 1 | Mistaken as the Monster Duke's Wife |
| 2 | The Villainess Lives Again |
| 3 | No, I Only Charmed the Princess! |
| 4 | Darling, Why Can't We Divorce? |

### *a. Summoning the Manhwa Stats*

Cella ingin mengambil data detail dari *manhwa* menggunakan [API Jikan](https://docs.api.jikan.moe/). Informasi yang diambil:

- Judul
- Status
- Tanggal rilis
- Genre
- Tema
- Author

Setelah data berhasil diambil, hasilnya harus disimpan ke dalam file teks, dengan nama file disesuaikan dengan *judul versi bahasa Inggris* (tanpa karakter khusus dan spasi diganti dengan underscore). Semua file teks disimpan dalam folder Manhwa.

*Contoh format isi file:*


Title: The Villain's Daughter-in-Law
Status: Publishing
Release: 2024-10-16
Genre: Fantasy, Romance
Theme: Time Travel
Author: Na, Reuyan, Kim, Dael


*&#128161; Hint*

Contoh Penggunaan Simple API:
1. Kunjungi situs *MyAnimeList*.

2. Cari *manhwa* yang diinginkan.

3. Sebagai contoh:

    https://myanimelist.net/manga/154063/The_Perks_of_Being_a_Villainess

3. Gunakan *154063* sebagai *ID* untuk melakukan pengambilan data (scraping).

4. Sehingga *endpoint API* akan menjadi seperti berikut:

    https://api.jikan.moe/v4/manga/154063

### *b. Seal the Scrolls*

Cella ingin agar setiap file .txt tadi di-*zip* satu per satu dan disimpan ke dalam folder baru bernama Archive. Yang dimana nama masing masing dari zip diambil dari *huruf kapital nama file*.

### *c. Making the Waifu Gallery*

Setiap manhwa memiliki heroine alias *Female Main Character (FMC). Cella ingin mengunduh gambar heroine dari internet, dengan jumlah unduhan sesuai dengan **bulan rilis manhwa*.

*Contoh:*

- Jika rilis bulan Februari → unduh *2 foto*
- Jika rilis bulan Desember → unduh *12 foto*
- Format nama file: Heroine_1.jpg, Heroine_2.jpg, dst.

Selain itu, Cella ingin melakukan pengunduhan *sesuai urutan* daftar manhwa yang tertera pada deskripsi di atas, dan proses pengunduhan harus menggunakan *thread*, karena Cella malas menunggu. Sebagai contohnya, gambar heroine dari manhwa Mistaken as the Monster Duke's Wife harus diunduh terlebih dahulu dan tidak boleh didahului oleh gambar heroine dari manhwa lainnya.

Seluruh gambar akan disimpan dalam folder Heroines. Di dalam folder Heroines, akan terdapat subfolder dengan nama depan atau nama panggilan heroine dari masing-masing manhwa.

Struktur folder yang diinginkan:


Heroines/
├── Alisha/
│   ├── Alisha_1.jpg
│   └── Alisha_2.jpg
└── Dorothea/
    ├── Dorothea_1.jpg
    └── Dorothea_2.jpg


### *d. Zip. Save. Goodbye*

Setelah semua gambar heroine berhasil diunduh, Cella ingin mengarsipkannya:

- Setiap folder heroine di-zip dengan format:
  
  [HURUFKAPITALNAMAMANHWA]_[namaheroine].zip
  
- Disimpan di folder Archive/Images
- Setelah zip selesai, gambar pada masing masing folder Heroine akan dihapus secara *urut dengan abjad*.

### Notes &#9888;

- Gunakan fork() dan exec() untuk soal a, b, dan, d.

- system() *HANYA BOLEH DIGUNAKAN (bila ingin)* untuk soal c.

- *Dilarang keras menggunakan external script!*.

- Dilarang menggunakan mkdir().

- Hanya solver.c yang dikumpulkan pada GitHub.

---

# *Cella’s Manhwa*

Cella, the queen of Facebook scrolling, suddenly got inspired to collect information and photos from her favorite *manhwa*. However, her coding skills are still basic, so she needs your help to create an automated script to make everything run smoothly. Your task is to assist Cella in processing manhwa data and its heroines.

The following is a list of shoujo/josei genre manhwa most favored by Cella:

|    No     |      Manhwa      |
| :--------: | :------------: |
| 1 | Mistaken as the Monster Duke's Wife |
| 2 | The Villainess Lives Again |
| 3 | No, I Only Charmed the Princess! |
| 4 | Darling, Why Can't We Divorce? |

### *a. Summoning the Manhwa Stats*

Cella wants to retrieve detailed data about *manhwa* using the [Jikan API](https://docs.api.jikan.moe/). The information to be retrieved includes:

- Title
- Status
- Release date
- Genre
- Theme
- Author

Once the data is successfully retrieved, the results must be saved into a text file, with the file name based on the *English title* (special characters removed and spaces replaced with underscores). All text files are stored in the Manhwa folder.

*Example file content format:*


Title: The Villain's Daughter-in-Law
Status: Publishing
Release: 2024-10-16
Genre: Fantasy, Romance
Theme: Time Travel
Author: Na, Reuyan, Kim, Dael


*&#128161; Hint*

Example of Simple API Usage:
1. Visit the *MyAnimeList* website.

2. Search for the desired *manhwa*.

3. For example:

    https://myanimelist.net/manga/154063/The_Perks_of_Being_a_Villainess

4. Use *154063* as the *ID* for scraping the data.

5. Therefore, the *API endpoint* will look like this:

    https://api.jikan.moe/v4/manga/154063

### *b. Seal the Scrolls*

Cella wants each .txt file to be *zipped* individually and stored in a new folder named Archive. The name of each zip file should be derived from the *uppercase letters of the file name*.

### *c. Making the Waifu Gallery*

Each manhwa has a heroine, also known as the *Female Main Character (FMC). Cella wants to download heroine images from the internet, with the number of downloads corresponding to the **release month of the manhwa*.

*Example:*

- If released in February → download *2 photos*
- If released in December → download *12 photos*
- File name format: Heroine_1.jpg, Heroine_2.jpg, and so on.

Additionally, Cella wants the downloads to be performed *in order* based on the manhwa list described above, and the download process must use *threads*, as Cella doesn’t want to wait. For example, the heroine images from the manhwa Mistaken as the Monster Duke's Wife must be downloaded first and cannot be preceded by images from other manhwa.

All images will be stored in the Heroines folder. Inside the Heroines folder, there will be subfolders named after the first name or nickname of the heroine from each manhwa.

Desired folder structure:


Heroines/
├── Alisha/
│   ├── Alisha_1.jpg
│   └── Alisha_2.jpg
└── Dorothea/
    ├── Dorothea_1.jpg
    └── Dorothea_2.jpg


### *d. Zip. Save. Goodbye*

After all heroine images are successfully downloaded, Cella wants them archived:

- Each heroine folder is zipped with the format:
  
  [UPPERCASEMANHWANAME]_[heroineName].zip
  
- Stored in the Archive/Images folder
- Once the zipping is complete, the images in each Heroine folder will be deleted *in alphabetical order*.

### Notes &#9888;

- Use fork() and exec() for questions a, b, and d.

- system() *MAY ONLY BE USED (if desired)* for question c.

- *Strictly prohibited from using external scripts!*

- Using mkdir() is not allowed.

- Only solver.c should be submitted on GitHub.


# Kode Akhir
```bash
c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>      
#include <pthread.h>

void Folder(const char *foldername){
    pid_t pid = fork();
    if (pid == 0) {
        execlp("mkdir", "mkdir", "-p", foldername, NULL);
        perror("mkdir failed");
        exit(EXIT_FAILURE);
    } 
    else wait(NULL);
}

void fetchJSON(const char *id){
    pid_t pid = fork();
    if (pid == 0){
        char url[256];
        snprintf(url, sizeof(url), "https://api.jikan.moe/v4/manga/%s/full", id);
        execlp("curl", "curl", "-s", "-o", "response.json", url, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } 
    else wait(NULL);
}

void cleanTitle(char *dest, const char *src){
    while (*src){
        if (isalnum(*src))*dest++ = *src;
        else if (*src == ' ')*dest++ = '_';
        src++;
    }
    *dest = '\0';
}

void parseAndSave(){
    FILE *fp = fopen("response.json", "r");
    if (!fp) {
        perror("Buka response.json gagal");
        return;
    }
    char buffer[8192];
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);
    // Title
    char *judulPtr = strstr(buffer, "\"title\"");
    char title[256] = "Unknown";
    if (judulPtr) sscanf(judulPtr, "\"title\": \"%[^\"]", title);
    // Status
    char *statusPtr = strstr(buffer, "\"status\"");
    char status[128] = "Unknown";
    if (statusPtr) sscanf(statusPtr, "\"status\": \"%[^\"]", status);
    // Release Date
    char *tglPtr = strstr(buffer, "\"from\"");
    char release[128] = "Unknown";
    if (tglPtr){
        char fullDate[128];
        sscanf(tglPtr, "\"from\": \"%[^\"]", fullDate);
        // ambil cmn 10 karakter pertama
        strncpy(release, fullDate, 10);
        release[10] = '\0'; 
    }
    // Genre
    char *genrePtr = strstr(buffer, "\"genres\":");
    char genre[512] = "";
    if (genrePtr){
        char *g = strstr(genrePtr, "\"name\":");
        while (g && g < strstr(buffer, "\"themes\"")){
            char temp[64];
            sscanf(g, "\"name\": \"%[^\"]", temp);
            strcat(genre, temp);
            strcat(genre, ", ");
            g = strstr(g + 1, "\"name\":");
        }
        if (strlen(genre)) genre[strlen(genre) - 2] = '\0';  
    }
    // Theme
    char *themePtr = strstr(buffer, "\"themes\":");
    char theme[256] = "";
    if (themePtr){
        char *themeEnd = strstr(themePtr, "\"demographics\"");
        char *t = strstr(themePtr, "\"name\":");
        while (t && (!themeEnd || t < themeEnd)){
            char temp[64];
            sscanf(t, "\"name\": \"%[^\"]", temp);
            strcat(theme, temp);
            strcat(theme, ", ");
            t = strstr(t + 1, "\"name\":");
        }
        if (strlen(theme)) theme[strlen(theme) - 2] = '\0'; 
    }
    // Author
    char *authorPtr = strstr(buffer, "\"authors\":");
    char authors[256] = "";
    if (authorPtr){
        char *authorEnd = strstr(authorPtr, "\"serializations\"");
        char *a = strstr(authorPtr, "\"name\":");
        while (a && (!authorEnd || a < authorEnd)){
            char temp[64];
            sscanf(a, "\"name\": \"%[^\"]", temp);
            strcat(authors, temp);
            strcat(authors, ", ");
            a = strstr(a + 1, "\"name\":");
        }
        if (strlen(authors)) authors[strlen(authors) - 2] = '\0';
    }

    // Save ke file
    char filename[256];
    cleanTitle(filename, title);
    char filepath[300];
    sprintf(filepath, "Manhwa/%s.txt", filename);
    FILE *out = fopen(filepath, "w");
    if (!out){
        perror("Gagal menulis file output");
        return;
    }
    fprintf(out, "Title: %s\n", title);
    fprintf(out, "Status: %s\n", status);
    fprintf(out, "Release: %s\n", release);
    fprintf(out, "Genre: %s\n", genre);
    fprintf(out, "Theme: %s\n", theme);
    fprintf(out, "Author: %s\n", authors);
    fclose(out);
}

void Uppercase(char *dest, const char *src) {
    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) if (isupper(src[i]))dest[j++] = src[i];
    dest[j] = '\0';
}

// Upprcase_Heroin
typedef struct {
    const char* heroine;
    const char* manhwa_code;
} HeroineInfo;
HeroineInfo heroines[] = {{"Adelia",  "NIOCTP"}, {"Artezia", "TVLA"}, {"Lia",     "MMDW"}, {"Ophelia", "DWCD"}};

void Zipmanga() {
    Folder("Archive");
    
    // Buka direct
    DIR *dir = opendir("Manhwa");
    if (!dir){
        perror("Cannot open Manhwa directory");
        return;
    }
    
   struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_type == DT_DIR || !strstr(entry->d_name, ".txt"))continue;

        printf("Found file: %s\n", entry->d_name);

        char filename[256];
        // delete ekstensi file
        strncpy(filename, entry->d_name, sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
        // path
        char source[512];
        snprintf(source, sizeof(source), "Manhwa/%s", entry->d_name);
        char *dot = strrchr(filename, '.');
        if (dot) *dot = '\0';
        char uppercase[256] = {0};
        Uppercase(uppercase, filename);
        char dest[512];
        snprintf(dest, sizeof(dest), "Archive/%s.zip", uppercase);
        printf("Zipping %s to %s\n", source, dest);

        // fork & exec untuk zip
        pid_t pid = fork();
        if (pid == 0) {
            execlp("zip", "zip", "-j", dest, source, NULL);
            perror("Zip failed");
            exit(EXIT_FAILURE);
        } 
        else wait(NULL);
    }
}

void ZipAkhir() {
    Folder("Archive/Images");

    const int n = sizeof(heroines) / sizeof(HeroineInfo);

    for (int i = 0; i < n; i++) {
        const char* heroine = heroines[i].heroine;
        const char* manhwa_code = heroines[i].manhwa_code;
        char zipname[256];
        snprintf(zipname, sizeof(zipname), "Archive/Images/%s_%s.zip", manhwa_code, heroine);
        char folder[256];
        snprintf(folder, sizeof(folder), "Heroines/%s", heroine);

        // fork & exec untuk zip
        pid_t pid = fork();
        if (pid == 0){
            execlp("zip", "zip", "-r", zipname, folder, NULL);
            perror("Zip failed");
            exit(EXIT_FAILURE);
        } 
        else wait(NULL);
    }
    for (int i = 0; i < n; i++) {
        const char* heroine = heroines[i].heroine;

        char folder[256];
        snprintf(folder, sizeof(folder), "Heroines/%s", heroine);

        DIR* dir = opendir(folder);
        if (!dir) {
            perror("opendir failed");
            continue;
        }

        struct dirent* entry;
        // delete isi direct
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".jpg")) {
                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s/%s", folder, entry->d_name);
                remove(filepath); 
            }
        }

        closedir(dir);
    }
}

// Buat folder (Soal C only)
void makeFolder(const char* path) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", path);
    system(cmd);
}

// Link" Heroine
typedef struct {
    const char* heroine;   
    const char* image_url; 
    int count;             
} DownloadTask;

void* downloadImages(void* arg) {
    DownloadTask* task = (DownloadTask*)arg;
    // utk folder heroine
    char folder[256];
    snprintf(folder, sizeof(folder), "Heroines/%s", task->heroine);
    makeFolder(folder);
    // donlod
    for (int i = 0; i < task->count; i++) {
        char filename[300];
        snprintf(filename, sizeof(filename), "Heroines/%s/%s_%d.jpg",
                 task->heroine, task->heroine, i + 1);
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "curl -s %s -o \"%s\"",
                 task->image_url, filename);
        system(cmd);
    }
    pthread_exit(NULL);
}

int main() {
    fetchJSON("168827");  // Mistake
    parseAndSave();
    fetchJSON("147205");  // The vilainess
    parseAndSave();
    fetchJSON("169731");  // No, i charmed
    parseAndSave();
    fetchJSON("175521");  // Divorce
    parseAndSave();
    Zipmanga();
    makeFolder("Heroines");
    const char* url_lia     = "https://i.imgur.com/CJuHmrb.png";
    const char* url_artezia = "https://i.imgur.com/5c9Lcs5.png";
    const char* url_adelia  = "https://i.imgur.com/zF3Jq8s.png";
    const char* url_ophelia = "https://i.imgur.com/5GyLoVN.png";
    Folder("Manhwa");
    DownloadTask tasks[] = {
        {"Lia",     url_lia,     3}, // Maret
        {"Artezia", url_artezia, 6}, // June
        {"Adelia",  url_adelia,  4}, // April
        {"Ophelia", url_ophelia, 10}  // Oktober
    };

    const int n = sizeof(tasks) / sizeof(DownloadTask);

    for (int i = 0; i < n; i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, downloadImages, (void*)&tasks[i]);
        pthread_join(tid, NULL);  // Downloadnya urut
    }

    ZipAkhir();

    return 0;    
}
```

## Kode untuk 3a. Summoning the Manhwa Stats

### Kode C:
```bash
c
void fetchJSON(const char *id){
    pid_t pid = fork();
    if (pid == 0){
        char url[256];
        snprintf(url, sizeof(url), "https://api.jikan.moe/v4/manga/%s/full", id);
        execlp("curl", "curl", "-s", "-o", "response.json", url, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }
    else wait(NULL);
}

void parseAndSave(){
    FILE *fp = fopen("response.json", "r");
    char buffer[8192];
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);
    ... // Ambil title, status, release, genre, theme, author pakai strstr + sscanf
    FILE *out = fopen(filepath, "w");
    fprintf(out, ...); // Tulis data ke file
    fclose(out);
}

int main() {
    fetchJSON("168827"); parseAndSave();
    fetchJSON("147205"); parseAndSave();
    fetchJSON("169731"); parseAndSave();
    fetchJSON("175521"); parseAndSave();
}
```

###  Penjelasan:
- pid_t pid = fork();
  - Membuat child process. Kalau pid == 0, berarti kita di proses anak. Parent lanjut ke bawah.

- snprintf(url, sizeof(url), ...)
  - membuat URL API JIKAN berdasarkan ID manhwa yang ditentukan.

- execlp("curl", "curl", "-s", "-o", "response.json", url, NULL);
  - Ganti proses anak jadi proses curl untuk mengambil file JSON dari API dan simpan ke file response.json.

- wait(NULL);
  - Parent menunggu anak selesai.

- fopen("response.json", "r");
  - Buka file hasil curl buat dibaca.

- fread(buffer, 1, sizeof(buffer), fp);
  - Baca seluruh isi JSON ke dalam buffer.

- strstr() + sscanf()
  - Cari dan ambil semua data penting yang dibutuhkan soal dari JSON, seperti judul, status, tanggal rilis, dll.

- fprintf(out, ...)
  - Tulis data ke file txt dengan format sesuai soal.

- cleanTitle() membersihkan nama file supaya aman, ganti spasi jadi underscore.

---

##  3b. Seal the Scrolls

###  Kode C:
```bash
c
void Zipmanga() {
    Folder("Archive");
    DIR *dir = opendir("Manhwa");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR || !strstr(entry->d_name, ".txt")) continue;
        ...
        Uppercase(uppercase, filename);
        snprintf(dest, ..., "Archive/%s.zip", uppercase);
        pid_t pid = fork();
        if (pid == 0) execlp("zip", "zip", "-j", dest, source, NULL);
        else wait(NULL);
    }
}
```

###  Penjelasan:
- opendir("Manhwa") + readdir()
  - Buka folder Manhwa dan cek semua file di dalamnya.

- entry->d_type == DT_DIR
  - Kalau ketemu folder selain`.txt` , maka dapat dilewati. Kita hanya mencari file .txt.

- Uppercase()
  - Ambil huruf besar dari nama file untuk dibuat menjadi jadi nama zip.

- snprintf(dest, ..., "Archive/%s.zip", uppercase);
  - Buat path lengkap untuk file zip.

- execlp("zip", "zip", "-j", dest, source, NULL);
  - Zip file .txt tanpa menyimpan struktur folder.

---

##  3c. Making the Waifu Gallery

###  Kode C:
```bash
c
typedef struct {
    const char* heroine;
    const char* image_url;
    int count;
} DownloadTask;

void* downloadImages(void* arg) {
    DownloadTask* task = (DownloadTask*)arg;
    snprintf(folder, ..., "Heroines/%s", task->heroine);
    makeFolder(folder);
    for (int i = 0; i < task->count; i++) {
        snprintf(filename, ..., "Heroines/%s/%s_%d.jpg", ...);
        snprintf(cmd, ..., "curl -s %s -o \"%s\"", ...);
        system(cmd);
    }
    pthread_exit(NULL);
}

int main() {
    DownloadTask tasks[] = { {"Lia", url, 3}, ... };
    for (int i = 0; i < n; i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, downloadImages, &tasks[i]);
        pthread_join(tid, NULL);
    }
}
```

###  Penjelasan:
- typedef struct DownloadTask membuat struktur buat nyimpen nama heroine, link gambar, dan jumlah gambar.

- pthread_create() buat jalankan thread baru yang ngejalanin downloadImages().

- pthread_join() pastikann download heroine sekarang selesai dulu sebelum lanjut ke heroine berikutnya.

- system(cmd) menjalankan perintah curl dari sistem buat download gambar (hanya boleh di problem C).

- snprintf(...) spesifikasikan nama folder dan nama file dengan aman.

- makeFolder() membuat folder pakai mkdir -p via system().

---

##  3d. Zip. Save. Goodbye

###  Kode C:
```bash
c
void ZipAkhir() {
    Folder("Archive/Images");
    for (int i = 0; i < n; i++) {
        snprintf(zipname, ..., "Archive/Images/%s_%s.zip", manhwa_code, heroine);
        pid_t pid = fork();
        if (pid == 0) execlp("zip", "zip", "-r", zipname, folder, NULL);
        else wait(NULL);
    }
    for (int i = 0; i < n; i++) {
        DIR* dir = opendir(...);
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".jpg")) remove(filepath);
        }
        closedir(dir);
    }
}
```

###  Penjelasan:
- Folder("Archive/Images")
  - Buat folder baru untuk menyimpan hasil zip dari gambar heroine.

- snprintf() buat nama zip seperti template soal DWCD_Ophelia.zip

- execlp("zip", ...) zip folder heroine satu per satu lewat fork + exec.

- Loop kedua buka folder heroine, cari file .jpg, lalu hapus memakai remove().

- strstr(entry->d_name, ".jpg") pastikan cuma file gambar yang dihapus.

## 4️⃣Task 4 (Pipip's Load Balancer)
Pipip, seorang pengembang perangkat lunak yang tengah mengerjakan proyek distribusi pesan dengan sistem load balancing, memutuskan untuk merancang sebuah sistem yang memungkinkan pesan dari client bisa disalurkan secara efisien ke beberapa worker. Dengan menggunakan komunikasi antar-proses (IPC), Pipip ingin memastikan bahwa proses pengiriman pesan berjalan mulus dan terorganisir dengan baik, melalui sistem log yang tercatat dengan rapi.

Soal:
### *a. Client Mengirimkan Pesan ke Load Balancer*

Pipip ingin agar proses client.c dapat mengirimkan pesan ke loadbalancer.c menggunakan IPC dengan metode *shared memory*. Proses pengiriman pesan dilakukan dengan format input dari pengguna sebagai berikut:


Halo A;10


*Penjelasan:*

- "Halo A" adalah isi pesan yang akan dikirim.
- 10 adalah jumlah pesan yang ingin dikirim, dalam hal ini sebanyak 10 kali pesan yang sama.

Selain itu, setiap kali pesan dikirim, proses client.c harus menuliskan aktivitasnya ke dalam *sistem.log* dengan format:


Message from client: <isi pesan>
Message count: <jumlah pesan>


Semua pesan yang dikirimkan dari client akan diteruskan ke loadbalancer.c untuk diproses lebih lanjut.

`client.c`:
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234       
#define MAX_MESSAGE 256    

typedef struct {
    char message[MAX_MESSAGE];
    int count;
} SharedData;

int main() {

    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) 
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    SharedData *data = (SharedData *) shmat(shmid, NULL, 0);

    if (data == (void *) -1) 
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    char input[300];
 
    printf("Masukkan pesan dan jumlah pesan (pesan;jumlah):\n");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = 0;

    char *token = strtok(input, ";");
   
    if (token == NULL) 
    {
        printf("Format input salah.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(data->message, token);

    token = strtok(NULL, ";");

    if (token == NULL) 
    {
        printf("Format input salah.\n");
        exit(EXIT_FAILURE);
    }
    data->count = atoi(token);

    FILE *logfile = fopen("sistem.log", "a");

    if (logfile == NULL) 
    {
        perror("Gagal membuka sistem.log");
        exit(EXIT_FAILURE);
    }

    fprintf(logfile, "Message from client: %s\n", data->message);
    fprintf(logfile, "Message count: %d\n", data->count);
    fclose(logfile);

    printf("Pesan berhasil dikirim ke shared memory.\n");

    shmdt(data);

    return 0;
}
```
Penjelasan:
- #define SHM_KEY 1234 alamat/key shared memory
- #define MAX_MESSAGE 256 ukuran maksimal pesan 256 karakter
- ``typedef struct {
    char message[MAX_MESSAGE];
    int count;
} SharedData;``
SharedData, message untuk menyimpan pesan dan count untuk menghitung berapa kali pesan dikirim
- ``  int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);`` untuk membuat/mengakses shared memory, `shmget()` untuk membuat/mengambil shared memory dengan parameter `SHM_KEY` (kunci shared memory), `sizeof(SharedData)` (ukuran memori yang diperlukan), dan `IPC_CREAT | 0666` (permission `0666`, izin untuk baca/tulis)
``    if (shmid == -1) 
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }`` (untuk keluar program kalau gagal)
- `` SharedData *data = (SharedData *) shmat(shmid, NULL, 0);`` untuk menghubungkan shared memory ke program, `shmat()` untuk menempelkan shared memory ke program agar bisa diakses lewat pointer `data`
`` if (data == (void *) -1) 
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }`` (sama, untuk keluar program kalau gagal)
- `` char input[300];
    printf("Masukkan pesan dan jumlah pesan (format: pesan;jumlah):\n");
    fgets(input, sizeof(input), stdin);`` untuk minta input, `fgets()` untuk ambil input (termasuk spasi)
    Hapus karakter newline (otomatis ditambahkan `fgets()): ``input[strcspn(input, "\n")] = 0;`
- `` char *token = strtok(input, ";");`` untuk memisahkan pesan dan juumlahnya, `strtok` untuk memotonng string dengan batas tanda `;` dengan bagian pertama adalah pesan dan bagian kedua jumlahnya
``    if (token == NULL) 
    {
        printf("Format input salah.\n");
        exit(EXIT_FAILURE);
    }`` jaga-jaga kalau format input salah
- ``    strcpy(data->message, token);`` untuk simpan ke shared memory, `strcpy` untuk salin isi string ke `data->message`
- ``    token = strtok(NULL, ";");
    if (token == NULL) 
    {
        printf("Format input salah.\n");
        exit(EXIT_FAILURE);
    }
    data->count = atoi(token);`` untuk ambil bagian kedua/jumlah pesan, `atoi()` untuk mengubah string jadi integer
- ``    FILE *logfile = fopen("sistem.log", "a");`` untuk menulis ke file log dengan, buka sistem.log dalam mode append (`a`) agar menulis di bagian akhir (tidak menghapus isi sebelumnya)
``    if (logfile == NULL) 
    {
        perror("Gagal membuka sistem.log");
        exit(EXIT_FAILURE);
    }`` jaga-jaga kalau gagal buka file
- ``    fprintf(logfile, "Message from client: %s\n", data->message);
    fprintf(logfile, "Message count: %d\n", data->count);
    fclose(logfile);`` untuk menulis informasi ke file log, `fprint()` menulis, `fclose()` menutuo
- ``    printf("Pesan berhasil dikirim ke shared memory.\n");`` tanda selesai
- ``    shmdt(data);`` untuk memutuskan hubungan dengan shared memory, `shmdt()` untuk detach shared memory agar memori tetap bersih

### *b. Load Balancer Mendistribusikan Pesan ke Worker Secara Round-Robin*

Setelah menerima pesan dari client, tugas loadbalancer.c adalah mendistribusikan pesan-pesan tersebut ke beberapa *worker* menggunakan metode *round-robin. Sebelum mendistribusikan pesan, loadbalancer.c terlebih dahulu mencatat informasi ke dalam *sistem.log** dengan format:


Received at lb: <isi pesan> (#message <indeks pesan>)


Contoh jika ada 10 pesan yang dikirimkan, maka output log yang dihasilkan adalah:


Received at lb: Halo A (#message 1)
Received at lb: Halo A (#message 2)
...
Received at lb: Halo A (#message 10)


Setelah itu, loadbalancer.c akan meneruskan pesan-pesan tersebut ke *n worker* secara bergiliran (round-robin), menggunakan *IPC message queue*. Berikut adalah contoh distribusi jika jumlah worker adalah 3:

- Pesan 1 → worker1
- Pesan 2 → worker2
- Pesan 3 → worker3
- Pesan 4 → worker1 (diulang dari awal)

Dan seterusnya.

Proses worker.c bertugas untuk mengeksekusi pesan yang diterima dan mencatat log ke dalam file yang sama, yakni *sistem.log*.

loadbalancer.c:
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>     

#define SHM_KEY 1234
#define MSG_KEY 5678
#define MAX_MESSAGE 256
#define MAX_WORKERS 3

typedef struct {
    char message[MAX_MESSAGE];
    int count;
} SharedData;

typedef struct {
    long mtype;
    char mtext[MAX_MESSAGE];
} Message;

int main() {

    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);
 
    if (shmid == -1) 
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    SharedData *data = (SharedData *) shmat(shmid, NULL, 0);
    
    if (data == (void *) -1) 
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
   
    if (msgid == -1) 
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    FILE *logfile = fopen("sistem.log", "a");
   
    if (logfile == NULL) 
    {
        perror("Gagal membuka sistem.log");
        exit(EXIT_FAILURE);
    }

    sleep(1); 

    for (int i = 0; i < data->count; i++) 
    {
        fprintf(logfile, "Received at lb: %s (message %d)\n", data->message, i + 1);
        fflush(logfile);

        Message msg;
        msg.mtype = (i % MAX_WORKERS) + 1; 
        strcpy(msg.mtext, data->message);

        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) 
        {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }

        usleep(100000);
    }

    fclose(logfile);
    shmdt(data);

    return 0;
}
```
Penjelasan:
- `#define MSG_KEY 5678` untuk key message queue
- `#define MAX_WORKERS 3` batas maks pekerja 3
- ``typedef struct {
    char message[MAX_MESSAGE];
    int count;
} SharedData;`` untuk shared memory
- ``typedef struct {
    long mtype;
    char mtext[MAX_MESSAGE];
} Message;`` untuk message queue
- ``    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);`` untuk ambil ID shared memory di `client.c`
- ``    SharedData *data = (SharedData *) shmat(shmid, NULL, 0); `` untuk mehubungkan shared memory ke loadbalancer
- ``    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);`` untuk ambil/buat message queue ddengan `MSG_KEY`, tempat antrian pesan ke worker
- ``    FILE *logfile = fopen("sistem.log", "a");`` untuk buka `sistem.log` dalam mode append
- ``    sleep(1); `` untuk tunda 1 detik, agar worker siap bacac pesan seeblum loadbalancer kirim
- ``    for (int i = 0; i < data->count; i++) `` untuk looping sebanyak jumlah pesan yang diminta client
- ``        fprintf(logfile, "Received at lb: %s (message %d)\n", data->message, i + 1);
        fflush(logfile);`` untuk menulis ke file log bahwa pesan ke-i telah diterima loadbalancer
- ``        Message msg;
        msg.mtype = (i % MAX_WORKERS) + 1; 
        strcpy(msg.mtext, data->message);`` untuk membuat pesan yang dikirim ke worker, `mtype` akan bergiliran dari 1,2,3,1,dst, `mtext` berisi pesan dari client
- ``        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) 
        {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }`` untuk mengirim pesan ke message queue, akan diambil worker dengan  nomor `mtype `
- ``        usleep(100000);`` untuk menunggu 0,1s sebelum kirim pesan selanjutnya, tujuannya agar distribusi lebih rata dan adil (seharusnya)
- ``    fclose(logfile);
    shmdt(data);`` unutk tutup file log dan keluar dari shared memory


### *c. Worker Mencatat Pesan yang Diterima*

Setiap worker yang menerima pesan dari loadbalancer.c harus mencatat pesan yang diterima ke dalam *sistem.log* dengan format log sebagai berikut:


WorkerX: message received


### *d. Catat Total Pesan yang Diterima Setiap Worker di Akhir Eksekusi*

Setelah proses selesai (semua pesan sudah diproses), setiap worker akan mencatat jumlah total pesan yang mereka terima ke bagian akhir file *sistem.log*.


Worker 1: 3 messages
Worker 2: 4 messages
Worker 3: 3 messages


*Penjelasan:*
3 + 4 + 3 = 10, sesuai dengan jumlah pesan yang dikirim pada soal a

`worker.c`:
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

#define MSG_KEY 5678
#define MAX_MESSAGE 256

typedef struct {
    long mtype;
    char mtext[MAX_MESSAGE];
} Message;

int total_received = 0; 
int worker_number;
FILE *logfile;

void handle_exit(int sig) 
{
    fprintf(logfile, "Worker%d: %d messages\n", worker_number, total_received);
    fflush(logfile);
    fclose(logfile);
    exit(0);
}

int main(int argc, char *argv[]) {
  
    if (argc != 2) 
    {
        printf("Usage: %s <worker_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    worker_number = atoi(argv[1]);

    int msgid = msgget(MSG_KEY, 0666);

    if (msgid == -1) 
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    logfile = fopen("sistem.log", "a");

    if (logfile == NULL) 
    {
        perror("Gagal membuka sistem.log");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_exit);

    Message msg;

    while (1) 
    {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), worker_number, 0) == -1) 
        {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }

        fprintf(logfile, "Worker%d: %s received\n", worker_number, msg.mtext);
        fflush(logfile);
        total_received++; 
    }

    return 0;
}
```
Penjelasan:
- `#define MSG_KEY 5678` key dari message queue (wajib sama dengan di `loadbalancer.c`)
- `#define MAX_MESSAGE 256` batas ukuran maks pesan
- ``typedef struct {
    long mtype;
    char mtext[MAX_MESSAGE];
} Message;`` untuk message queue, sama dengan di loadbalancer, `mtype` untuk menentukan worker nomor berapa yang bakal menerima
- ``int total_received = 0; 
int worker_number;
FILE *logfile;`, ``total_received` untuk menghitung banyak pesan yang diterima
-``void handle_exit(int sig) 
{
    fprintf(logfile, "Worker%d: %d messages\n", worker_number, total_received);
    fflush(logfile);
    fclose(logfile);
    exit(0);
}`, untuk handler keluar, saat dapat sinyal SIGINT (ctrl +c) di terminal, maka worker akan menulis ke ``sistem.log` banyak pesan yang diterima, lalu menutup file dan keluar dengan `exit(0)`
- ``    if (argc != 2) 
    {
        printf("Usage: %s <worker_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }`` untuk ambil nomor worker dari argumen
- ``    worker_number = atoi(argv[1]);`` untuk ubah argumen string jadi integer
- ``    int msgid = msgget(MSG_KEY, 0666);`` untuk buka dan ambil message queue yang dibuat loadbalancer
- ``    logfile = fopen("sistem.log", "a");`` untuk buka file log
- ``    signal(SIGINT, handle_exit);`` untuk buat sinyal SIGINT, jika menekan `ctrl+c` di terminal akan memanggil `handle_exit()`
- ``    while (1) 
    {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), worker_number, 0) == -1) 
        {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        fprintf(logfile, "Worker%d: %s received\n", worker_number, msg.mtext);
        fflush(logfile);
        total_received++; 
    }`` untuk menerima dan menulis di file log, looping terus: worker tunggu pesan dengan `mtype()` (nomor worker) dan langsung tulis saat dapat pesan, `total_received++` untuk menambah counter tiap menerima pesan
    
Output `sistem.log`:
![image](https://github.com/user-attachments/assets/1153162a-06db-446b-ae5a-6aa8f5ea3025)
