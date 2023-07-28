#if defined inc_dia_gangmenu
	#endinput
#endif
#define inc_dia_gangmenu

dialogShowStandart(GangMenu, DIALOG_STYLE_LIST);
dialogResponse(GangMenu)
{
	if(response)
	{
	    if(listitem >= 1 && listitem <= 13)
	    {
		    if(!IsGangExist(GetPlayerGang(playerid)))
		    {
		        ShowDialogAgain();
		        SendMessage(playerid, "wGangMenu1");
		        return 1;
		    }
	    }
		new Item:player = itemGetByID(Player, playerid);
	    itemSetInt(player, "doplistitem", GetPlayerGang(playerid));
		switch(listitem)
		{
		    case 0:
			{
			    if(IsGangExist(GetPlayerGang(playerid)))
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu2");
			        return 1;
			    }
			    if(GetPlayerServerMoney(playerid) < 1000000)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu3");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuCreate);
			}
		    case 1:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuDelete);
			}
		    case 2:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuSkins);
			}
		    case 3:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuSpawn);
			}
		    case 4:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuSpawnCancel);
			}
		    case 5:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuRang);
			}
		    case 6:
			{
			    if(GetPlayerGangLevel(playerid) < 4)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu5");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuInvite);
			}
		    case 7:
			{
			    if(GetPlayerGangLevel(playerid) < 5)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu6");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuKick);
			}
		    case 8:
		    {
			    if(IsGangExist(GetPlayerGang(playerid)))
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu7");
			        return 1;
			    }
			    PlayerSetInt(playerid, "GangInvite", !PlayerGetInt(playerid, "GangInvite"));
		    }
		    case 9:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuType);
			}
		    case 10:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuName);
			}
		    case 11:
			{
			    if(GetPlayerGangLevel(playerid) < 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu4");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuColor);
			}
		    case 12:
			{
			    if(GetPlayerGangLevel(playerid) == 6)
			    {
			        ShowDialogAgain();
			        SendMessage(playerid, "wGangMenu8");
			        return 1;
			    }
				ShowDialog(playerid, Dialog:GangMenuLeave);
			}
		    case 13:	ShowDialog(playerid, Dialog:GangMenuInfo);
		}
	}
	else
	{
	    ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuCreate, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuCreate)
{
	if(response)
	{
	    if(IsGangExist(GetPlayerGang(playerid)))
	    {
	        SendMessage(playerid, "wGangMenuCreate1");
	        return 1;
	    }
	    if(GetPlayerServerMoney(playerid) < 1000000)
	    {
	        SendMessage(playerid, "wGangMenu3");
	        return 1;
	    }
	    if(!strlen(inputtext))
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuCreate3");
	        return 1;
	    }
	    if(strlen(inputtext) > MAX_GANG_NAME_LEN)
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuCreate4", MAX_GANG_NAME_LEN);
	        return 1;
	    }
	    if(!IsInputGang(inputtext))
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuCreate2");
	        return 1;
	    }
		if(IsInputGangDark(inputtext))
		{
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuCreate5");
	        return 1;
		}
	    itemSetPointer(itemGetByID(Player, playerid), "doplistitem", MEM_new_arr(inputtext, strlen(inputtext)), false);
	    ShowDialog(playerid, Dialog:GangMenuCreateColor);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuCreateColor, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuCreateColor)
{
	if(response)
	{
	    if(IsGangExist(GetPlayerGang(playerid)))
	    {
	        SendMessage(playerid, "wGangMenuCreate1");
	        return 1;
	    }
	    if(GetPlayerServerMoney(playerid) < 1000000)
	    {
	        SendMessage(playerid, "wGangMenu3");
	        return 1;
	    }
	    new RGB:color;
		if(!HexToRGB(inputtext, color))
		{
		    SendMessage(playerid, "wGangMenuCreateColor1");
		    ShowDialogAgain();
		    return 1;
		}
		if(_:color < 0x404040)
		{
		    SendMessage(playerid, "wGangMenuCreateColor2");
		    ShowDialogAgain();
		    return 1;
		}
		
		static name[MAX_GANG_NAME_LEN];
		new Pointer:ptr = itemGetPointer(itemGetByID(Player, playerid), "doplistitem");
		MEM_get_arr(ptr, 0, name, MEM_get_size(ptr));
		name[MEM_get_size(ptr)] = '\0';
		
		new gangid = CreateGang(name, color, 0);
		if(!IsGangExist(gangid))
		{
		    SendMessage(playerid, "wGangMenuCreateColor3");
		    return 1;
		}
		
		PlayerEnterGang(playerid, gangid);
		PlayerSetGangLevel(playerid, 6);
		SendMessage(playerid, "wGangMenuCreateColor4", name, _:color);
		printf("Игрок %s создал банду [%s] [ID:%i] (цвет %06x)", PlayerName(playerid), name, gangid, _:color);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenuCreate);
	}
	return 1;
}

