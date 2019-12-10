#ifndef __HTTP_DOWN_H__
#define __HTTP_DOWN_H__

#include "stdafx.h"

struct tNode
{
	FILE *fp;
	long startPos;
	long endPos;
	void *curl;
	pthread_t tid;
};

static size_t writeFunc(void *ptr,size_t size,size_t nmemb,void *userdata);
int progressFunc(void *ptr,double totalToDownload,double nowDownloaded,double totalToUpLoad,double nowUpLoaded);
long getDownloadFileLength(const char *url);
void* workThread(void *pData);
bool downLoad(int threadNum,string url,string Path,string fileName);


#endif
