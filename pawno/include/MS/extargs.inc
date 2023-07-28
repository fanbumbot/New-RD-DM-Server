#if defined mdg_extarg_inc
	#endinput
#endif
#define mdg_extarg_inc

const cellbytes = cellbits/4;

static arg_marker_check;
static arg_marker;
static arg_marker_num;

forward SysreqFix();
public SysreqFix()
{
	new temp[2];
	format(temp, sizeof(temp), "");//from zlang
	strlen(temp);
	strmid(temp, temp, 0, 0, 0);
	return 1;
}

stock argGetMarker()   return arg_marker_check == 0 ? 0 : arg_marker;
stock argGetNum()   return arg_marker_check == 0 ? 0 : arg_marker_num;//Количество необязательных аргументов

stock argGet(arg)
{
    if(arg_marker_check == 0)   return 0;
	arg = clamp(arg, 0, arg_marker_num);
    new temp = arg_marker + (arg<<2);
    #emit LREF.S.pri temp
    #emit STOR.S.pri temp
    return temp;
}

stock argGetRef(arg)
{
    if(arg_marker_check == 0)   return 0;
	arg = clamp(arg, 0, arg_marker_num);
    new temp = arg_marker + (arg<<2);
    #emit LREF.S.pri temp
    #emit STOR.S.pri temp
    #emit LREF.S.pri temp
    #emit STOR.S.pri temp
    return temp;
}

stock argGetStringLen(arg)
{
    argGet(arg);
    #emit PUSH.pri
    #emit PUSH.C 4
    #emit SYSREQ.C strlen
    #emit STACK 8
    #emit RETN
	return 0;
}

stock argGetString(arg, dest[], maxlenght = sizeof(dest))
{
	static len;
    len = argGetStringLen(arg);//Узнаём длину строки
    //Переписываем строку в dest
    argGet(arg);
    #emit PUSH.S maxlenght
    #emit PUSH len
    #emit PUSH.C 0
    #emit PUSH.pri
    #emit PUSH.S dest
    #emit PUSH.C 20
    #emit SYSREQ.C strmid
	#emit STACK 24
	return 1;
}

//num_arg - количество обязательных аргументов (но мы учитываем только необязательные)
stock argInit(num_arg)
{
	//Узнаём адрес начала функции из которой произошёл вызов
	#emit LCTRL 4//Берём STK
	#emit LOAD.I//Грузим предыдущий FRM
	#emit ADD.C 8//Переходим к количеству параметров функции из которой был вызов
    #emit STOR.pri arg_marker//Сохраняем адрес

    //Указываем сколько необязательных параметров используется
	#emit LOAD.I
	#emit CONST.alt 2
	#emit SHR//Переводим оффсет в количество параметров
	#emit STOR.pri arg_marker_num//Сохраняем количество
	#emit LOAD.S.alt num_arg//Берём количество обязательных аргументов
	#emit SUB//Ищем количество необязательных аргументов

	//Если их количество меньше или равно 0, тогда указываем, что их количество 0
	#emit STOR.pri arg_marker_num
	arg_marker_num = clamp(arg_marker_num, 0);

	//Переводим arg_marker в положение начала необязательных параметров
	#emit LOAD.pri arg_marker
	#emit ADD.C 4//Переводим к началу параметров
	#emit LOAD.S.alt num_arg
	#emit SHL.C.alt 2
	#emit ADD//Добавляем обязательные параметры
	#emit STOR.pri arg_marker

	arg_marker_check = 1;
	return 1;
}

//Передача адреса, куда производится вызов, происходит путём занесения его
//в стек перед вызовом функции
//Эмуляция CALL
stock AddressCall()
{
	#emit POP.pri//PRI = FRM.old
	#emit SCTRL 5//FRM = FRM.old
	#emit POP.pri//PRI = адрес вызова откуда
	#emit STACK 4//Убираем количество параметров
	#emit POP.alt//ALT = адрес вызова куда
	#emit PUSH.pri//Устанавливаем куда вернуться после RET
	#emit MOVE.pri//PRI = адрес вызова откуда
	#emit SCTRL 6//Переходим на нужный адрес
	return 0;
}

