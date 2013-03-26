#include "file_size.h"

long getFileSize(FILE* file){
	long size;
	fseek (file , 0 , SEEK_END);
	size = ftell (file);
	rewind (file);
	return size;
}
