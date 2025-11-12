#include "shell.h"
#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void shell() {
  char buf[64];
  char cmd[64];
  char arg[2][64];

  byte cwd = FS_NODE_P_ROOT;

  while (true) {
    printString("MengOS:");
    printCWD(cwd);
    printString("$ ");
    readString(buf);
    parseCommand(buf, cmd, arg);

    if (strcmp(cmd, "cd")) cd(&cwd, arg[0]);
    else if (strcmp(cmd, "ls")) ls(cwd, arg[0]);
    else if (strcmp(cmd, "mv")) mv(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cp")) cp(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cat")) cat(cwd, arg[0]);
    else if (strcmp(cmd, "mkdir")) mkdir(cwd, arg[0]);
    else if (strcmp(cmd, "clear")) clearScreen();
    else printString("Invalid command\n");
  }
}

// TODO: 4. Implement printCWD function
void printCWD(byte cwd) {
    int i;
    struct node_fs node_fs_buf;
    byte path_wd[64]; 
    int len = 0;
    byte curr_node = cwd;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    if (cwd == 0xFF) {
        printString("/");
        return;
    }

    while (curr_node != 0xFF){
        path_wd[len++] = curr_node;
        curr_node = node_fs_buf.nodes[curr_node].parent_index;
    }

    for (i = len - 1; i >= 0; i--){
        printString("/");
        printString(node_fs_buf.nodes[path_wd[i]].node_name);
    }
}

// TODO: 5. Implement parseCommand function
void parseCommand(char* buf, char* cmd, char arg[2][64]) {
    int i = 0, j = 0, k = 0;
  
    strcpy(arg[0], "");
    strcpy(arg[1], "");
  
    while (buf[i] == ' ') i++;
  
    for (j = 0; buf[i] != ' ' && buf[i] != '\0'; i++, j++) cmd[j] = buf[i];
    cmd[j] = '\0';
    if (buf[i] == '\0') return; 
  
    while (buf[i] == ' ') i++;
  
    for (j = 0; buf[i] != ' ' && buf[i] != '\0'; i++, j++) arg[0][j] = buf[i];
    arg[0][j] = '\0';
    if (buf[i] == '\0') return; 
  
    while (buf[i] == ' ') i++;
    for (k = 0; buf[i] != ' ' && buf[i] != '\0'; i++, k++) arg[1][k] = buf[i];
    
    arg[1][k] = '\0';
}

// TODO: 6. Implement cd function
void cd(byte* cwd, char* dirname) {
    struct node_fs node_fs_buf;
    int i;
    bool found;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    if (strcmp(dirname, "..") == true) {
        if (*cwd != FS_NODE_SECTOR_NUMBER) *cwd = node_fs_buf.nodes[*cwd].parent_index;
        return;
    }

    if (strcmp(dirname, "/") == true) {
        *cwd = FS_NODE_P_ROOT;
        return;
    }

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, dirname) == true && node_fs_buf.nodes[i].parent_index == *cwd && node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
            *cwd = i;
            found = true;
            break;
        }
        found = false;
    }
    if (!found) printString("Error not found\n");
}

// TODO: 7. Implement ls function
void ls(byte cwd, char* dirname) {
    struct node_fs node_fs_buf;
    byte target_dir;
    int i;
    bool found = false;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    if (!dirname[0] || strcmp(dirname, ".")) {
        target_dir = cwd;
        found = true;
    } else {
        for (i = 0; i < FS_MAX_NODE; ++i) {
            if (strcmp(node_fs_buf.nodes[i].node_name, dirname) && node_fs_buf.nodes[i].parent_index == cwd && node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
                target_dir = i;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        printString("Error not found\n");
        return;
    }

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == target_dir && node_fs_buf.nodes[i].node_name[0] != '\0') {
            printString(node_fs_buf.nodes[i].node_name);
            printString(" ");
        }
    }
    printString("\n");
}


// TODO: 8. Implement mv function
void mv(byte cwd, char* src, char* dst) {
    struct node_fs node_fs_buf;
    int i;
    int src_node_idx = -1;
    byte dest_parent_idx;
    char dest_name[MAX_FILENAME];
    char dest_dir_name[MAX_FILENAME];
    int last_slash_idx = -1;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, src)) {
            src_node_idx = i;
            break;
        }
    }

    if (src_node_idx == -1) {
        printString("mv: file not found: ");
        printString(src);
        printString("\r\n");
        return;
    }

    if (node_fs_buf.nodes[src_node_idx].data_index == FS_NODE_D_DIR) {
        printString("mv: cannot move a directory.\r\n");
        return;
    }

    for (i = 0; dst[i] != '\0'; i++) {
        if (dst[i] == '/') {
            last_slash_idx = i;
        }
    }

    if (last_slash_idx == -1) {
        dest_parent_idx = cwd;
        strcpy(dest_name, dst);
    } else {
        strcpy(dest_name, dst + last_slash_idx + 1);

        if (last_slash_idx > 0) memcpy(dest_dir_name, dst, last_slash_idx);
        dest_dir_name[last_slash_idx] = '\0'; 

        if (last_slash_idx == 0) dest_parent_idx = FS_NODE_P_ROOT;
        else if (strcmp(dest_dir_name, "..")) {
            if (cwd == FS_NODE_P_ROOT) dest_parent_idx = FS_NODE_P_ROOT;
            else dest_parent_idx = node_fs_buf.nodes[cwd].parent_index;
        } else {
            int dest_dir_node_idx = -1;
            for (i = 0; i < FS_MAX_NODE; i++) {
                if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, dest_dir_name)) {
                    dest_dir_node_idx = i;
                    break;
                }
            }

            if (dest_dir_node_idx == -1) {
                printString("mv: destination directory not found: ");
                printString(dest_dir_name);
                printString("\r\n");
                return;
            }
            if (node_fs_buf.nodes[dest_dir_node_idx].data_index != FS_NODE_D_DIR) {
                printString("mv: destination is not a directory: ");
                printString(dest_dir_name);
                printString("\r\n");
                return;
            }
            dest_parent_idx = dest_dir_node_idx;
        }
    }

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == dest_parent_idx && strcmp(node_fs_buf.nodes[i].node_name, dest_name)) {
            printString("mv: destination '");
            printString(dst);
            printString("' already exists.\r\n");
            return;
        }
    }

    node_fs_buf.nodes[src_node_idx].parent_index = dest_parent_idx;
    strcpy(node_fs_buf.nodes[src_node_idx].node_name, dest_name);

    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

}


