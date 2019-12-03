#include "MyString.h"

int StrLen(const char *pStr)
{
	if(NULL == pStr)
	{
		return -1;
	}

	int nLen = 0;

	while('\0' != pStr[nLen])
	{
		nLen++;
	}

	return nLen;
}

bool StrCmp(const char *pDest,const char *pStr)
{
	if(NULL != pDest && NULL != pStr)
	{
		int i = 0;

		for(i = 0;pDest[i] == pStr[i];i++)
		{
			if('\0' == pDest[i])
			{
				return true;
			}
		}
	}

	return false;
}

char* StrCpy(char *pDest,const char *pStr)
{
	if(NULL == pDest || NULL == pStr)
	{
		return NULL;
	}

	if(StrLen(pDest) < StrLen(pStr))
	{
		return NULL;
	}

	int i = 0;
	
	for(i = StrLen(pStr);i >= 0;i--)
	{
		pDest[i] = pStr[i];
	}

	return pDest;
}

char* StrCat(char *pDest,const char *pStr)
{
	if(NULL == pDest || NULL == pStr)
	{
		return NULL;
	}

	int i = 0;
	int j = 0;

	for(i = 0;'\0' != pDest[i];i++)
		;

	for(j = 0;'\0' != (pDest[i] = pStr[j]);i++,j++);
	
	return pDest;
}
