#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>

#define GET_CPU_USAGE_DELAY 500000
float getCpuUsage(int delay);

int main(int argc, char *argv[])
{
	int sock, rc, wc;
	struct sockaddr_in server;
	char *temp = malloc(sizeof(char)*1000);
	
	int debugflag = 0;	

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		fprintf(stderr, "Could not create socket.\n");
		exit(EXIT_FAILURE);
	}
	else 
	{	fprintf(stdout, "Socket created,\n");	}
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons((u_short)atoi(argv[1]));

	if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		fprintf(stderr, "Connect failed, error.\n");
		exit(EXIT_FAILURE);
	}
	else
	{	fprintf(stdout, "Conneted to server.\n");	}

	int val = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, val | O_NONBLOCK);
//	char *hiMsg = "Hi, server.\n";
//	write(sock, hiMsg, strlen(hiMsg));
	while(1)
	{
		if( read(sock, temp, 999) )
		{	
			printf("%s\n",temp);
			bzero(temp, 999);
		}

		if( debugflag < 1000 )
		{
			float usage = getCpuUsage(GET_CPU_USAGE_DELAY);
			char usageMsg[20];
		        snprintf(usageMsg, sizeof(usageMsg), "CPU %0.2f", usage);
        		write(sock, usageMsg, strlen(usageMsg));
			debugflag++;
		}
		else
		{
			close(sock);
			break;
		}
		//read(sock, temp, 1000);
		//fprintf(stdout, "%s", temp);
		
		
	}
	return 0;

}

float getCpuUsage(int delay)
{
    float a[4], b[4], load;
    FILE *fp;

    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%*s %f %f %f %f", &a[0], &a[1], &a[2], &a[3]);
    fclose(fp);

    usleep(delay);

    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%*s %f %f %f %f", &b[0], &b[1], &b[2], &b[3]);
    fclose(fp);

    load = ( (b[0] + b[1] + b[2]) - (a[0] + a[1] + a[2]) ) / ( (b[0] + b[1] + b[2] + b[3]) - (a[0] + a[1] + a[2] + a[3]) );

    printf("\rCPU Usage: %0.2f%%  ", load * 100);
    fflush(stdout);

    return load * 100;
}