dialogShowStandart(GangMenuDelete, DIALOG_STYLE_MSGBOX);
dialogResponse(GangMenuDelete)
{
	if(response)
	{
	    static name[MAX_GANG_NAME_LEN];

	    new gangid = GetPlayerGang(playerid);
	    GetGangName(gangid, name);
        SendGangMessage(gangid, "wGangMenuDelete1", PlayerName(playerid));
        printf("Лидер %s удалил банду %s [ID:%i]", PlayerName(playerid), name, gangid);
		DestroyGang(gangid);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuSkins, DIALOG_STYLE_LIST);
dialogResponse(GangMenuSkins)
{
	if(response)
	{
		itemSetInt(itemGetByID(Player, playerid), "doplistitem", listitem);
	    ShowDialog(playerid, Dialog:GangMenuSkins2);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuSkins2, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuSkins2)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    new skin = strval(inputtext);
	    new rang = itemGetInt(itemGetByID(Player, playerid), "doplistitem")+1;
	    if((skin < 0 || skin > 311) && skin != -1)
	    {
	        SendClientMessage(playerid, 0xFF0000FF, "wGangMenuSkin1");
	        ShowDialogAgain();
	        return 1;
	    }
      	SetGangSkin(gangid, rang, skin);
	    if(skin != -1)
	    {
			SendGangMessage(gangid, "mNewGangRang", PlayerName(playerid), skin, rang);
		}
		else
		{
		    SendGangMessage(gangid, "mResetGangRang", PlayerName(playerid), rang);
		}
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenuSkins);
	}
	return 1;
}

