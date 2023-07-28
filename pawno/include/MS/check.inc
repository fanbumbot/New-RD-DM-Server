#if defined mdg_check_inc
	#endinput
#endif
#define mdg_check_inc

stock IsDigit(character)
{
	return (character >= '0' && character <= '9');
}

stock IsHexChar(character)
{
    return ((character >= '0' && character <= '9') || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F'));
}

stock IsAlphabet(character)
{
	return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'));
}

stock IsRusAlphabet(character)
{
	return ((character >= 'а' && character <= 'я') || (character >= 'А' && character <= 'Я'));
}

stock IsInteger(string[])
{
	new len = strlen(string);
	for(new i = 0; i < len; i++)
	{
	    if(!IsDigit(string[i])) return 0;
	}
	return 1;
}

stock IsFloat(string[])
{
	new len = strlen(string);
	new mark = strfind(string, ".");
	if(mark == -1 || mark == 0 || mark == (len-1))  return 0;
	for(new i = 0; i < len; i++)
	{
	    if(!IsDigit(string[i])) return 0;
	}
	return 1;
}

stock IsHex(string[])
{
	new len = strlen(string);
	for(new i = 0; i < len; i++)
	{
	    if(!IsHexChar(string[i])) return 0;
	}
	return 1;
}

stock IsInput(string[])
{
	new len = strlen(string);
	new c;
	for(new i = 0; i < len; i++)
	{
		c = string[i];
		if(!IsDigit(c) && !IsAlphabet(c) && c != '_')	return 0;
	}
	return 1;
}

stock IsInputRus(string[])
{
	new len = strlen(string);
	new c;
	for(new i = 0; i < len; i++)
	{
		c = string[i];
		if(!IsDigit(c) && !IsAlphabet(c) && !IsRusAlphabet(c) && c != '_')	return 0;
	}
	return 1;
}

