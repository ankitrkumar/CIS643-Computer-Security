#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *socket_path = "\0hidden";

int sendfd(int sock, int fd)
{
  struct msghdr hdr;
  struct iovec data;

  char cmsgbuf[CMSG_SPACE(sizeof(int))];

  char dummy = '*';
  data.iov_base = &dummy;
  data.iov_len = sizeof(dummy);
  
 /* initialize socket message */
  memset(&hdr, 0, sizeof(hdr));
  hdr.msg_name = NULL;
  hdr.msg_namelen = 0;
  hdr.msg_iov = &data;
  hdr.msg_iovlen = 1;
  hdr.msg_flags = 0;
  hdr.msg_control = cmsgbuf;
  hdr.msg_controllen = CMSG_LEN(sizeof(int));

  /* initialize a single ancillary data element for file descriptor passing */ 
  struct cmsghdr* cmsg = CMSG_FIRSTHDR(&hdr);
  cmsg->cmsg_len   = CMSG_LEN(sizeof(int));
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type  = SCM_RIGHTS;
  *(int*)CMSG_DATA(cmsg) = fd;

  int n = sendmsg(sock, &hdr, 0);
  if(n == -1) printf("sendmsg() failed: \n");

  return n;
}
int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  char buf[100];
  int fd,rc;
  int file_d;
  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }
  /* init the sockaddr_un memory space */
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }
  /* open file descriptor and send out */
  file_d = open("./test",O_WRONLY); 
  sendfd(fd,file_d); 
  close(file_d);
  printf("file descriptor sent \n");

  return 0;
}
