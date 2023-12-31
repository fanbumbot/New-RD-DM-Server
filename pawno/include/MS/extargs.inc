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
stock argGetNum()   return arg_marker_check == 0 ? 0 : arg_marker_num;//���������� �������������� ����������

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
    len = argGetStringLen(arg);//����� ����� ������
    //������������ ������ � dest
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

//num_arg - ���������� ������������ ���������� (�� �� ��������� ������ ��������������)
stock argInit(num_arg)
{
	//����� ����� ������ ������� �� ������� ��������� �����
	#emit LCTRL 4//���� STK
	#emit LOAD.I//������ ���������� FRM
	#emit ADD.C 8//��������� � ���������� ���������� ������� �� ������� ��� �����
    #emit STOR.pri arg_marker//��������� �����

    //��������� ������� �������������� ���������� ������������
	#emit LOAD.I
	#emit CONST.alt 2
	#emit SHR//��������� ������ � ���������� ����������
	#emit STOR.pri arg_marker_num//��������� ����������
	#emit LOAD.S.alt num_arg//���� ���������� ������������ ����������
	#emit SUB//���� ���������� �������������� ����������

	//���� �� ���������� ������ ��� ����� 0, ����� ���������, ��� �� ���������� 0
	#emit STOR.pri arg_marker_num
	arg_marker_num = clamp(arg_marker_num, 0);

	//��������� arg_marker � ��������� ������ �������������� ����������
	#emit LOAD.pri arg_marker
	#emit ADD.C 4//��������� � ������ ����������
	#emit LOAD.S.alt num_arg
	#emit SHL.C.alt 2
	#emit ADD//��������� ������������ ���������
	#emit STOR.pri arg_marker

	arg_marker_check = 1;
	return 1;
}

//�������� ������, ���� ������������ �����, ���������� ���� ��������� ���
//� ���� ����� ������� �������
//�������� CALL
stock AddressCall()
{
	#emit POP.pri//PRI = FRM.old
	#emit SCTRL 5//FRM = FRM.old
	#emit POP.pri//PRI = ����� ������ ������
	#emit STACK 4//������� ���������� ����������
	#emit POP.alt//ALT = ����� ������ ����
	#emit PUSH.pri//������������� ���� ��������� ����� RET
	#emit MOVE.pri//PRI = ����� ������ ������
	#emit SCTRL 6//��������� �� ������ �����
	return 0;
}

//��������� ����� ���� � ����� + DAT (�� COD � DAT)
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
	//���� ����� ��������� MOVS
	#emit LCTRL 6
	#emit ADD.C 0x60

	//��������� ����� �� COD � DAT ������
	#emit PUSH.pri
	#emit PUSH.C 4
	#emit PUSH.C AddressCode
	AddressCall();

 	#emit PUSH.pri//���������� �����
    #emit LOAD.S.pri numbytes//����� ���������� ����
	#emit SREF.S.pri 0xFFFFFFFC//-4 - ����� ���������� ���� � ����������� �����
	#emit STACK 4//������� PUSH

    #emit LOAD.S.pri addr_from
	#emit LOAD.S.alt addr_to

	#emit MOVS 0
	return 1;
}

stock MemCopy(addr_to[], addr_from[], index_to, index_from, numbytes)
{
	#emit LCTRL 6
	#emit ADD.C 0x7C

	//��������� ���������
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
	#emit PUSH.pri//���������� �����

	//�����
	#emit LREF.S.pri address
	#emit STOR.S.pri temp
	if(temp != 0)
	{
	    #emit INC.alt

		#emit LOAD.S.pri address
		#emit ADD.C 4
		#emit STOR.S.pri address

		#emit LOAD.S.pri 0xFFFFFFF8//-8
		#emit SCTRL 6//������� �� �����
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

	#emit POP.pri//������ ���������� FRM
	#emit SCTRL 5//��������� FRM
	#emit POP.pri//������ ����� ������
	#emit STOR.pri retn//��������� ����� ������
	#emit STACK 8//����������� �� ���������� � �� ����������

	//��������� ����
	#emit LCTRL 4
	#emit LOAD.alt num_temp
	#emit SUB
	#emit SCTRL 4//��������� ����

	//������ ����� MemCopyA
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
	#emit POP.pri//������ ���������� FRM
	#emit SCTRL 5//��������� FRM
	#emit POP.pri//������ ����� ������
	#emit STACK 4//����������� �� ���������� � �� ����������
	#emit LOAD.alt arg_marker_num
	#emit SHL.C.alt 2
	#emit PUSH.alt//������� ���������� ���������� � ����
	#emit SCTRL 6
	return 0;
}


/*
	i - ����� �����
	f - ������� �����
	s - ������
	a[x] - ������ ������� x
*/

stock argFormatTest(string[], templ[], string_s = sizeof(string))
{
	if(string_s != string_s)	format(string, 0, string);//���� format �� ����� �� ���� �� �������������
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
	        //���� �����
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
    
    num = argGetNum()+3;//�������������� + ������������

	//��������� ����
	#emit LCTRL 4
	#emit STOR.pri stack

	//������� ��� �������������� ���������
    argPush();
    
    //������� ������������ ���������
    #emit PUSH.S text
    #emit PUSH.S len
    #emit PUSH.S output
    
    //������� ���������� ����������
    #emit LOAD.pri num
    #emit SHL.C.pri 2
    #emit PUSH.pri
    
	//����� format
   	#emit SYSREQ.C format
   	
   	//��������������� ����
   	#emit LOAD.pri stack
	#emit SCTRL 4
    
    return 1;
}
