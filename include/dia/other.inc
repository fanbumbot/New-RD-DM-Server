#if defined inc_dia_other
	#endinput
#endif
#define inc_dia_other

static radiostreams[][] =
{
	"http://ep128server.streamr.ru:8030/ep128",
	"http://ice2.somafm.com/dubstep-128-mp3"
};

dialogShowStandart(HelpCMD, DIALOG_STYLE_MSGBOX);

dialogShowStandart(RulesCMD, DIALOG_STYLE_MSGBOX);
dialogResponse(RulesCMD)
{
	if(!response)
	{
	    ShowDialog(playerid, Dialog:GMenuRules);
	}
	return 1;
}

dialogShow(ChoiseLanguage)
{
	new Lang:lang = Lang:PlayerGetInt(playerid, "Lang");
	if(lang == LANG_NONE)
	{
	    PlayerSetInt(playerid, "Lang", _:Lang[RU]);
	    SetPlayerLang(playerid, Lang[RU]);
	}
	dialogSetStyle(DIALOG_STYLE_LIST);
	return 1;
}

dialogResponse(ChoiseLanguage)
{
	if(response)
	{
	    new Lang:lang;
	    switch(listitem)
	    {
	        case 0:     lang = Lang[RU];
	        case 1:     lang = Lang[EN];
	    }
	    PlayerSetInt(playerid, "Lang", _:lang);
	    SetPlayerLang(playerid, lang);
		ShowDialogAgain();
	}
	else
	{
	    if(!IsPlayerRegistered(playerid))
	    {
	        ShowDialog(playerid, Dialog:Registration);
	    }
	    else
	    {
	        if(GetPlayerWorld(playerid) == worldGetByName("Reglog"))
	        {
	        	ShowDialog(playerid, Dialog:Login);
			}
		}
	}
	return 1;
}

dialogShowStandart(Radio, DIALOG_STYLE_LIST);
dialogResponse(Radio)
{
	if(response)
	{
	    if(listitem == 0)
	    {
	        SendMessage(playerid, "mRadioOff");
			StopAudioStreamForPlayer(playerid);
	    }
	    else if(listitem == 1)
	    {
	        ShowDialog(playerid, Dialog:RadioSelect);
	    }
	    else
	    {
			if((listitem-2) < 0 || (listitem-2) >= sizeof(radiostreams))
			{
				SendMessage(playerid, "wRadioCant");
			    return 1;
			}
  			PlayAudioStreamForPlayer(playerid, radiostreams[listitem-2]);
  			SendMessage(playerid, "mRadioOn", inputtext);
		}
	}
	return 1;
}

dialogShowStandart(RadioSelect, DIALOG_STYLE_INPUT);
dialogResponse(RadioSelect)
{
	if(response)
	{
	    PlayAudioStreamForPlayer(playerid, inputtext);
	    ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:Radio);
	}
	return 1;
}

dialogShow(GMenuStats)
{
	new doplistitem = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
    dialogFormatCaption(PlayerName(doplistitem));

	static date[64];
	static gangname[MAX_GANG_NAME_LEN];
	static ganghead[MAX_PLAYER_NAME];
	new color;
	new money, skin, minutes, vip, admin, mute, mutesec, prison, prisonsec, kills, deaths;
	new gangid;

    new ORMCache:cache = ORMCache:itemGetInt(itemGetByID(Player, doplistitem), "Cache");
    ormCacheGet(cache, "DateReg", date);
    ormCacheGetInt(cache, "Money", money);
    ormCacheGetInt(cache, "Skin", skin);
    ormCacheGetInt(cache, "PlayTime", minutes);
    ormCacheGetInt(cache, "VIP", vip);
    ormCacheGetInt(cache, "Admin", admin);
    ormCacheGetInt(cache, "Mute", mute);
    ormCacheGetInt(cache, "MuteSec", mutesec);
    ormCacheGetInt(cache, "Prison", prison);
    ormCacheGetInt(cache, "PrisonSec", prisonsec);
    ormCacheGetInt(cache, "Kills", kills);
    ormCacheGetInt(cache, "Deaths", deaths);
    ormCacheGetInt(cache, "GangID", gangid);

    dialogFormatInfo(ormCacheID(cache), date, money, skin, minutes, vip, admin, mute, mutesec, prison, prisonsec, kills, deaths);

	if(IsGangExist(gangid))
	{
	    cache = ORMCache:itemGetInt(itemGetByID(Gang, gangid), "Cache");
	    ormCacheGet(cache, "Name", gangname);
	    ormCacheGetInt(cache, "Color", color);
	    ormCacheGet(cache, "Head", ganghead);
	    dialogFormatAppendNewLine("d2GMenuStats", (color >>> 8), gangname, ganghead);
	}

	return 1;
}




