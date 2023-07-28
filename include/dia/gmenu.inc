#if defined inc_dia_gmenu
	#endinput
#endif
#define inc_dia_gmenu

new veh_menu[][32] = {
	{
		562, 565, 559, 561, 560, 558
	},{
		429, 541, 415, 480, 434, 494, 502, 503, 411, 506,
		451, 555, 477
	},{
		536, 575, 534, 567, 535, 576, 412
	},{
		602, 496, 401, 518, 402, 527, 542, 589, 419, 587,
		533, 526, 474, 545, 517, 410, 600, 603, 436, 439,
	 	475, 549, 491
	},{
		445, 507, 585, 466, 492, 546, 551, 516, 467, 426,
		547, 405, 580, 409, 550, 566, 540, 421, 529
	},{
		579, 400, 404, 489, 479, 458
	},{
		499, 498, 609, 524, 532, 578, 486, 406, 573, 455,
		588, 403, 423, 414, 443, 515, 514, 531, 456
	},{
		459, 422, 482, 530, 418, 572, 582, 413, 440, 543,
		583, 478, 554
	},{
		568, 424, 504, 457, 483, 508, 571, 500, 444, 556,
		557, 471, 495, 539, 612
	},{
		481, 509, 510, 581, 462, 521, 463, 522, 461, 448,
		468, 586
	},{
	    485, 431, 438, 437, 574, 420, 525, 408, 552
	},{
		416, 433, 427, 490, 528, 407, 544, 523, 470, 596,
		597, 598, 599, 432, 428, 601
	},{
		592, 577, 511, 548, 512, 593, 425, 417, 487, 553,
		488, 497, 563, 519, 460, 469, 513, 520
	},{
	    472, 473, 493, 595, 484, 453, 452, 446, 454
	},{
	    441, 594, 465, 564, 604, 605
	}
};


public OnScriptInit()
{
	itemInfoAdd(Player, "TunSelf");
	itemInfoAdd(Player, "TunInfo");

#if defined dia_gmenu_OnScriptInit
    dia_gmenu_OnScriptInit();
#endif
	return 1;
}
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
    #define _ALS_OnScriptInit
#endif
#if defined OnScriptInit
	#undef OnScriptInit
#endif
#define OnScriptInit dia_gmenu_OnScriptInit
#if defined dia_gmenu_OnScriptInit
forward dia_gmenu_OnScriptInit();
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
#if defined dia_gmenu_OnPlayerKeyStateChang
    dia_gmenu_OnPlayerKeyStateChang(playerid, newkeys, oldkeys);
#endif

	new World:world = GetPlayerWorld(playerid);
	if(world != worldGetByName("Reglog"))
	{
		if(!PlayerGetInt(playerid, "MenuButton"))
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
				if((newkeys & KEY_SUBMISSION) && !(oldkeys & KEY_SUBMISSION))
				{
			    	ShowDialog(playerid, Dialog:GlobalMenu);
				}
			}
			else
			{
				if((newkeys & KEY_WALK) && !(oldkeys & KEY_WALK))
				{
			    	ShowDialog(playerid, Dialog:GlobalMenu);
				}
			}
		}
		else
		{
		    if((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
		    {
		        ShowDialog(playerid, Dialog:GlobalMenu);
		    }
		}
	}

	return 1;
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange dia_gmenu_OnPlayerKeyStateChang
#if defined dia_gmenu_OnPlayerKeyStateChang
forward dia_gmenu_OnPlayerKeyStateChang(playerid, newkeys, oldkeys);
#endif

stock FindNearPlayerToAnim(playerid)
{
	new Float:x, Float:y, Float:z, Float:x2, Float:y2, Float:z2, Float:angle, Float:angle2, Float:anglexx;
	new plint, plvw;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, angle);
	plint = GetPlayerInterior(playerid);
	plvw = GetPlayerVirtualWorld(playerid);
	
	new List:list = GetListPlayers();
	listForeach(i:list)
	{
		if(GetPlayerInterior(i) == plint && GetPlayerVirtualWorld(i) == plvw && playerid != i)
		{
			GetPlayerPos(i, x2, y2, z2);
			GetPlayerFacingAngle(i, angle2);
			anglexx = angle - angle2;
			if(anglexx < 0){anglexx = anglexx * -1;}
			if((-1<(x-x2)<1 && -1<(y-y2)<1 && -1<(z-z2)<1) && (x != x2 || y != y2 || z != z2) && (anglexx > 150 && anglexx < 210))
			{
				return i;
			}
		}
	}
	return -1;
}

forward DopAnim(playerid, nanim);
public DopAnim(playerid, nanim)
{
	if(nanim == 2)
	{
		ApplyAnimation(playerid,"Attractors", "Stepsit_in", 4.1, 0, 0, 0, 1, 0, 0);//сесть-2
		return 1;
	}
	if(nanim == 3)
	{
		ApplyAnimation(playerid,"CRACK", "crckidle2", 4.1, 0, 0, 0, 1, 0,0);//лечь
	}
	return 1;
}

forward DopAnim22(playerid, playdop);
public DopAnim22(playerid, playdop)
{
	ApplyAnimation(playerid,"KISSING", "Playa_Kiss_02", 4.1, 0, 0, 0, 0, 0, 0);//поцелуй
	ApplyAnimation(playdop,"KISSING", "Playa_Kiss_02", 4.1, 0, 0, 0, 0, 0, 0);//поцелуй
	return 1;
}

dialogShow(GlobalMenu)
{
	if(IsAdminSpec(playerid))
	{
	    SendMessage(playerid, "wASpecNoMenu");
	    return 0;
	}
	dialogSetStyle(DIALOG_STYLE_LIST);

	new h, mi;
	gettime(h, mi);
	dialogFormatCaption(h, mi);
	return 1;
}

