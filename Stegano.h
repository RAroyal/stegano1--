// Stegano.h: interface for the Stegano class.
//
//////////////////////////////////////////////////////////////////////
#include "windows.h"
class Stegano  
{
public:
	BYTE GetMask();
	BYTE GetBitsSize();
	void Coding(BYTE dbByte2Code);
	bool Decoding(BYTE dbByte2Decode, BYTE* dbMess);
	void SetCode(BYTE dbBit);
	Stegano();
	virtual ~Stegano();
protected:	
	BYTE	m_dbBit;
	BYTE*	m_pdbBits;
	BYTE	m_dbSize;
	BYTE	m_dbMask;
};

