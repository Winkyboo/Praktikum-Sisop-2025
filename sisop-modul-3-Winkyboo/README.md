Kami dari kelas Sistem Operasi D, kelompok D13 yang beranggotakan:
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241089 | Rhea Debora Sianturi |
| 5025241096 | Willy Marcelius |
| 5025241113 | Rafa Huga Nirando |
| 5025241124 | Melvan Hapianan Allo Ponglabba |

Berikut hasil laporan praktikum Sistem Operasi modul 3

# Praktikum Modul 3 _(Module 3 Lab Work)_

### Laporan Resmi Praktikum Modul 3 _(Module 3 Lab Work Report)_

Di suatu pagi hari yang cerah, Budiman salah satu mahasiswa Informatika ditugaskan oleh dosennya untuk membuat suatu sistem operasi sederhana. Akan tetapi karena Budiman memiliki keterbatasan, Ia meminta tolong kepadamu untuk membantunya dalam mengerjakan tugasnya. Bantulah Budiman untuk membuat sistem operasi sederhana!

_One sunny morning, Budiman, an Informatics student, was assigned by his lecturer to create a simple operating system. However, due to Budiman's limitations, he asks for your help to assist him in completing his assignment. Help Budiman create a simple operating system!_

### Soal 1

> Sebelum membuat sistem operasi, Budiman diberitahu dosennya bahwa Ia harus melakukan beberapa tahap terlebih dahulu. Tahap-tahapan yang dimaksud adalah untuk **mempersiapkan seluruh prasyarat** dan **melakukan instalasi-instalasi** sebelum membuat sistem operasi. Lakukan seluruh tahapan prasyarat hingga [perintah ini](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) pada modul!

> _Before creating the OS, Budiman was informed by his lecturer that he must complete several steps first. The steps include **preparing all prerequisites** and **installing** before creating the OS. Complete all the prerequisite steps up to [this command](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) in the module!_

**Answer:**

- **Code:**

  ### Mempersiapkan kernel linux

  Langkah-langkah:
  1. Perbarui perangkat lunak yang dibutuhkan untuk membuat os ini
     ```bash
     sudo apt -y update
     sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev libssl-dev mtools grub-pc-bin xorriso tmux
     ```
  2. Buat direktori "osboot" dan pindah ke direktori tersebut
     ```bash
     mkdir -p osboot
     cd osboot
     ```
  3. Mengunduh kode sumber kernel Linux versi 6.1.1. dan mengekstraknya ke dalam direktori `linux-6.1.1`.
     ```bash
     wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.1.tar.xz
     tar -xvf linux-6.1.1.tar.xz
     cd linux-6.1.1
     ```
  4. Melakukan konfigurasi Kernel, dimulai dengan konfigurasi minimal menggunakan `make tinyconfig` kemudian dilanjutkan dengan konfigurasi lanjutan menggunakan `make menuconfig` dengan mengaktifkan fitur-fitur di bawah ini
     ```bash
     64-Bit Kernel
     General Setup > Configure standard kernel features > Enable support for printk
     General Setup > Configure standard kernel features > Enable futex support
     General Setup > Initial RAM filesystem and RAM disk (initramfs/initrd) support
     General Setup > Control Group Support
     Enable the block layer > Legacy autoloading support
     Enable the block layer > Partition type > Advanced Partition Selection
     Device Drivers > Character devices > Enable TTY
     Device Drivers > Character devices > Virtio console
     Device Drivers > Character devices > /dev/mem virtual device support
     Device Drivers > Network device support > Virtio Network Driver
     Device Drivers > Serial Ata / Paralel ATA
     Device Drivers > Block Devices > Virtio block driver
     Device Drivers > Block Devices > loopback device support
     Device Drivers > Block Devices > RAM block device support
     Device Drivers > Virtio drivers
     Device Drivers > Virtualization Drivers
     Device Drivers > Generic  Driver Options > Maintain a devtmpfs at filesystems
     Device Drivers > Generic Driver Options > Automount devtmpfs at /dev
     Executable file formats > Kernel Support for ELF binaries
     Executable file formats > Kernel Support scripts starting with #!
     File Systems > FUSE support
     File Systems > The extended 3 filesystem
     File Systems > The extended 4 filesystem
     File Systems > Second extended fs support
     File Systems > Virtio Filesystem
     File Systems > Kernel automounter support
     File Systems > Pseudo File Systems > /proc file system support
     File Systems > Pseudo File Systems > sysctl support
     File Systems > Pseudo File Systems > sysfs file system support
     Networking Support > Networking options > Unix domain sockets
     Networking Support > Networking options > TCP/IP Networking
     ```
      Setelah selesai mengonfigurasi, simpan file konfigurasi `.config` dan melanjutkan ke langkah berikutnya.
  
  5. Kompilasi Kernel
     ```bash
     make -j$(nproc)
     ```
  6. Hasilkan file 'bzimage'
     ```bash
     cp arch/x86/boot/bzImage ..
     ```

  ### Busybox

    Install busybox dengan melakukan command ini pada terminal
    ```bash
    sudo apt install -y busybox-static
    ```
    
