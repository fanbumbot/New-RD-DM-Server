#if defined inc_dia_logskin
	#endinput
#endif
#define inc_dia_logskin

dialogShowStandart(LoginSkin, DIALOG_STYLE_INPUT);
dialogResponse(LoginSkin)
{
	if(response)
	{
		new skin = strval(inputtext);
		if(skin < 0 || skin > 311)
		{
			SendMessage(playerid, "wGMenuSkinInValid");
		}
		PlayerSetInt(playerid, "Skin", skin);
		SetPlayerSkin(playerid, skin);
		SetPlayerGeneralSkin(playerid, skin);
		ShowDialogAgain();
	}
	else
	{
	    ShowDialog(playerid, Dialog:LoginInfo);
	    SetPlayerWorld(playerid, "Hub", 0);
	}
	return 1;
}

