// TODO: 9. Implement cp function
void cp(byte cwd, char* src, char* dst) {
    struct file_metadata metadata_src, metadata_dst;
    enum fs_return status;
    byte dest_parent_idx;
    char dest_name[MAX_FILENAME], dest_dir_name[MAX_FILENAME];
    int last_slash_idx = -1;
    struct node_fs node_fs_buf;
    int i;
    int src_node_idx = -1;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, src)) {
            src_node_idx = i;
            break;
        }
    }

    if (src_node_idx == -1) {
        printString("cp: file not found: ");
        printString(src);
        printString("\r\n");
        return;
    }

    if (node_fs_buf.nodes[src_node_idx].data_index == FS_NODE_D_DIR) {
        printString("cp: cannot copy a directory.\r\n");
        return;
    }
    metadata_src.parent_index = cwd;
    strcpy(metadata_src.node_name, src);
    
    fsRead(&metadata_src, &status);
    
    if (status != FS_SUCCESS) {
        printString("cp: error reading source file.\r\n");
        return;
    }

    for (i = 0; dst[i] != '\0'; i++) {
        if (dst[i] == '/') {
            last_slash_idx = i;
        }
    }
    
    if (last_slash_idx == -1) {
        printString("cp: invalid destination format. Use <dirname>/<outputname>, /<outputname>, or ../<outputname>\r\n");
        return;
    }
    strcpy(dest_name, dst + last_slash_idx + 1);

    if (last_slash_idx > 0) memcpy(dest_dir_name, dst, last_slash_idx);
    dest_dir_name[last_slash_idx] = '\0';

    if (last_slash_idx == 0) dest_parent_idx = FS_NODE_P_ROOT;
    else if (strcmp(dest_dir_name, "..")) dest_parent_idx = (cwd == FS_NODE_P_ROOT) ? FS_NODE_P_ROOT : node_fs_buf.nodes[cwd].parent_index;
    else { 
        int dest_dir_node_idx = -1;
        for (i = 0; i < FS_MAX_NODE; i++) {
            if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, dest_dir_name)) {
                dest_dir_node_idx = i;
                break;
            }
        }

        if (dest_dir_node_idx == -1 || node_fs_buf.nodes[dest_dir_node_idx].data_index != FS_NODE_D_DIR) {
            printString("cp: destination directory not found or is not a directory.\r\n");
            return;
        }
        dest_parent_idx = dest_dir_node_idx;
    }

    metadata_dst.parent_index = dest_parent_idx;
    strcpy(metadata_dst.node_name, dest_name);
    metadata_dst.filesize = metadata_src.filesize;
    memcpy(metadata_dst.buffer, metadata_src.buffer, metadata_src.filesize);

    fsWrite(&metadata_dst, &status);

    if (status == FS_W_SUCCESS || status == FS_SUCCESS) { 
    } else if (status == FS_W_NODE_ALREADY_EXISTS) printString("cp: destination file already exists.\r\n");
    else if (status == FS_W_NOT_ENOUGH_SPACE) printString("cp: not enough space on disk.\r\n");
    else printString("cp: an unknown error occurred during write.\r\n");
}


// TODO: 10. Implement cat function
void cat(byte cwd, char* filename) {
    struct file_metadata metadata;
    enum fs_return status;
    metadata.parent_index = cwd;

    strcpy(metadata.node_name, filename);
    fsRead(&metadata, &status);

    if (status == FS_SUCCESS) {
        printString(metadata.buffer);
        printString("\n");
    } else if (status == FS_R_NODE_NOT_FOUND) {
        printString("cat: '");
        printString(filename);
        printString("': No such file or directory\n"); 
    } else if (status == FS_R_TYPE_IS_DIRECTORY) {
        printString("cat: '");
        printString(filename);
        printString("': Is a directory\n");
    } else printString("cat: An unknown error occurred.\n");
}


// TODO: 11. Implement mkdir function
void mkdir(byte cwd, char* dirname) {
    struct node_fs node_fs_buf;
    int i;
    int free_node_index = -1;

    if (dirname[0] == '\0') { 
        printString("mkdir: missing operand\n");
        return;
    }

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd &&
            strcmp(node_fs_buf.nodes[i].node_name, dirname)) { 
            printString("mkdir: cannot create directory '");
            printString(dirname);
            printString("': File exists\n");
            return;
        }
    }

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].node_name[0] == '\0') {
            free_node_index = i;
            break;
        }
    }

    if (free_node_index == -1) {
        printString("mkdir: failed to create directory: No free node available\n");
        return;
    }

    node_fs_buf.nodes[free_node_index].data_index = FS_NODE_D_DIR;
    node_fs_buf.nodes[free_node_index].parent_index = cwd;
    strcpy(node_fs_buf.nodes[free_node_index].node_name, dirname);
    
    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}
