#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static const uint8_t byte_buffer_size = 2;
static const uint8_t maxblocksize = 8;

int size_of_file(FILE *fileptr){
  fseek(fileptr, 0L, SEEK_END);
  int filesize = ftell(fileptr);
  rewind(fileptr);
  return filesize;
}

void fetch_block_hex(FILE *fileptr, unsigned char *blockbuff){
  if(byte_buffer_size==1) {
    sprintf(blockbuff, "%02x", fgetc(fileptr));
    return;
  }

  unsigned char ro_next_byte;
  for(uint8_t byte_index=1; byte_index<byte_buffer_size; byte_index++){
    ro_next_byte = fgetc(fileptr);
    sprintf(blockbuff, "%s%02x", blockbuff, fgetc(fileptr));
    sprintf(blockbuff, "%s%02x", blockbuff, ro_next_byte);
  }
}

int main(int argc, char *argv[]){
  FILE *file = fopen(argv[1], "rb");
  unsigned long filesize = size_of_file(file);
  unsigned char *blockbuff = (unsigned char *) malloc(sizeof(unsigned char) * byte_buffer_size);

  for(unsigned long filepos=0; filepos<=filesize; filepos++){
    printf("%07lx\t", ftell(file));
    for(uint8_t block_index=0; block_index<maxblocksize; block_index++){
      memset(blockbuff, 0, sizeof(unsigned char) * byte_buffer_size);
      fetch_block_hex(file, blockbuff);
      printf("%s ", blockbuff);
    }
    printf("\n");
  }

  free(blockbuff);
  return EXIT_SUCCESS;
}