dialogShowStandart(GangMenuSpawn, DIALOG_STYLE_MSGBOX);
dialogResponse(GangMenuSpawn)
{
	if(response)
	{
	    static name[MAX_GANG_NAME_LEN];
	
	    new gangid = GetPlayerGang(playerid);
	    new Float:x, Float:y, Float:z, Float:rz;
	    new interior = GetPlayerInterior(playerid);
	    GetPlayerPos(playerid, x, y, z);
	    if(IsPlayerInAnyVehicle(playerid))
	    {
	        GetVehicleZAngle(GetPlayerVehicleID(playerid), rz);
	    }
	    else
	    {
	        GetPlayerFacingAngle(playerid, rz);
	    }
	    
	    SetGangSpawn(gangid, x, y, z, rz, interior);
	    
	    GetGangName(gangid, name);
		
		SendGangMessage(gangid, "wGangMenuSpawn1", PlayerName(playerid), x, y, z, interior);
		printf("Лидер %s банды %s [ID:%i] установил новый спавн банды (X: %0.1f, Y: %0.1f, Z: %0.1f, интерьер: %i)",
		PlayerName(playerid), name, gangid, x, y, z, interior);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuSpawnCancel, DIALOG_STYLE_MSGBOX);
dialogResponse(GangMenuSpawnCancel)
{
	if(response)
	{
	    static name[MAX_GANG_NAME_LEN];
	
	    new gangid = GetPlayerGang(playerid);
	    
	    new interior;
	    GetGangSpawn(gangid, .interior = interior);

	    if(interior == -1)
	    {
	        SendClientMessage(playerid, 0xFF0000FF, "wGangMenuSpawnCancel1");
	        return 1;
	    }
	    
	    SetGangSpawn(gangid, 0.0, 0.0, 0.0, 0.0, -1);
	    
	    GetGangName(gangid, name);

		SendGangMessage(gangid, "wGangMenuSpawnCancel2", PlayerName(playerid));
		printf("Лидер %s банды %s [ID: %i] удалил спавн банды", PlayerName(playerid),
		name, gangid);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuRang, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuRang)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
	        SendMessage(playerid, "wGangMenu1");
	        return 1;
	    }
	    if(GetPlayerGangLevel(playerid) < 6)
	    {
	        SendMessage(playerid, "wGangMenuRang1");
	        return 1;
	    }
	    new pl = strval(inputtext);
	    if(!IsPlayerConnected(pl))
	    {
	        SendMessage(playerid, "wGangMenuRang2");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(GetPlayerGang(pl) != gangid)
	    {
	        SendMessage(playerid, "wGangMenuRang3");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(pl == playerid)
	    {
	        SendMessage(playerid, "wGangMenuRang4");
	        ShowDialogAgain();
	        return 1;
	    }
	    itemSetInt(itemGetByID(Player, playerid), "doplistitem", pl);
		ShowDialog(playerid, Dialog:GangMenuRang2);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuRang2, DIALOG_STYLE_LIST);
dialogResponse(GangMenuRang2)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    new doplistitem = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	    if(!IsGangExist(gangid))
	    {
	        SendMessage(playerid, "wGangMenu1");
	        return 1;
	    }
	    if(GetPlayerGangLevel(playerid) < 6)
	    {
	        SendMessage(playerid, "wGangMenuRang1");
	        return 1;
	    }
	    if(!IsPlayerConnected(doplistitem))
	    {
	        SendMessage(playerid, "wGangMenuRang2");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(GetPlayerGang(doplistitem) != gangid)
	    {
	        SendMessage(playerid, "wGangMenuRang3");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(doplistitem == playerid)
	    {
	        SendMessage(playerid, "wGangMenuRang4");
	        ShowDialogAgain();
	        return 1;
	    }
	    new rang = strval(inputtext);
		if(rang < 1 || rang > 6)
		{
		    SendMessage(playerid, "wGangMenuRang01");
		    ShowDialogAgain();
			return 1;
		}
		if(rang == 6)
		{
		    ShowDialog(playerid, Dialog:GangMenuRang3);
		}
		else
		{
		    static name[MAX_GANG_NAME_LEN];
		    GetGangName(gangid, name);

		    SendGangMessage(gangid, "wGangMenuRang02", PlayerName(playerid), PlayerName(doplistitem), rang);
		    printf("Лидер %s банды %s [ID: %i] установил игроку %s ранг #%i", PlayerName(playerid),
		    name, gangid, PlayerName(doplistitem), rang);
			PlayerSetGangLevel(doplistitem, rang);
		}
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenuRang);
	}
	return 1;
}

dialogShow(GangMenuRang3)
{
	new gangid = GetPlayerGang(playerid);
	if(!IsGangExist(gangid))
	{
	    return 1;
	}
	new doplistitem = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(doplistitem))
	{
	    return 1;
	}
	dialogFormatInfo(PlayerName(doplistitem));
	return 1;
}
dialogResponse(GangMenuRang3)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    new doplistitem = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	    if(!IsGangExist(gangid))
	    {
	        SendMessage(playerid, "wGangMenu1");
	        return 1;
	    }
	    if(GetPlayerGangLevel(playerid) < 6)
	    {
	        SendMessage(playerid, "wGangMenuRang1");
	        return 1;
	    }
	    if(!IsPlayerConnected(doplistitem))
	    {
	        SendMessage(playerid, "wGangMenuRang2");
	        return 1;
	    }
	    if(GetPlayerGang(doplistitem) != gangid)
	    {
	        SendMessage(playerid, "wGangMenuRang3");
	        return 1;
	    }
	    if(doplistitem == playerid)
	    {
	        SendMessage(playerid, "wGangMenuRang4");
	        return 1;
	    }
	    static name[MAX_GANG_NAME_LEN];
	    GetGangName(gangid, name);
	    
	    PlayerSetGangLevel(doplistitem, 6);
	    PlayerSetGangLevel(playerid, 1);

		SendGangMessage(gangid, "mNewGangLeader", PlayerName(playerid), PlayerName(doplistitem));
		printf("%s передал полномочия лидера банды %s [ID: %i] к %s", PlayerName(playerid),
		name, gangid, PlayerName(doplistitem));
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenuRang2);
	}
	return 1;
}

