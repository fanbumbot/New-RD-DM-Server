#if defined mdg_dia_inc
	#endinput
#endif
#define mdg_dia_inc

#include <MS/lang>

#if !defined DIALOG_CAPTION_PREFIX
	#define DIALOG_CAPTION_PREFIX #c
#endif

#if !defined DIALOG_INFO_PREFIX
	#define DIALOG_INFO_PREFIX #d
#endif

#if !defined DIALOG_BUTTON1_PREFIX
	#define DIALOG_BUTTON1_PREFIX #b1
#endif

#if !defined DIALOG_BUTTON2_PREFIX
	#define DIALOG_BUTTON2_PREFIX #b2
#endif

#if !defined MAX_DIALOG_NAME
	#define MAX_DIALOG_NAME 64
#endif

#if !defined MAX_DIALOG_CAPTION_SIZE
	#define MAX_DIALOG_CAPTION_SIZE 64
#endif

#if !defined MAX_DIALOG_SIZE
	#define MAX_DIALOG_SIZE 4096
#endif

#if !defined MAX_DIALOG_BUTTON_SIZE
	#define MAX_DIALOG_BUTTON_SIZE 64
#endif

#if !defined USED_DIALOG_ID
	#define USED_DIALOG_ID 2
#endif

#define Dialog: #

#define dialogShow(%0) \
	forward ds_%0(playerid); \
	public ds_%0(playerid)
	
#define dialogResponse(%0) \
	forward dr_%0(playerid, response, listitem, inputtext[]); \
	public dr_%0(playerid, response, listitem, inputtext[])
	
#define dialogInterrupt(%0) \
	forward di_%0(playerid); \
	public di_%0(playerid)

#define dialogShowStandart(%0,%1); \
	dialogShow(%0) \
	{ \
	    dialogSetStyle(%1); \
		return 1; \
	}

#define ShowDialogAgain(); ShowDialogAgainA(playerid);

forward OnDialogCheat(playerid, dialogid);
forward OnDialogWrong(dialog[]);
forward OnPlayerDialogShow(playerid, dialog[]);

static dialogbuff[MAX_DIALOG_SIZE];

static dialangcaption[MAX_DIALOG_CAPTION_SIZE];
static dialanginfo[MAX_DIALOG_SIZE];
static dialangbut1[MAX_DIALOG_BUTTON_SIZE];
static dialangbut2[MAX_DIALOG_BUTTON_SIZE];
static diastyle;

static curdia[MAX_PLAYERS][MAX_DIALOG_NAME];
static Lang:curlang;
static List:diatextlist;

stock IsPlayerOnDialog(playerid)
{
	return (!IsPlayerConnected(playerid) || strlen(curdia[playerid]) == 0) ? 0 : 1;
}

stock GetPlayerDialog(playerid, dialog[], size = sizeof(dialog))
{
    if(!IsPlayerConnected(playerid))    return 0;
	strmid(dialog, curdia[playerid], 0, strlen(curdia[playerid]), size);
	return 1;
}

stock ShowDialog(playerid, dialog[], ignorelang = false)
{
	if(!IsPlayerConnected(playerid))    return 0;
	
	if(strlen(curdia[playerid]) && strcmp(dialog, curdia[playerid]))
	{
	    format(dialogbuff, sizeof(dialogbuff), "di_%s", curdia[playerid]);
	    CallLocalFunction(dialogbuff, "i", playerid);
	}
	
	if(!ignorelang)
	{
		format(dialangcaption, sizeof(dialangcaption), "%s%s", DIALOG_CAPTION_PREFIX, dialog);
		langGetPlayer(playerid, dialangcaption, dialangcaption);
		format(dialanginfo, sizeof(dialanginfo), "%s%s", DIALOG_INFO_PREFIX, dialog);
		langGetPlayer(playerid, dialanginfo, dialanginfo);
		format(dialangbut1, sizeof(dialangbut1), "%s%s", DIALOG_BUTTON1_PREFIX, dialog);
		langGetPlayer(playerid, dialangbut1, dialangbut1);
		format(dialangbut2, sizeof(dialangbut2), "%s%s", DIALOG_BUTTON2_PREFIX, dialog);
		langGetPlayer(playerid, dialangbut2, dialangbut2);
	}
    diastyle = DIALOG_STYLE_MSGBOX;
    curlang = GetPlayerLang(playerid);

	if(CallLocalFunction("OnPlayerDialogShow", "is", playerid, dialog) || funcidx("OnPlayerDialogShow") == -1)
	{
		format(dialogbuff, sizeof(dialogbuff), "ds_%s", dialog);
		if(!CallLocalFunction(dialogbuff, "i", playerid))
		{
	     	curdia[playerid][0] = '\0';
		    CallLocalFunction("OnDialogWrong", "s", dialog);
		}
		else
		{
		    strmid(curdia[playerid], dialog, 0, strlen(dialog), MAX_DIALOG_NAME);
		    ShowPlayerDialog(playerid, USED_DIALOG_ID, diastyle, dialangcaption, dialanginfo, dialangbut1, dialangbut2);
		}
	}
	return 1;
}

stock dialogSetStyle(style)
{
    diastyle = style;
	return 1;
}

stock dialogGetStyle(style)
{
    diastyle = style;
	return 1;
}

stock dialogSetCaption(caption[])
{
    strmid(dialangcaption, caption, 0, strlen(caption), sizeof(dialangcaption));
    return 1;
}

stock dialogGetCaption(caption[], caption_size = sizeof(caption))
{
    strmid(caption, dialangcaption, 0, strlen(dialangcaption), caption_size);
    return 1;
}

