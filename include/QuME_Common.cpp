#include "QuME_Common.h"

void ReplaceAll(std::wstring *str, const std::wstring& from, const std::wstring& to)
{
    size_t start_pos = 0;
    while((start_pos = str->find(from, start_pos)) != std::wstring::npos)
	{
        str->replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

wxUint32 copys2ws(wxUint8* in, std::wstring *out)
{
	wxUint32 len = 0;
	if(in == nullptr) return 0;
	if(out == nullptr) return 0;

	while(in[len] != 0)
	{
		len++;
	}

	wchar_t* buffer = new wchar_t[len+1]; //here be a squashed bug...off by one error!

	for(wxUint32 i = 0; in[i] != 0; i++)
	{
		buffer[i] = static_cast<wchar_t>(in[i]);
	}
	buffer[len] = 0; //append null terminator

	*out = std::wstring(buffer);
	return len;
}

