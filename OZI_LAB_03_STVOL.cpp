// OZI_LAB_03_STVOL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SteganoMessage.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	HANDLE hFile, hFileMess, hFileCont;
	BYTE *pdbFileByte, *pdbMessByte;
	
	const BYTE dbKeySize = 8;
	//BYTE dbKey[dbKeySize]={4,16,0,8,24,20,28,32};	
	BYTE dbKey[dbKeySize]={1,2,7,3,0,5,4,6};	
	//BYTE dbKey[dbKeySize]={0,1,2,3,4,5,6,7};	

	DWORD dwMessSize,dwFileSize,dwRealFile,dwRealMess;
	DWORD *dwOffsetPict,*dwPictSize;

	hFile = CreateFile(argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	dwFileSize = GetFileSize(hFile,NULL);
	pdbFileByte = new BYTE[dwFileSize];
	ReadFile(hFile,pdbFileByte,dwFileSize,&dwRealFile,NULL);
	
	dwOffsetPict =(DWORD*) &pdbFileByte[10];
	dwPictSize = (DWORD*) &pdbFileByte[34];

	SteganoMessage sm;
	int  bBitsCount = 7;
	do
	{
		cout<<"Bits: ";
		cin>>bBitsCount;
	} while( bBitsCount > 8 );
	sm.SetCode(bBitsCount);
	sm.SetContainer(&pdbFileByte[*dwOffsetPict],*dwPictSize);		
	sm.SetKey(dbKey,dbKeySize);
	
	if(argc >= 3)
	{
		hFileMess = CreateFile(argv[2],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	
		char cFileContName[256];
		strcpy(cFileContName,argv[1]);
		sprintf(&cFileContName[strlen(argv[1])-4],"_bits%d.bmp\0",bBitsCount);

		hFileCont = CreateFile(cFileContName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);	
		BYTE dbFileNameLength = strlen(argv[2]);		
		for(int i = dbFileNameLength; (i>0) && (argv[2][i]!=0x5C);i--);		
		i++;
		dbFileNameLength-=(i-1);
		DWORD dwMessFileSize = GetFileSize(hFileMess,NULL);
		dwMessSize = dwMessFileSize + 4 + dbFileNameLength;
		pdbMessByte = new BYTE[dwMessSize];		
		DWORD dwTempSize = dwMessSize - 4;
		strcpy((char*)&pdbMessByte[4],&argv[2][i]);
		for(i=0;i<4;i++)
		{
			pdbMessByte[i] = (BYTE) dwTempSize;
			dwTempSize >>= 8;
		}				
		ReadFile(hFileMess,&pdbMessByte[4+dbFileNameLength],dwMessFileSize,&dwRealMess,NULL);		
		sm.SetMessage(pdbMessByte,dwMessSize);
		if(sm.CheckSize())
		{
			cout<<"Start coding"<<endl;
			sm.Encipher();		
		}
		else
		{
			cout<<"Message is too large"<<endl;
			cin.get();
		}
		
		WriteFile(hFileCont,pdbFileByte,dwFileSize,&dwRealFile,NULL);	
		CloseHandle(hFileCont);
	}
	else
	{		
		sm.Decode(&pdbMessByte,&dwMessSize);		
		for(int i = 0;(i<dwMessSize) && (pdbMessByte[i]!=0x00);i++);
		i++;
		hFileMess = CreateFile((char*)pdbMessByte,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
		WriteFile(hFileMess,&pdbMessByte[i],dwMessSize-i,&dwRealMess,NULL);		

	}
	CloseHandle(hFile);
	CloseHandle(hFileMess);		
	delete pdbMessByte;
	delete pdbFileByte;	
	return 0;
}
//c:\test.bmp c:\text.txt
//c:\res.bmp

