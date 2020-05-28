#include "pch.h"
#include <string>
using namespace std;


namespace luteplayer
{
	//[DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
	//static extern int RegisterWindowMessage(string lpString);
	int _messageId;

	void init()
	{
		_messageId = RegisterWindowMessage(L"MyUniqueMessageIdentifier");

	}
	void update()
	{
		_messageId = RegisterWindowMessage(L"MyUniqueMessageIdentifier");

	}
};