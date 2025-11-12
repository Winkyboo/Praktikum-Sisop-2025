#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void fsInit() {
  struct map_fs map_fs_buf;
  int i = 0;

  readSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
  for (i = 0; i < 16; i++) map_fs_buf.is_used[i] = true;
  for (i = 256; i < 512; i++) map_fs_buf.is_used[i] = true;
  writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
}

// TODO: 2. Implement fsRead function
void fsRead(struct file_metadata* metadata, enum fs_return* status) {
    struct node_fs node_fs_buf;
    struct data_fs data_fs_buf;
    int i, j; bool isfile = false;

    readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, metadata->node_name) && node_fs_buf.nodes[i].parent_index == metadata->parent_index) {
            isfile = true;
            break;
        }
    }

    if (!isfile) {
        *status = FS_R_NODE_NOT_FOUND;
        return;
    }

    if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
        *status = FS_R_TYPE_IS_DIRECTORY;
        return;
    }

    metadata->filesize = 0;

    for (j = 0; j < FS_MAX_SECTOR; j++) {
        if (data_fs_buf.datas[node_fs_buf.nodes[i].data_index].sectors[j] == 0x00) break; 
        else if (data_fs_buf.datas[node_fs_buf.nodes[i].data_index].sectors[j] != 0){
            readSector(metadata->buffer + (j * SECTOR_SIZE), data_fs_buf.datas[node_fs_buf.nodes[i].data_index].sectors[j]);
            metadata->filesize += SECTOR_SIZE;
        }
    }
    *status = FS_SUCCESS;
}

// TODO: 3. Implement fsWrite function
void fsWrite(struct file_metadata* metadata, enum fs_return* status) {
    struct map_fs map_fs_buf;
    struct node_fs node_fs_buf;
    struct data_fs data_fs_buf;
    int isempty, idx;
    int n = (metadata->filesize + SECTOR_SIZE - 1)/SECTOR_SIZE;
    int count=0;
    int j= 0;
    int i;
  
    readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    readSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
    
    for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, metadata->node_name) && node_fs_buf.nodes[i].parent_index == metadata->parent_index) {
          *status = FS_W_NODE_ALREADY_EXISTS;
          return;   
        }
    }

    isempty = -1;
      for (i = 0; i < FS_MAX_NODE; i++) {
          if (node_fs_buf.nodes[i].node_name[0]=='\0'){
            isempty = i;
            break;
          }
      }
  
    if (isempty == -1){
      *status = FS_W_NO_FREE_NODE;
      return;
    }
  
    idx = -1;
    for(i = 0; i < FS_MAX_DATA; i++){
      if(data_fs_buf.datas[i].sectors[0]==0x00){
        idx = i;
        break;
      }
    }
    if (idx == -1){
        *status = FS_W_NO_FREE_DATA;
        return;
    }

    for (i = 16; i < 256; i++) {
        if (map_fs_buf.is_used[i] == false) {
        count++;
        }
    }

    if (count < n) {
        *status = FS_W_NOT_ENOUGH_SPACE;
        return;
    }
    
    strcpy(node_fs_buf.nodes[isempty].node_name, metadata->node_name);
    node_fs_buf.nodes[isempty].parent_index = metadata->parent_index;
    node_fs_buf.nodes[isempty].data_index = idx;

    for (i=16; i<256; i++) {
      if(j>=n) break;
      if (map_fs_buf.is_used[i] == false) {
        map_fs_buf.is_used[i] = true;
        data_fs_buf.datas[idx].sectors[j]=i;
        writeSector(metadata->buffer +  j * SECTOR_SIZE, i);
        j++;
      }
    }

    writeSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
    *status = FS_SUCCESS;
  }
