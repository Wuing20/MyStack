
#include "MyString.h"

using namespace std;

int main()
{
	char szStr[30] = "abcd";
	char szBuf[20] = "ghjklmnopq";

	cout<<StrLen(szStr)<<" : "<<szStr<<endl;
	cout<<StrLen(szBuf)<<" : "<<szBuf<<endl;

	cout<<StrCat(szStr,szBuf)<<endl;
	cout<<StrCpy(szStr,szBuf)<<endl;

	if(StrCmp(szStr,szBuf))
	{
		cout<<szBuf<<endl;
	}

	return 0;
}
