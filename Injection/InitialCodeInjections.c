#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

void *infinite_thread(void *a);
void *table_update_thread(void *a);

//Injects a thread that runs forever at the background, PINCE will switch to that thread when attached
void inject_infinite_thread()
{
	int *a=1;
	pthread_t* threadinf=(pthread_t*) malloc(sizeof(pthread_t));
	pthread_create(threadinf, NULL, infinite_thread, (void*) a);
}

void *infinite_thread(void *a)
{
	while(1)
	{
		sleep(100000);
	}
}

//Injects a thread that updates the address table constantly
void inject_table_update_thread()
{
	int *a=1;
	pthread_t* threadinf=(pthread_t*) malloc(sizeof(pthread_t));
	pthread_create(threadinf, NULL, table_update_thread, (void*) a);
}

void *table_update_thread(void *a)
{
	int selfpid=getpid();
	char status_file[100];
	char recv_file[100];
	char send_file[100];
	char abort_verify_file[100];
	sprintf(recv_file, "/tmp/PINCE/%d/PINCE-to-Inferior.txt", selfpid);
	sprintf(send_file, "/tmp/PINCE/%d/Inferior-to-PINCE.txt", selfpid);
	sprintf(status_file, "/tmp/PINCE/%d/status.txt", selfpid);
	sprintf(abort_verify_file, "/tmp/PINCE/%d/abort-verify.txt", selfpid);
	FILE *fp;
	FILE *status;
	FILE *exit;
	FILE *initialize;
	char buff[255]="";
	char status_word[255]="";
	initialize = fopen(status_file, "r");
	while(initialize==NULL){
		initialize = fopen(status_file, "r");
	}
	fclose(initialize);
	while(1){
		sleep(1);
		status = fopen(status_file, "w");
		fputs("sync-request-recieve", status);
		fclose(status);
		exit = fopen(abort_verify_file, "r");
		if(exit!=NULL){
			fclose(exit);
			//remove(abort_verify_file);
			//remove(status_file);
			//remove(recv_file);
			//remove(send_file);
			//pthread_exit(0);
		}
		fp = fopen(recv_file, "r");
		fscanf(fp, "%s", buff);
		//printf("%s\n", buff );
		fclose(fp);
		fp = fopen(send_file, "w");
		fputs("a", fp);
		fclose(fp);
	}
}