stock dialogSetInfo(info[])
{
    strmid(dialanginfo, info, 0, strlen(info), sizeof(dialanginfo));
    return 1;
}

stock dialogGetInfo(info[], info_size = sizeof(info))
{
    strmid(info, dialanginfo, 0, strlen(dialanginfo), info_size);
    return 1;
}

stock dialogSetButton1(button[])
{
    strmid(dialangbut1, button, 0, strlen(button), sizeof(dialangbut1));
    return 1;
}

stock dialogGetButton1(button[], button_size = sizeof(button))
{
    strmid(button, dialangbut1, 0, strlen(dialangbut1), button_size);
    return 1;
}

stock dialogSetButton2(button[])
{
    strmid(dialangbut2, button, 0, strlen(button), sizeof(dialangbut2));
    return 1;
}

stock dialogGetButton2(button[], button_size = sizeof(button))
{
    strmid(button, dialangbut2, 0, strlen(dialangbut2), button_size);
    return 1;
}

stock dialogFormatCaption({_, Float}:...)
{
	argInit(0);
	argFormat(dialangcaption, sizeof(dialangcaption), dialangcaption);
	return 1;
}

stock dialogFormatInfo({_, Float}:...)
{
	argInit(0);
	argFormat(dialanginfo, sizeof(dialanginfo), dialanginfo);
	return 1;
}

stock dialogFormatButton1({_, Float}:...)
{
	argInit(0);
	argFormat(dialangbut1, sizeof(dialangbut1), dialangbut1);
	return 1;
}

stock dialogFormatButton2({_, Float}:...)
{
	argInit(0);
	argFormat(dialangbut2, sizeof(dialangbut2), dialangbut2);
	return 1;
}

stock dialogAppend(langtext[])
{
    langGet(curlang, langtext, dialogbuff);
	return strcat(dialanginfo, dialogbuff);
}

stock dialogAppendNewLine(langtext[])
{
    langGet(curlang, langtext, dialogbuff);
    strcat(dialanginfo, "\n");
	return strcat(dialanginfo, dialogbuff);
}

stock dialogFormatAppend(langtext[], {_, Float}:...)
{
	if(!strlen(langtext))   return 0;
    argInit(1);
    langGet(curlang, langtext, dialogbuff);
    argFormat(dialogbuff, sizeof(dialogbuff), dialogbuff);
	strcat(dialanginfo, dialogbuff);
	return 1;
}

stock dialogFormatAppendNewLine(langtext[], {_, Float}:...)
{
	if(!strlen(langtext))   return 0;
    langGet(curlang, langtext, dialogbuff);
    argInit(1);
    argFormat(dialogbuff, sizeof(dialogbuff), dialogbuff);
    strcat(dialanginfo, "\n");
	strcat(dialanginfo, dialogbuff);
	return 1;
}

stock dialogClose(playerid)
{
    curdia[playerid][0] = '\0';
	ShowPlayerDialog(playerid, -1, 0, "", "", "", "");
	return 1;
}

stock dialogInputInit(num=-1)
{
    diatextlist = strtokMarker(dialogbuff, num);
	return 1;
}

stock dialogInputGet(num, out[], out_size = sizeof(out))
{
    return strSeparator(dialogbuff, out, num, diatextlist, false, out_size);
}

stock dialogInputGetInt(num)
{
    strSeparator(dialogbuff, dialogbuff, num, diatextlist, false, sizeof(dialogbuff));
    return strval(dialogbuff);
}

stock Float:dialogInputGetFloat(num)
{
    strSeparator(dialogbuff, dialogbuff, num, diatextlist, false, sizeof(dialogbuff));
    return floatstr(dialogbuff);
}

stock dialogInputGetNum()
{
	return listLen(diatextlist)/2;
}

stock ShowDialogAgainA(playerid)
{
	ShowDialog(playerid, curdia[playerid]);
	return 1;
}

public OnPlayerConnect(playerid)
{
	curdia[playerid][0] = '\0';
#if defined mdg_dia_OnPlayerConnect
    mdg_dia_OnPlayerConnect(playerid);
#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect mdg_dia_OnPlayerConnect
#if defined mdg_dia_OnPlayerConnect
forward mdg_dia_OnPlayerConnect(playerid);
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(!strlen(curdia[playerid]) || dialogid != USED_DIALOG_ID)
	{
		CallLocalFunction("OnDialogCheat", "ii", playerid, dialogid);
	    return 1;
	}
    format(dialogbuff, sizeof(dialogbuff), "dr_%s", curdia[playerid]);
    diatextlist = listCreate();
    if(inputtext[0] == '\0')
    {
        new string[2];
        string[0] = '\1';
        string[1] = '\0';
	    if(CallLocalFunction(dialogbuff, "iiis", playerid, response, listitem, string))
		{
		    listDestroy(diatextlist);
		    return 1;
		}
    }
    else
    {
	    if(CallLocalFunction(dialogbuff, "iiis", playerid, response, listitem, inputtext))
		{
		    listDestroy(diatextlist);
		    return 1;
		}
	}
	listDestroy(diatextlist);
#if defined mdg_dia_OnDialogResponse
    mdg_dia_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
#endif
	return 0;
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse mdg_dia_OnDialogResponse
#if defined mdg_dia_OnDialogResponse
forward mdg_dia_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

stock mdg_dia_strlen(const string[])
{
	if (string[0] == '\1')	return 0;
	return strlen(string);
}
#if defined _ALS_strlen
	#undef strlen
#else
	#define _ALS_strlen
#endif

#define strlen mdg_dia_strlen


