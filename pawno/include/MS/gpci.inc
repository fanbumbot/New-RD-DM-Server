#if defined mdg_gpci_inc
	#endinput
#endif
#define mdg_gpci_inc

#if !defined gpci
    native gpci(playerid, serial[], len);
#endif

static gpcibuffer[42];

stock IsGPCIAndroid(serial[])
{
	return (!strcmp(serial, "5638413348335738345A4536524D4A524539334B"));
}

stock IsGPCIPlayerAndroid(playerid)
{
    gpci(playerid, gpcibuffer, sizeof(gpcibuffer));
    return IsAndroid(gpcibuffer);
}

stock IsGPCIWrong(gpci[])
{
	new len = strlen(gpci);
	if(len != 40)  return 0;
	new c;
	for(new i = 0; i < len; i++)
	{
		c = gpci[i];
	    if((!(c >= 'a' && c <= 'z')) && (!(c >= 'A' && c <= 'Z')) && (!(c >= '0' && c <= '9')))
	    {
	        return 0;
	    }
	}
	return 1;
}

