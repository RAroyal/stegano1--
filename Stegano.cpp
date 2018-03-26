// Stegano.cpp: implementation of the Stegano class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stegano.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Stegano::Stegano()
{
	m_pdbBits = NULL;
}

Stegano::~Stegano()
{
	if (m_pdbBits)
		delete m_pdbBits;
}

void Stegano::SetCode(BYTE dbBit)
{
	m_dbBit = dbBit;	
	m_dbSize = 8/dbBit;
	if(8 % dbBit)
		m_dbSize++;
	m_pdbBits = new BYTE[m_dbSize];
	m_dbMask = (0xFF>>m_dbBit)<<m_dbBit;
}

void Stegano::Coding(BYTE dbByte2Code)
{			
	for(BYTE i = 0,j=0; i<8;i+=m_dbBit,j++)
	{
		m_pdbBits[j] = dbByte2Code;
		m_pdbBits[j] <<= (8 - m_dbBit);
		m_pdbBits[j] >>= (8 - m_dbBit);
		dbByte2Code >>=	m_dbBit;			
	}	
}

bool Stegano::Decoding(BYTE dbByte2Decode, BYTE* dbMess)
{	
	
	static BYTE i = 0;
	BYTE dbShift = m_dbBit;
	if(m_dbBit>(8-i))
		dbShift = 8-i;
	dbByte2Decode <<= (8 - dbShift);
	*dbMess >>= dbShift;
	*dbMess+=dbByte2Decode;	
	i+=dbShift;	
	if(i==8)
	{
		i = 0;
		return true;
	}
	return false;
}

BYTE Stegano::GetBitsSize()
{
	return m_dbSize;
}

BYTE Stegano::GetMask()
{
	return m_dbMask;
}
