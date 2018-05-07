#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
int open_clientfd(char *hostname, int port){
        int clientfd;
        struct hostent *hp;
        struct sockaddr_in serveraddr;

        if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                return -1;  /* Check errno for cause of error */

        /* Fill in the server's IP address and port */
        if ((hp = gethostbyname(hostname)) == NULL)
                return -2;  /* Check h_errno for cause of error */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)hp->h_addr_list[0],
              (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
        serveraddr.sin_port = htons(port);

        /* Establish a connection with the server */
        if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
                return -1;
        return clientfd;
}
int main(int argc, char **argv){
    if (argc != 3) {
        std::cout << "./a.out HOST PORT" << std::endl;
    	return -1;
    }
    char *host = argv[1];
    int port = atoi(argv[2]);

    int clientfd = open_clientfd(host, port);

    // int fd= open("./test.xml",O_RDONLY);
    // int a = read(fd,buf,1024);
    std::string buf = "asd";
	write(clientfd, buf.c_str(), buf.length());
    sleep(1000);
    close(clientfd); //line:netp:echoclient:close
    return 1;
}
/* $end echoclientmain */