dialogShowStandart(GangMenuInvite, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuInvite)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
			SendMessage(playerid, "wGangMenu1");
	        ShowDialogAgain();
	        return 1;
	    }
		new pl = strval(inputtext);
	    if(!IsPlayerConnected(pl))
	    {
	        SendMessage(playerid, "wGangMenuInvite1");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(pl == playerid)
	    {
            SendMessage(playerid, "wGangMenuInvite2");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(IsGangExist(GetPlayerGang(pl)))
	    {
	        SendMessage(playerid, "wGangMenuInvite3");
	        ShowDialogAgain();
	        return 1;
	    }
	    static name[MAX_GANG_NAME_LEN];
	    GetGangName(gangid, name);
	    
	    itemSetInt(itemGetByID(Player, pl), "doplistitem", gangid);
	    ShowDialog(pl, Dialog:GangMenuInviteAccept);

		SendGangMessage(gangid, "wGangMenuInvite4", PlayerName(playerid), PlayerName(pl));
		printf("%s отправил приглашение в банду %s [ID: %i] игроку %s", PlayerName(playerid),
		name, gangid, PlayerName(pl));
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShow(GangMenuInviteAccept)
{
	new gangid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsGangExist(gangid))
	{
	    return 1;
	}
	
	static name[MAX_GANG_NAME_LEN];
	GetGangName(gangid, name);
	new color = GetGangColor(gangid);
	
	dialogFormatInfo(color, name);
	return 1;
}
dialogResponse(GangMenuInviteAccept)
{
	new gangid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsGangExist(gangid))
	{
	    if(response)
	    {
	        SendMessage(playerid, "GangMenuInviteAccept1");
	    }
	    return 1;
	}
	static name[MAX_GANG_NAME_LEN];
	GetGangName(gangid, name);
	
	if(response)
	{
	    PlayerEnterGang(playerid, gangid);
		SendGangMessage(gangid, "GangMenuInviteAccept2", PlayerName(playerid));
	    printf("Игрок %s принял приглашение в банду %s [ID: %i]", PlayerName(playerid),
		name, gangid);
	}
	else
	{
	    SendGangMessage(gangid, "GangMenuInviteAccept3", PlayerName(playerid));
	    printf("Игрок %s отклонил приглашение в банду %s [ID: %i]", PlayerName(playerid),
		name, gangid);
	}
	return 1;
}
dialogInterrupt(GangMenuInviteAccept)
{
	new gangid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(IsGangExist(gangid))
	{
		static name[MAX_GANG_NAME_LEN];
		GetGangName(gangid, name);

		SendGangMessage(gangid, "GangMenuInviteAccept4", PlayerName(playerid));
	    printf("Игрок %s не смог ответить на приглашение в банду %s [ID: %i]", PlayerName(playerid),
		name, gangid);
	}
	return 1;
}

dialogShowStandart(GangMenuKick, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuKick)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
	        SendMessage(playerid, "wGangMenu1");
            ShowDialogAgain();
	        return 1;
	    }
		new rang1 = GetPlayerGangLevel(playerid);
		if(rang1 < 4)
		{
	        SendMessage(playerid, "wGangMenuKick1");
	        ShowDialogAgain();
		    return 1;
		}
	    new pl = strval(inputtext);
	    if(!IsPlayerConnected(pl))
	    {
	        SendMessage(playerid, "wGangMenuKick2");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(pl == playerid)
	    {
	        SendMessage(playerid, "wGangMenuKick3");
	        ShowDialogAgain();
	        return 1;
	    }
	    if(GetPlayerGang(pl) != gangid)
	    {
	        SendMessage(playerid, "wGangMenuRang3");
	        ShowDialogAgain();
	        return 1;
	    }
		new rang2 = GetPlayerGangLevel(pl);
	    if(rang1 <= rang2)
	    {
	        SendMessage(playerid, "wGangMenuKick4");
	        ShowDialogAgain();
	        return 1;
	    }

		static name[MAX_GANG_NAME_LEN];
		GetGangName(gangid, name);

		SendGangMessage(gangid, "wGangMenuKick5", PlayerName(playerid), PlayerName(pl));
	    printf("%s исключил %s из банды %s [ID: %i]", PlayerName(playerid), PlayerName(pl),
		name, gangid);
	    
	    PlayerLeaveGang(pl);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuType, DIALOG_STYLE_LIST);
