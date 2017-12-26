/***************************************************************
 * Name:
 * Purpose:
 * Author:    J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright: J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:   GPL v3
 **************************************************************/

#include "QuME_BSP_Entities.h"

QuME_KeyPair::QuME_KeyPair()
:KeyName(L""), KeyValue(L"")
{
}

QuME_KeyPair::QuME_KeyPair(const std::wstring& inName, const std::wstring& inValue)
:KeyName(inName), KeyValue(inValue)
{
}

QuME_KeyPair::~QuME_KeyPair()
{
}

QuME_KeyPair& QuME_KeyPair::operator=(const QuME_KeyPair& o)
{
	this->KeyName = o.KeyName;
	this->KeyValue = o.KeyValue;

	return *this;
}

enum KEY_PARSE_STATE
{
	KEY_NAME_START,
    KEY_NAME,
    KEY_VAL_START,
    KEY_VAL
};

bool QuME_KeyPair::ParseKeyPair(wxUint8* buffer, wxUint32& currentIndex, wxUint32 length)
{
    bool success = true;
    wxUint32 localIndex = 0;
    wchar_t kn[256]; //temporary buffers
    wchar_t kv[256];
    bool done = false;
    KEY_PARSE_STATE state = KEY_NAME_START;

    while((!done) && (currentIndex < length))
    {
        switch(state)
        {
        case KEY_NAME_START:
            if(buffer[currentIndex] == '\"') //found start of key name, now to parse it
            {
                currentIndex++;
                state = KEY_NAME;
            }
            else if(buffer[currentIndex] <= ' ') //whitespace, just consume it
            {
                currentIndex++;
            }
            else //screwy value found, fuggedaboudit!
			{
				success = false;
				done = true;
			}
            break;

        case KEY_NAME:
            if(buffer[currentIndex] == '\"') //end of key name, now parse key value
            {
                currentIndex++;
                kn[localIndex] = 0; //terminate key name string with null
                localIndex = 0;
                state = KEY_VAL_START;
            }
            else if(localIndex > 255) //string went on too long, error out
            {
                success = false;
                done = true;
            }
            else //copy over key name character to temp buffer
            {
                kn[localIndex++] = buffer[currentIndex++];
            }
            break;

		case KEY_VAL_START:
            if(buffer[currentIndex] == '\"') //found start of key value, now to parse it
            {
                currentIndex++;
                state = KEY_VAL;
            }
            else if(buffer[currentIndex] <= ' ') //whitespace, just consume it
            {
                currentIndex++;
            }
            else //screwy value found, cheese it!
			{
				success = false;
				done = true;
			}
            break;

        case KEY_VAL:
            if(buffer[currentIndex] == '\"') //end of key value, done!
            {
                currentIndex++;
                kv[localIndex] = 0; //terminate key value string with null
                done = true;
            }
            else if(localIndex > 255) //string went on too long, error out
            {
                success = false;
                done = true;
            }
            else //copy key value character over to temp buffer
            {
                kv[localIndex++] = buffer[currentIndex++];
            }
            break;
        }
    }
    if(success)
	{
		this->KeyName = kn;
		this->KeyValue = kv;
	}

    return success;
}

QuME_Entity::QuME_Entity()
{
	this->KeyArray = nullptr;
	this->KeyCount = 0;
}

QuME_Entity::QuME_Entity(const QuME_Entity& o)
{
//    SAFE_ARRAY_DELETE(this->KeyArray);
    this->KeyArray = nullptr;
    this->KeyCount = 0;
    this->Keys.Append(o.Keys);
    this->KeyCount = this->Keys.ToArray(this->KeyArray);
}

QuME_Entity::~QuME_Entity()
{
	KeyCount = 0;
	SAFE_ARRAY_DELETE(KeyArray);
	KeyArray = nullptr;
}

QuME_Entity& QuME_Entity::operator=(const QuME_Entity& o)
{
	SAFE_ARRAY_DELETE(this->KeyArray);
	this->KeyArray = nullptr;
	this->KeyCount = 0;
	this->Keys.Append(o.Keys);
	this->KeyCount = this->Keys.ToArray(this->KeyArray);
	return *this;
}