//Перевести адрес кода в адрес + DAT (из COD в DAT)
stock AddressCode(address)
{
	#emit LCTRL 1
	#emit NEG//-DAT
	#emit MOVE.alt
	#emit LCTRL 0
	#emit ADD//+COD
	#emit LOAD.S.alt address
	#emit ADD//+address
	#emit RETN
	return 0;
}

stock MemCopyA(addr_to, addr_from, numbytes)
{
	//Ищем адрес параметра MOVS
	#emit LCTRL 6
	#emit ADD.C 0x60

	//Переводим адрес из COD в DAT формат
	#emit PUSH.pri
	#emit PUSH.C 4
	#emit PUSH.C AddressCode
	AddressCall();

 	#emit PUSH.pri//Запоминаем адрес
    #emit LOAD.S.pri numbytes//Узнаём количество байт
	#emit SREF.S.pri 0xFFFFFFFC//-4 - пишем количество байт в сохранённый адрес
	#emit STACK 4//Убираем PUSH

    #emit LOAD.S.pri addr_from
	#emit LOAD.S.alt addr_to

	#emit MOVS 0
	return 1;
}

stock MemCopy(addr_to[], addr_from[], index_to, index_from, numbytes)
{
	#emit LCTRL 6
	#emit ADD.C 0x7C

	//Сохраняем параметры
	#emit PUSH.pri
	#emit PUSH.C 4
	#emit PUSH.C AddressCode
	AddressCall();

 	#emit PUSH.pri
    #emit LOAD.S.pri numbytes
	#emit SREF.S.pri 0xFFFFFFFC//-4
	#emit STACK 4

	#emit LOAD.S.pri addr_to
	#emit LOAD.S.alt index_to
	#emit ADD
	#emit PUSH.pri

	#emit LOAD.S.pri addr_from
	#emit LOAD.S.alt index_from
	#emit POP.alt

	#emit MOVS 0
	return 1;
}

stock strlenA(address)
{
	new temp;
	#emit CONST.alt 0
	#emit LCTRL 6
	#emit ADD.C 12
	#emit PUSH.pri//Сохранение метки

	//Метка
	#emit LREF.S.pri address
	#emit STOR.S.pri temp
	if(temp != 0)
	{
	    #emit INC.alt

		#emit LOAD.S.pri address
		#emit ADD.C 4
		#emit STOR.S.pri address

		#emit LOAD.S.pri 0xFFFFFFF8//-8
		#emit SCTRL 6//Переход на метку
	}
	else
	{
		#emit MOVE.pri
		#emit STACK 8
		#emit RETN
	}
	return 0;
}

stock argPush(num=cellmax)
{
	if(arg_marker_check == 0)   return 0;

	static retn;
	static num_temp;

	num_temp = clamp(num, 0, arg_marker_num) << 2;

	#emit POP.pri//Грузим предыдущий FRM
	#emit SCTRL 5//Сохраняем FRM
	#emit POP.pri//Грузим адрес вызова
	#emit STOR.pri retn//Сохраняем адрес вызова
	#emit STACK 8//Избавляемся от параметров и их количества

	//Переносим стек
	#emit LCTRL 4
	#emit LOAD.alt num_temp
	#emit SUB
	#emit SCTRL 4//Переносим стек

	//Делаем вызов MemCopyA
	#emit PUSH num_temp
	#emit PUSH arg_marker
	#emit PUSH.pri
	#emit PUSH.C 12
	#emit PUSH.C MemCopyA
	AddressCall();

	#emit LOAD.pri retn
	#emit SCTRL 6
	return 0;
}

