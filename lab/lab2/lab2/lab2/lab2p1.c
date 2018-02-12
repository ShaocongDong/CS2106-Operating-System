#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>

// Port Number
#define PORTNUM			80

// Maximum size of HTTP response
#define MAX_BUFFER_LEN	65535

// Maximum length of one line
#define LINE_BUFFER_LEN	8192

// Maximum size of a file
#define MAX_FILE_SIZE	65000

// Maximum length of a log entry
#define LOG_BUFFER_LEN	1024

// Maximum length of a filename
#define MAX_FILENAME_LEN	128

// HTTP methods
enum
{
	GET,
	POST,
	PUT,
	HEAD
};

void startServer(uint16_t portNum);
void formHTTPResponse(char *buffer, uint16_t maxBufferLen, uint16_t returnCode, 
	char *returnMessage, char *body, uint16_t bodyLength);
void deliverHTTP(int connfd);
char *getCurrentTime(void);
void writeLog(const char *format, ...);
void parseHTTP(const char *buffer, int *method, char *filename);

int main(int ac, char **av)
{
	startServer(PORTNUM);
}

char *getCurrentTime()
{
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	char *timeStr = asctime(tm);

	// Get rid of \n at the end
	timeStr[strlen(timeStr)-1]='\0';

	return timeStr;
}

void formHTTPResponse(char *buffer, uint16_t maxBufferLen, uint16_t returnCode, 
	char *returnMessage, char *body, uint16_t bodyLength)
{
	sprintf(buffer, "HTTP/1.1 %d %s\n", returnCode,
		returnMessage);
	sprintf(buffer, "%sDate: %s\n", buffer, getCurrentTime());
	sprintf(buffer, "%sServer: CS2106/1.1.0\n", buffer);
	sprintf(buffer, "%sContent-Length: %d\n", buffer, bodyLength);
	sprintf(buffer, "%sContent-Type: text/html\n", buffer);
	sprintf(buffer, "%sConnection: Closed\n", buffer);
	sprintf(buffer, "%s\n\n%s\n", buffer, body);
	writeLog("Response: %d:%s", returnCode, returnMessage);
}

void readHTML(FILE *fp, char *fileBuffer, uint16_t maxBufferLen)
{
	if(fp != NULL)
	{
		/*char lineBuffer[LINE_BUFFER_LEN];
		uint16_t currCharCount=0;

		// Zero the buffer first
		fileBuffer[0]='\0';*/

		fread(fileBuffer, sizeof(char), maxBufferLen, fp);
		/*while(!feof(fp) && currCharCount < maxBufferLen)
		{
			fgets(lineBuffer, LINE_BUFFER_LEN, fp);
			sprintf(fileBuffer, "%s%s", fileBuffer, lineBuffer);
			currCharCount += strlen(lineBuffer);
		} // while*/
	}

}

void parseHTTP(const char *buffer, int *method, char *filename)
{
	char tmpBuffer[MAX_BUFFER_LEN];
	char *mtd, *fname;

	strncpy(tmpBuffer, buffer, MAX_BUFFER_LEN);

	// Tokenize the request. This is pretty 
	// fragile but we just want to do it quick, so...
	mtd=strtok(tmpBuffer, " ");
	fname=strtok(NULL, " ");

	printf("PARSING METHOD\n");
	if(strcmp(mtd, "HEAD") == 0)
		*method= GET;
	else
		if(strcmp(mtd, "POST") == 0)
			*method = POST;
		else
			if(strcmp(mtd, "PUT") == 0)
				*method = PUT;
			else
				*method = GET;
	
	printf("Copying filename\n");
	strncpy(filename, fname, MAX_FILENAME_LEN);
	printf("Done. Filename is %s\n", filename);
}

void deliverHTTP(int connfd)
{
	FILE *fptr;
	char HTTPBuffer[MAX_BUFFER_LEN];
	char fileBuffer[MAX_FILE_SIZE];

	read(connfd, HTTPBuffer, MAX_BUFFER_LEN);

	int method;
	char filename[MAX_FILENAME_LEN];
	char fetchName[MAX_FILENAME_LEN];

	parseHTTP(HTTPBuffer, &method, filename);
	printf("Method = %d filename = %s\n", method,filename);

	if(method == HEAD)
		formHTTPResponse(HTTPBuffer, MAX_BUFFER_LEN, 200, "OK", NULL, 0);
	else
	{
		if(strcmp(filename, "/") == 0)
			strcpy(fetchName, "./index.html");
		else
			sprintf(fetchName, ".%s", filename);

		writeLog("Fetching %s", fetchName);
		fptr = fopen(fetchName, "r");
		if(fptr == NULL)
		{
			writeLog("Cannot find %s", filename);
			sprintf(fileBuffer,"<html><body><h1>%s NOT FOUND</h1></body></html>", filename);
			formHTTPResponse(HTTPBuffer, MAX_BUFFER_LEN, 404, "NOT FOUND", fileBuffer, strlen(fileBuffer));
		}
		else
		{
			readHTML(fptr, fileBuffer, MAX_FILE_SIZE);
			fclose(fptr);
			formHTTPResponse(HTTPBuffer, MAX_BUFFER_LEN, 200, "OK", fileBuffer, strlen(fileBuffer));
			writeLog("Serving %s", HTTPBuffer);
		}
	}

	write(connfd, HTTPBuffer, strlen(HTTPBuffer));
	close(connfd);
}

void writeLog(const char *format, ...)
{
	char logBuffer[LOG_BUFFER_LEN];
	va_list args;
	
	va_start(args, format);
	vsprintf(logBuffer, format, args);
	va_end(args);

	printf("%s: %s\n", getCurrentTime(), logBuffer);

}

void startServer(uint16_t portNum)
{
	static int listenfd, connfd;
	static struct sockaddr_in serv_addr;


	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if(listenfd<0)
	{
		perror("Unable to make socket.");
		exit(-1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portNum);

	if(bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		perror("Unable to bind.");
		exit(-1);
	}

	if(listen(listenfd, 10)<0)
	{
		perror("Unable to listen.");
		exit(-1);
	}

	writeLog("Web server started at port number %d", portNum);

	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
		writeLog("Connection received.");

		deliverHTTP(connfd);
	}
}
