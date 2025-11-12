// PENTING!!
//
// Jika mau di run, compile dlu pake ini
// gcc -Wall fusecure.c `pkg-config fuse3 --cflags --libs` -o fusecure
// mkdir /mnt/secure_fs
// ./fusecure /mnt/secure_fs -f -o allow_other
//

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

// cari UID
char *get_username(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) return NULL;
    return pw->pw_name;
}

// konvert path fuse ke path 
static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", source_dir, path);
}

// Read doang
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

    // mastiin read onyl
    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return -EACCES;

    // cari ID user
    uid_t uid = fuse_get_context()->uid;
    char *user = get_username(uid);

    // Cek akses folder
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

    // Get user ID
    uid_t uid = fuse_get_context()->uid;
    char *user = get_username(uid);

    // Restrict access to private folders
    if (strncmp(path, "/private_yuadi/", 15) == 0 && strcmp(user, "yuadi") != 0) return -EACCES;

    if (strncmp(path, "/private_irwandi/", 17) == 0 && strcmp(user, "irwandi") != 0) return -EACCES;

    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;
    int res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);
    return res;
}

// Nolak semua operasi modify
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
