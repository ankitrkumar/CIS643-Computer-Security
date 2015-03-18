/*vulp.c*/

#include <stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
char * fn = "/tmp/XYZ";
char buffer[60];
FILE *fp1, *fp2;
struct stat filestat1, filestat2;

/* get user input */

scanf("%50s", buffer );

	if(!access(fn, W_OK))
	{
	fp1 = fopen(fn, "a+");
	lstat(fn, &filestat1);
	printf("\nIn first access");
//	printf("\n fd1 %d", filestat1.st_ino);
	}
	if(!access(fn, W_OK))
	{
	fp2 = fopen(fn, "a+");
	lstat(fn, &filestat2);
	printf("\nIn second access");
//	printf("\n fd2 %d", filestat2.st_ino);
//	}
	/*if(filestat1.st_ino == filestat2.st_ino)
	{*/
	fwrite("\n", sizeof(char), 1, fp2);
	fwrite(buffer, sizeof(char), strlen(buffer), fp2);
	fclose(fp1);
	fclose(fp2);
	}
	else {
	printf("\nNo permission");
	}
	printf("\n");
}