bool QuME_Entity::ParseEntity(wxUint8* buffer, wxUint32& currentIndex, wxUint32 length)
{
    bool success = true;
    bool done = false;
    QuME_KeyPair kp;

    while((!done) && (currentIndex < length))
    {
    	if(buffer[currentIndex] <= ' ') //eat whitespace
		{
			currentIndex++;
		}
    	else if(buffer[currentIndex] == '}') //end of entity, done
		{
			currentIndex++;
			done = true;
		}
		else if(buffer[currentIndex] == '\"') //another key pair to process
		{
			success = kp.ParseKeyPair(buffer, currentIndex, length);
			if(!success)
			{
				done = true;
			}
			else
			{
				this->Keys.Append(kp);
			}
		}
		else //something weird showed up, abort!
		{
			success = false;
			done = true;
		}
    }
    if(success)
	{
		this->KeyCount = this->Keys.ToArray(this->KeyArray);
		//this->EntityList.Append(CurrentEntity);
	}
    return success;
}

QuME_BSP_Entities::QuME_BSP_Entities()
{
    this->EntityCount = 0;
    this->EntityArray = nullptr;
}

QuME_BSP_Entities::~QuME_BSP_Entities()
{
    SAFE_ARRAY_DELETE(this->EntityArray);
    this->EntityArray = nullptr;
}

enum ENT_PARSE_STATE
{
    START = 0,
    NEW_ENT
};

bool QuME_BSP_Entities::ParseEntities(wxUint8* buffer, wxUint32 length)
{
    bool success = true;
    wxUint32 currentIndex = 0;
    ENT_PARSE_STATE state = START;
    QuME_Entity* CurrentEntity = new QuME_Entity();
    bool done = false;

    //state machine to parse entity key name/value pairs
    while((currentIndex < length) && (!done))
    {
        switch(state)
        {
        case START:
        	if(buffer[currentIndex] <= ' ') //eat whitespace at start
			{
				currentIndex++;
			}
        	else if(buffer[currentIndex] == '{') //start of entities
			{
				currentIndex++;
				state = NEW_ENT;
			}
			else //something not expected was in the buffer, error out
			{
				success = false;
				done = true;
			}
            break;

        case NEW_ENT:
        	success = CurrentEntity->ParseEntity(buffer, currentIndex, length);
            //charsEaten = ParseEntity(&buffer[i], length - i, CurrentEntity);
            if(!success) //error case
            {
                done = true;
            }
            else
            {
                this->EntityList.Append(*CurrentEntity);
                SAFE_DELETE(CurrentEntity);
                CurrentEntity = new QuME_Entity();
                state = START;
            }
            break;
        }
    }
    if(success)
	{
		this->EntityCount = this->EntityList.ToArray(this->EntityArray);
	}
	SAFE_DELETE(CurrentEntity);
	CurrentEntity = nullptr;
    return success;
}

bool QuME_BSP_Entities::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    wxDataInputStream* binData = new wxDataInputStream(*infile);

    if(!binData->IsOk())
    {
        return false;
    }

    if (infile->SeekI(offset, wxFromStart) == wxInvalidOffset)
    {
        return false;
    }

    wxUint8* ent_text_buf = new wxUint8[length];	//temporary buffer to parse from
													//remember, not Unicode in BSP file!

    binData->Read8(ent_text_buf, length);

    if(!binData->IsOk())
    {
        SAFE_ARRAY_DELETE(ent_text_buf);
        return false;
    }

    delete binData;
    bool result = ParseEntities(ent_text_buf, length);

	SAFE_ARRAY_DELETE(ent_text_buf);

    return result;
}

void QuME_Entity::DebugDump(wxTextOutputStream& out)
{
    out << L"Keys: " << this->KeyCount << L"\n";
    for(wxUint32 i = 0; i < this->KeyCount; i++)
    {
        out << this->KeyArray[i].KeyName << L": " << this->KeyArray[i].KeyValue << L"\n";
    }

}

void QuME_BSP_Entities::DebugDump(wxTextOutputStream& out)
{
    out << L"Entities: " << this->EntityCount << L"\n";
    for(wxUint32 i = 0; i < this->EntityCount; i++)
    {
        out << L"Entity #" << i << L"\n";
        this->EntityArray[i].DebugDump(out);
    }
    out << L"\n------------------------------------------------\n";
}
