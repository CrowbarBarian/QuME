#include "QuME_BSP_Entities.h"

QuME_BSP_Entities::QuME_BSP_Entities()
{
    this->Count = 0;
    this->EntList = nullptr;
    this->Entity = nullptr;
}

QuME_BSP_Entities::~QuME_BSP_Entities()
{
    for(QuME_BSP_EntList* i = this->EntList; i != nullptr;) //delete entities
    {
        QuME_BSP_EntList* t = i->next;
        delete i;
        i = t;
    }
    delete[] this->Entity;
}

QuME_BSP_EntList* QuME_BSP_Entities::createNewEnt(wxUint32 index)
{
    if (this->EntList == nullptr) //list is empty, initialize first entry
    {
        this->EntList = new QuME_BSP_EntList;
        this->EntList->next = nullptr;
        this->EntList->entity.index = index;
        this->Count++;
        return EntList;
    }
    else
    {
        QuME_BSP_EntList* t = this->EntList;
        while(t->next != nullptr) //find end of list
        {
            t = t->next;
        }
        t->next = new QuME_BSP_EntList; //found it, create new entity
        this->Count++;
        t = t->next;
        t->next = nullptr;
        t->entity.index = index;
        return t;
    }
}

QuME_BSP_Entity* QuME_BSP_Entities::GetEntity(wxUint32 index)
{
    for(QuME_BSP_EntList* t = this->EntList; t != nullptr; t = t->next)
    {
        if(t->entity.GetIndex() == index)
            return &t->entity;
    }
    return nullptr;
}


enum STRING_PARSE_STATE
{
    STR_START = 0,
    CHAR_DATA,
    BAD_STR,
    END_STR,
    STR_DONE
};


wxInt32 readStringFromBuffer(wxUint8* buffer, wxUint32 offset, wxUint32 length, wxUint8** out)
{
    wxUint32 i = offset;

    if(i >= length)
    {
        return 0;
    }
    if(buffer[i] == '\"')
    {
        i++; //skip over starting quote
    }
    STRING_PARSE_STATE sps = STR_START;

    while((i < length) && (sps != STR_DONE))
    {
        switch(sps)
        {
            case STR_START:

                if(i < length)
                {
                    *out = &buffer[i];
                    i++;
                    sps = CHAR_DATA;
                }
                else sps = BAD_STR;
                break;

            case CHAR_DATA:

                if(i < length)
                {
                    if(buffer[i] == '\"')
                    {
                        sps = END_STR;
                    }
                    else
                    {
                        i++;
                    }
                }
                else sps = BAD_STR;
                break;

            case END_STR:

                if(i < length)
                {
                    buffer[i] = 0; //append terminating null
                    i++;
                    sps = STR_DONE;
                }
                else sps = BAD_STR;
                break;

            case BAD_STR: //log this?? make error report??

                sps = STR_DONE;
                break;

            case STR_DONE: break;
        }
    }

    return i-offset;
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

    wxUint8 *ent_text_buf = new wxUint8[length]; //temporary buffer to parse from
    //remember, not Unicode in BSP file!

    binData->Read8(ent_text_buf, length);

    if(!binData->IsOk())
    {
        delete[] ent_text_buf;
        return false;
    }

    delete binData;

    ENT_PARSE_STATE state = ENT_START;

    wxUint32 new_index = 0;
    wxUint32 i = 0;

    wxUint8* keyNameStart = nullptr;
    wxUint8* keyValStart = nullptr;

    std::wstring keyName;
    std::wstring keyVal;

    QuME_BSP_EntList* e = nullptr;

    while((i < length) && (state != END_ENTS))
    {
        switch(state)
        {
            //eat whitespace at start
            case ENT_START:

                while((i < length) && (ent_text_buf[i] < ' ')) i++;
                state = LOOKING_FOR_ENT;
                break;

            //search for the next entity in the buffer
            case LOOKING_FOR_ENT:

                while((i < length) && (ent_text_buf[i] != '{')) i++;
                if(i >= length) state = END_ENTS; //read past the end of the buffer
                else state = NEW_ENT;
                break;

            case NEW_ENT:

                e = this->createNewEnt(new_index); //found an entity, make an entry in memory
                new_index++;
                i++;
                state = KEY_NAME;
                break;

            case KEY_NAME:

                while((i < length) && (ent_text_buf[i] != '\"') && (ent_text_buf[i] != '}')) i++;
                if(ent_text_buf[i] == '}')
                {
                    state = END_ENT;
                    i++;
                }
                else
                {
                    i += readStringFromBuffer(ent_text_buf, i, length, &keyNameStart);

                    if(keyNameStart == nullptr)
                    {
                        state = BAD_ENTS;
                    }
                    else
                    {

                        state = KEY_VAL;
                    }
                }
                break;

            case KEY_VAL:

                while((i < length) && (ent_text_buf[i] != '\"') && (ent_text_buf[i] != '}')) i++;
                if(ent_text_buf[i] == '}') state = BAD_ENTS;

                i += readStringFromBuffer(ent_text_buf, i, length, &keyValStart);
                if(keyValStart == nullptr)
                {
                    state = BAD_ENTS;
                }
                else
                {
                    state = KEY_NAME;
                }
                //arg, there has to be a better way!
                copys2ws(keyNameStart, &keyName);

                copys2ws(keyValStart, &keyVal);

                e->entity.addKey(keyName, keyVal);
                break;

            case END_ENT:

                state = LOOKING_FOR_ENT;
                break;

            case END_ENTS: //all done!

                break;

            case BAD_ENTS:

                delete[] ent_text_buf; //clean up
                return false;
                break;
        }
    }

    //convert linked list into array, for speed
    this->Entity = new QuME_BSP_Entity[this->Count];
    wxUint32 counter = 0;
    for(QuME_BSP_EntList* i = this->EntList; i != nullptr; i = i->next)
    {
        this->Entity[counter++] = i->entity;
    }
    delete[] ent_text_buf; //clean up
    return true;
}

void QuME_BSP_Entities::DebugDump(wxTextOutputStream& out)
{
    out << L"Entities: " << this->Count << L"\n";
    for(QuME_BSP_EntList* t = this->EntList; t != nullptr; t = t->next)
    {
        t->entity.DebugDump(out);
    }
    out << L"\n------------------------------------------------\n";
}
