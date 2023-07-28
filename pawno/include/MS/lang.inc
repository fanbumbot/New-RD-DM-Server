#if defined mdg_lang_inc
	#endinput
#endif
#define mdg_lang_inc

#include <MS/file>
#include <MS/extargs>

#if !defined LANG_NONE
	#define LANG_NONE Lang:-1
#endif

/*
	struct:
	    FileNode (the pointer will be written to the variable)
*/

static langtext[2048];
static langtext2[2048];
static langtext3[2048];

static Lang:pllangs[MAX_PLAYERS];
static Lang:defaultlang = LANG_NONE;

stock Lang:langOpen(file[])
{
    return Lang:iniOpen(file, "=", "#");
}

stock langClose(Lang:lang)
{
	return iniClose(ININode:lang);
}

stock langUpdate(Lang:lang)
{
	return iniReopen(ININode:lang, "=", "#");
}

stock langTemplate(Lang:lang, text[], size_text = sizeof(text))
{
	new mark1;
	new mark2;
	while(mark1 == mark1)
	{
		mark1 = strfind(text, "\\v(");// \v(
		if(mark1 == -1)
		{
		    break;
		}
	    mark1++;
	    mark2 = strfind(text, ")", false, mark1);
		if(mark2 == -1)
		{
		    break;
		}
	    strmid(langtext, text, mark1+2, mark2);
	    iniGetString(ININode:lang, langtext, langtext);
	    strmid(langtext2, text, mark2+1, strlen(text));
	    strmid(langtext3, text, 0, max(mark1-1, 0));
	    format(text, size_text, "%s%s%s", langtext3, langtext, langtext2);
	}
	/*
	while(mark1 == mark1)
	{
		mark1 = strfind(text, "\\n");// \n
		if(mark1 == -1)
		{
		    break;
		}
		strmid(text[mark1+1], text, mark1+2, strlen(text)+1);
		text[mark1] = '\n';
	}
	while(mark1 == mark1)
	{
		mark1 = strfind(text, "\\r");// \r
		if(mark1 == -1)
		{
		    break;
		}
		strmid(text[mark1+1], text, mark1+2, strlen(text));
		text[mark1] = '\r';
	}
	while(mark1 == mark1)
	{
		mark1 = strfind(text, "\\t");// \t
		if(mark1 == -1)
		{
		    break;
		}
		strmid(text[mark1+1], text, mark1+2, strlen(text));
		text[mark1] = '\t';
	}
	*/
	
	mark1 = 0;
	mark2 = 0;
	new c = 0;
	new len = strlen(text);
	for( ; mark1 < len; mark1++)
	{
	    if(text[mark1] == '\\')
	    {
	        mark1++;
	        if(mark1 == len)    break;
	        c = text[mark1];
			switch(c)
			{
			    case 'n':
				{
					text[mark2] = '\n';
				}
				case 'r':
				{
					text[mark2] = '\r';
				}
				case 't':
				{
					text[mark2] = '\t';
				}
				default:
				{
					mark1--;
				}
			}
			mark2++;
			continue;
	    }
	    text[mark2] = text[mark1];
	    mark2++;
	}
	mark2 = min(size_text-1, mark2);
	text[mark2] = '\0';
	return 1;
}

stock langIsTextExist(Lang:lang, text[])
{
    return iniIsKeyExist(ININode:lang, text);
}

stock langGet(Lang:lang, text[], out[], out_size = sizeof(out), {_, Float}:...)
{
    if(!iniGetString(ININode:lang, text, out, out_size))
    {
        strmid(out, text, 0, strlen(text), out_size);
        return 1;
    }
    langTemplate(lang, out, out_size);
    argInit(4);
    if(argGetNum() != 0)
    {
		argFormat(out, out_size, out);
	}
	return 1;
}

stock Lang:GetPlayerLang(playerid)
{
	if(!IsPlayerConnected(playerid))    return Lang:LANG_NONE;
	return pllangs[playerid];
}

stock SetPlayerLang(playerid, Lang:lang)
{
    if(!IsPlayerConnected(playerid))    return 0;
	pllangs[playerid] = lang;
	return 1;
}

stock langSetDefault(Lang:lang)
{
    defaultlang = lang;
    return 1;
}

stock Lang:langGetDefault()
{
    return defaultlang;
}

stock langGetPlayer(playerid, text[], out[], out_size = sizeof(out))
{
	if(!IsPlayerConnected(playerid))    return 0;
    langGet(GetPlayerLang(playerid), text, out, out_size);
    return 1;
}

public OnPlayerConnect(playerid)
{
    SetPlayerLang(playerid, defaultlang);
#if defined mdg_lang_OnPlayerConnect
    mdg_lang_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect mdg_lang_OnPlayerConnect
#if defined mdg_lang_OnPlayerConnect
forward mdg_lang_OnPlayerConnect(playerid);
#endif
