#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

static const char *spicy_info_path = "/very_spicy_info.txt";
static const char *upload_path = "/upload.txt";
static const char *spicy_info_secret_content = "DainTontas' personal secret!!.txt\n";
static const char *spicy_info_bait_content = "Very spicy internal developer information: leaked roadmap.docx\n";
static const char *ascii_art_reward =

"███████╗███████╗██╗     ██╗         ███████╗ ██████╗ ██████╗     ██╗████████╗     █████╗  ██████╗  █████╗ ██╗███╗   ██╗    ██████╗ ███████╗██╗    ██╗ █████╗ ██████╗ ██████╗ \n"
"██╔════╝██╔════╝██║     ██║         ██╔════╝██╔═══██╗██╔══██╗    ██║╚══██╔══╝    ██╔══██╗██╔════╝ ██╔══██╗██║████╗  ██║    ██╔══██╗██╔════╝██║    ██║██╔══██╗██╔══██╗██╔══██╗\n"
"█████╗  █████╗  ██║     ██║         █████╗  ██║   ██║██████╔╝    ██║   ██║       ███████║██║  ███╗███████║██║██╔██╗ ██║    ██████╔╝█████╗  ██║ █╗ ██║███████║██████╔╝██║  ██║\n"
"██╔══╝  ██╔══╝  ██║     ██║         ██╔══╝  ██║   ██║██╔══██╗    ██║   ██║       ██╔══██║██║   ██║██╔══██║██║██║╚██╗██║    ██╔══██╗██╔══╝  ██║███╗██║██╔══██║██╔══██╗██║  ██║\n"
"██║     ███████╗███████╗███████╗    ██║     ╚██████╔╝██║  ██║    ██║   ██║       ██║  ██║╚██████╔╝██║  ██║██║██║ ╚████║    ██║  ██║███████╗╚███╔███╔╝██║  ██║██║  ██║██████╔╝\n"
"╚═╝     ╚══════╝╚══════╝╚══════╝    ╚═╝      ╚═════╝ ╚═╝  ╚═╝    ╚═╝   ╚═╝       ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝    ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ \n";

static const char *trap_flag_file = "/tmp/.troll_triggered";
static uid_t daintontas_uid = -1;

static int istrap(){
    return (access(trap_flag_file, F_OK) == 0);
}

static int xmp_getattr(const char *path, struct stat *stbuf){
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0){
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }else if (strcmp(path, spicy_info_path) == 0 || strcmp(path, upload_path) == 0){
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        if (istrap()) stbuf->st_size = strlen(ascii_art_reward);
        else if (strcmp(path, spicy_info_path) == 0){
            struct fuse_context *ctx = fuse_get_context();
            if (ctx->uid == daintontas_uid) stbuf->st_size = strlen(spicy_info_bait_content);
            else stbuf->st_size = strlen(spicy_info_secret_content);
        } else stbuf->st_size = 0;
    } else res = -ENOENT;
    return res;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi){
    (void) offset;
    (void) fi;
    if (strcmp(path, "/") != 0) return -ENOENT;
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, "very_spicy_info.txt", NULL, 0);
    filler(buf, "upload.txt", NULL, 0);
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi){
    if (strcmp(path, spicy_info_path) != 0 && strcmp(path, upload_path) != 0) return -ENOENT;
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi){
    (void) fi;
    size_t len;
    const char *content = NULL;

    if (istrap() && (strcmp(path, spicy_info_path) == 0 || strcmp(path, upload_path) == 0)) content = ascii_art_reward;
    else if (strcmp(path, spicy_info_path) == 0) {
        struct fuse_context *ctx = fuse_get_context();
        if (ctx->uid == daintontas_uid) content = spicy_info_bait_content;
        else content = spicy_info_secret_content;
    } else if (strcmp(path, upload_path) == 0) return 0;
    else return -ENOENT;

    len = strlen(content);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, content + offset, size);
    } else size = 0;
    
    return size;
}

static int xmp_write(const char *path, const char *buf, size_t size,off_t offset, struct fuse_file_info *fi){
    (void) buf;
    (void) offset;
    (void) fi;

    if (strcmp(path, upload_path) == 0) {
        struct fuse_context *ctx = fuse_get_context();
        if (ctx->uid == daintontas_uid) {
            int fd = creat(trap_flag_file, 0644);
            if (fd != -1) close(fd);
        }
        return size;
    }
    return -EACCES;
}

static int xmp_truncate(const char *path, off_t size){
    (void) path;
    (void) size;
    return 0;
}

static struct fuse_operations troll_oper = {
    .getattr  = xmp_getattr,
    .readdir  = xmp_readdir,
    .open     = xmp_open,
    .read     = xmp_read,
    .write    = xmp_write,
    .truncate = xmp_truncate, 
};

int main(int argc, char *argv[]){
    remove(trap_flag_file);

    struct passwd *pw = getpwnam("DainTontas");
    if (pw == NULL) {
        fprintf(stderr, "Error: User 'DainTontas' tidak ditemukan. Pastikan user sudah dibuat.\n");
        return 1;
    }
    daintontas_uid = pw->pw_uid;

    return fuse_main(argc, argv, &troll_oper, NULL);
}
