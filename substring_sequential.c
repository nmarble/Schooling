#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024

int total = 0;
int n1,n2;
char *s1,*s2;
FILE *fp;

int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1);  /*length of s1*/
	n2=strlen(s2)-1; /*length of s2*/
	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		return -1;
}

//Function for threads
void *num_substring(void *arg)
{
	int offset = (int)arg;
	char subs1[n2];
	
	//set substring offest	
	memcpy(subs1, s1 + offset, n2);
	//add end of line for easy compare
	strcat(subs1, "\n");
	//Add to total if the substring matches the second
	if (strcmp(s2, subs1) == 0)
		total++;
}

//Function with loop to create and join threads
void create_threads() 
{
	int rc;
	int i = 0;
	int optThreads = n1-n2;
	void *status;
	pthread_t tid[optThreads];
	
	//Loop through each possible combination
	while(i < (n1 - n2))
	{
		//Create thread and output any errors
		rc = pthread_create(&(tid[i]), NULL, &num_substring, (void*)i);
		if (rc != 0)
			printf("Thread %d failed to create, error: [%s]", i, strerror(rc));
		i++;		
	}
	//Loop to join all threads 
	for(i=0; i<optThreads; i++) {
		//Join current thread and output any errors
		rc = pthread_join(tid[i], &status);
      		if (rc) {
         		printf("ERROR; return code from pthread_join() is %d\n", rc);
         		exit(-1);
         	} 
	}
}

int main(int argc, char *argv[])
{
 	//Read the strings
	readf(fp);

	//Create all the threads
	create_threads();

	//Total based on matches of the substring compared to main screen
 	printf("The number of substrings is: %d\n", total);

	return 1;
}











