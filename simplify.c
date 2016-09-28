/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
  gcc -L/usr/local/lib -lssl -lcrypto c_test.c
*/

#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "list.h"
#include <openssl/sha.h> //依赖openssl库
static const int file_size=1024;
static int hello_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;
	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, hello_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	} else
		res = -ENOENT;

	return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, hello_path + 1, NULL, 0);

	return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, hello_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, hello_path) != 0)
		return -ENOENT;

	len = strlen(hello_str);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, hello_str + offset, size);
	} else
		size = 0;

	return size;
}
static int pmfs_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int len=0;
	int len_tem;
	int i;
	int file_size;
	int shadow_size=0;//最后那个区域的大小
	char buffer[65536];
	char** buf_path;
	memset(buffer,0,sizeof(buffer));
	fd=open(path,O_RDONLY);
     if(fp<0)
     {
         printf("Can't open file\n");
         return 0;
     } 
	while((len_tem=read(fd,buffer,65536))>0){
		len=len+len_tem;
	}
	//len=read(fd,buffer,65536);
	len=len/21;//个数
	buf_path = (char**)malloc(sizeof(char*) * len);
	for(i=0;i<len;i=i++)
     {
        buf_path[i]=substring(buffer,i*21,21);
     }
	memset(buffer,0,sizeof(buffer));
	if(buf_path[len-1][0]==0){
		shadow_size=(buf_path[len-1][1]-48)*1000+(buf_path[len-1][2]-48)*100+(buf_path[len-1][3]-48)*10+(buf_path[len-1][4]-48);
		len-1;
		}
	file_size=len*size+shadow_size;
		close(fd);
	if(offset+sizeof(buf)>=file_size){
		if(offset+sizeof(buf)-file_size>size-shadow_size){

			fd=open(buf_path[len-1],O_WRONLY|O_APPEND);
			write(fd,)
		}
	}
	}
char* substring(char* ch,int pos,int length)  //专门
{  
    char* pch=ch;//定义一个字符指针，指向传递进来的ch地址。  
    char* subch=(char*)calloc(sizeof(char),length+1);  //通过calloc来分配一个length长度的字符数组，返回的是字符指针。  
    int i;  //只有在C99下for循环中才可以声明变量，这里写在外面，提高兼容性。  
    pch=pch+pos;  //是pch指针指向pos位置。  
    for(i=0;i<length;i++)  
    {  
        subch[i]=*(pch++);  //循环遍历赋值数组。  
    }  
    subch[length]='\0';//加上字符串结束符。  
    return subch;       //返回分配的字符数组地址。  
} 
static int file_sha_calculate(const char *path){
	 SHA_CTX stx;
     unsigned char outmd[20];//注意这里的字符个数为20
     char buffer[1024];
     //char filename[32];
     int len=0;
     int i;
     int fp=0;
     memset(outmd,0,sizeof(outmd));
     //memset(filename,0,sizeof(filename));
     memset(buffer,0,sizeof(buffer));
     //printf("请输入文件名，用于计算SHA1值:");
     //scanf("%s",filename);
     fp=open(path,O_RDONLY);
     if(fp<0)
     {
         printf("Can't open file\n");
         return 0;
     } 
     SHA1_Init(&stx);
     while((len=fread(buffer,1,1024,fp))>0)
     {
         SHA1_Update(&stx,buffer,len);
         memset(buffer,0,sizeof(buffer));
     }
     SHA1_Final(outmd,&stx);
 
     for(i=0;i<20;i<i++)
     {
        printf("%02X",outmd[i]);
     }
     printf("\n");
    return 0;
}
static struct fuse_operations pmfs_oper = {
	.open		= pmfs_open,
	.read		= pmfs_read,
	.write      = pmfs_write,
};

int main(int argc, char *argv[])
{
	int fd;
	if((fd=open("file.connet",O_RDWR|O_CREAT)) <0)
  	printf("create error\n");
	return fuse_main(argc, argv, &pmfs_oper, NULL);
}