- **Explanation:**

  1. Perintah update dan install paket
     ```bash
     sudo apt -y update
     sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev mtools grub-pc-bin xorriso tmux
     ```
     Penjelasan:
     - `sudo apt -y update` memperbarui daftar paket yang tersedia secara otomatis tanpa konfirmasi (-y).
     - `sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev mtools grub-pc-bin xorriso tmux` menginstal paket-paket yang diperlukan untuk membangun kernel dan menjalankan emulator QEMU, termasuk compiler, pustaka, dan alat pendukung lainnya.

  2. Membuat direktori
     ```bash
     mkdir -p osboot
     cd osboot
     ```
     Penjelasan:
     - `mkdir -p osboot` membuat direktori bernama osboot jika belum ada
     - `cd osboot` masuk ke direktori tersebut
    
  3. ```bash
     wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.1.tar.xz
     tar -xvf linux-6.1.1.tar.xz
     cd linux-6.1.1
     ```
     Penjelasan:
     - `wget` mengunduh file arsip kernel Linux versi 6.1.1
     - `tar -xvf` mengekstrak isi arsip tersebut
     - `cd linux-6.1.1` masuk ke folder hasil ekstrak
    
  4. - `make tinyconfig` membuat konfigurasi kernel minimal
     - `make menuconfig` membuka menu konfigurasi interaktif untuk mengaktifkan fitur-fitur kernel yang diperlukan, seperti dukungan 64-bit, driver perangkat, sistem file, dan jaringan
    
  5. ```bash
     make -j$(nproc)
     ```
     Penjelasan:
     - `make` memulai proses kompilasi kernel
     - `-j$(nproc)` menjalankan kompilasi secara paralel sesuai jumlah core CPU agar lebih cepat
    
  6. ```bash
     cp arch/x86/boot/bzImage ..
     ```
     Penjelasan:
     - Menyalin file kernel hasil kompilasi (bzImage) ke direktori atas untuk digunakan selanjutnya
    
  7. ```bash
     sudo apt install -y busybox-static
     ```
     Penjelasan:
     - Menginstal paket `busybox-static` yang menyediakan utilitas dasar Linux dalam satu executable statis, berguna untuk sistem operasi minimal.


- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/8e135c6a-1239-4178-81ec-777b7a9c69d4)
  ![image](https://github.com/user-attachments/assets/81716775-4785-41f8-b82b-a4d1488cce92)



### Soal 2

> Setelah seluruh prasyarat siap, Budiman siap untuk membuat sistem operasinya. Dosen meminta untuk sistem operasi Budiman harus memiliki directory **bin, dev, proc, sys, tmp,** dan **sisop**. Lagi-lagi Budiman meminta bantuanmu. Bantulah Ia dalam membuat directory tersebut!

> _Once all prerequisites are ready, Budiman is ready to create his OS. The lecturer asks that the OS should contain the directories **bin, dev, proc, sys, tmp,** and **sisop**. Help Budiman create these directories!_

**Answer:**

- **Code:**

  Langkah-langkah:

  1. Pada terminal, di direktori osboot Masuk ke mode superuser
     ```bash
     sudo bash
     ```
     
  2. Buat Direktori untuk Initramfs
     ```bash
     mkdir -p myramdisk/{bin,dev,proc,sys,tmp,sisop,etc,root,home/Budiman,guest,praktikan1,praktikan2}
     ```

  3. Salin File Device ke Direktori dev
     ```bash
     cp -a /dev/null myramdisk/dev
     cp -a /dev/tty* myramdisk/dev
     cp -a /dev/zero myramdisk/dev
     cp -a /dev/console myramdisk/dev
     ```

  4. Salin BusyBox ke Direktori bin
     ```bash
     cp /usr/bin/busybox myramdisk/bin
     cd myramdisk/bin
     ./busybox --install .
     ```

- **Explanation:**

  1. ```bash
     sudo bash
     ```
     Penjelasan:
     - `sudo` menjalankan perintah dengan hak akses administrator
     - `bash` membuka shell baru dengan hak akses root agar bisa membuat direktori dan menyalin file sistem yang memerlukan izin khusus
    
  2. ```bash
     mkdir -p myramdisk/{bin,dev,proc,sys,tmp,sisop,etc,root,home/Budiman,guest,praktikan1,praktikan2}
     ```
     Penjelasan:
     - `mkdir` membuat direktori
     - `-p` membuat seluruh jalur direktori sekaligus jika belum ada
     - `myramdisk/{...}` menggunakan ekspansi brace untuk membuat beberapa subdirektori sekaligus di dalam folder myramdisk
     - Direktori yang dibuat meliputi: `bin, dev, proc, sys, tmp, sisop` sesuai permintaan, serta beberapa direktori tambahan seperti `etc, root`, dan beberapa direktori user.
    
  3. ```bash
     cp -a /dev/null myramdisk/dev
     cp -a /dev/tty* myramdisk/dev
     cp -a /dev/zero myramdisk/dev
     cp -a /dev/console myramdisk/dev
     ```
     Penjelasan:
     - `cp` menyalin file
     - `-a` menjaga atribut file seperti permission, timestamp, dan symbolic link
     - Menyalin device file penting seperti `null`, `tty*` (terminal devices), `zero`, dan `console` ke direktori `dev` di dalam `myramdisk`
    
  4. ```bash
     cp /usr/bin/busybox myramdisk/bin
     cd myramdisk/bin
     ./busybox --install .
     ```
     Penjelasan:
     - Menyalin executable BusyBox ke direktori `bin` di `myramdisk`
     - `./busybox --install .` menjalankan BusyBox untuk membuat symlink ke berbagai utilitas dasar Unix/Linux di direktori saat ini (.)

- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/fa2e5f27-abcc-4cd2-98d9-ee072b6ea8ba)
  ![image](https://github.com/user-attachments/assets/d5830d64-b2e2-4459-8070-e5fad7863f4a)
  ![image](https://github.com/user-attachments/assets/19042d65-4b58-4c5f-a9a2-38cbe7b3ee72)


### Soal 3

> Budiman lupa, Ia harus membuat sistem operasi ini dengan sistem **Multi User** sesuai permintaan Dosennya. Ia meminta kembali kepadamu untuk membantunya membuat beberapa user beserta directory tiap usernya dibawah directory `home`. Buat pula password tiap user-usernya dan aplikasikan dalam sistem operasi tersebut!

> _Budiman forgot that he needs to create a **Multi User** system as requested by the lecturer. He asks your help again to create several users and their corresponding home directories under the `home` directory. Also set each user's password and apply them in the OS!_

**Format:** `user:pass`

```
root:Iniroot
Budiman:PassBudi
guest:guest
praktikan1:praktikan1
praktikan2:praktikan2
```

**Answer:**

- **Code:**

  Langkah-langkah:

  1. Buat password untuk masing-masing user
     ```bash
     openssl passwd -1 Iniroot
     openssl passwd -1 PassBudi
     openssl passwd -1 guest
     openssl passwd -1 praktikan1
     openssl passwd -1 praktikan2 
     ```
  2. Buat file passwd di direktori ~/osboot/myramdisk/etc
     ```bash
     root:$1$IX2Nq570$Vcm2C0NhPpaAp.IIi.sBT.:0:0:root:/root:/bin/sh
     Budiman:$1$llyz1ucs$9lD2FEk75aDZouNHTHQu8.:1001:100:Budiman:/home/Budiman:/bin/sh
     guest:$1$JYFZ1czh$uPVLjoQNVZVTKrX3L6Mtz1:1002:100:guest:/home/guest:/bin/sh
     praktikan1:$1$4dJj9YuF$Me4h2APdQMRLBGiMyl/9s/:1003:100:praktikan1:/home/praktikan1:/bin/sh
     praktikan2:$1$piwJjkZ9$sahhIDEPmnSkOfqryKObX.:1004:100:praktikan2:/home/praktikan2:/bin/sh
     ```
  3. Buat file group di direktori ~/osboot/myramdisk/etc
     ```bash
     root:x:0:
     bin:x:1:root
     sys:x:2:root
     tty:x:5:root,Budiman,guest,praktikan1,praktikan2
     disk:x:6:root
     wheel:x:10:root,Budiman,guest,praktikan1,praktikan2
     users:x:100:Budiman,guest,praktikan1,praktikan2
     ```

  4. Buat file init di direktori ~/osboot/myramdisk
    ```bash
     #!/bin/sh
    /bin/mount -t proc none /proc
    /bin/mount -t sysfs none /sys
    while true
    do
        /bin/getty -L tty1 115200 vt100
        sleep 1
    done
    exec /bin/sh
   ```


- **Explanation:**

  1. ```bash
     openssl passwd -1 Iniroot
     openssl passwd -1 PassBudi
     openssl passwd -1 guest
     openssl passwd -1 praktikan1
     openssl passwd -1 praktikan2 
     ```
     Penjelasan:
     - `openssl passwd -1 <password>` menghasilkan hash password menggunakan algoritma MD5 (ditandai dengan opsi -1)
  
- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/14ec1380-a6b3-4b64-90ed-82b7fdf01c99)


### Soal 4

> Dosen meminta Budiman membuat sistem operasi ini memilki **superuser** layaknya sistem operasi pada umumnya. User root yang sudah kamu buat sebelumnya akan digunakan sebagai superuser dalam sistem operasi milik Budiman. Superuser yang dimaksud adalah user dengan otoritas penuh yang dapat mengakses seluruhnya. Akan tetapi user lain tidak boleh memiliki otoritas yang sama. Dengan begitu user-user selain root tidak boleh mengakses `./root`. Buatlah sehingga tiap user selain superuser tidak dapat mengakses `./root`!

> _The lecturer requests that the OS must have a **superuser** just like other operating systems. The root user created earlier will serve as the superuser in Budiman's OS. The superuser should have full authority to access everything. However, other users should not have the same authority. Therefore, users other than root should not be able to access `./root`. Implement this so that non-superuser accounts cannot access `./root`!_

**Answer:**

- **Code:**

  Pada active direktori ~/osboot/myramdisk, di terminal lakukan command ini
  ```
  chown 0:0 /root
  chmod 700 /root
  ```
- **Explanation:**

  ```bash
  chown 0:0 /root
  chmod 700 /root
  ```
  Penjelasan:
  - `chown` adalah perintah untuk mengubah kepemilikan file atau direktori
  - `0:0` berarti mengubah pemilik (owner) dan grup (group) menjadi user dengan UID 0 dan GID 0, yaitu user root dan grup root
  - `chmod` mengubah hak akses (permissions) file atau direktori
  - `700` berarti:
    - `7` untuk pemilik (owner): baca (4) + tulis (2) + eksekusi (1) = 7 (akses penuh)
    - `0` untuk grup: tidak ada akses
    - `0` untuk pengguna lain: tidak ada akses

- **Screenshot:**

  User lain selain root tidak bisa mengakses /root
  ![image](https://github.com/user-attachments/assets/8ff83c31-d8f8-4663-aa1e-235f66a2d276)
  ![image](https://github.com/user-attachments/assets/4c5bee4a-db37-442b-a75e-9c410743f1a7)
  ![image](https://github.com/user-attachments/assets/d58f3bf5-0f54-441e-b3c9-944fb177f7f6)
  ![image](https://github.com/user-attachments/assets/3fe09e39-1ab1-4e8b-8bf0-e82538e4dea7)

### Soal 5

> Setiap user rencananya akan digunakan oleh satu orang tertentu. **Privasi dan otoritas tiap user** merupakan hal penting. Oleh karena itu, Budiman ingin membuat setiap user hanya bisa mengakses dirinya sendiri dan tidak bisa mengakses user lain. Buatlah sehingga sistem operasi Budiman dalam melakukan hal tersebut!

> _Each user is intended for an individual. **Privacy and authority** for each user are important. Therefore, Budiman wants to ensure that each user can only access their own files and not those of others. Implement this in Budiman's OS!_

**Answer:**

- **Code:**

  Pada active direktori ~/osboot/myramdisk/home, di terminal lakukan command ini
  ```
  chown 1001:100 /Budiman
  chown 1002:100 /guest
  chown 1003:100 /praktikan1
  chwon 1004:100 /praktikan2
  chmod 700 /Budiman
  chmod 700 /guest
  chmod 700 /praktikan1
  chmod 700 /praktikan2 
  ```
- **Explanation:**

  ```bash
  chown 1001:100 /Budiman
  chown 1002:100 /guest
  chown 1003:100 /praktikan1
  chown 1004:100 /praktikan2

  chmod 700 /Budiman
  chmod 700 /guest
  chmod 700 /praktikan1
  chmod 700 /praktikan2 
  ```
  Penjelasan:
  - chown mengubah kepemilikan direktori.
  - <UID> adalah User ID dari user yang bersangkutan (misalnya 1001 untuk Budiman).
  - <GID> adalah Group ID, di sini menggunakan 100 yang sesuai dengan grup users pada konfigurasi sebelumnya.
  - Contohnya, chown 1001:100 /Budiman menjadikan user dengan UID 1001 dan grup 100 sebagai pemilik direktori /Budiman.
  - chmod mengatur hak akses direktori.
  - `700` berarti:
    - `7` untuk pemilik (owner): baca (4) + tulis (2) + eksekusi (1) = 7 (akses penuh)
    - `0` untuk grup: tidak ada akses
    - `0` untuk pengguna lain: tidak ada akses


- **Screenshot:**

  User hanya bisa mengakses dirinya sendiri dan tidak bisa mengakses user lain
  ![image](https://github.com/user-attachments/assets/8e0fa0c5-c57e-45c6-bb3f-7517e52f8b8f)
  ![image](https://github.com/user-attachments/assets/6e3be615-51d6-4ecd-bdb0-b9d95ea66d70)



### Soal 6

> Dosen Budiman menginginkan sistem operasi yang **stylish**. Budiman memiliki ide untuk membuat sistem operasinya menjadi stylish. Ia meminta kamu untuk menambahkan tampilan sebuah banner yang ditampilkan setelah suatu user login ke dalam sistem operasi Budiman. Banner yang diinginkan Budiman adalah tulisan `"Welcome to OS'25"` dalam bentuk **ASCII Art**. Buatkanlah banner tersebut supaya Budiman senang! (Hint: gunakan text to ASCII Art Generator)

> _Budiman wants a **stylish** operating system. Budiman has an idea to make his OS stylish. He asks you to add a banner that appears after a user logs in. The banner should say `"Welcome to OS'25"` in **ASCII Art**. Use a text to ASCII Art generator to make Budiman happy!_ (Hint: use a text to ASCII Art generator)

**Answer:**

- **Code:**

  Pergi ke website https://www.asciiart.eu/text-to-ascii-art, lalu pilih jenis font yang diinginkan, setelah itu copy text to clipboard.
  Setelah itu, pada direktori
  ```
  cd ~/osboot/myramdisk/etc
  ```
  Buat dua file bernama `profile`(nama ini harus `profile`) dan `wackyboo`(nama ini bebas, tidak harus sama)       
  ```
  touch profile
  touch wackyboo
  ```
  Pada file `wackyboo`, paste text yang sudah dicopy dari ascii art generator tadi.
  Lalu, edit file "profile" menjadi seperti ini
  ```
  #!/bin/bash
  clear
  cat /etc/wackyboo
  ```

  
- **Explanation:**

  1. ```bash
     cd ~/osboot/myramdisk/etc
     touch profile
     touch wackyboo
     ```
     Penjelasan:
     - `touch profile` membuat file bernama profile yang akan berisi script untuk menampilkan banner saat login
     - `touch wackyboo` membuat file wackyboo (nama bebas) yang akan berisi ASCII Art hasil generate
    
  2. ```bash
     #!/bin/bash
     clear
     cat /etc/wackyboo
     ```
     Penjelasan:
     - `#!/bin/bash` menandakan file ini adalah script shell
     - `clear` membersihkan layar terminal agar tampilan bersih
     - `cat /etc/wackyboo` menampilkan isi file ASCII Art sebagai banner
     - File `profile` ini bisa dijalankan otomatis saat user login, misalnya dengan mengeksekusi source `/etc/profile` atau mengatur shell login untuk memanggil file ini

- **Screenshot:**

  Isi file "wackyboo"
  ![image](https://github.com/user-attachments/assets/91d3e1d1-d06a-4413-b693-f119fe0f8fbb)

  Isi file "profile"
  ![image](https://github.com/user-attachments/assets/f7acf94b-bbcf-42fd-a4dd-e4d36c360fd1)

  Tampilan saat login user
  ![image](https://github.com/user-attachments/assets/19fbc729-88be-475c-80fd-03ae44c29bff)

  Dapat terlihat banner `Welcome to OS'25` sudah muncul ketika user login


### Soal 7

> Melihat perkembangan sistem operasi milik Budiman, Dosen kagum dengan adanya banner yang telah kamu buat sebelumnya. Kemudian Dosen juga menginginkan sistem operasi Budiman untuk dapat menampilkan **kata sambutan** dengan menyebut nama user yang login. Sambutan yang dimaksud berupa kalimat `"Helloo %USER"` dengan `%USER` merupakan nama user yang sedang menggunakan sistem operasi. Kalimat sambutan ini ditampilkan setelah user login dan setelah banner. Budiman kembali lagi meminta bantuanmu dalam menambahkan fitur ini.

> _Seeing the progress of Budiman's OS, the lecturer is impressed with the banner you created. The lecturer also wants the OS to display a **greeting message** that includes the name of the user who logs in. The greeting should say `"Helloo %USER"` where `%USER` is the name of the user currently using the OS. This greeting should be displayed after user login and after the banner. Budiman asks for your help again to add this feature._

**Answer:**

- **Code:**

   Edit file "profile" pada direktori ~/osboot/myramdisk/etc menjadi seperti ini:
   ```
   #!/bin/bash
  clear
  cat /etc/wackyboo
  echo "Helloo $(whoami)"
  ```

- **Explanation:**

  ```bash
  #!/bin/bash
  clear
  cat /etc/wackyboo
  echo "Helloo $(whoami)"
  ```
  Penjelasan:
  - `#!/bin/bash` menandakan bahwa file ini adalah skrip shell yang dijalankan menggunakan Bash
  - `clear` membersihkan layar terminal agar tampilan lebih rapi sebelum menampilkan banner dan pesan
  - `cat /etc/wackyboo` menampilkan isi file wackyboo yang berisi ASCII Art banner "Welcome to OS'25"
  - `echo "Helloo $(whoami)"`:
    - `echo` digunakan untuk menampilkan teks ke terminal
    - `$(whoami)` menjalankan perintah whoami yang mengembalikan nama user yang sedang login


- **Screenshot:**

  Isi file "Profile"
  ![image](https://github.com/user-attachments/assets/1c311973-9228-444b-869c-932dd526d14f)

  Tampilan saat login user
  ![image](https://github.com/user-attachments/assets/0fb46f22-52cf-438a-8ab7-0f7771258fc0)

  Dapat terlihat muncul kalimat bertuliskan `Helloo [user]` ketika user tertentu login 
  

### Soal 8

> Dosen Budiman sudah tua sekali, sehingga beliau memiliki kesulitan untuk melihat tampilan terminal default. Budiman menginisiatif untuk membuat tampilan sistem operasi menjadi seperti terminal milikmu. Modifikasilah sistem operasi Budiman menjadi menggunakan tampilan terminal kalian.

> _Budiman's lecturer is quite old and has difficulty seeing the default terminal display. Budiman takes the initiative to make the OS look like your terminal. Modify Budiman's OS to use your terminal appearance!_

**Answer:**

- **Code:**

  Kembali ke direktori ~/osboot/myramdisk, edit file "init" menjadi seperti ini
  ```bash
  #!/bin/sh
  hostname Wackyboo
  /bin/mount -t proc none /proc
  /bin/mount -t sysfs none /sys
  while true
  do
    /bin/getty -L tty1 115200 vt100
    sleep 1
  done
  exec /bin/sh
  ```

  Lalu edit file "Profile" di direktori ~/osboot/myramdisk/etc menjadi seperti ini
  ```bash
  #!/bin/bash
  clear
  cat /etc/wackyboo
  echo "Helloo $(whoami)"
  export PS1="\[\033[01;32m\]\u@\h:\w\$ \[\033[00m\]"
  ```


- **Explanation:**

  1. ```bash
     #!/bin/sh
     hostname Wackyboo
     /bin/mount -t proc none /proc
     /bin/mount -t sysfs none /sys
     while true
     do
       /bin/getty -L tty1 115200 vt100
       sleep 1
     done
     exec /bin/sh
     ```
     Penjelasan:
     - `hostname Wackyboo` mengatur nama host sistem menjadi "Wackyboo" yang akan muncul di prompt terminal dan informasi sistem
     - `baris mount /proc dan /sys` memasang filesystem virtual penting agar kernel dan sistem dapat beroperasi dengan baik
     - Loop `while true` menjalankan `getty` pada terminal tty1 dengan baud rate 115200 dan tipe terminal vt100
     - `getty` bertugas menampilkan prompt login dan menerima input user
    
  2. ```bash
     #!/bin/bash
     clear
     cat /etc/wackyboo
     echo "Helloo $(whoami)"
     export PS1="\[\033[01;32m\]\u@\h:\w\$ \[\033[00m\]"
     ```
     Penjelasan:
     - `clear` membersihkan layar terminal agar tampilan bersih
     - `cat /etc/wackyboo` menampilkan banner ASCII art yang sudah dibuat sebelumnya
     - `echo "Helloo $(whoami)"` menampilkan sambutan dengan nama user yang sedang login
     - export PS1="..." mengatur variabel lingkungan PS1 yang menentukan tampilan prompt shell Bash. Penjelasan kode warna dan format:
       - `$$\033[01;32m$$` mengatur warna teks menjadi hijau terang (bold green)
       - `\u` menampilkan username user
       - `@` karakter '@' literal
       - `\h` menampilkan hostname 
       - `:` karakter ':' literal
       - `\w` menampilkan direktori kerja saat ini (full path)
       - `\$` menampilkan $ untuk user biasa atau # untuk root
       - `$$\033[00m$$` reset warna ke default


- **Screenshot:**

  Isi file init
  ![image](https://github.com/user-attachments/assets/a9e48530-b47b-4a55-9e38-8aed9b8745ec)

  Tampilan di os
  ![image](https://github.com/user-attachments/assets/3a5edbf4-288a-4dc0-bc55-feac7869e6f9)
  Bisa dilihat tampilan sistem operasi sudah terlihat mirip dengan tampilan terminal di ubuntu 




### Soal 9

> Ketika mencoba sistem operasi buatanmu, Budiman tidak bisa mengubah text file menggunakan text editor. Budiman pun menyadari bahwa dalam sistem operasi yang kamu buat tidak memiliki text editor. Budimanpun menyuruhmu untuk menambahkan **binary** yang telah disiapkan sebelumnya ke dalam sistem operasinya. Buatlah sehingga sistem operasi Budiman memiliki **binary text editor** yang telah disiapkan!

> _When trying your OS, Budiman cannot edit text files using a text editor. He realizes that the OS you created does not have a text editor. Budiman asks you to add the prepared **binary** into his OS. Make sure Budiman's OS has the prepared **text editor binary**!_

**Answer:**

- **Code:**

  Langkah-langkah:

  1. Clone repository text editor budiman
     ```bash
     git clone https://github.com/morisab/budiman-text-editor.git
     cd budiman-text-editor
     ```
  2. Compile program
     ```bash
     g++ -static main.cpp -o budiman
     ```
  3. Menyalin file text editor tersebut ke direktori ~/osboot/myramdisk/bin
     ```bash
     cp Budiman ~/osboot/myramdisk/bin
     ```
- **Explanation:**

  1. ```bash
     git clone https://github.com/morisab/budiman-text-editor.git
     cd budiman-text-editor
     ```
     Penjelasan:
     - `git clone` mengunduh seluruh kode sumber text editor yang sudah disiapkan dari GitHub
     - `cd` masuk ke direktori hasil clone untuk melakukan kompilasi
    
  2. ```bash
     g++ -static main.cpp -o budiman
     ```
     Penjelasan:
     - `g++` adalah compiler C++
     - `-static` menginstruksikan compiler untuk membuat binary statis, artinya binary tersebut tidak bergantung pada shared libraries saat dijalankan, sehingga dapat berjalan di berbagai sistem tanpa masalah dependensi
     - `main.cpp` adalah file sumber program
     - `-o budiman` menentukan nama output binary menjadi budiman
    
  3. ```bash
     cp budiman ~/osboot/myramdisk/bin
     ```
     Penjelasan:
     - Menyalin file binary budiman ke direktori `bin` di dalam `myramdisk`, sehingga binary text editor tersedia di sistem operasi yang akan dibuat


- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/c6718b86-589d-4df5-9562-e1d75ae6b855)
  ![image](https://github.com/user-attachments/assets/0364aec9-01f0-49c8-8930-4938749176d7)

  
### Soal 10

> Setelah seluruh fitur yang diminta Dosen dipenuhi dalam sistem operasi Budiman, sudah waktunya Budiman mengumpulkan tugasnya ini ke Dosen. Akan tetapi, Dosen Budiman tidak mau menerima pengumpulan selain dalam bentuk **.iso**. Untuk terakhir kalinya, Budiman meminta tolong kepadamu untuk mengubah seluruh konfigurasi sistem operasi yang telah kamu buat menjadi sebuah **file .iso**.

> After all the features requested by the lecturer have been implemented in Budiman's OS, it's time for Budiman to submit his assignment. However, Budiman's lecturer only accepts submissions in the form of **.iso** files. For the last time, Budiman asks for your help to convert the entire configuration of the OS you created into a **.iso file**.

**Answer:**

- **Code:**

  Langkah-langkah:
  
  1. Masuk ke direktori osboot
     ```bash
     cd osboot
     ```
  2. Buat struktur direktori ISO
     ```bash
     mkdir -p mylinuxiso/boot/grub
     ```
  3. Salin file kernel dan root filesystem
     ```bash
     cp bzImage mylinuxiso/boot
     cp myramdisk.gz mylinuxiso/boot
     ```
  4. Buat file konfigurasi GRUB: Buat file grub.cfg di mylinuxiso/boot/grub dengan isi:
     ```bash
     set timeout=5
     set default=0

     menuentry "MyLinux" {
       linux /boot/bzImage
       initrd /boot/myramdisk.gz
     }
     ```
  5. Buat file ISO bootable dengan jalankan perintah berikut di direktori osboot
     ```bash
     grub-mkrescue -o mylinux.iso mylinuxiso
     ```
     
- **Explanation:**

  1. ```bash
     cd osboot
     ```
     Penjelasan:
     - memastikan berada di folder utama tempat file kernel, ramdisk, dan konfigurasi ISO disiapkan
    
  2. ```bash
     mkdir -p mylinuxiso/boot/grub
     ```
     Penjelasan:
     - Membuat folder `mylinuxiso` sebagai root ISO
     - Membuat subfolder `boot` untuk menyimpan kernel dan ramdisk
     - Membuat subfolder `boot/grub` untuk menyimpan konfigurasi bootloader GRUB
    
  3. ```bash
     cp bzImage mylinuxiso/boot
     cp myramdisk.gz mylinuxiso/boot
     ```
     Penjelasan:
     - `bzImage` adalah kernel Linux hasil kompilasi
     - `myramdisk.gz` adalah image root filesystem (initramfs) yang sudah dikompresi
     - Pada langkah ini, keduanya disalin ke folder boot dalam struktur ISO agar GRUB dapat memuatnya saat boot
    
  4. ```bash
     set timeout=5
     set default=0

     menuentry "MyLinux" {
       linux /boot/bzImage
       initrd /boot/myramdisk.gz
     }
     ```
     Penjelasan:
     - `set timeout=5` menentukan waktu tunggu menu boot selama 5 detik
     - `set default=0` memilih entri menu pertama sebagai default
     - `menuentry` mendefinisikan opsi boot bernama "MyLinux" yang memuat kernel dan initramfs
    
  5. ```bash
     grub-mkrescue -o mylinux.iso mylinuxiso
     ```
     Penjelasan:
     - `grub-mkrescue` membuat image ISO bootable dari folder mylinuxiso
     - `-o mylinux.iso` menentukan nama file ISO output


- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/3ad07035-9632-47c4-9ab1-c37b94caedec)


---

Pada akhirnya sistem operasi Budiman yang telah kamu buat dengan susah payah dikumpulkan ke Dosen mengatasnamakan Budiman. Kamu tidak diberikan credit apapun. Budiman pun tidak memberikan kata terimakasih kepadamu. Kamupun kecewa tetapi setidaknya kamu telah belajar untuk menjadi pembuat sistem operasi sederhana yang andal. Selamat!

_At last, the OS you painstakingly created was submitted to the lecturer under Budiman's name. You received no credit. Budiman didn't even thank you. You feel disappointed, but at least you've learned to become a reliable creator of simple operating systems. Congratulations!_
