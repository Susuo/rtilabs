////////////
//ICU�Ȃ�Ďg��Ȃ��K�`���R�����B
//������������A�I���W�i���ƈꕔ�Ⴄ�Ƃ��낪���邩���E�E�E
//�o�O�������Ă������Ȃ��B

//�݂�ȑ�D�� PHP��mb_convert_kana�̈ڐA
//n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
//N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
//s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂��iU+3000 -> U+0020�j�B
//S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂��iU+0020 -> U+3000�j�B
//k	 �u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
//K	 �u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
//h	 �u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
//H	 �u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//c	 �u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//C	 �u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
std::string XLStringUtil::mb_convert_kana(const std::string &inTarget,const std::string& option)
{
	std::string ret = inTarget;
	static const char *replaceTableAplha[] = {
		 "�`","A"
		,"�a","B"
		,"�b","C"
		,"�c","D"
		,"�d","E"
		,"�e","F"
		,"�f","G"
		,"�g","H"
		,"�h","I"
		,"�i","J"
		,"�j","K"
		,"�k","L"
		,"�l","M"
		,"�m","N"
		,"�n","O"
		,"�o","P"
		,"�p","Q"
		,"�q","R"
		,"�r","S"
		,"�s","T"
		,"�t","U"
		,"�u","V"
		,"�v","W"
		,"�w","X"
		,"�x","Y"
		,"�y","Z"
		,"��","a"
		,"��","b"
		,"��","c"
		,"��","d"
		,"��","e"
		,"��","f"
		,"��","g"
		,"��","h"
		,"��","i"
		,"��","j"
		,"��","k"
		,"��","l"
		,"��","m"
		,"��","n"
		,"��","o"
		,"��","p"
		,"��","q"
		,"��","r"
		,"��","s"
		,"��","t"
		,"��","u"
		,"��","v"
		,"��","w"
		,"��","x"
		,"��","y"
		,"��","z"
		,"�","�["
		,"�e","'"
		,"�f","'"
		,"�g","\""
		,"�h","\""
		,"�i","("
		,"�j",")"
		,"�k","["
		,"�l","]"
		,"�m","["
		,"�n","]"
		,"�o","{"
		,"�p","}"
		,"�q","<"
		,"�r",">"
		,"�s","<"
		,"�t",">"
		,"�u","{"
		,"�v","}"
		,"�w","{"
		,"�x","}"
		,"�y","["
		,"�z","]"
		,"�E","�"
		,"�I","!"
		,"��","#"
		,"��","&"
		,"��","$"
		,"�H","?"
		,"�F",":"
		,"�G",";"
		,"�^","/"
		,"�_","\\"
		,"��","@"
		,"�b","|"
		,"�|","-"
		,"��","="
		,"��","="
		,"��","%"
		,"�{","+"
		,"�|","-"
		,"��","/"
		,"��","*"
		,NULL,NULL
	};
//r	 �u�S�p�v�p�����u���p�v�ɕϊ����܂��B
//R	 �u���p�v�p�����u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
	if ( option.find("r") != -1 ||   option.find("a") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableAplha,false );
	}
	else if ( option.find("R") != -1 ||  option.find("A") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableAplha,true );
	}

	static const char *replaceTableNum[] = {
		 "�P","1"
		,"�Q","2"
		,"�R","3"
		,"�S","4"
		,"�T","5"
		,"�U","6"
		,"�V","7"
		,"�W","8"
		,"�X","9"
		,"�O","0"
		,NULL,NULL
	};
//n	 �u�S�p�v�������u���p�v�ɕϊ����܂��B
//N	 �u���p�v�������u�S�p�v�ɕϊ����܂��B
//a	 �u�S�p�v�p�������u���p�v�ɕϊ����܂��B
//A	 �u���p�v�p�������u�S�p�v�ɕϊ����܂� 
	if ( option.find("n") != -1 ||  option.find("a") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableNum,false );
	}
	else if ( option.find("N") != -1 ||  option.find("A") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableNum,true );
	}

	static const char *replaceTableSpace[] = {
		 "�@"," "
		,NULL,NULL
	};
