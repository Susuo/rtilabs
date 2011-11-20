// RSetupLoad.cpp: RSetupLoad �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////


#include "comm.h"

#include "regex.h"

#include "RSetupLoad.h"
#include "RConv.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSetupLoad::RSetupLoad()
{

}

RSetupLoad::~RSetupLoad()
{
	for (OneMenu::iterator i = Menus.begin() ; i != Menus.end() ; ++i)	delete (*i).second;

}

void RSetupLoad::Load(string inSetupFilename,string inSection) 
			throw(RIOCannotOpenException,RIOCannotReadException,RParseException)
{
	RStdioFile	StdFile;
	StdFile.Open(inSetupFilename ,"rb");

	string buf;

	//�Z�N�V�����V�[�N.
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':' && buf.substr(1) == inSection) break;
	}

	//�L�[�擾.
	boost::reg_expression<char> keyRegex  = "KEY\\s*([A-Z]+)\\s*([0-9]+)\\s*(.*)$";
	//���j���[
	boost::reg_expression<char> menuRegex  = "MENU\\s*([^\\s]+)\\s*([^\\s]+)\\s*(.*)$";

	//�L�[
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':') break;		//�ʂ̃Z�N�V�����ɂ͂������̂ŁA���̃L�[�͂Ȃ��Ƃ������Ƃ�.

		if (buf.size() >= 5)
		{
			if ( buf.substr(0,3) == "KEY")
			{
				//�L�[��`.
				boost::match_results<const char*> results;
				if ( boost::regex_search(buf.c_str() , results, keyRegex) ) 
				{
					//�ǉ� / �}��
					KeyDowns.SetKey( ToKey(results.str(1)), (MODIFY_KEY) atoi(results.str(2).c_str()) , results.str(3) );
				}
			}
			if (buf.substr(0,4) == "MENU")
			{
				//���j���[��`.
				boost::match_results<const char*> results;
				if ( boost::regex_search(buf.c_str() , results, menuRegex) ) 
				{
					//���j���[�̊Ǘ��@�\��ǉ� / �}��
					//�Ǘ��@�\�̒��ŖړI�̃��j���[������.
					MenuSentence* menu;
					OneMenu::iterator j =  Menus.find( results.str(1) );
					if (j == Menus.end() )
					{
						menu = new MenuSentence;
						Menus[results.str(1)] = menu;
					}
					else
					{
						menu = ((*j).second);
					}
					//�ړI�̃��j���[�ɍ��ڂ�ǉ�.
					string script = RConv::ltrim(results.str(3));
					if (script[0] == '@' )
					{//����q
						OneMenu::iterator k =  Menus.find( script.substr(1) );
						if (k == Menus.end() )
						{
							throw RParseException("�l�X�g�����悤���Ă��� %s �Ƃ������j���[�͒�`����Ă��܂���",script.c_str() );
						}
						menu->CreateIncludeMenu(results.str(2) , ((*k).second)->getMenu() );
					}
					else
					{//����.
						menu->CreateNode(results.str(2) , script );
					}
				}
			}
		}
	}

}


//�L�[�R�[�h�̕ϊ�
unsigned char RSetupLoad::ToKey(string inKeyname) const
{
	if (inKeyname.size() == 1)
	{
		return inKeyname[0];
	}
	if (inKeyname == "RETURN" || inKeyname == "RET" )	return VK_RETURN;
	if (inKeyname == "TAB" )	return VK_TAB;
	if (inKeyname == "BACK" )	return VK_BACK;
	if (inKeyname == "ESCAPE"  || inKeyname == "ESC"  )	return VK_ESCAPE;
	if (inKeyname == "INSERT"  || inKeyname == "INS"  )	return VK_INSERT;
	if (inKeyname == "DELETE"  || inKeyname == "DEL"  )	return VK_DELETE;
	if (inKeyname == "SPACE" )	return VK_SPACE;
	if (inKeyname == "UP" )	return VK_UP;
	if (inKeyname == "RIGHT" )	return VK_RIGHT;
	if (inKeyname == "DOWN" )	return VK_DOWN;
	if (inKeyname == "LEFT" )	return VK_LEFT;
	if (inKeyname == "SPACE" )	return VK_SPACE;
	if (inKeyname == "END" )	return VK_END;
	if (inKeyname == "HOME" )	return VK_HOME;
	if (inKeyname == "LBUTTON" )	return VK_LBUTTON;
	if (inKeyname == "RBUTTON" )	return VK_RBUTTON;
	if (inKeyname == "MBUTTON" )	return VK_MBUTTON;

	if (inKeyname == "F1" )	return VK_F1;
	if (inKeyname == "F2" )	return VK_F2;
	if (inKeyname == "F3" )	return VK_F3;
	if (inKeyname == "F4" )	return VK_F4;
	if (inKeyname == "F5" )	return VK_F5;
	if (inKeyname == "F6" )	return VK_F6;
	if (inKeyname == "F7" )	return VK_F7;
	if (inKeyname == "F8" )	return VK_F8;
	if (inKeyname == "F9" )	return VK_F9;
	if (inKeyname == "F10" )	return VK_F10;
	if (inKeyname == "F11" )	return VK_F11;
	if (inKeyname == "F12" )	return VK_F12;

	return VK_SPACE;
}



//�L�[�_�E��.
bool RSetupLoad::KeyDown(unsigned char inKeyCode ,MODIFY_KEY inModifyKey) const throw (RParseException)
{
	return this->KeyDowns.OnKeyDown(inKeyCode , inModifyKey) ;
}

//�L�[�_�E��.
bool RSetupLoad::KeyDown(unsigned char inKeyCode ) const throw (RParseException)
{
	int modifyKey = 0;
	if (GetKeyState(VK_SHIFT) & 0x8000)		modifyKey &= MODIFY_KEY_SHIFT;
	if (GetKeyState(VK_CONTROL) & 0x8000)	modifyKey &= MODIFY_KEY_CTRL;
	if (GetKeyState(VK_MENU) & 0x8000)		modifyKey &= MODIFY_KEY_ALT;	//�ǂ����� MENU �� ALT �Ȃ��? �Q�l http://www.experts-exchange.com/Programming/Programming_Languages/MFC/Q_10271543.html

	return this->KeyDowns.OnKeyDown(inKeyCode , (MODIFY_KEY)modifyKey) ;
}


//���j���[���o��.
string RSetupLoad::Popup(const string inMenuName, POINT inScreenPoint , HWND inOwner) const throw (RParseException,RNotFoundException,RWin32Exception)
{
	OneMenu::const_iterator i = this->Menus.find(inMenuName);
	if (i == this->Menus.end() )	
	{
		throw RNotFoundException("�w�肳�ꂽ %s �Ƃ������O�̃��j���[�͂���܂���\n", inMenuName.c_str() );
	}
	const MenuSentence* menuSentence = (*i).second;
	
	//���j���[���|�b�v�A�b�v.
	HMENU menu = menuSentence->getMenu();
	DWORD ret = TrackPopupMenu( menu , TPM_LEFTALIGN|TPM_LEFTBUTTON | TPM_RETURNCMD,
							inScreenPoint.x , inScreenPoint.y , 0 , inOwner ,NULL);
	if (ret == 0)
	{
		throw RWin32Exception("�|�b�v�A�b�v���j���[ %s �ŃG���[�����o���܂���." , inMenuName.c_str() );
	}
	return menuSentence->Run(ret);
}