dialogResponse(GlobalMenu)
{
	if(response)
	{
		if(listitem != 5 && listitem != 6 && listitem != 7 && listitem != 9 && listitem != 10 && PlayerGetInt(playerid, "PrisonSec") > 0)
		{
		    SendMessage(playerid, "wAMenuCPrisonPlayer");
		    ShowDialogAgain();
		    return 1;
		}
		if(listitem == 0 || listitem == 5)
		{
			new World:world = GetPlayerWorld(playerid);
			if(world == worldGetByName("DM"))
			{
				SendMessage(playerid, "wNotInDM");
				return 1;				
			}
		}
		switch(listitem)
		{
			case 0: ShowDialog(playerid, Dialog:GMenuVeh);
			case 1: ShowDialog(playerid, Dialog:GMenuStuff);
			case 2:
			{
			    new World:world = worldGetByName("Hub");
			    if(GetPlayerWorld(playerid) == world)
			    {
					if(GetPlayerLobby(playerid) != lobbyGetByID(world, 0))
					{
					    ShowDialog(playerid, Dialog:GMenuFreeWeap);
					}
					else
					{
					    ShowDialog(playerid, Dialog:GMenuWeap);
					}
				}
				else
				{
				    ShowDialog(playerid, Dialog:GMenuFreeWeap);
				}
			}
			case 3: ShowDialog(playerid, Dialog:GMenuTP1);
			case 4: ShowDialog(playerid, Dialog:GMenuActions);
			case 5: ShowDialog(playerid, Dialog:GMenuRules);
			case 6:
			{
			    itemSetInt(itemGetByID(Player, playerid), "doplistitem", 0);
				ShowDialog(playerid, Dialog:GMenuAdmins);
			}
			case 7: ShowDialog(playerid, Dialog:GMenuAbout);
			case 8: ShowDialog(playerid, Dialog:GangMenu);
		}
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
//---------------------------------Транспорт----------------------------------//
/*-----------------------------------Начало-----------------------------------*/
/*----------------------------------------------------------------------------*/

dialogShowStandart(GMenuVeh, DIALOG_STYLE_LIST);
dialogResponse(GMenuVeh)
{
	if(response)
	{
	    new Item:player;
		player = itemGetByID(Player, playerid);
	    if(listitem == 2 || listitem == 4 || listitem == 5)
	    {
	        if(GetPlayerWorld(playerid) != worldGetByName("Hub"))
	        {
	            SendMessage(playerid, "wGMenuNotHere");
	            return 1;
	        }
		}
		switch(listitem)
		{
			case 0: ShowDialog(playerid, Dialog:GMenuVehSpawn);
		    case 1:	ShowDialog(playerid, Dialog:GMenuTun);
		    case 2:
		    {
		        if(IsPlayerAutoRepair(playerid))
		        {
		            PlayerAutoRepairDisable(playerid);
					SendMessage(playerid, "mAutorepairOff");
				}
		        else
		        {
		            PlayerAutoRepairEnable(playerid);
					SendMessage(playerid, "mAutorepairOn");
				}
		    }
		    case 3://Тюнинг в любом/своём транспорте
		    {
		        new tun = !itemGetInt(player, "TunSelf");
		        itemSetInt(player, "TunSelf", tun);
		        if(tun)
		        {
		            SendMessage(playerid, "mTunSelfVeh");
		        }
		        else
		        {
		            SendMessage(playerid, "mTunAllVeh");
		        }
		    }
		    case 4:
			{
			    if(!IsValidVehicle(GetPlayerSelfVehicle(playerid)))
			    {
			        SendMessage(playerid, "wGMenuNoSelfVeh");
			        return 1;
			    }
				DestroySelfVehicle(playerid);
			}
		    case 5:
			{
			    new vehicleid = GetPlayerVehicleID(playerid);
			    if(!IsValidVehicle(vehicleid))
			    {
			        SendMessage(playerid, "wGMenuNoCurVeh");
					return 1;
				}
				DestroyVehicle(vehicleid);
			}
			case 6:
			{
			    new vehicleid = GetPlayerVehicleID(playerid);
			    if(IsValidVehicle(vehicleid))
			    {
					new Float:x, Float:y, Float:z, Float:rz;
					GetVehiclePos(vehicleid, x, y, z);
					GetVehicleZAngle(vehicleid, rz);
					SetVehiclePos(vehicleid, x, y, z+1.0);
					SetVehicleZAngle(vehicleid, rz);
    			}
			}
		}
	}
	else
	{
	    ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShow(GMenuVehSpawn)
{
	static string[64];

	dialogSetStyle(DIALOG_STYLE_LIST);
	dialogSetInfo("");
	new Lang:lang = GetPlayerLang(playerid);
	
	new i = 1;
	while(i == i)
	{
	    format(string, sizeof(string), "%s%iGMenuVehSpawn", DIALOG_INFO_PREFIX, i);
	    if(!langIsTextExist(lang, string))  break;
	    dialogAppendNewLine(string);
	    i++;
	}
	return 1;
}
dialogResponse(GMenuVehSpawn)
{
	if(response)
	{
		itemSetInt(itemGetByID(Player, playerid), "doplistitem", listitem);
		ShowDialog(playerid, Dialog:GMenuVehSpawnSub);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuVeh);
	}
	return 1;
}

dialogShow(GMenuVehSpawnSub)
{
	static string[64];

	dialogSetStyle(DIALOG_STYLE_LIST);
	dialogSetInfo("");
	
	new doplistitem = itemGetInt(itemGetByID(Player, playerid), "doplistitem");
	if(doplistitem >= sizeof(veh_menu)) return 1;
	format(string, sizeof(string), "%s%iGMenuVehSpawn", DIALOG_INFO_PREFIX, doplistitem+1);
	langGet(GetPlayerLang(playerid), string, string, sizeof(string));
    dialogFormatCaption(string);
    
    new i = 0;
	new model;
	while(i == i)
    {
        model = veh_menu[doplistitem][i];
        if(model == 0)  break;
        format(string, sizeof(string), "vehName%i", model);
        dialogAppendNewLine(string);
        i++;
    }
    
	return 1;
}
dialogResponse(GMenuVehSpawnSub)
{
	if(response)
	{
		new doplistitem = itemGetInt(itemGetByID(Player, playerid), "doplistitem");

		if(doplistitem >= sizeof(veh_menu) || listitem >= sizeof(veh_menu[]))
		{
		    return 1;
		}
		new modelid = veh_menu[doplistitem][listitem];
		if(modelid < 400 || modelid > 611)  return 1;

		if(GetPlayerLobby(playerid) == lobbyGetByID(worldGetByName("Hub"), 0))
		{
			if(modelid == 425)//Hunter
			{
			    ShowDialog(playerid, Dialog:BuyHunter);
			    return 1;
			}
			if(modelid == 432)//Rhino
			{
			    ShowDialog(playerid, Dialog:BuyRhino);
			    return 1;
			}
			if(modelid == 520)//Hydra
			{
			    ShowDialog(playerid, Dialog:BuyHydra);
			    return 1;
			}
		}

		if(modelid == 612)
		{
		    DestroySelfVehicle(playerid);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
			SendMessage(playerid, "mNewVehicle", inputtext, 0, 0);
		}
		else
		{
		    new color1 = random(256);
			new color2 = random(256);

			if(modelid == 416)	color1 = 8, color2 = 3;
			if(modelid == 407 || modelid == 544)
			{
			    color1 = 3, color2 = 8;
			}
			if(modelid == 433 || modelid == 427 || modelid == 490 || modelid == 528 ||
			modelid == 523 || modelid == 470 || modelid == 596 || modelid == 597 ||
			modelid == 598 || modelid == 599 || modelid == 432 || modelid == 428 ||
			modelid == 601)
			{
				color1 = 0, color2 = 1;
			}
			if(modelid == 420 || modelid == 438)
			{
			    color1 = 6, color2 = 6;
			}
			FastCreateSelfVehicle(playerid, modelid, color1, color2);
			SendMessage(playerid, "mNewVehicle", inputtext, color1, color2);
		}
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuVehSpawn);
	}
	return 1;
}

dialogShowStandart(BuyHunter, DIALOG_STYLE_MSGBOX);
dialogResponse(BuyHunter)
{
	if(GetPlayerServerMoney(playerid) < 1000000)
	{
	    SendMessage(playerid, "wNoHunter");
//	    ShowVehicleMenu(playerid, 0, GetPlayerLang(playerid));
	    return 1;
	}
	GivePlayerServerMoney(playerid, -1000000);
	FastCreateSelfVehicle(playerid, 425, 0, 0);
	SendMessage(playerid, "mNewHunter");
	return 1;
}

dialogShowStandart(BuyRhino, DIALOG_STYLE_MSGBOX);
dialogResponse(BuyRhino)
{
	if(GetPlayerServerMoney(playerid) < 1000000)
	{
	    SendMessage(playerid, "wNoRhino");
//	    ShowVehicleMenu(playerid, 0, GetPlayerLang(playerid));
	    return 1;
	}
	GivePlayerServerMoney(playerid, -1000000);
	FastCreateSelfVehicle(playerid, 432, 0, 0);
	SendMessage(playerid, "mNewRhino");
	return 1;
}

dialogShowStandart(BuyHydra, DIALOG_STYLE_MSGBOX);
dialogResponse(BuyHydra)
{
	if(GetPlayerServerMoney(playerid) < 1000000)
	{
	    SendMessage(playerid, "wNoHydra");
//	    ShowVehicleMenu(playerid, 0, GetPlayerLang(playerid));
	    return 1;
	}
	GivePlayerServerMoney(playerid, -1000000);
	FastCreateSelfVehicle(playerid, 520, 0, 0);
	SendMessage(playerid, "mNewHydra");
	return 1;
}

#define DialogTunTemplate(); \
new vehicleid; \
if(!itemGetInt(itemGetByID(Player, playerid), "TunSelf")) \
{ \
    vehicleid = GetPlayerVehicleID(playerid); \
    if(!IsValidVehicle(vehicleid) || GetPlayerVehicleSeat(playerid) != 0) \
    { \
        SendMessage(playerid, "wGMenuTunNoVeh"); \
        return 1; \
    } \
} \
else \
{ \
    vehicleid = GetPlayerSelfVehicle(playerid); \
    if(!IsValidVehicle(vehicleid)) \
    { \
        SendMessage(playerid, "wGMenuTunNoSelfVeh"); \
        return 1; \
    } \
}

dialogShowStandart(GMenuTun, DIALOG_STYLE_LIST);
dialogResponse(GMenuTun)
{
	if(response)
	{
        DialogTunTemplate();
        
	    new info;
	    new temp;
	    switch(listitem)
	    {
			case 0: ShowDialog(playerid, Dialog:GMenuTunWheels);
			case 1:
			{
			    AddVehicleHydraulic(vehicleid);
			    PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
			}
			case 2:
			{
			    RemoveVehicleHydraulic(vehicleid);
			    PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
			case 3: ShowDialog(playerid, Dialog:GMenuTunColor);
			case 4: ShowDialog(playerid, Dialog:GMenuTunPaintjob);
			case 5:	ShowDialog(playerid, Dialog:GMenuTunBody);
			case 6:	ShowDialog(playerid, Dialog:GMenuTunOther);
			case 7:
			{
			    GetVehicleDamageStatus(vehicleid, temp, temp, info, temp);
			    itemSetInt(itemGetByID(Player, playerid), "TunInfo", info);
				ShowDialog(playerid, Dialog:GMenuTunLights);
			}
			case 8: ShowDialog(playerid, Dialog:GMenuTunRemoveDetail);
			case 9: ShowDialog(playerid, Dialog:GMenuTunRemoveBumpers);
			case 10:
			{
			    GetVehicleDamageStatus(vehicleid, temp, temp, temp, info);
			    itemSetInt(itemGetByID(Player, playerid), "TunInfo", info);
				ShowDialog(playerid, Dialog:GMenuTunPopTire);
			}
	    }
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuVeh);
	}
	return 1;
}

dialogShowStandart(GMenuTunWheels, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunWheels)
{
	if(response)
	{
	    DialogTunTemplate();
		new wheeltype = 0;
		switch(listitem)
		{
		    case 0:     wheeltype = 1025;
		    case 1:		wheeltype = 1073;
		    case 2:		wheeltype = 1074;
		    case 3:		wheeltype = 1076;
		    case 4:		wheeltype = 1077;
		    case 5:		wheeltype = 1075;
		    case 6:		wheeltype = 1079;
		    case 7:		wheeltype = 1078;
		    case 8:		wheeltype = 1080;
		    case 9:		wheeltype = 1081;
		    case 10:	wheeltype = 1082;
		    case 11:	wheeltype = 1083;
		    case 12:	wheeltype = 1084;
		    case 13:	wheeltype = 1085;
		    case 14:    wheeltype = 1096;
		    case 15:    wheeltype = 1097;
		    case 16:    wheeltype = 1098;
		}
		if(wheeltype == 0)
		{
		    new comp = GetVehicleComponentInSlot(vehicleid, 7);
		    if(comp)
		    {
				RemoveVehicleComponent(vehicleid, comp);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
			ShowDialogAgain();
			return 1;
		}
  		AddVehicleComponent(vehicleid, wheeltype);
  		PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
  		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunColor, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunColor)
{
	if(response)
	{
	    DialogTunTemplate();
		new color = 0;
		switch(listitem)
		{
		    case 0:		color = 3;
		    case 1:		color = 175;
		    case 2:		color = 79;
		    case 3:		color = 211;
		    case 4:		color = 6;
		    case 5:		color = 222;
		    case 6:		color = 86;
		    case 7:     color = 186;
		    case 8:     color = 126;
		    case 9:		color = 155;
		    case 10:	color = 9;
		    case 11:	color = 0;
		    case 12:	color = 1;
		    case 13:
			{
			    ShowDialog(playerid, Dialog:GMenuTunColor2);
			    return 1;
			}
			case 14:
			{
				color = random(256);
				SendMessage(playerid, "mRandomColour", color);
			}
			case 15:
			{
			    new color1 = random(256);
				new color2 = random(256);
				ChangeVehicleColor(vehicleid, color1, color2);
				PlayerPlaySound(playerid,1134,0.0,0.0,0.0);
				SendMessage(playerid, "mRandomColours", color1, color2);
				ShowDialogAgain();
				return 1;
			}
		}
		ChangeVehicleColor(vehicleid, color, color);
		PlayerPlaySound(playerid,1134,0.0,0.0,0.0);
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunColor2, DIALOG_STYLE_INPUT);
dialogResponse(GMenuTunColor2)
{
	if(response)
	{
		DialogTunTemplate();
		new color = strval(inputtext);
		if(color < 0 || color > 255)
		{
		    SendMessage(playerid, "wIncorrectColor");
		    ShowDialogAgain();
		    return 1;
		}
		itemSetInt(itemGetByID(Player, playerid), "doplistitem", color);
		ShowDialog(playerid, Dialog:GMenuTunColor3);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTunColor);
	}
	return 1;
}

dialogShowStandart(GMenuTunColor3, DIALOG_STYLE_INPUT);
dialogResponse(GMenuTunColor3)
{
	if(response)
	{
	    DialogTunTemplate();
		new color = strval(inputtext);
		if(color < 0 || color > 255)
		{
		    SendMessage(playerid, "wIncorrectColor");
		    ShowDialogAgain();
		    return 1;
		}
		ChangeVehicleColor(vehicleid, itemGetInt(itemGetByID(Player, playerid), "doplistitem"), color);
		PlayerPlaySound(playerid,1134,0.0,0.0,0.0);
	}
	ShowDialog(playerid, Dialog:GMenuTunColor);
	return 1;
}

dialogShowStandart(GMenuTunPaintjob, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunPaintjob)
{
	if(response)
	{
	    DialogTunTemplate();
		ChangeVehiclePaintjob(vehicleid,listitem);
	    PlayerPlaySound(playerid,1134,0.0,0.0,0.0);
	    ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunBody, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunBody)
{
	if(response)
	{
     	DialogTunTemplate();
		new modelid = GetVehicleModel(vehicleid);
		if(modelid != 562 && modelid != 560 && modelid != 565 &&
		modelid != 559 && modelid != 561 && modelid != 558)
		{
		    SendMessage(playerid, "wGMenuTunBodyNoArch");
		    ShowDialogAgain();
		    return 1;
		}
	
		if(listitem == 0)//Передний бампер X-flow
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1172);
			if(modelid == 560)AddVehicleComponent(vehicleid,1170);
			if(modelid == 565)AddVehicleComponent(vehicleid,1152);
			if(modelid == 559)AddVehicleComponent(vehicleid,1173);
			if(modelid == 561)AddVehicleComponent(vehicleid,1157);
			if(modelid == 558)AddVehicleComponent(vehicleid,1165);

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 1)//Передний бампер Alien
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1171);
			if(modelid == 560)AddVehicleComponent(vehicleid,1169);
			if(modelid == 565)AddVehicleComponent(vehicleid,1153);
			if(modelid == 559)AddVehicleComponent(vehicleid,1160);
			if(modelid == 561)AddVehicleComponent(vehicleid,1155);
			if(modelid == 558)AddVehicleComponent(vehicleid,1166);

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 2)//Передний бампер стандарт
		{
			new dop;
			dop = GetVehicleComponentInSlot(vehicleid, 10);
			if(dop != 0)
			{
				RemoveVehicleComponent(vehicleid, dop);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
		}
		if(listitem == 3)//Задний бампер X-Flow
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1148);
			if(modelid == 560)AddVehicleComponent(vehicleid,1140);
			if(modelid == 565)AddVehicleComponent(vehicleid,1151);
			if(modelid == 559)AddVehicleComponent(vehicleid,1161);
			if(modelid == 561)AddVehicleComponent(vehicleid,1156);
			if(modelid == 558)AddVehicleComponent(vehicleid,1167);

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 4)//Задний бампер Alien
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1149);
			if(modelid == 560)AddVehicleComponent(vehicleid,1141);
			if(modelid == 565)AddVehicleComponent(vehicleid,1150);
			if(modelid == 559)AddVehicleComponent(vehicleid,1159);
			if(modelid == 561)AddVehicleComponent(vehicleid,1154);
			if(modelid == 558)AddVehicleComponent(vehicleid,1168);

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 5)//Задний бампер стандарт
		{
			new dop;
			dop = GetVehicleComponentInSlot(vehicleid, 11);
			if(dop != 0)
			{
				RemoveVehicleComponent(vehicleid, dop);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
		}
		if(listitem == 6)//Спойлер X-Flow
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1146);
			if(modelid == 560)AddVehicleComponent(vehicleid,1139);
			if(modelid == 565)AddVehicleComponent(vehicleid,1050);
			if(modelid == 559)AddVehicleComponent(vehicleid,1158);
			if(modelid == 561)AddVehicleComponent(vehicleid,1060);
			if(modelid == 558)AddVehicleComponent(vehicleid,1163);

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 7)//Спойлер Alien
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1147);
			if(modelid == 560)AddVehicleComponent(vehicleid,1138);
			if(modelid == 565)AddVehicleComponent(vehicleid,1049);
			if(modelid == 559)AddVehicleComponent(vehicleid,1162);
			if(modelid == 561)AddVehicleComponent(vehicleid,1058);
			if(modelid == 558)AddVehicleComponent(vehicleid,1164);

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 8)//Удалить спойлер
		{
			new dop;
			dop = GetVehicleComponentInSlot(vehicleid, 0);
			if(dop != 0)
			{
				RemoveVehicleComponent(vehicleid, dop);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
		}
		if(listitem == 9)//Боковые юбки X-Flow
		{
			if(modelid == 562)
			{
				AddVehicleComponent(vehicleid,1041);
				AddVehicleComponent(vehicleid,1039);
			}
			if(modelid == 560)
			{
				AddVehicleComponent(vehicleid,1031);
				AddVehicleComponent(vehicleid,1030);
			}
			if(modelid == 565)
			{
				AddVehicleComponent(vehicleid,1052);
				AddVehicleComponent(vehicleid,1048);
			}
			if(modelid == 559)
			{
				AddVehicleComponent(vehicleid,1070);
				AddVehicleComponent(vehicleid,1072);
			}
			if(modelid == 561)
			{
				AddVehicleComponent(vehicleid,1057);
				AddVehicleComponent(vehicleid,1063);
			}
			if(modelid == 558)
			{
				AddVehicleComponent(vehicleid,1093);
            	AddVehicleComponent(vehicleid,1095);
			}

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 10)//Боковые юбки Alien
		{
			if(modelid == 562)
			{
				AddVehicleComponent(vehicleid,1036);
				AddVehicleComponent(vehicleid,1040);
			}
			if(modelid == 560)
			{
				AddVehicleComponent(vehicleid,1026);
				AddVehicleComponent(vehicleid,1027);
			}
			if(modelid == 565)
			{
				AddVehicleComponent(vehicleid,1051);
				AddVehicleComponent(vehicleid,1047);
			}
			if(modelid == 559)
			{
				AddVehicleComponent(vehicleid,1069);
				AddVehicleComponent(vehicleid,1071);
			}
			if(modelid == 561)
			{
				AddVehicleComponent(vehicleid,1056);
				AddVehicleComponent(vehicleid,1062);
			}
			if(modelid == 558)
			{
				AddVehicleComponent(vehicleid,1090);
            	AddVehicleComponent(vehicleid,1094);
			}

			PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 11)//Боковые юбки стандарт
		{
			new dop;
			dop = GetVehicleComponentInSlot(vehicleid, 3);
			if(dop != 0)
			{
				RemoveVehicleComponent(vehicleid, dop);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
		}
		if(listitem == 12)//Воздухозаборник X-Flow
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1035);
			if(modelid == 560)AddVehicleComponent(vehicleid,1033);
			if(modelid == 565)AddVehicleComponent(vehicleid,1053);
			if(modelid == 559)AddVehicleComponent(vehicleid,1068);
			if(modelid == 561)AddVehicleComponent(vehicleid,1061);
			if(modelid == 558)AddVehicleComponent(vehicleid,1091);

        	PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 13)//Воздухозаборник Alien
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1038);
			if(modelid == 560)AddVehicleComponent(vehicleid,1032);
			if(modelid == 565)AddVehicleComponent(vehicleid,1054);
			if(modelid == 559)AddVehicleComponent(vehicleid,1067);
			if(modelid == 561)AddVehicleComponent(vehicleid,1055);
			if(modelid == 558)AddVehicleComponent(vehicleid,1088);

            PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 14)//Удалить воздухозаборник
		{
			new dop;
			dop = GetVehicleComponentInSlot(vehicleid, 2);
			if(dop != 0)
			{
				RemoveVehicleComponent(vehicleid, dop);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
		}
		if(listitem == 15)//Выхлоп X-flow
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1037);
			if(modelid == 560)AddVehicleComponent(vehicleid,1029);
			if(modelid == 565)AddVehicleComponent(vehicleid,1045);
			if(modelid == 559)AddVehicleComponent(vehicleid,1066);
			if(modelid == 561)AddVehicleComponent(vehicleid,1059);
			if(modelid == 558)AddVehicleComponent(vehicleid,1089);

            PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
        if(listitem == 16)//Выхлоп Alien
		{
			if(modelid == 562)AddVehicleComponent(vehicleid,1034);
			if(modelid == 560)AddVehicleComponent(vehicleid,1028);
			if(modelid == 565)AddVehicleComponent(vehicleid,1046);
			if(modelid == 559)AddVehicleComponent(vehicleid,1065);
			if(modelid == 561)AddVehicleComponent(vehicleid,1064);
			if(modelid == 558)AddVehicleComponent(vehicleid,1092);

            PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		if(listitem == 17)//Выхлоп стандарт
		{
			new dop;
			dop = GetVehicleComponentInSlot(vehicleid, 6);
			if(dop != 0)
			{
				RemoveVehicleComponent(vehicleid, dop);
				PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
			}
		}
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunOther, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunOther)
{
	if(response)
	{
	    DialogTunTemplate();
		switch(listitem)
		{
		    case 0:
			{
				ShowDialog(playerid, Dialog:GMenuTunNeon);
				return 1;
			}
			case 1:
			{
				ShowDialog(playerid, Dialog:GMenuTunNumPlate);
				return 1;
			}
			case 2:		VehicleBootOpen(vehicleid);
		    case 3: 	VehicleBootClose(vehicleid);
		    case 4:		VehicleBonnetOpen(vehicleid);
		    case 5: 	VehicleBonnetClose(vehicleid);
		    case 6:		VehicleLightsEnable(vehicleid);
		    case 7: 	VehicleLightsDisable(vehicleid);
		    case 8:		VehicleDoorsOpen(vehicleid);
		    case 9: 	VehicleDoorsClose(vehicleid);
		    case 10:	VehicleAlarmEnable(vehicleid);
		    case 11: 	VehicleAlarmDisable(vehicleid);
		    case 12:	VehicleEngineEnable(vehicleid);
		    case 13: 	VehicleEngineDisable(vehicleid);
		}
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunNeon, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunNeon)
{
	if(response)
	{
	    DialogTunTemplate();
		new Neon:neontype;
		switch(listitem)
		{
		    case 0:     neontype = NEON_RED;
		    case 1:     neontype = NEON_BLUE;
		    case 2:     neontype = NEON_GREEN;
		    case 3:     neontype = NEON_YELLOW;
		    case 4:     neontype = NEON_PINK;
		    case 5:     neontype = NEON_WHITE;
		    case 6:     return RemoveVehicleNeon(vehicleid);
		    default:    neontype = NEON_NONE;
		}
        AddVehicleNeon(vehicleid, neontype);
        ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTunOther);
	}
	return 1;
}

