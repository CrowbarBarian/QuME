/***************************************************************
 * Name:		QuME_UniqueStrings
 * Purpose:		Code to determine if a string has been seen before
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_UniqueStrings.h"

QuME_UniqueStrings::QuME_UniqueStrings()
{
}

QuME_UniqueStrings::~QuME_UniqueStrings()
{
}

bool QuME_UniqueStrings::IsUnique(std::wstring& t)
{
	if(this->UniqueStringsList.isUnique(t))
	{
		this->UniqueStringsList.Add(t);
		return true;
	}
	else
	{
		return false;
	}
}