//s	 �u�S�p�v�X�y�[�X���u���p�v�ɕϊ����܂�
//S	 �u���p�v�X�y�[�X���u�S�p�v�ɕϊ����܂�
	if ( option.find("s") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableSpace,false );
	}
	else if ( option.find("S") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableSpace,true );
	}

	const char *replaceTableHankanaToHiragana[] = {
		 "��","���J"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","��"
		,"�","�["
		,NULL,NULL
	};
	const char *replaceTableKatakanaToHiragana[] = {
		 "��","���J"
		,"�A","��"
		,"�C","��"
		,"�E","��"
		,"�G","��"
		,"�I","��"
		,"�J","��"
		,"�L","��"
		,"�N","��"
		,"�P","��"
		,"�R","��"
		,"�T","��"
		,"�V","��"
		,"�X","��"
		,"�Z","��"
		,"�\","��"
		,"�^","��"
		,"�`","��"
		,"�c","��"
		,"�e","��"
		,"�g","��"
		,"�i","��"
		,"�j","��"
		,"�k","��"
		,"�l","��"
		,"�m","��"
		,"�n","��"
		,"�q","��"
		,"�t","��"
		,"�w","��"
		,"�z","��"
		,"�}","��"
		,"�~","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�@","��"
		,"�B","��"
		,"�D","��"
		,"�F","��"
		,"�H","��"
		,"�K","��"
		,"�M","��"
		,"�O","��"
		,"�Q","��"
		,"�S","��"
		,"�U","��"
		,"�W","��"
		,"�Y","��"
		,"�[","��"
		,"�]","��"
		,"�_","��"
		,"�a","��"
		,"�d","��"
		,"�f","��"
		,"�h","��"
		,"�o","��"
		,"�r","��"
		,"�u","��"
		,"�x","��"
		,"�{","��"
		,"�p","��"
		,"�s","��"
		,"�v","��"
		,"�y","��"
		,"�|","��"
		,"��","��"
		,"��","��"
		,"��","��"
		,"�b","��"
		,"��","��"
		,NULL,NULL
	};
//k	 �u�S�p�J�^�J�i�v���u���p�J�^�J�i�v�ɕϊ����܂��B
//K	 �u���p�J�^�J�i�v���u�S�p�J�^�J�i�v�ɕϊ����܂��B
	if ( option.find("k") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,false );
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,true );
	}
	else if ( option.find("K") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,false );
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,true );
	}

//c	 �u�S�p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
//C	 �u�S�p�Ђ炪�ȁv���u�S�p�J�^�J�i�v�ɕϊ����܂��B
	if ( option.find("c") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,false );
	}
	else if ( option.find("C") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,true );
	}

//h	 �u�S�p�Ђ炪�ȁv���u���p�J�^�J�i�v�ɕϊ����܂��B
//H	 �u���p�J�^�J�i�v���u�S�p�Ђ炪�ȁv�ɕϊ����܂��B
	if ( option.find("h") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,true );
	}
	else if ( option.find("H") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,false );
	}

	return ret;
}

//////////////////
//���܂�
//////////////////

