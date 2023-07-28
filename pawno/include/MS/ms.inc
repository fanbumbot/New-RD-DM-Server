#if defined mdg
	#endinput
#endif
#define mdg

#if !defined OnScriptInit
	#if !defined FILTERSCRIPT
		#define OnScriptInit OnGameModeInit
	#else
		#define OnScriptInit OnFilterScriptInit
	#endif
	main(){}
#endif
#if !defined OnScriptExit
	#if !defined FILTERSCRIPT
		#define OnScriptExit OnGameModeExit
	#else
		#define OnScriptExit OnFilterScriptExit
	#endif
#endif

#include <MS/utils>
#include <MS/gpci>
#include <MS/check>
#include <MS/ip>
#include <MS/color>
#include <MS/separator>
#include <MS/extargs>
#include <MS/messages>
#include <MS/list>
#include <MS/vector>
#include <MS/map>
#include <MS/file>
#include <MS/lang>
#include <MS/cmd>
#include <MS/dialogs>
#include <MS/orm>
#include <MS/item>
#include <MS/world>
#include <MS/player>
#include <MS/vehicle>
#include <MS/afk>
#include <MS/fix>