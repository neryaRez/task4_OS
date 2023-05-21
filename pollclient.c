#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#define PORT 9034   // Port we're listening on
int flag_con = 0;
int sock_fd = -1;



int recv_message(void* args){

char input[8192] = {0};

flag_con = 1;

int bytes = 0;

while ( (bytes = recv(sock_fd, input, 8192, 0)) != -1)
{
    if (bytes == 0)
    {
        flag_con = 0;
        break;
    }

    printf("%s\n", input);

    if(strcmp(input, "exit") == 0)
    {
        flag_con = 0;
        break;
    }
    bzero(input, 8192);
    
}

    return 0;

}

int send_message(void* args){

    char input[8192] = {0};

    while (flag_con != 0)
    {
        fgets(input, 8192, 0);
        int sendi = send(sock_fd, input, sizeof(input) + 1, 0);
        if(sendi == -1){
            perror("send\n");
            return -1;
        }
        bzero(input, 8192);

    }
    
return 0;
}

int main(int argc, char* argv[]){
    sock_fd = socket(AF_INET, SOCK_STREAM,0);
    if(sock_fd == -1){
        perror("socket\n");
        return -1;
    }
    signal(SIGPIPE, SIG_IGN);
    struct sockaddr_in server_Adress;
    memset(&server_Adress, 0, sizeof(server_Adress));

    server_Adress.sin_family = AF_INET;
    server_Adress.sin_port = htons(PORT);
    server_Adress.sin_addr.s_addr = inet_addr("127.0.0.1");

    int con = connect(sock_fd, (struct sockaddr*)&server_Adress, sizeof(server_Adress));

    if(con == -1){
        perror("connect\n");
        return -1;
    }

    
    pthread_t chat[2];

    flag_con = 1;

    pthread_create(&chat[0], NULL, (void*)recv_message,NULL);

    pthread_create(&chat[1], NULL, (void*)send_message,NULL);

    pthread_join(chat[1], NULL);

    pthread_kill(chat[1], 0);

    close(sock_fd);

    return 0;
}

