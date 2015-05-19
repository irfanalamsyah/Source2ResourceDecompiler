#include "fileinfo.h"

filedata* loadfile(char* filename) {
	FILE* file;
	file = fopen(filename,"r");
	if(file == NULL) {
		fprintf(stderr,"Error: unable to open file '%s'\n",filename);
		exit(2);
	}
	filedata* ret = (filedata*)calloc(1,sizeof(filedata));
	fseek(file, 0, SEEK_END);
	ret->length = ftell(file);
	fseek(file, 0, SEEK_SET);
	ret->contents = malloc((size_t)ret->length);
	fread(ret->contents, ret->length, 1, file);
	fclose(file);
	parse(ret);
	return ret;
}

void parse(filedata* fd) {
	if(strncmp(fd->contents,"<!--",4) == 0) {
		fprintf(stderr,"Parsing as dmx...\n");
		fd->filetype = UNK_DMX;
	} else if( *((uint32_t*)fd->contents) == 0x55aa1234) {
		fprintf(stderr,"Parsing as vpk...\n");
		fd->filetype = VPK;
	} else if( *((uint32_t*)fd->contents) == 0xFADEBEAD) {
		fprintf(stderr,"Parsing as gnv...\n");
		fd->filetype = GNV;
	} else if(strncmp(fd->contents,"VBSP",4) == 0) {
		fprintf(stderr,"Parsing as bsp. Currently not handled and out of scope.\n");
		fd->filetype = BSP;
	} else if( *((uint32_t*)fd->contents) == fd->length) {
		fprintf(stderr,"Parsing as one of valve's stupid new formats without magic numbers...\n");
		parse_svf(fd);
	} else {
		fprintf(stderr,"Unknown file type.\n");
		fd->filetype = UNKNOWN;
	}
}