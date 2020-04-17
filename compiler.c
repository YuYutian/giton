#include <stdio.h>
#include <string.h>
#include "status.h"

int getCompilerPath(char *buf, char *whichpath, char *cn)
{
	FILE *fp = NULL;
	char command[1024] = {0};
	snprintf(command, 1024, "%s %s", whichpath, cn);
	fp = popen(command,"r");
	if(fp == NULL)
	{
		return NOT_COMPLETED;
	}
	fread(buf, 1024, 1, fp);
	buf[strlen(buf)-1] = '\0';
	pclose(fp);
	if(strlen(buf) != 0)
		return 1;
	else
		return 0;
}

status getCompilerVersion(char *buf, char *cp, char *ver)
{
	FILE *fp = NULL;
	char command[1024] = {0};
	snprintf(command, 1024, "%s %s", cp, ver);
	fp = popen(command,"r");
	if(fp == NULL)
	{
		return NOT_COMPLETED;
	}
	fread(buf, 1024, 1, fp);
	char* index = NULL;
	index = strchr(buf, '\n');
	if(NULL != index)
	    buf[index-buf] = '\0';
	pclose(fp);
	return OK;
}

/*int main()
{
char buf[1024] = {0};
char buf2[1024] = {0};
getCompilerPath(buf, "which", "go");
printf("%s", buf);
//getCompilerVersion(buf2, buf, "--version");
//printf("[%s]", buf2);
}*/