//���[�}�����Ђ炪�Ȃɂ��܂��B
std::string XLStringUtil::RomajiToHiragana(const std::string &inTarget)
{
	static const char *replaceTableRoma[] = {
		 "ltsu","��"
		,"whu","��"
		,"lyi","��"
		,"xyi","��"
		,"lye","��"
		,"xye","��"
		,"wha","����"
		,"whi","����"
		,"whe","����"
		,"who","����"
		,"kyi","����"
		,"kye","����"
		,"kya","����"
		,"kyu","����"
		,"kyo","����"
		,"kwa","����"
		,"qwa","����"
		,"qwi","����"
		,"qyi","����"
		,"qwu","����"
		,"qwe","����"
		,"qye","����"
		,"qwo","����"
		,"qya","����"
		,"qyu","����"
		,"qyo","����"
		,"syi","����"
		,"swi","����"
		,"sha","����"
		,"shu","����"
		,"sho","����"
		,"syi","����"
		,"sya","����"
		,"syu","����"
		,"syo","����"
		,"shi","��"
		,"swa","����"
		,"swi","����"
		,"swu","����"
		,"swe","����"
		,"swo","����"
		,"cyi","����"
		,"tyi","����"
		,"che","����"
		,"cye","����"
		,"tye","����"
		,"cha","����"
		,"cya","����"
		,"tya","����"
		,"chu","����"
		,"cyu","����"
		,"tyu","����"
		,"cho","����"
		,"cyo","����"
		,"tyo","����"
		,"chi","��"
		,"tsa","��"
		,"tsi","��"
		,"tse","��"
		,"tso","��"
		,"tsu","��"
		,"ltu","��"
		,"xtu","��"
		,"thi","�Ă�"
		,"the","�Ă�"
		,"tha","�Ă�"
		,"thu","�Ă�"
		,"tho","�Ă�"
		,"twa","�Ƃ�"
		,"twi","�Ƃ�"
		,"twu","�Ƃ�"
		,"twe","�Ƃ�"
		,"two","�Ƃ�"
		,"nyi","�ɂ�"
		,"nye","�ɂ�"
		,"nya","�ɂ�"
		,"nyu","�ɂ�"
		,"nyo","�ɂ�"
		,"hyi","�Ђ�"
		,"hye","�Ђ�"
		,"hya","�Ђ�"
		,"hyu","�Ђ�"
		,"hyo","�Ђ�"
		,"fwa","�ӂ�"
		,"fwi","�ӂ�"
		,"fyi","�ӂ�"
		,"fwu","�ӂ�"
		,"few","�ӂ�"
		,"fye","�ӂ�"
		,"fwo","�ӂ�"
		,"fya","�ӂ�"
		,"fyu","�ӂ�"
		,"fyo","�ӂ�"
		,"myi","�݂�"
		,"mye","�݂�"
		,"mya","�݂�"
		,"myu","�݂�"
		,"myo","�݂�"
		,"lya","��"
		,"xya","��"
		,"lyu","��"
		,"xyu","��"
		,"lyo","��"
		,"xyo","��"
		,"ryi","�股"
		,"rye","�肥"
		,"rya","���"
		,"ryu","���"
		,"ryo","���"
		,"gyi","����"
		,"gye","����"
		,"gya","����"
		,"gyu","����"
		,"gyo","����"
		,"gwa","����"
		,"gwi","����"
		,"gwu","����"
		,"gwe","����"
		,"gwo","����"
		,"jyi","����"
		,"zyi","����"
		,"jye","����"
		,"zye","����"
		,"jya","����"
		,"zya","����"
		,"lwa","��"
		,"xwa","��"
		,"jyu","����"
		,"zyu","����"
		,"jyo","����"
		,"zyo","����"
		,"dyi","����"
		,"dye","����"
		,"dya","����"
		,"dyu","����"
		,"dyo","����"
		,"dhi","�ł�"
		,"dhe","�ł�"
		,"dha","�ł�"
		,"dhu","�ł�"
		,"dho","�ł�"
		,"dwa","�ǂ�"
		,"dwi","�ǂ�"
		,"dwu","�ǂ�"
		,"dwe","�ǂ�"
		,"dwo","�ǂ�"
		,"byi","�т�"
		,"bye","�т�"
		,"bya","�т�"
		,"byu","�т�"
		,"byo","�т�"
		,"pyi","�҂�"
		,"pye","�҂�"
		,"pya","�҂�"
		,"pyu","�҂�"
		,"pyo","�҂�"
		,"lka","��" //��
		,"xka","��" //��
		,"lke","��" //��
		,"xke","��" //��
		,"vyi","���J��"
		,"vye","���J��"
		,"vya","���J��"
		,"vyu","���J��"
		,"vyo","���J��"
		,"wu","��"
		,"la","��"
		,"li","��"
		,"xi","��"
		,"lu","��"
		,"xu","��"
		,"le","��"
		,"xe","��"
		,"lo","��"
		,"xo","��"
		,"ye","����"
		,"ka","��"
		,"ca","��"
		,"ki","��"
		,"qa","����"
		,"qi","����"
		,"qe","����"
		,"qo","����"
		,"ku","��"
		,"cu","��"
		,"qu","��"
		,"ke","��"
		,"ko","��"
		,"co","��"
		,"sa","��"
		,"si","��"
		,"ci","��"
		,"su","��"
		,"se","��"
		,"ce","��"
		,"so","��"
		,"ta","��"
		,"ti","��"
		,"tu","��"
		,"te","��"
		,"to","��"
		,"na","��"
		,"ni","��"
		,"nu","��"
		,"ne","��"
		,"no","��"
		,"ha","��"
		,"hi","��"
		,"fa","�ӂ�"
		,"fa","�ӂ�"
		,"fi","�ӂ�"
		,"fe","�ӂ�"
		,"fo","�ӂ�"
		,"hu","��"
		,"fu","��"
		,"he","��"
		,"ho","��"
		,"ma","��"
		,"mi","��"
		,"mu","��"
		,"me","��"
		,"mo","��"
		,"ya","��"
		,"yu","��"
		,"yo","��"
		,"ra","��"
		,"ri","��"
		,"ru","��"
		,"re","��"
		,"wa","��"
		,"wo","��"
		,"nn","��"
		,"xn","��"
		,"ga","��"
		,"gi","��"
		,"gu","��"
		,"ge","��"
		,"go","��"
		,"za","��"
		,"je","����"
		,"ja","����"
		,"ju","����"
		,"jo","����"
		,"zi","��"
		,"ji","��"
		,"zu","��"
		,"ze","��"
		,"zo","��"
		,"ji","��"
		,"da","��"
		,"di","��"
		,"du","��"
		,"de","��"
		,"do","��"
		,"ba","��"
		,"bi","��"
		,"bu","��"
		,"be","��"
		,"bo","��"
		,"pa","��"
		,"pi","��"
		,"pu","��"
		,"pe","��"
		,"po","��"
		,"va","���J��"
		,"vi","���J��"
		,"ve","���J��"
		,"vo","���J��"
		,"vu","���J"
		,"a","��"
		,"i","��"
		,"u","��"
		,"e","��"
		,"o","��"
		,NULL,NULL
	};
	return XLStringUtil::replace(inTarget ,replaceTableRoma,false );
}

