// SteganoMessage.h: interface for the SteganoMessage class.
//
//////////////////////////////////////////////////////////////////////
#include "Stegano.h"

class SteganoMessage : public Stegano  
{
public:
	bool CheckSize();
	void Decode(BYTE** pdbMessage, DWORD* dwMessSize);
	void Decode();
	void Encipher();
	void SetKey(BYTE* pdbKey, BYTE dbKeySize);
	void SetMessage(BYTE* pdbMessage, DWORD dwSize);
	void SetContainer(BYTE* pdbContainer, DWORD dwSize);
	SteganoMessage();
	virtual ~SteganoMessage();
protected:
	BYTE*	m_pdbContainer;
	DWORD	m_dwContSize;

	BYTE*	m_pdbMessage;
	DWORD	m_dwMessSize;

	BYTE*	m_pdbKey;
	BYTE	m_dbKeySize;
	BYTE	m_dbKeyStep;
};