dialogResponse(GangMenuType)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenu1");
	        return 1;
	    }
	    if(GetPlayerGangLevel(playerid) != 6)
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuName1");
	        return 1;
	    }
		SetGangType(gangid, listitem);
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuName, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuName)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenu1");
	        return 1;
	    }
	    if(GetPlayerGangLevel(playerid) != 6)
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuName1");
	        return 1;
	    }
	    if(!strlen(inputtext))
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuName3");
	        return 1;
	    }
	    if(strlen(inputtext) > MAX_GANG_NAME_LEN)
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuName4", MAX_GANG_NAME_LEN);
	        return 1;
	    }
	    if(!IsInputGang(inputtext))
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuName2");
	        return 1;
	    }
		if(IsInputGangDark(inputtext))
		{
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuName6");
	        return 1;
		}

		static name[MAX_GANG_NAME_LEN];
		GetGangName(gangid, name);
	    
	    printf("%s переименовал банду [ID: %i] из [%s] в [%s]", PlayerName(playerid), gangid,
		name, inputtext);
		SendGangMessage(gangid, "wGangMenuName5", PlayerName(playerid), inputtext);
	    
	    SetGangName(gangid, inputtext);
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuColor, DIALOG_STYLE_INPUT);
dialogResponse(GangMenuColor)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenu1");
	        return 1;
	    }
	    if(GetPlayerGangLevel(playerid) != 6)
	    {
            ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuColor1");
	        return 1;
	    }
	    if(!strlen(inputtext))
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuColor2");
	        return 1;
	    }
	    new RGB:color;
	    if(!HexToRGB(inputtext, color))
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuColor3");
	        return 1;
	    }
	    if(_:color < 0x404040)
	    {
	        ShowDialogAgain();
	        SendMessage(playerid, "wGangMenuColor4");
	        return 1;
	    }
	    
		static name[MAX_GANG_NAME_LEN];
		GetGangName(gangid, name);
		
		new oldcolor = GetGangColor(gangid);
	    
	    printf("%s изменил цвет банды %s [ID: %i] из %06x в %06x", PlayerName(playerid),
		name, gangid, oldcolor,  _:color);

	    SetGangColor(gangid, color);
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShowStandart(GangMenuLeave, DIALOG_STYLE_MSGBOX);
dialogResponse(GangMenuLeave)
{
	if(response)
	{
	    new gangid = GetPlayerGang(playerid);
	    if(!IsGangExist(gangid))
	    {
	        SendMessage(playerid, "wGangMenuLeave1");
	        return 1;
	    }
	    
		static name[MAX_GANG_NAME_LEN];
		GetGangName(gangid, name);
	    
	    PlayerLeaveGang(playerid);
        printf("%s вышел из банды %s [ID: %i]", PlayerName(playerid),
		name, gangid);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GangMenu);
	}
	return 1;
}

dialogShow(GangMenuInfo)
{
	new gangid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsGangExist(gangid))
	{
	    return 1;
	}
	
	static string[64];
	static name[MAX_GANG_NAME_LEN];
	static plname[MAX_PLAYER_NAME];
	static color[10];
	new colorval = GetGangColor(gangid);
	new ORMResult:result;
	
	RGBToHex(colorval, color);
	
	format(string, sizeof(string), "GangID = %i", gangid);
	result = ormSelect(AccountTable, string, "id");
	new count = ormResultNumRows(result);
	
	format(string, sizeof(string), "GangID = %i AND GangLvl = 6", gangid);
	result = ormSelect(AccountTable, string, "Name");
	ormGetField(result, 0, plname);
	
	dialogFormatInfo(name, color, plname, count);
	return 1;
}
dialogResponse(GangMenuInfo)
{
	ShowDialog(playerid, Dialog:GangMenu);
	return 1;
}

