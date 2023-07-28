#if defined inc_dia_login
	#endinput
#endif
#define inc_dia_login

dialogShowStandart(Login, DIALOG_STYLE_INPUT);
dialogResponse(Login)
{
	new check = 0;
	new Lang:langcheck;
	
	if(response)
	{
		if(!strlen(inputtext))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wLogPasswordEmpty");
		    return 1;
		}
		new password[MAX_PASSWORD_LEN];
		PlayerGet(playerid, "Password", password, sizeof(password));
		if(strcmp(inputtext, password))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wLogPasswordWrong");
		    return 1;
		}
		new Lang:lang = Lang:PlayerGetInt(playerid, "Lang");
		if(Lang:AccountGetInt(PlayerName(playerid), "Lang") != lang)
		{
			check = 1;
			langcheck = lang;
		}
		if(!PlayerLogin(playerid))
		{
		    ShowDialogAgain();
		    SendMessage(playerid, "wFailLog");
		    return 1;
		}
		if(check == 1)
		{
			PlayerSetInt(playerid, "Lang", _:langcheck);
		}
		if(PlayerGetInt(playerid, "Skin") == -1)
		{
		    ShowDialog(playerid, Dialog:LoginSkin);
		}
		else
		{
		    SetPlayerWorld(playerid, "Hub", 0);
		    ShowDialog(playerid, Dialog:LoginInfo);
		}
		SendMessage(playerid, "mSuccessLog");
	}
	else
	{
	    SendMessageToAll("mNoLogin", PlayerName(playerid), playerid);
	    printf("Игрок %s [%i] был кикнут за отказ от логирования", PlayerName(playerid), playerid);
	    KickPlayer(playerid);
	}
	return 1;
}

dialogShowStandart(LoginInfo, DIALOG_STYLE_MSGBOX);