dialogShowStandart(GMenuTunNumPlate, DIALOG_STYLE_INPUT);
dialogResponse(GMenuTunNumPlate)
{
	if(response)
	{
	    DialogTunTemplate();
	    if(!strlen(inputtext))
	    {
	        SendMessage(playerid, "wGMenuTunNumPlateEmpty");
            ShowDialogAgain();
	        return 1;
		}
		if(!IsInput(inputtext))
		{
		    SendMessage(playerid, "wGMenuInValidSymbol");
			ShowDialogAgain();
			return 1;
		}
	    if(strlen(inputtext) > 10)
	    {
	        SendMessage(playerid, "wGMenuTunNumPlateTooLong");
            ShowDialogAgain();
	        return 1;
		}
    
	    new List:playerlist = lobbyGroupGet(Lobby:itemGetItem(itemGetByID(Vehicle, vehicleid), "Lobby"), "Player");
	    new List:templist = listCreate();
	    new Pointer:tempptr;

	    listForeach(plid:playerlist)
	    {
	        if(GetPlayerVehicleID(plid) != vehicleid) continue;
	        tempptr = MEM_new(2);
	        MEM_set_val(tempptr, 0, plid);
	        MEM_set_val(tempptr, 1, GetPlayerVehicleSeat(plid));
         	listPush(templist, tempptr);
	    }
        
        new Float:x, Float:y, Float:z, Float:rz;
        GetVehiclePos(vehicleid, x, y, z);
        GetVehicleZAngle(vehicleid, rz);
        SetVehicleNumberPlate(vehicleid, inputtext);
		SetVehicleToRespawn(vehicleid);
        SetVehiclePos(vehicleid, x, y, z);
		SetVehicleZAngle(vehicleid, rz);
        
	    new plid;
	    new seatid;
	    listForeachPtr(node:templist)
	    {
	        plid = MEM_get_val(node, 0);
	        seatid = MEM_get_val(node, 1);
	        PutPlayerInVehicle(plid, vehicleid, seatid);
	    }
		listDestroyPtr(templist);
        
        ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTunOther);
	}
	return 1;
}

