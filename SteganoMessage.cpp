// SteganoMessage.cpp: implementation of the SteganoMessage class.

#include "stdafx.h"
#include "SteganoMessage.h"
#include <iostream>

using namespace std;

SteganoMessage::SteganoMessage()
{
	
}

SteganoMessage::~SteganoMessage()
{

}

void SteganoMessage::SetContainer(BYTE *pdbContainer, DWORD dwSize)
{
	m_pdbContainer = pdbContainer;
	m_dwContSize = dwSize;
}

void SteganoMessage::SetMessage(BYTE *pdbMessage, DWORD dwSize)
{
	m_pdbMessage = pdbMessage;
	m_dwMessSize = dwSize;
}

bool SteganoMessage::CheckSize()
{
	DWORD dwStepConut = m_dwMessSize * m_dbSize;
	DWORD dwStepNeeded = (dwStepConut * m_dbKeyStep) / m_dbKeySize;
	if(dwStepNeeded < m_dwContSize)
		return true;
	else
		return false;
}

void SteganoMessage::SetKey(BYTE *pdbKey, BYTE dbKeySize)
{
	m_pdbKey = pdbKey;
	m_dbKeySize = dbKeySize;
	m_dbKeyStep = pdbKey[0];

	for(int i = 0;i<dbKeySize;i++)
	{
		if(m_dbKeyStep<pdbKey[i])
			m_dbKeyStep = pdbKey[i];
	}
	m_dbKeyStep++;
}

void SteganoMessage::Encipher()
{

	BYTE dbMask = GetMask();
	Coding(m_pdbMessage[0]);
	DWORD i=0,j=0,l=0,k=0,n=0;
	for(;i<(m_dwMessSize*m_dbSize);i++)
	{							
		if(l == m_dbSize)
		{
			l = 0;						
			n++;
			Coding(m_pdbMessage[n]);				
		}
		
		if(k == m_dbKeySize)
		{
			k = 0;
			j+=m_dbKeyStep;
		}
		m_pdbContainer[j+m_pdbKey[k]]&=dbMask;
		m_pdbContainer[j+m_pdbKey[k]]|=m_pdbBits[l];
		k++;
		l++;
	}
}

void SteganoMessage::Decode()
{	
	static DWORD j=0,l=0,k=0;
	DWORD i=0,m=0;
	m_pdbMessage[m] = 0;
	for(;i<(m_dwMessSize*m_dbSize);i++)
	{					
		if(l == m_dbSize)
		{
			l = 0;						
			m++;
			m_pdbMessage[m] = 0;			
		}
		if(k == m_dbKeySize)
		{
			k = 0;
			j+=m_dbKeyStep;
		}
		Decoding(m_pdbContainer[j+m_pdbKey[k]],&m_pdbMessage[m]);		
		l++;
		k++;
	}
	if(l == m_dbSize)
		l = 0;								
}

void SteganoMessage::Decode(BYTE **pdbMessage, DWORD *dwMessSize)
{
	m_pdbMessage = new BYTE[4];
	m_dwMessSize = 4;
	Decode();
	DWORD *dwTemtSize = (DWORD*) &m_pdbMessage[0];
	m_dwMessSize = *dwTemtSize;
	delete m_pdbMessage;

	*pdbMessage = new BYTE[m_dwMessSize];
	m_pdbMessage = *pdbMessage;
	*dwMessSize = m_dwMessSize;
	Decode();
}

