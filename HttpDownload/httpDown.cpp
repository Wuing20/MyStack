#include "httpDown.h"

int thread_num = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static size_t writeFunc(void *ptr,size_t size,size_t nmemb,void *userdata)
{
	tNode *tData = (tNode *)userdata;
	size_t written = 0;
	
	pthread_mutex_lock(&g_mutex);
	if(tData->startPos + size * nmemb <= tData->endPos)
	{
		fseek(tData->fp,tData->startPos,SEEK_SET);
		written = fwrite(ptr,size,nmemb,tData->fp);
		tData->startPos += size * nmemb;
	}
	else
	{
		fseek(tData->fp,tData->startPos,SEEK_SET);
		written = fwrite(ptr,1,tData->endPos - tData->startPos + 1,tData->fp);
		tData->startPos = tData->endPos;
	}

	pthread_mutex_unlock(&g_mutex);

	return written;
}

int progressFunc(void *ptr,double totalToDownload,double nowDownloaded,double totalToUpLoad,double nowUpLoaded)
{
	int percent = 0;
	
	if(totalToDownload > 0)
	{
		percent = (int) (nowDownloaded / totalToDownload *100);
	}

	if(0 == (percent % 20))
	{
                pthread_mutex_lock (&g_mutex);
                cout<<"下载进度: "<<percent<<endl;
                pthread_mutex_unlock (&g_mutex);
	}

	return 0;		
}

long getDownloadFileLength(const char *url)
{
	long downloadFileLength = -1;

	if(NULL == url)
	{
		cout<<"this url is empty!"<<endl;

		return -1;
	}

	CURL *handle = curl_easy_init();
	
	curl_easy_setopt(handle,CURLOPT_URL,url);
	curl_easy_setopt(handle,CURLOPT_HEADER,1);   //获取header头部信息
	curl_easy_setopt(handle,CURLOPT_NOBODY,1);

	if(CURLE_OK == (curl_easy_perform(handle)))
	{
		curl_easy_getinfo(handle,CURLINFO_CONTENT_LENGTH_DOWNLOAD,&downloadFileLength);
	}

	return downloadFileLength;
}

void* workThread(void *pData)
{
	tNode *pNode = (tNode *) pData;

        int res = curl_easy_perform (pNode->curl);

        if (res != 0)
        {
		cout<<"this error info: "<<curl_easy_perform(pNode->curl)<<endl;
        }

        curl_easy_cleanup (pNode->curl);

        pthread_mutex_lock (&g_mutex);
        thread_num--;
        printf ("thred %ld exit\n", pNode->tid);
        pthread_mutex_unlock (&g_mutex);
        delete pNode;
        pthread_exit (0);

        return NULL;
}

bool downLoad (int threadNum, string Url, string Path, string fileName)
{
        long fileLength = getDownloadFileLength (Url.c_str ());

        if (fileLength <= 0)
        {
                printf ("get the file length error...");
                return false;
        }

        // Create a file to save package.
        const string outFileName = Path + fileName;
        FILE *fp = fopen (outFileName.c_str (), "wb");
        if (!fp)
        {
                return false;
        }

        long partSize = fileLength / threadNum;

        for (int i = 0; i <= threadNum; i++)
        {
                tNode *pNode = new tNode ();

                if (i < threadNum)
                {
                        pNode->startPos = i * partSize;
                        pNode->endPos = (i + 1) * partSize - 1;
                }
                else
                {
                        if (fileLength % threadNum != 0)
                        {
                                pNode->startPos = i * partSize;
                                pNode->endPos = fileLength - 1;
                        }
                        else
                                break;
                }

                CURL *curl = curl_easy_init ();

		pNode->curl = curl;
                pNode->fp = fp;

                char range[64] = { 0 };
                snprintf (range, sizeof (range), "%ld-%ld", pNode->startPos, pNode->endPos);

                // Download pacakge
                curl_easy_setopt (curl, CURLOPT_URL, Url.c_str ());
                curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writeFunc);
                curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *) pNode);
                curl_easy_setopt (curl, CURLOPT_NOPROGRESS, 0L);
                curl_easy_setopt (curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
                curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1L);
                curl_easy_setopt (curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
                curl_easy_setopt (curl, CURLOPT_LOW_SPEED_TIME, 5L);
                curl_easy_setopt (curl, CURLOPT_RANGE, range);

                pthread_mutex_lock (&g_mutex);
                thread_num++;
                pthread_mutex_unlock (&g_mutex);
                int rc = pthread_create (&pNode->tid, NULL, workThread,(void *)pNode);
        }

        while (thread_num > 0)
        {
                usleep (1000000L);
        }

        fclose (fp);

        printf ("download succed......\n");
        return true;
}