dialogShow(GMenuTunLights)
{
	new tun = itemGetInt(itemGetByID(Player, playerid), "TunInfo");

	new front_left = tun & 0b00000001;
	new front_right = tun & 0b00000100;
	new back = tun & 0b01000000;

	new color1, color2, color3;
	if(!front_left)	color1 = 0x00FF00;
		else	color1 = 0xFF0000;
		
	if(!front_right)	color2 = 0x00FF00;
		else	color2 = 0xFF0000;
		
	if(!back)	color3 = 0x00FF00;
		else	color3 = 0xFF0000;

    dialogFormatInfo(color1, color2, color3);

	return 1;
}

dialogResponse(GMenuTunLights)
{
	if(response)
	{
	    DialogTunTemplate();
	    
	    new Item:player = itemGetByID(Player, playerid);
	    new tun = itemGetInt(player, "TunInfo");
	    
	    switch(listitem)
	    {
	        case 0: tun ^= 0b00000001;
	        case 1: tun ^= 0b00000100;
	        case 2: tun ^= 0b01000000;
	    }
	    itemSetInt(player, "TunInfo", tun);
		
		new panels, door, tires, temp;
		GetVehicleDamageStatus(vehicleid, panels, door, temp, tires);
		UpdateVehicleDamageStatus(vehicleid, panels, door, tun, tires);
        ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunRemoveDetail, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunRemoveDetail)
{
	if(response)
	{
	    DialogTunTemplate();
	    
	    if(IsPlayerAutoRepair(playerid))
	    {
	        SendMessage(playerid, "Авторемонт отключен");
            PlayerAutoRepairDisable(playerid);
		}
    
		new panels, doors, lights, tires;
		GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
		
		new dop = 0;
		switch(listitem)
		{
		    case 0:     dop = 4;
		    case 1:     dop = 1024;
		    case 2:     dop = 1028;
		    case 3:     dop = 262144;
		    case 4:     dop = 262148;
		    case 5:     dop = 263168;
		    case 6:     dop = 263172;
		    case 7:     dop = 67108864;
		    case 8:     dop = 67108868;
		    case 9:		dop = 67109888;
		    case 10:	dop = 67109892;
		    case 11:	dop = 67371008;
		    case 12:	dop = 67371012;
		    case 13:	dop = 67372032;
		    case 14:	dop = 67372036;
		}
		if(dop == 0)
		{
			if(doors != 0)
			{
				UpdateVehicleDamageStatus(vehicleid, panels, 0, lights, tires);
            	PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
			}
		}
		else
		{
			UpdateVehicleDamageStatus(vehicleid, panels, 0, lights, tires);
			UpdateVehicleDamageStatus(vehicleid, panels, dop, lights, tires);
            PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
		}
		
	    ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShowStandart(GMenuTunRemoveBumpers, DIALOG_STYLE_LIST);
dialogResponse(GMenuTunRemoveBumpers)
{
	if(response)
	{
	    DialogTunTemplate();
	    
	    if(IsPlayerAutoRepair(playerid))
	    {
	        SendMessage(playerid, "wGMenuAutorepairOff");
            PlayerAutoRepairDisable(playerid);
		}
    
		new panels, doors, lights, tires;
		GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	    
	    new dop = 0;
	    switch(listitem)
	    {
	        case 0: dop = 3145728;
	        case 1: dop = 50331648;
	        case 2: dop = 53477376;
	    }
		
		if(dop == 0)
		{
			UpdateVehicleDamageStatus(vehicleid, 0, doors, lights, tires);
        	PlayerPlaySound(playerid,1133,0.0,0.0,0.0);
		}
		else
		{
			UpdateVehicleDamageStatus(vehicleid, 0, doors, lights, tires);
			UpdateVehicleDamageStatus(vehicleid, dop, doors, lights, tires);
            PlayerPlaySound(playerid,5202,0.0,0.0,0.0);
		}

        ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

dialogShow(GMenuTunPopTire)
{
	new tun = itemGetInt(itemGetByID(Player, playerid), "TunInfo");

	new front_left = tun & 0b0010;
	new front_right = tun & 0b1000;
	new back_left = tun & 0b0100;
	new back_right = tun & 0b0001;
	
	new color1, color2, color3, color4;

	if(!front_left) color1 = 0x00FF00;
	    else    color1 = 0xFF0000;

	if(!front_right) color2 = 0x00FF00;
	    else    color2 = 0xFF0000;
	    
	if(!back_left) color3 = 0x00FF00;
	    else    color3 = 0xFF0000;
	    
	if(!back_right) color4 = 0x00FF00;
	    else    color4 = 0xFF0000;

	dialogFormatInfo(color1, color2, color3, color4);
	return 1;
}

dialogResponse(GMenuTunPopTire)
{
	if(response)
	{
	    DialogTunTemplate();
	    
	    new Item:player = itemGetByID(Player, playerid);
	    
	    new tun = itemGetInt(player, "TunInfo");

		switch(listitem)
		{
		    case 0: tun ^= 0b0010;
		    case 1: tun ^= 0b1000;
		    case 2: tun ^= 0b0100;
		    case 3: tun ^= 0b0001;
		}
		
		itemSetInt(player, "TunInfo", tun);

		new panels, door, lights, temp;
		GetVehicleDamageStatus(vehicleid, panels, door, lights, temp);
		UpdateVehicleDamageStatus(vehicleid, panels, door, lights, tun);
        ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuTun);
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
//---------------------------------Транспорт----------------------------------//
/*-----------------------------------Конец------------------------------------*/
/*----------------------------------------------------------------------------*/




dialogShowStandart(GMenuStuff, DIALOG_STYLE_LIST);
dialogResponse(GMenuStuff)
{
	if(response)
	{
	    new weap = 0, ammo = 0;
	    switch(listitem)
	    {
			case 0:		weap = 1, ammo = 1;
			case 1: 	weap = 2, ammo = 1;
			case 2: 	weap = 3, ammo = 1;
			case 3: 	weap = 4, ammo = 1;
			case 4: 	weap = 5, ammo = 1;
			case 5: 	weap = 6, ammo = 1;
			case 6: 	weap = 7, ammo = 1;
			case 7: 	weap = 8, ammo = 1;
			case 8: 	weap = 9, ammo = 1;
			case 9: 	weap = 41, ammo = 1000;
			case 10: 	weap = 42, ammo = 1000;
			case 11: 	weap = 43, ammo = 1000;
			case 12: 	weap = 14, ammo = 1;
			case 13: 	weap = 15, ammo = 1;
			case 14: 	weap = 44, ammo = 1;
			case 15: 	weap = 45, ammo = 1;
			case 16: 	weap = 46, ammo = 1;
			case 17: 	weap = 10, ammo = 1;
			case 18: 	weap = 11, ammo = 1;
			case 19: 	weap = 12, ammo = 1;
			case 20: 	weap = 13, ammo = 1;
			case 21:
			{
			    ClearSlotGeneralWeapon(playerid, 0);
			    ClearSlotGeneralWeapon(playerid, 1);
			    ClearSlotGeneralWeapon(playerid, 9);
			    ClearSlotGeneralWeapon(playerid, 10);
			    ClearSlotGeneralWeapon(playerid, 11);
			    UpdatePlayerGeneralWeapon(playerid);
			/*
				new weap_buffer[13], ammo_buffer[13];
				for(new i = 0; i < 13; i++)
				{
					GetPlayerWeaponData(playerid, i, weap_buffer[i], ammo_buffer[i]);
				}
				ResetPlayerWeapons(playerid);
			    for(new i = 2; i < 9; i++)
			    {
			        GivePlayerWeapon(playerid, weap_buffer[i], ammo_buffer[i]);
			    }
				GivePlayerWeapon(playerid, weap_buffer[12], ammo_buffer[12]);
			*/
			    ShowDialogAgain();
				return 1;
			}
		}
		new slot = GetWeaponSlot(weap);
 		if(slot < 0 || slot > 12)
		{
		    return 1;
		}
		new pweap, pammo;
		GetPlayerGeneralWeapon(playerid, slot, pweap, pammo);
//        GetPlayerWeaponData(playerid, slot, pweap, pammo);

		if(ammo == 1)
		{
		    if(pweap != weap)
		    {
		        GivePlayerGeneralWeapon(playerid, weap, ammo);
//				GivePlayerWeapon(playerid, weap, ammo);
			}
		}
		else
		{
	        if(pweap == weap && pammo+ammo > 1000)
	        {
				SendMessage(playerid, "wGMenuWeapTooMany");
				ShowDialogAgain();
				return 1;
   			}
   			GivePlayerGeneralWeapon(playerid, weap, ammo);
//		    GivePlayerWeapon(playerid, weap, ammo);
		}
		UpdatePlayerGeneralWeapon(playerid);
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShowStandart(GMenuWeap, DIALOG_STYLE_LIST);
dialogResponse(GMenuWeap)
{
	if(response)
	{
	    new weap = 0, ammo = 0, money = 0;
	    switch(listitem)
	    {
			case 0:		weap = 22, ammo = 100, money = 10000;
			case 1:		weap = 23, ammo = 100, money = 10000;
			case 2:		weap = 24, ammo = 100, money = 10000;
			case 3:		weap = 25, ammo = 100, money = 30000;
			case 4:		weap = 26, ammo = 100, money = 30000;
			case 5:		weap = 27, ammo = 100, money = 30000;
			case 6:		weap = 28, ammo = 300, money = 30000;
			case 7:		weap = 29, ammo = 300, money = 30000;
			case 8:		weap = 32, ammo = 300, money = 30000;
			case 9:		weap = 30, ammo = 300, money = 30000;
			case 10:	weap = 31, ammo = 300, money = 30000;
			case 11:	weap = 33, ammo = 100, money = 30000;
			case 12:	weap = 34, ammo = 100, money = 30000;
            case 13:	weap = 35, ammo = 100, money = 50000;
            case 14:	weap = 36, ammo = 100, money = 50000;
            case 15:	weap = 37, ammo = 500, money = 10000;
            case 16:	weap = 38, ammo = 500, money = 50000;
            case 17:	weap = 16, ammo = 100, money = 50000;
            case 18:	weap = 17, ammo = 100, money = 50000;
            case 19:	weap = 18, ammo = 100, money = 50000;
            case 20:	weap = 39, ammo = 100, money = 50000;
			case 21:
			{
			    /*
				new weap_buffer[13], ammo_buffer[13];
				for(new i = 0; i < 13; i++)
				{
					GetPlayerWeaponData(playerid, i, weap_buffer[i], ammo_buffer[i]);
				}
				ResetPlayerWeapons(playerid);
			    GivePlayerWeapon(playerid, weap_buffer[0], ammo_buffer[0]);
			    GivePlayerWeapon(playerid, weap_buffer[1], ammo_buffer[1]);
			    GivePlayerWeapon(playerid, weap_buffer[9], ammo_buffer[9]);
			    GivePlayerWeapon(playerid, weap_buffer[10], ammo_buffer[10]);
			    GivePlayerWeapon(playerid, weap_buffer[11], ammo_buffer[11]);
			    */
			    ClearSlotGeneralWeapon(playerid, 2);
			    ClearSlotGeneralWeapon(playerid, 3);
			    ClearSlotGeneralWeapon(playerid, 4);
			    ClearSlotGeneralWeapon(playerid, 5);
			    ClearSlotGeneralWeapon(playerid, 6);
			    ClearSlotGeneralWeapon(playerid, 7);
			    ClearSlotGeneralWeapon(playerid, 8);
			    ClearSlotGeneralWeapon(playerid, 12);
			    UpdatePlayerGeneralWeapon(playerid);
				
				ShowDialogAgain();
				return 1;
			}
		}
		
		new slot = GetWeaponSlot(weap);
 		if(slot < 0 || slot > 12)
		{
		    return 1;
		}

		if(GetPlayerAdmin(playerid) == 0 && weap >= 35 && weap <= 38)
		{
		    SendMessage(playerid, "wGMenuNoAdminNoWeap");
		    return 1;
		}

		new pl_money = GetPlayerServerMoney(playerid);
		if(pl_money-money < 0)
		{
		    SendMessage(playerid, "wGMenuWeapNoMoney");
		    ShowDialogAgain();
		    return 1;
		}
		
		new pweap, pammo;
//        GetPlayerWeaponData(playerid, slot, pweap, pammo);
        GetPlayerGeneralWeapon(playerid, slot, pweap, pammo);
		if(weap != pweap && pweap != 0)
		{
		/*
		    new bufweap[13];
		    new bufammo[13];

			for(new i = 0; i < 13; i++)
			{
                GetPlayerWeaponData(playerid, i, bufweap[i], bufammo[i]);
			}
		    bufweap[slot] = weap;
		    ResetPlayerWeapons(playerid);
			for(new i = 0; i < 13; i++)
			{
			    GivePlayerWeapon(playerid, bufweap[i], bufammo[i]);
			}
		*/
		    SetPlayerGeneralWeapon(playerid, weap, pammo);
		}
		else
		{
            if(slot == 2 && pammo+ammo > 300 ||
			slot == 3 && pammo+ammo > 300 ||
			slot == 4 && pammo+ammo > 900 ||
			slot == 5 && pammo+ammo > 900 ||
			slot == 6 && pammo+ammo > 300 ||
			slot == 7 && pammo+ammo > 2500 ||
			slot == 8 && pammo+ammo > 300)
			{
			    SendMessage(playerid, "wGMenuWeapTooMany");
			    SetPlayerArmedWeapon(playerid, weap);
			    ShowDialogAgain();
			    return 1;
			}
			//GivePlayerWeapon(playerid, weap, ammo);
			GivePlayerGeneralWeapon(playerid, weap, ammo);
		}
		SetPlayerServerMoney(playerid, pl_money-money);
		SetPlayerArmedWeapon(playerid, weap);
		ShowDialogAgain();
		
		UpdatePlayerGeneralWeapon(playerid);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShowStandart(GMenuFreeWeap, DIALOG_STYLE_LIST);
dialogResponse(GMenuFreeWeap)
{
	if(response)
	{
	    new weap = 0, ammo = 0;
     	#pragma unused ammo
	    switch(listitem)
	    {
			case 0:		weap = 22;
			case 1:		weap = 23;
			case 2:		weap = 24;
			case 3:		weap = 25;
			case 4:		weap = 26;
			case 5:		weap = 27;
			case 6:		weap = 28;
			case 7:		weap = 29;
			case 8:		weap = 32;
			case 9:		weap = 30;
			case 10:	weap = 31;
			case 11:	weap = 33;
			case 12:	weap = 34;
            case 13:	weap = 35;
            case 14:	weap = 36;
            case 15:	weap = 37;
            case 16:	weap = 38;
            case 17:	weap = 16;
            case 18:	weap = 17;
            case 19:	weap = 18;
            case 20:	weap = 39;
			case 21:
			{
				new weap_buffer[13], ammo_buffer[13];
				for(new i = 0; i < 13; i++)
				{
					GetPlayerWeaponData(playerid, i, weap_buffer[i], ammo_buffer[i]);
				}
				ResetPlayerWeapons(playerid);
			    GivePlayerWeapon(playerid, weap_buffer[0], ammo_buffer[0]);
			    GivePlayerWeapon(playerid, weap_buffer[1], ammo_buffer[1]);
			    GivePlayerWeapon(playerid, weap_buffer[9], ammo_buffer[9]);
			    GivePlayerWeapon(playerid, weap_buffer[10], ammo_buffer[10]);
			    GivePlayerWeapon(playerid, weap_buffer[11], ammo_buffer[11]);

				ShowDialogAgain();
				return 1;
			}
		}

		new slot = GetWeaponSlot(weap);
 		if(slot < 0 || slot > 12)
		{
		    return 1;
		}

		new pweap, pammo;
        GetPlayerWeaponData(playerid, slot, pweap, pammo);
		if(weap != pweap && pweap != 0)
		{
		    new bufweap[13];
		    new bufammo[13];

			for(new i = 0; i < 13; i++)
			{
                GetPlayerWeaponData(playerid, i, bufweap[i], bufammo[i]);
			}
		    bufweap[slot] = weap;
		    bufammo[slot] = 10000;
		    ResetPlayerWeapons(playerid);
			for(new i = 0; i < 13; i++)
			{
			    GivePlayerWeapon(playerid, bufweap[i], bufammo[i]);
			}
		}
		else
		{
			GivePlayerWeapon(playerid, weap, 10000);
		}
		SetPlayerArmedWeapon(playerid, weap);
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

stock GMenuTP(playerid, Float:x, Float:y, Float:z, Float:rz, interior)
{
	if(!IsPlayerConnected(playerid))    return 0;
	
	new World:world = GetPlayerWorld(playerid);
	if(world != worldGetByName("Hub") && world != worldGetByName("DM"))
	{
	    SendMessage(playerid, "wWhereTFAreYou");
	    return 1;
	}
	if(world == worldGetByName("DM"))
	{
		PlayerEnterLobby(playerid, lobbyGetByID(worldGetByName("Hub"), 0));
	}
	UniversalTeleport(playerid, x, y, z, rz, interior);
	return 1;
}

dialogShowStandart(GMenuTP1, DIALOG_STYLE_LIST);
dialogResponse(GMenuTP1)
{
	if(response)
	{
	    new Float:x, Float:y, Float:z, Float:rz = 0.0, interior = 0;
	    
	    switch(listitem){

		case 0:     x = -312.7, y = 1532.72, z = 75.36+1, rz = 190.00;//ухо
		case 1:     x = 2492.34, y = -1666.33, z = 13.34+1;//грув
		case 2:     x = 334.21, y = -1812.10, z = 4.4+1;//пляж лс
		case 3:     x = 1263.47, y = -2027.47, z = 59.32+1;//холм лс
		case 4:     x = 1543.83, y = -1353.65, z = 329.47+1;//небоскреб лс
		case 5:     x = 1439.31, y = -2593.28, z = 13.55+1, rz = 270.00;//взлётка аэропорта лс
		case 6:     x = -14.90, y = -290.84, z = 5.05+1;//завод
		case 7:     x = 2413.09, y = 91.08, z = 26.47+1, rz = 90.00;//паломино крик
		case 8:     x = -2333.85, y = -1621.60, z = 483.72+1, rz = 215.00;//чиллиад
		case 9:     x = -1426.55, y = -949.91, z = 201.09+1;//холм села
		case 10:	x = -2205.31, y = -2148.74, z = 46.26+1;//запад села
		case 11:	x = 1243.24, y = 118.28, z = 20.39+1;//восток села
		case 12:	x = 22.46, y = -2659.57, z = 40.15+1;//кафе 69
		case 13:	x = -1108.11, y = -1621.52, z = 75.99+1, rz = 270.00;//ферма правды
		case 14:	x = -2633.45, y = 1359.5, z = 7.12+1;//клуб джиззи
		case 15:	x = -1981.07, y = 444.14, z = 35.17+1;//центр сф
		case 16:	x = -2405.46, y = -597.09, z = 132.65+1;//холм сф
		case 17:	x = -1653.02, y = -224.51, z = 14.14+1;//взлетка аэропорта сф
		case 18:	x = -2261.65, y = 2301.37, z = 4.44+1;//бэйсайд
		case 19:	x = -1514.96, y = 2533.43, z = 55.31+1;//эль квебрадос
		case 20:	x = 417.29, y = 2503.67, z = 16.48+1;//заброшка
        case 21:	x = 1766.46, y = 2769.93, z = 10.83+1, rz = 180.00;//прикл пайн
        case 22:	x = 214.16, y = 1866.24, z = 13.14+1;//зона 51
        case 23:	x = 2169.01, y = 1676.55, z = 10.82+1, rz = 82.72;//калигула
        case 24:	x = 685.67, y = 895.24, z = -39.61+1;//карьер
        case 25:	x = 1517.47, y = 1280.76, z = 10.81+1;//взлетка аэропорта лв

		}
	    GMenuTP(playerid, x, y, z, rz, interior);
	}
	else
	{
		ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShowStandart(GMenuActions, DIALOG_STYLE_LIST);
dialogResponse(GMenuActions)
{
	if(response)
	{
	    switch(listitem)
	    {
	        case 0:
	        {
				SetPlayerHealth(playerid, 100.0);
				SetPlayerArmour(playerid, 100.0);
				SendMessage(playerid, "mHealed");
	        }
			case 1:	ShowDialog(playerid, Dialog:GMenuAnim);
			case 2:	ShowDialog(playerid, Dialog:GMenuColor);
			case 3:	ShowDialog(playerid, Dialog:GMenuSkin);
			case 4:	ShowDialog(playerid, Dialog:GMenuTime);
			case 5:	ShowDialog(playerid, Dialog:GMenuWeather);
			case 6:	ShowDialog(playerid, Dialog:GMenuPassword);
			case 7:	ShowDialog(playerid, Dialog:GMenuFightingStyle);
			case 8: ShowDialog(playerid, Dialog:ChoiseLanguage);
	        case 9:
	        {
				SetPlayerHealth(playerid, 0.0);
				SetPlayerArmour(playerid, 0.0);
	        }
	        case 10:	ShowPlayerStats(playerid, playerid);
	    }
	}
	else
	{
		ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShowStandart(GMenuAnim, DIALOG_STYLE_LIST);
dialogResponse(GMenuAnim)
{
	if(response)
	{
	    switch(listitem)
	    {
	        case 0:		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);//поднять руки
			case 1: 	ApplyAnimation(playerid,"PED", "SEAT_down", 4.1, 0, 0, 0, 1, 0, 0);//сесть-1
			case 2: 	ApplyAnimation(playerid,"Attractors", "Stepsit_in", 4.1, 0, 0, 0, 1, 0, 0);//сесть-2
			case 3: 	ApplyAnimation(playerid,"CRACK", "crckidle2", 4.1 ,0 ,0 ,0 ,1 ,0 ,0);//лечь
			case 4: 	ApplyAnimation(playerid,"PED", "IDLE_chat", 4.1, 1, 1, 1, 0, 0, 0);//разговаривать
			case 5:
			{
			    new player = FindNearPlayerToAnim(playerid);
			    if(!IsPlayerConnected(player))    return 1;
				ApplyAnimation(playerid,"GANGS", "hndshkfa", 4.1, 0, 0, 0, 0, 0, 0);//рукопожатие
				ApplyAnimation(player,"GANGS", "hndshkfa", 4.1, 0, 0, 0, 0, 0, 0);//рукопожатие
			}
			case 6:
			{
			    new player = FindNearPlayerToAnim(playerid);
			    if(!IsPlayerConnected(player))    return 1;
				ApplyAnimation(playerid,"KISSING", "Playa_Kiss_02", 4.1, 0, 0, 0, 0, 0, 0);//поцелуй
				ApplyAnimation(player,"KISSING", "Playa_Kiss_02", 4.1, 0, 0, 0, 0, 0, 0);//поцелуй
				SetTimerEx("DopAnim22", 400, 0, "ii", playerid, player);
			}
			case 7:     SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);//звонить по телефону
			case 8:     SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);//убрать телефон
			case 9:     SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);//танец-1
			case 10:    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);//танец-2
			case 11:    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);//танец-3
			case 12:    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);//танец-4
			case 13:    ApplyAnimation(playerid,"PED", "WALK_drunk", 4.1, 1, 1, 1, 0, 0, 0);//набухаться
			case 14:    SetPlayerSpecialAction(playerid, 68);
			case 15:    ClearAnimations(playerid);//остановить анимацию
			case 16:    SetPlayerDrunkLevel(playerid, 0);//убрать степень опьянения
	    }
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuColor, DIALOG_STYLE_LIST);
dialogResponse(GMenuColor)
{
	if(response)
	{
		new color;
		switch(listitem)
  		{
			case 0:		color = 0xFF0000FF;
			case 1:		color = 0xFF3F3FFF;
			case 2:		color = 0xFF3F00FF;
			case 3:		color = 0xBF3F00FF;
			case 4:		color = 0xFF7F3FFF;
			case 5:		color = 0xFF7F00FF;
			case 6:		color = 0xFFFF00FF;
			case 7:		color = 0x3FFF3FFF;
			case 8:		color = 0x00FF00FF;
			case 9:		color = 0x00BF00FF;
			case 10:	color = 0x00FFFFFF;
			case 11:	color = 0x00BFFFFF;
			case 12:	color = 0x3F3FFFFF;
			case 13:	color = 0x0000FFFF;
			case 14:	color = 0x7F3FFFFF;
			case 15:	color = 0x7F00FFFF;
			case 16:	color = 0xFF00FFFF;
			case 17:	color = 0x7F7F7FFF;
			case 18:	color = 0x00FF0000;
		}
        SetPlayerGeneralColor(playerid, color);
		SetPlayerColor(playerid, color);
		SetPlayerMarkerForAll(playerid, color);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuSkin, DIALOG_STYLE_INPUT);
dialogResponse(GMenuSkin)
{
	if(response)
	{
		if(!IsInput(inputtext))
		{
		    SendMessage(playerid, "wGMenuInValidSymbol");
			ShowDialogAgain();
			return 1;
		}
		new skin = strval(inputtext);
	    if(skin < 0 || skin > 311)
		{
			SendMessage(playerid, "wGMenuSkinInValid");
			ShowDialogAgain();
			return 1;
		}
		PlayerSetInt(playerid, "Skin", skin);
		SetPlayerGeneralSkin(playerid, skin);
		SetPlayerSkin(playerid, skin);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuTime, DIALOG_STYLE_LIST);
dialogResponse(GMenuTime)
{
	if(response)
	{
	    if(listitem < 0 || listitem >= 24)  return 1;
		new time = listitem;
		SetPlayerTime(playerid, time, 0);
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuWeather, DIALOG_STYLE_INPUT);
dialogResponse(GMenuWeather)
{
	if(response)
	{
		new weat = strval(inputtext);
		
		if(weat == -1)
		{
		    weat = random(256);
		}
		SendMessage(playerid, "mGMenuWeather", weat);
		SetPlayerWeather(playerid, weat);
		
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuPassword, DIALOG_STYLE_INPUT);
dialogResponse(GMenuPassword)
{
	if(response)
	{
	    new len = strlen(inputtext);
		if(len == 0 || len < MIN_PASSWORD_LEN || len > MAX_PASSWORD_LEN)
		{
		    SendMessage(playerid, "wPasswordLen");
			ShowDialogAgain();
			return 1;
		}
		if(!PasswordControl(inputtext))
		{
		    SendMessage(playerid, "wPasswordInValid");
			ShowDialogAgain();
		    return 1;
		}
		new password[MAX_PASSWORD_LEN];
		PlayerGet(playerid, "Password", password);
		if(!strcmp(password, inputtext))
		{
		    SendMessage(playerid, "wGMenuPasswordAgain");
			ShowDialogAgain();
		    return 1;
		}
		printf("Игрок ''%s'' сменил пароль от аккаунта с ''%s'' на ''%s''", PlayerName(playerid), password, inputtext);
		SendMessage(playerid, "mGMenuPassword", inputtext);
		
		PlayerSet(playerid, "Password", password);
		ormCacheSave(ORMCache:itemGetInt(itemGetByID(Player, playerid), "Cache"), "Password");
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuFightingStyle, DIALOG_STYLE_LIST);
dialogResponse(GMenuFightingStyle)
{
	if(response)
	{
		new style;
		switch(listitem)
		{
		    case 0:     style = FIGHT_STYLE_NORMAL;
		    case 1:     style = FIGHT_STYLE_BOXING;
		    case 2:     style = FIGHT_STYLE_KUNGFU;
		    case 3:     style = FIGHT_STYLE_KNEEHEAD;
		    case 4:     style = FIGHT_STYLE_GRABKICK;
		    case 5:     style = FIGHT_STYLE_ELBOW;
		}
		SetPlayerFightingStyle(playerid, style);
	}
	else
	{
	    ShowDialog(playerid, Dialog:GMenuActions);
	}
	return 1;
}

dialogShowStandart(GMenuRules, DIALOG_STYLE_MSGBOX);
dialogResponse(GMenuRules)
{
	if(!response)
	{
	    ShowDialog(playerid, Dialog:GlobalMenu);
	}
	return 1;
}

dialogShow(GMenuAdmins)
{
    static tempstring[128];
    
	dialogSetStyle(DIALOG_STYLE_MSGBOX);
	
	new List:list = GetListAdmins();
	dialogFormatInfo(listLen(list));
	listForeach(plid:list)
	{
	    if(IsPlayerAdmin(plid))
	    {
	        dialogFormatAppendNewLine("iAdminNameRCON", PlayerName(plid), plid);
	    }
    	format(tempstring, sizeof(tempstring), "iAdminName%i", GetPlayerAdmin(plid));
        dialogFormatAppendNewLine(tempstring, PlayerName(plid), plid);
	}
	return 1;
}
dialogResponse(GMenuAdmins)
{
	if(response && itemGetInt(itemGetByID(Player, playerid), "doplistitem") != -1)
	{
		ShowDialogAgain();
	}
	return 1;
}

dialogShowStandart(GMenuAbout, DIALOG_STYLE_MSGBOX);




