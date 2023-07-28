#if defined inc_lang
	#endinput
#endif
#define inc_lang

enum eLang
{
	Lang:EN,
	Lang:RU
}

new Lang[eLang];

public OnScriptInit()
{
    Lang[EN] = langOpen("scriptfiles/languages/en.ini");
    Lang[RU] = langOpen("scriptfiles/languages/ru.ini");
    
    langSetDefault(Lang[RU]);
    
#if defined lang_OnScriptInit
    lang_OnScriptInit();
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
#define OnScriptInit lang_OnScriptInit
#if defined lang_OnScriptInit
forward lang_OnScriptInit();
#endif
