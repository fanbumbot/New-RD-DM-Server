#if defined inc_dia_reg
	#endinput
#endif
#define inc_dia_reg

dialogShowStandart(Registration, DIALOG_STYLE_INPUT);
dialogResponse(Registration)
{
	ShowDialogAgain();
	if(response)
	{
	    new len = strlen(inputtext);
		if(len < MIN_PASSWORD_LEN || len > MAX_PASSWORD_LEN)
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
		if(!PlayerRegister(playerid, inputtext))
		{
		    ShowDialogAgain();
		    return 1;
		}
		if(!PlayerLogin(playerid))
		{
		    ShowDialogAgain();
		    return 1;
		}
		
  		ShowDialog(playerid, Dialog:LoginSkin);
		SendMessage(playerid, "mSuccessReg");
	}
	else
	{
	    SendMessageToAll("mNoRegister", PlayerName(playerid));
	    printf("Игрок %s [%i] был кикнут за отказ от регистрации", PlayerName(playerid), playerid);
	    KickPlayer(playerid);
	}
	return 1;
}
