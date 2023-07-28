#if defined mdg_color_inc
	#endinput
#endif
#define mdg_color_inc

#include <MS/check>

enum RGB
{
	R = 0xFF0000,
	G = 0x00FF00,
	B = 0x0000FF
}

enum RGBA
{
	R = 0xFF000000,
	G = 0x00FF0000,
	B = 0x0000FF00,
	A = 0x000000FF
}

stock RGB:operator=(RGBA:oper1)
{
	return RGB:((_:oper1 >>> 8) & 0xFFFFFF);
}
stock RGBA:operator=(RGB:oper1)
{
	return RGBA:(((_:oper1 & 0xFFFFFF) << 8)|0xFF);
}
stock RGB:operator=(oper1)
{
	return RGB:oper1;
}
stock RGBA:operator=(oper1)
{
	return RGBA:oper1;
}
stock _:operator=(RGB:oper1)
{
	return _:oper1;
}
stock _:operator=(RGBA:oper1)
{
	return _:oper1;
}

stock IsHexRGB(string[])
{
	new len = strlen(string);
	if(len > 6)     return 0;
	for(new i = 0; i < len; i++)
	{
	    if(!IsHexChar(string[i]))   return 0;
	}
	return 1;
}

stock IsHexRGBA(string[])
{
	new len = strlen(string);
	if(len > 8)     return 0;
	for(new i = 0; i < len; i++)
	{
	    if(!IsHexChar(string[i]))   return 0;
	}
	return 1;
}

stock HexCharToInt(character)
{
	if(character >= 'a' && character <= 'z')
	{
     	return character-'a'+10;
	}
	else if(character >= 'A' && character <= 'Z')
	{
	    return character-'A'+10;
	}
	else if(character >= '0' && character <= '9')
	{
        return character-'0';
	}
	return -1;
}

stock IntToHexChar(value)
{
	if(value >= 16 || value < 0) return -1;
	if(value >= 10) return value-10+'A';
	return value+'0';
}

stock HexToRGB(string[], &RGB:color)
{
	static string2[7];
	new c;
	strCut(string, string2);
	new len = strlen(string2);
	if(len != 6)    return 0;
	for(new i = 0; i < len; i++)
	{
	    c = HexCharToInt(string2[len-i-1]);
	    if(c == -1) return 0;
	    color = RGB:(_:color | (c << (i << 2)));
	}
	return 1;
}

stock HexToRGBA(string[], &RGBA:color)
{
	static string2[7];
	new c;
	strCut(string, string2);
	new len = strlen(string2);
	if(len != 8)    return 0;
	for(new i = 0; i < len; i++)
	{
	    c = HexCharToInt(string2[len-i-1]);
	    if(c == -1) return 0;
	    color = RGB:(_:color | (c << (i << 2)));
	}
	return 1;
}

stock RGBToHex(RGB:color, string[], string_s = sizeof(string))
{
	new start_check = 0;
	new l;
	new i = 0;
	for( ; i < 6 && i < string_s-1; i++)
	{
	    l = IntToHexChar((_:color >> 20) & 0xF);
	    color = RGB:(_:color << 4);
	    if(l == '0' && start_check == 0)  continue;
		if(l == -1)
		{
		    string[i] = '\0';
		    return 0;
		}
		start_check = 1;
		string[i] = l;
	}
	new len = min(i+1, string_s-1);
	if(start_check == 0)
	{
	    string[len] = '0';
	}
	len = min(i+2, string_s-1);
	string[len] = '\0';
	return 1;
}

stock RGBAToHex(RGBA:color, string[], string_s = sizeof(string))
{
	new start_check = 0;
	new l;
	for(new i = 0; i < 8 && i < string_s-1; i++)
	{
	    color = RGBA:(_:color << 4);
	    l = IntToHexChar(_:color & 0xF);
	    if(l == 0 && start_check == 0)  continue;
		if(l == -1)
		{
		    string[i] = '\0';
		    return 0;
		}
		start_check = 1;
		string[i] = l;
	}
	if(start_check == 0)
	{
	    string[i] = '0';
	}
	string[i] = '\0';
	return 1;
}

stock GetColorLightness({_, RGB, RGBA}:color, gettag = tagof(color))
{
	if(gettag == tagof(RGBA))
	{
	    color >>= 8;
	}
	return (color & 0xFF) + ((color >> 8) & 0xFF) + ((color >> 16) & 0xFF);
}

stock IsColorDark({_, RGB, RGBA}:color, gettag = tagof(color))
{
	return GetColorLightness(color, gettag) < 0xC0;
}
