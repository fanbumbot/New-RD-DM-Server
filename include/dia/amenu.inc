#if defined inc_dia_amenu
	#endinput
#endif
#define inc_dia_amenu

dialogShow(AdminMenu)
{
	dialogSetStyle(DIALOG_STYLE_LIST);
	new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
	    SendMessage(playerid, "wAMenuCConnect");
	    return 0;
	}
	dialogFormatCaption(PlayerName(targetid), targetid);
	return 1;
}

#define CheckAdmin<%0> \
if(GetPlayerAdmin(playerid) < %0)\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCAdmin");\
	return 1;\
}

#define CheckVIP<%0> \
if(GetPlayerVIP(playerid) < %0 && !GetPlayerAdmin(playerid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCVIP");\
	return 1;\
}

#define CheckPrisonPlayer<> \
if(itemGetInt(itemGetByID(Player, playerid), "Prisoned"))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCPrisonPlayer");\
	return 1;\
}

#define CheckPrisonTarget<> \
if(itemGetInt(itemGetByID(Player, targetid), "Prisoned"))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCPrisonTarget");\
	return 1;\
}

#define CheckFreezPlayer<> \
if(IsPlayerFreezed(playerid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCFreezPlayer");\
	return 1;\
}

#define CheckFreezTarget<> \
if(IsPlayerFreezed(targetid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCFreezTarget");\
	return 1;\
}

#define CheckSpecPlayer<> \
if(IsPlayerSpec(playerid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCSpecPlayer");\
	return 1;\
}

#define CheckSpecTarget<> \
if(IsPlayerSpec(targetid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCSpecTarget");\
	return 1;\
}

#define CheckLoginTarget<> \
if(!IsPlayerLogin(targetid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCLoginTarget");\
	return 1;\
}

#define CheckAdminTarget<> \
if(GetPlayerAdmin(targetid) > GetPlayerAdmin(playerid))\
{\
    ShowDialogAgain();\
	SendMessage(playerid, "wAMenuCAdminTarget");\
	return 1;\
}

dialogResponse(AdminMenu)
{
	if(response)
	{
	    new Item:player = itemGetByID(Player, playerid);
		new targetid = itemGetInt(player, "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			return 1;
		}
	
		switch(listitem)
		{
		    case 0:
		    {
		        CheckAdmin<1>
		        CheckLoginTarget<>
		        CheckPrisonPlayer<>
		        CheckFreezPlayer<>
		        CheckSpecPlayer<>
		        CheckSpecTarget<>
		        if(targetid == playerid)
		        {
		            ShowDialogAgain();
		            SendMessage(playerid, "wAMenuCTP1");
		            return 1;
		        }
		        if(GetPlayerLobby(playerid) != GetPlayerLobby(targetid))
		        {
		            PlayerEnterLobby(playerid, GetPlayerLobby(targetid));
				}
				UniversalTeleportToPlayer(playerid, targetid, .offsetz = 2.0);
				//SendMessage(playerid, "wAMenuSuccessTeleport1", PlayerName(playerid));
		    }
		    case 1:
		    {
		        CheckAdmin<1>
		        CheckLoginTarget<>
		        if(targetid == playerid)
		        {
		            ShowDialogAgain();
		            SendMessage(playerid, "wAMenuCTP2");
		            return 1;
		        }
		        CheckAdminTarget<>
		        CheckPrisonTarget<>
		        CheckSpecPlayer<>
		        CheckSpecTarget<>
		        if(GetPlayerLobby(playerid) != GetPlayerLobby(targetid))
		        {
		            PlayerEnterLobby(targetid, GetPlayerLobby(playerid));
				}
				UniversalTeleportToPlayer(targetid, playerid, .offsetz = 2.0);
				SendMessageToAll("wAMenuSuccessTeleport2", PlayerName(playerid), PlayerName(targetid));
		    }
			case 2:
			{
			    CheckAdmin<1>
			    CheckLoginTarget<>
			    CheckAdminTarget<>
			    CheckPrisonPlayer<>
			    CheckFreezPlayer<>
			    CheckSpecTarget<>
		        if(targetid == playerid)
		        {
		            ShowDialogAgain();
		            SendMessage(playerid, "wAMenuSpec1");
		            return 1;
		        }
			    //SendMessage(playerid, "wAMenuSpecSuccess1");
			    AdminSpecEnable(playerid, targetid);
			}
			case 3:
			{
			    CheckAdmin<1>
			    if(!IsAdminSpec(playerid))
			    {
			        ShowDialogAgain();
		            SendMessage(playerid, "wAMenuSpec2");
		            return 1;
			    }
			    if(AdminSpecDisable(playerid))
			    {
			        SendMessage(playerid, "wAMenuSpecSuccess2");
			    }
			}
			case 4:
			{
			    CheckAdmin<1>
			    CheckLoginTarget<>
			    SetPlayerHealth(targetid, 100.0);
			    SetPlayerArmour(targetid, 100.0);
			    SendMessageToAll("mAHealed", PlayerName(playerid), PlayerName(targetid));
			    printf("Админ %s пополнил здоровье игроку %s", PlayerName(playerid), PlayerName(targetid));
			}
		    case 5:
			{
				CheckAdmin<3>
		        if(targetid == playerid)
		        {
		            ShowDialogAgain();
		            SendMessage(playerid, "wAMenuBan1");
		            return 1;
		        }
				if(GetPlayerAdmin(targetid) > 0 && GetPlayerAdmin(playerid) != 4)
				{
				    ShowDialogAgain();
				    SendMessage(playerid, "wAMenuBan4");
				    return 1;
				}
				ShowDialog(playerid, Dialog:AMenuBan);
			}
		    case 6:
			{
				CheckAdmin<3>
		        if(targetid == playerid)
		        {
		            ShowDialogAgain();
		            SendMessage(playerid, "wAMenuBan1");
		            return 1;
		        }
				if(GetPlayerAdmin(targetid) > 0 && GetPlayerAdmin(playerid) != 4)
				{
				    ShowDialogAgain();
				    SendMessage(playerid, "wAMenuBan4");
				    return 1;
				}
				ShowDialog(playerid, Dialog:AMenuBanTm);
			}
			case 7:
			{
				CheckAdmin<1>
		        if(targetid == playerid)
		        {
		            ShowDialogAgain();
		            SendMessage(playerid, "wAMenuKick1");
		            return 1;
		        }
				if(GetPlayerAdmin(targetid) > 0 && GetPlayerAdmin(playerid) != 4)
				{
				    ShowDialogAgain();
				    SendMessage(playerid, "wAMenuKick3");
				    return 1;
				}
			    ShowDialog(playerid, Dialog:AMenuKick);
			}
			case 8:
			{
			    CheckAdmin<1>
			    CheckLoginTarget<>
				CheckAdminTarget<>
				CheckSpecPlayer<>
				if(IsPlayerFreezed(targetid))
				{
				    ShowDialogAgain();
					SendMessage(playerid, "wAMenuFreez1");
					return 1;
				}
				ShowDialog(playerid, Dialog:AMenuFreez);
			}
			case 9://Разморозить
			{
			    CheckAdmin<1>
			    CheckLoginTarget<>
				CheckAdminTarget<>
				CheckSpecPlayer<>
				if(!IsPlayerFreezed(targetid))
				{
				    ShowDialogAgain();
					SendMessage(playerid, "wAMenuPlayerNotFreeze");
					return 1;
				}
				UnFreezPlayer(targetid);
				SendMessageToAll("mAdminPlayerUnfreeze", PlayerName(playerid), PlayerName(targetid));
				printf("Админ %s разморозил игрока %s", PlayerName(playerid), PlayerName(targetid));
			}
			case 10://Убить
			{
			    CheckAdmin<1>
			    CheckLoginTarget<>
			    CheckSpecPlayer<>
			    ShowDialog(playerid, Dialog:AMenuKill);
			}
		    case 11://Заткнуть
			{
				CheckAdmin<1>
				CheckLoginTarget<>
				ShowDialog(playerid, Dialog:AMenuMute);
			}
		    case 12://Посадить в тюрьму
			{
				CheckAdmin<1>
				ShowDialog(playerid, Dialog:AMenuPrison);
			}
			case 13://Тп себя в тюрьму
			{
			    CheckAdmin<1>
			    SetPlayerPos(playerid, 193.31, 173.87, 1003.02);
				SetPlayerFacingAngle(playerid, 0.44);
				SetCameraBehindPlayer(playerid);
				SetPlayerInterior(playerid, 3);
			}
			case 14://Тп себя в полицейский участок
			{
                CheckAdmin<1>
                CheckLoginTarget<>
                SetPlayerPos(playerid, 202.65, 179.72, 1003.03);
				SetPlayerFacingAngle(playerid, 96.66);
				SetCameraBehindPlayer(playerid);
				SetPlayerInterior(playerid, 3);
			}
			case 15://Посмотреть статистику
			{
			    CheckAdmin<2>
			    ShowPlayerStats(playerid, targetid);
			}
		    case 16://Сменить скин
			{
				CheckAdmin<1>
                ShowDialog(playerid, Dialog:AMenuSkin);
			}
			case 17://Узнать IP
			{
			    CheckAdmin<2>
			    static string[256];
			    GetPlayerIp(targetid, string, sizeof(string));
			    SendMessage(playerid, "mAdminCheckIP", PlayerName(targetid), string);
			    ShowDialogAgain();
			}
			case 18://Слапнуть
			{
			    CheckAdmin<1>
				new Float:x, Float:y, Float:z;
				GetPlayerPos(targetid, x, y, z);
				SetPlayerPos(targetid, x, y, z+1.0);
				SendMessageToAll("mAdminSlapPlayer", PlayerName(playerid), PlayerName(targetid));
				printf("Админ %s слапнул игрока %s", PlayerName(playerid), PlayerName(targetid));
			}
		}
	}
	return 1;
}
dialogShow(AMenuBan)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuBan)
{
	if(response)
	{
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			return 1;
		}
        if(targetid == playerid)
        {
            ShowDialogAgain();
            SendMessage(playerid, "wAMenuBan1");
            return 1;
        }
		if(!IsInputRus(inputtext))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBan2");
		    return 1;
		}
		if(GetPlayerAdmin(targetid) > 0 && GetPlayerAdmin(playerid) != 4)
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBan4");
		    return 1;
		}
		if(strlen(inputtext) >= 32)
		{
			SendMessage(playerid, "wAMenuBan5");
			ShowDialogAgain();
			return 1;
		}
		AdminBanPlayer(playerid, targetid, inputtext);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShow(AMenuBanTm)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuBanTm)
{
	if(response)
	{
	    static string[256];
	    dialogInputInit(2);
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			return 1;
		}
        if(targetid == playerid)
        {
            ShowDialogAgain();
            SendMessage(playerid, "wAMenuBanTm1");
            return 1;
        }
		if(!IsInputRus(inputtext))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBanTm2");
		    return 1;
		}
		if(dialogInputGetNum() == 0)
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBanTm3");
		    return 1;
		}
		if(GetPlayerAdmin(targetid) > 0 && GetPlayerAdmin(playerid) != 4)
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBanTm4");
		    return 1;
		}
		
		new time = dialogInputGetInt(0);
		if(time < 1 || time > 30)
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBanTm5");
		    return 1;
		}
		if(dialogInputGetNum() != 2)
		{
		    string[0] = '\0';
		}
		else
		{
			dialogInputGet(1, string);
		}
		if(strlen(string) >= 32)
		{
			SendMessage(playerid, "wAMenuBanTm6");
			ShowDialogAgain();
			return 1;
		}
		AdminBanPlayer(playerid, targetid, string, time);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShow(AMenuKick)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuKick)
{
	if(response)
	{
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			return 1;
		}
        if(targetid == playerid)
        {
            ShowDialogAgain();
            SendMessage(playerid, "wAMenuKick1");
            return 1;
        }
		if(GetPlayerAdmin(targetid) > 0 && GetPlayerAdmin(playerid) != 4)
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuKick3");
		    return 1;
		}
		if(strlen(inputtext) >= 32)
		{
			SendMessage(playerid, "wAMenuKick4");
			ShowDialogAgain();
			return 1;
		}
		AdminKickPlayer(playerid, targetid, inputtext);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShow(AMenuMute)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuMute)
{
    if(response)
    {
	    static string[256];
	    dialogInputInit(2);
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			ShowDialogAgain();
			return 1;
		}
		if(!IsPlayerRegistered(targetid))
		{
			SendMessage(playerid, "wAMenuMute1");
			ShowDialogAgain();
			return 1;
		}
		if(dialogInputGetNum() == 0)
		{
			SendMessage(playerid, "wAMenuMute2");
			ShowDialogAgain();
		    return 1;
		}
		new time = dialogInputGetInt(0);
		if(dialogInputGetNum() != 2)
		{
		    string[0] = '\0';
		}
		else
		{
			dialogInputGet(1, string);
		}
		if(strlen(string) >= 32)
		{
			SendMessage(playerid, "wAMenuMute3");
			ShowDialogAgain();
			return 1;
		}
		if(!IsInputRus(string))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBan2");
		    return 1;
		}
		printf("%i %i %i %s", playerid, targetid, time, string);
		AdminMutePlayer(playerid, targetid, time, string);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShow(AMenuPrison)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuPrison)
{
    if(response)
    {
	    static string[256];
	    dialogInputInit(2);
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			ShowDialogAgain();
			return 1;
		}
		if(!IsPlayerRegistered(targetid))
		{
			SendMessage(playerid, "wAMenuPrison1");
			ShowDialogAgain();
			return 1;
		}
		if(dialogInputGetNum() == 0)
		{
			SendMessage(playerid, "wAMenuPrison2");
			ShowDialogAgain();
		    return 1;
		}
		new time = dialogInputGetInt(0);
		if(dialogInputGetNum() != 2)
		{
		    string[0] = '\0';
		}
		else
		{
			dialogInputGet(1, string);
		}
		if(strlen(string) >= 32)
		{
			SendMessage(playerid, "wAMenuPrison3");
			ShowDialogAgain();
			return 1;
		}
		if(!IsInputRus(string))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wAMenuBan2");
		    return 1;
		}
		AdminPrisonPlayer(playerid, targetid, time, string);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShowStandart(AMenuSkin, DIALOG_STYLE_INPUT);
dialogResponse(AMenuSkin)
{
    if(response)
    {
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			ShowDialogAgain();
			return 1;
		}
		if(!IsPlayerLogin(targetid))
		{
		    SendMessage(playerid, "wAMenuSkin1");
		    ShowDialogAgain();
		    return 1;
		}
		if(IsPlayerSpec(targetid))
		{
		    SendMessage(playerid, "wAMenuSkin2");
		    ShowDialogAgain();
		    return 1;
		}
		new skin = strval(inputtext);
		if(skin < 0 || skin > 311)
		{
			SendMessage(playerid, "wAMenuSkin3");
			ShowDialogAgain();
			return 1;
		}
		SetPlayerSkin(targetid, skin);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShow(AMenuKill)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuKill)
{
    if(response)
    {
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			ShowDialogAgain();
			return 1;
		}
		if(!IsPlayerLogin(targetid))
		{
			SendMessage(playerid, "wAMenuKill1");
			ShowDialogAgain();
		    return 1;
		}
		if(IsPlayerSpec(targetid))
		{
			SendMessage(playerid, "wAMenuKill2");
			ShowDialogAgain();
		    return 1;
		}
		if(strlen(inputtext) >= 32)
		{
			SendMessage(playerid, "wAMenuKill3");
			ShowDialogAgain();
			return 1;
		}
		SetPlayerHealth(targetid, 0.0);
		SetPlayerArmour(targetid, 0.0);
		SendMessageToAll("mAdminKillPlayer", PlayerName(playerid), PlayerName(targetid), inputtext);
		printf("Админ %s убил игрока %s. Причина: %s", PlayerName(playerid), PlayerName(targetid), inputtext);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}

dialogShow(AMenuFreez)
{
	dialogSetStyle(DIALOG_STYLE_INPUT);
    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(!IsPlayerConnected(targetid))
	{
		SendMessage(playerid, "wAMenuCConnect");
		return 1;
	}
	dialogFormatInfo(PlayerName(targetid), targetid);
	return 1;
}
dialogResponse(AMenuFreez)
{
    if(response)
    {
	    new targetid = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
		if(!IsPlayerConnected(targetid))
		{
			SendMessage(playerid, "wAMenuCConnect");
			ShowDialogAgain();
			return 1;
		}
		if(GetPlayerAdmin(targetid) > GetPlayerAdmin(playerid))
		{
			SendMessage(playerid, "wAMenuCAdminTarget");
			ShowDialogAgain();
			return 1;
		}
		if(IsPlayerSpec(targetid))
		{
			SendMessage(playerid, "wAMenuFreez5");
			ShowDialogAgain();
		    return 1;
		}
		if(!IsPlayerLogin(targetid))
		{
			SendMessage(playerid, "wAMenuFreez6");
			ShowDialogAgain();
		    return 1;
		}
		if(strlen(inputtext) >= 32)
		{
			SendMessage(playerid, "wAMenuFreez7");
			ShowDialogAgain();
			return 1;
		}
		FreezPlayer(targetid);
		SendMessageToAll("mAdminFreezePlayer", PlayerName(playerid), PlayerName(targetid), inputtext);
		printf("Админ %s заморозил игрока %s. Причина: %s", PlayerName(playerid), PlayerName(targetid), inputtext);
	}
	else
	{
	    ShowDialog(playerid, Dialog:AMenu);
	}
	return 1;
}