//���ȓ��͂̐l�����Ȃ����Y�ꂽ�Ƃ��ɓ��͂���镶������Ђ炪�Ȃɂ��܂��B
std::string XLStringUtil::KanaTypoHiragana(const std::string &inTarget)
{
	static const char *replaceTableRoma[] = {
		 "4@","���J"
		,"a","��"
		,"e","��"
		,"4","��"
		,"5","��"
		,"6","��"
		,"t","��"
		,"g","��"
		,"h","��"
		,":","��"
		,"b","��"
		,"x","��"
		,"d","��"
		,"r","��"
		,"p","��"
		,"c","��"
		,"q","��"
		,"a","��"
		,"z","��"
		,"w","��"
		,"s","��"
		,"u","��"
		,"i","��"
		,"1","��"
		,",","��"
		,"k","��"
		,"f","��"
		,"v","��"
		,"2","��"
		,"^","��"
		,"-","��"
		,"j","��"
		,"n","��"
		,"]","��"
		,"/","��"
		,"m","��"
		,"7","��"
		,"8","��"
		,"9","��"
		,"o","��"
		,"l","��"
		,".","��"
		,";","��"
		,"\\","��"
		,"0","��"
		//,"","��"
		,"y","��"
		,"#","��"
		,"E","��"
		,"$","��"
		,"%","��"
		,"&","��"
		,"t@","��"
		,"g@","��"
		,"h@","��"
		,":@","��"
		,"b@","��"
		,"x@","��"
		,"d@","��"
		,"r@","��"
		,"p@","��"
		,"c@","��"
		,"q@","��"
		,"a@","��"
		,"z@","��"
		,"w@","��"
		,"s@","��"
		,"f@","��"
		,"v@","��"
		,"2@","��"
		,"^@","��"
		,"-@","��"
		,"f[","��"
		,"v[","��"
		,"2[","��"
		,"^[","��"
		,"-[","��"
		,"'","��"
		,"(","��"
		,")","��"
		,"Z","��"
		//,"��","��"
		,NULL,NULL
	};
	return XLStringUtil::replace(inTarget ,replaceTableRoma,false );
}



////////////
//�x�[�X
////////////

//strstr�̃}���`�o�C�g�Z�[�t �����񌟍�
const char* XLStringUtil::strstr(const std::string& target, const std::string & need )
{
#ifdef _WINDOWS
	//SJIS���Ƃ���Ȋ������ȁE�E�E
	return (const char*) _mbsstr( (unsigned char*) target.c_str() ,(unsigned char*) need.c_str() );
#else
	//UTF-8���Ɖ��肵�Ă���w
	return strstr( target.c_str() ,need.c_str() );
#endif
}


//�}���`�o�C�g�Ή� �Ȃ��� std::string �ɕW���ŗp�ӂ���Ă��Ȃ��u���B�ӂ������B
std::string XLStringUtil::replace(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	const char * p = inTarget.c_str();
	const char * match;
	while( match = XLStringUtil::strstr( p , inOld.c_str() ) )
	{
		//ret += std::string(p,0,(int)(match - p));
		ret.append(p,(int)(match - p));
		ret += inNew;

		p = match + inOld.size();
	}
	//�c��𑫂��Ă����܂�.
	return ret + p;
}



//�}���`�o�C�g�Ή� �����̌����ꊇ�u�� const char * replacetable[] = { "A","��"  ,"I","��"  , "��","����" , NULL , NULL}  //�K��2�����ŏ����Ă�
std::string XLStringUtil::replace(const std::string &inTarget ,const char** replacetable,bool isrev)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	if (inTarget.empty())
	{
		return inTarget;
	}

	const char * p = inTarget.c_str();
	for(; *p ; )
	{
		const char * pp = p;
		p = nextChar(p);

		int compareIndex = isrev == false ? 0 : 1;
		int replaceIndex = isrev == false ? 1 : 0;
		const char ** r1 = replacetable;
		for( ; *r1 != NULL ; r1+=2)
		{
			const char * ppp = pp;
			const char * r2 = *(r1+compareIndex);
			for( ; 1 ; ++r2,++ppp )
			{
				if ( *r2 == NULL || *ppp != *r2)
				{
					break;
				}
			}
			if (*r2 == NULL)  //������r������̕����I�[�ɂ��ǂ����
			{
				ret.append(*(r1+replaceIndex));
				p = ppp;
				break;
			}
		}
		if ( *r1 == NULL )
		{
			ret.append(pp,(int) (p - pp));
		}
	}
	return ret;
}