stock argNumPush()
{
	if(arg_marker_check == 0)   return 0;
	#emit POP.pri//Грузим предыдущий FRM
	#emit SCTRL 5//Сохраняем FRM
	#emit POP.pri//Грузим адрес вызова
	#emit STACK 4//Избавляемся от параметров и их количества
	#emit LOAD.alt arg_marker_num
	#emit SHL.C.alt 2
	#emit PUSH.alt//Заносим количество аргументов в стек
	#emit SCTRL 6
	return 0;
}


/*
	i - целое число
	f - дробное число
	s - строка
	a[x] - массив размера x
*/

stock argFormatTest(string[], templ[], string_s = sizeof(string))
{
	if(string_s != string_s)	format(string, 0, string);//Если format до этого ни разу не использовался
	if(arg_marker_check == 0)   return 0;
	new temp;
	new len = argGetNum();
	new marker = 0;
	new type;
	new templ_s = strlen(templ);

	static substring[2048];

	new marker_dest = 0;
	new subs_s;

	new buffer;

	for(new i = 0, address = argGetMarker(); i < len; i++, address += 4)
    {
        #emit LREF.S.pri address
        #emit STOR.S.pri temp

		type = templ[marker];

		if(type == 'i' || type == 'f')
		{
	        #emit LREF.S.pri temp
			#emit STOR.S.pri temp
			if(type == 'i')
			{
				format(substring, sizeof(substring), "%i,", temp);
			}
			else
			{
			    format(substring, sizeof(substring), "%f,", temp);
			}
			subs_s = strlen(substring);
			if(marker_dest+subs_s >= string_s)   return 0;
			MemCopy(string, substring, marker_dest<<2, 0, subs_s<<2);
			marker_dest += subs_s;
		}
		else if(type == 's')
		{
		    #emit LOAD.S.pri string
		    #emit STOR.S.pri buffer

		    string[marker_dest] = '\'';
            marker_dest++;

			subs_s = strlenA(temp);
            MemCopyA(buffer+(marker_dest<<2), temp, subs_s<<2);
            marker_dest += subs_s;

            string[marker_dest] = '\'';
            marker_dest++;
            string[marker_dest] = ',';
            marker_dest++;
		}
		else if(type == 'a')
		{
	        //Ищем длину
	        if(marker+3 >= templ_s) return 0;
			if(strfind(templ, "[", .pos = marker) != marker+1)  return 0;
			new end = strfind(templ, "]", .pos = marker+2);
			if(end == -1 || end == marker+2)    return 0;
			new sss[11];
			strmid(sss, templ, marker+2, end);
			subs_s = strval(sss);
			marker = end;

		    #emit LOAD.S.pri string
		    #emit STOR.S.pri buffer

		    string[marker_dest] = '\'';
            marker_dest++;

            MemCopyA(buffer+(marker_dest<<2), temp, subs_s<<2);
            marker_dest += subs_s;

            string[marker_dest] = '\'';
            marker_dest++;
            string[marker_dest] = ',';
            marker_dest++;
		}
		marker++;
    }
    string[marker_dest] = EOS;
    strdel(string, strlen(string)-1, strlen(string));
	return 1;
}

stock argFormat(output[], len, text[])
{
    if(arg_marker_check == 0)   return 0;
	if(argGetNum() == 0)
	{
	    format(output, len, text);
	    return 1;
	}
    
    static num;
    static stack;
    
    num = argGetNum()+3;//Необязательные + обязательные

	//Сохраняем стек
	#emit LCTRL 4
	#emit STOR.pri stack

	//Заносим все необязательные аргументы
    argPush();
    
    //Заносим обязательные аргументы
    #emit PUSH.S text
    #emit PUSH.S len
    #emit PUSH.S output
    
    //Заносим количество аргументов
    #emit LOAD.pri num
    #emit SHL.C.pri 2
    #emit PUSH.pri
    
	//Вызов format
   	#emit SYSREQ.C format
   	
   	//Восстанавливаем стек
   	#emit LOAD.pri stack
	#emit SCTRL 4
    
    return 1;
}
