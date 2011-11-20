// RFunctionScript.cpp: RFunctionScript �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RFunctionScript.h"
#include "RConv.h"
#include "Filer.h"
#include "RInputDialog.h"
#include "RExpr.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�\�����p�[�X���܂��B
string Sentence::Parse(string inSentence) throw(RParseException)
{
	bool isNest = false;
	int nestStart = 0;

	int parseStart = 0;
	string runnable;

	string stdOut;

	int size = inSentence.size();
	for( int i = 0; i < size ; i++)
	{
		char c = inSentence[i]; 

		//2Byte ������������X�L�b�v.
		if ( MultiString::isKanji(c , inSentence[i+1]) )
		{
			i ++;		continue;
		}

		if (!isNest)
		{
			if (c == '`')
			{//�R�}���h�u�� �J�n
				//���s�\������ɒǉ�.
				runnable += inSentence.substr(parseStart , i - parseStart);
				isNest = true;
				nestStart = i+1;
			}
			else if (c == '|')
			{//�p�C�v
				runnable += inSentence.substr(parseStart , i - parseStart);
				stdOut = Run(runnable,stdOut);

				//���ׂĂ�������.
				runnable = "";
				parseStart = i + 1;
				nestStart = 0;
				isNest = false;
			}
			else if (c == ';')
			{//�����\��.
				runnable += inSentence.substr(parseStart , i - parseStart);
				(void) Run(runnable,stdOut);

				//���ׂĂ�������.
				runnable = "";
				parseStart = i + 1;
				nestStart = 0;
				isNest = false;
			}
		}
		else
		{	
			if (c == '`')
			{//�R�}���h�u�� �I��
				Sentence newSentence;
				runnable += newSentence.Parse( inSentence.substr(nestStart , i - nestStart) );

				parseStart = i + 1;
				nestStart = 0;
				isNest = false;
			}
		}
	}
	runnable += inSentence.substr(parseStart , i- parseStart);
	return Run(runnable,stdOut);
}


//�ꕶ�ɂȂ��Ă̂Ŏ��s���܂��B
string Sentence::Run(string inRunnable,string inStdOut) throw(RParseException)
{
	//���E�̋󔒃X�y�[�X�̍폜.
	string pure = RConv::ltrim( RConv::rtrim(inRunnable) );

	vector<string> param;
	bool isQuart = false;

	//�\�����.
	int size = pure.size();
	for(int i = 0; i < size ; i++)
	{
		//2Byte ������������X�L�b�v.
		if ( MultiString::isKanji(pure[i], pure[i+1]) )
		{
			i ++;		continue;
		}

		//�_�u���N�E�H�[�g��������_�u���N�E�H�[�Ƃ̏I���܂�.
		if ( pure[i] == '"')
		{
			int start = i ;
			for(int p = i+1; p < size ; p++)
			{
				//2Byte ������������X�L�b�v.
				if ( MultiString::isKanji(pure[p], pure[p+1]) )
				{
					p ++;		continue;
				}
				//�I���ł���?
				if (pure[p] == '"')
				{
					break;
				}
			}
			//�X�^�b�N�� push
			param.push_back( pure.substr(start + 1, p - start - 1) );

			i = p;
		}
		else if ( ! TRIM_SPACE(pure[i]) )
		{//�󔒈ȊO�����ꂽ�炻������󔒈ȊO���������܂�
			int start = i ;
			for(int p = i+1; p < size ; p++)
			{
				//2Byte ������������X�L�b�v.
				if ( MultiString::isKanji(pure[p], pure[p+1]) )
				{
					p ++;		continue;
				}
				//�󔒏I���?
				if (TRIM_SPACE(pure[p]))
				{
					break;
				}
			}
			//�X�^�b�N�� push
			param.push_back( pure.substr(start , p - start ) );

			i = p;
		}
	}
	//�p�����[�^�Ȃ�
	if ( param.size() <= 0 ) return "";

	//�R�}���h�ȊO�̃p�����[�^���Ȃ��ꍇ...
	bool isNonParam = param.size() <= 1;

	//�o�͂��邠����.
	string retStdOut ;
/*
  &VIEW		�����G�f�B�^
  &EXEC		�֘A�t�����g���Ď��s
  &NISEOLE	�֘A�t�����g���Ď��s NISEOLE
  &RELOAD	�p�X�̍ēǂݍ���
  &BACK		�q�X�g���[�o�b�N
  &NEXT		�q�X�g���[�t�H���[�h
  &UPDIR	�A�b�v
  &TAB		�w�肳�ꂽ�^�u�Ɉړ�

  &MASK		�w�肳�ꂽ�}�X�N
  &CLIP		�N���b�v�{�[�h�ɓ]��

  &TOW		���Α��̃E�B���h�E��

  &FOR		$MS , ,`&ECHO`

  &EXPR		�v�Z.

  &MARK		�����Ƀ}�b�`����t�@�C�����}�[�N
  &PRPTY	�v���p�e�B���J��
  &ECHO		��ʂɏo��
  &GREP		�t�@�C����茟��
*/
	//�E�B���h�E"�Ǘ�"�l
	const WindowsMain*	main = WindowsMain::getMainSystem();
	//�t�@�C���["�Ǘ�"�l
	const Filer*		filer = Filer::getFiler();
	//�^�u����"�Ǘ�"�l
	FilerSub*		ActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	FilerSub*		NonActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	//���݃A�N�e�B���ȃE�B���h�E��"�Ǘ�"�l
	Window*		ActiveWindow = ActiveFiler->getActiveWindow();
	Window*		TargetWindow = ActiveFiler->getTargetWindow();
	Window*		NonActiveWindow = NonActiveFiler->getActiveWindow();

	const string command = param[0];
	try
	{
		if (command == "&VIEW")
		{
			if (isNonParam)		ActiveWindow->PreviewOrLs(inStdOut);
			else				ActiveWindow->PreviewOrLs( param[1] );
		}
		else if (command == "&EXEC")
		{
			if (isNonParam)		::ShellExecute( main->getWindowHandle() , NULL , inStdOut.c_str() , NULL , NULL , SW_NORMAL);
			else				::ShellExecute( main->getWindowHandle() , NULL ,  param[1].c_str()  , NULL , NULL , SW_NORMAL);
		}
		else if (command == "&RELOAD")
		{
			ActiveWindow->Refresh();
		}
		else if (command == "&NEXT")
		{
			ActiveWindow->HistoryNext();
		}
		else if (command == "&BACK")
		{
			ActiveWindow->HistoryBack();
		}
		else if (command == "&UPDIR")
		{
			ActiveWindow->UpDirectory();
		}
		else if (command == "&MENU")
		{
			if (isNonParam)		ActiveWindow->PopupMenu(inStdOut);
			else				ActiveWindow->PopupMenu( param[1] );
		}
		else if (command == "&INPUT")
		{
			if ( param.size() <= 1)
			{
				throw RParseException("&INPUT �̃p�����[�^�͍Œ�ł�2�K�v�ł�");
			}
			//���̓_�C�A���O���쐬���A���͂����߂�.
			RInputDialog dia;
			if ( param.size() == 2)		retStdOut = dia.DoModal(param[1] , param[2] , inStdOut);
			else						retStdOut = dia.DoModal(param[1] , param[2] , param[3]);
		}
		else if (command == "&EXPR")
		{
			RExpr expr;
			if (isNonParam)		retStdOut = expr.Expr(inStdOut);
			else				retStdOut = expr.Expr(param[1]);
		}
		else if (command == "&ECHO")
		{
			if (isNonParam)		retStdOut = inStdOut;
			else				retStdOut = param[1];
		}
		else if (command == "&TABCLOSE")
		{
			ActiveFiler->CloseWindow(TargetWindow);
		}
		else if (command == "&STOPTASKS")
		{
			ActiveWindow->StopTasks();
		}
		else if (command == "&MASK")
		{
			if (isNonParam)		ActiveWindow->setMask(inStdOut);
			else				ActiveWindow->setMask( param[1] );
		}
	}
	catch(RException e)
	{
		throw RParseException("�R�}���h���s���ɃG���[���������܂���\n%s\n",e.getMessage() );
	}

	return retStdOut;
}




RFunctionScript::RFunctionScript()
{

}

RFunctionScript::~RFunctionScript()
{

}

string RFunctionScript::Interpreter(const string inScript) const throw (RParseException)
{
	Sentence	s;
	return s.Parse( MacroReplace(inScript) );
}

/*
      $F  .....  �J�[�\���ʒu�̃t�@�C�����ɒu���������܂��B
      $W  .....  �J�[�\���ʒu�̊g���q���������t�@�C�����ɒu���������܂��B
      $E  .....  �J�[�\���ʒu�̃t�@�C�����̊g���q�ɒu���������܂��B
      $P  .....  �J�[�\���̂�����̃p�X��
      $O  .....      �V    ���Α��̃p�X��

                   ��L�� $f $w $p $o �̂悤�ɏ������ŋL�q�����
                   �V���[�g�t�@�C���l�[���ɂȂ�܂��B
                   �V���[�g�t�@�C���l�[���������Ȃ������ꍇ��
                   �啶���ŋL�q�������Ɠ��l�̓���ɂȂ�܂��B

                   �J�[�\���� ..�i�e�t�H���_�j���w���Ă��鎞
                   �t�@�C������ NULL�i�󕶎��j��Ԃ��܂��B

      $K  .....  �L�q�����ʒu�ɃJ�[�\�����Z�b�g���ăL�[���̓E�C���h�E��
                 �J���܂��B

      $$  .....  $ ��\���܂��B

      $JW  ..... ���݂̃E�B���h�E �� ���� ��\���܂��B
      $JH  ..... ���݂̃E�B���h�E �� �c�� ��\���܂��B

	  $Z  .....  �}�X�N�̎擾.

      $V"env"
          .....  ���s���_�ł̊��ϐ� env �ɒu���������܂��B

                 ���ϐ������ɑS���������� afx �Ǝw�肷���
                 ���ӂ̋N���p�X�𓾂鎖���o���܂��B(���� \ ����)
                 ���̏ꍇ�P�����ł��啶�������������ꍇ��
                 ���ϐ� AFX �̓��e�ɒu���������܂��B

      $0 �` $9
          .....  �ϐ��i0�`9�j�ɓ����Ă��镶����ɒu���������܂��B
                 ����͓������� &SET �ōs���܂��B
*/
string RFunctionScript::MacroReplace(const string inScript) const throw(RParseException)
{
	//�E�B���h�E"�Ǘ�"�l
	const WindowsMain*	main = WindowsMain::getMainSystem();
	//�t�@�C���["�Ǘ�"�l
	const Filer*		filer = Filer::getFiler();
	//�^�u����"�Ǘ�"�l
	const FilerSub*		ActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	const FilerSub*		NonActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	//���݃A�N�e�B���ȃE�B���h�E��"�Ǘ�"�l
	const Window*		ActiveWindow = ActiveFiler->getActiveWindow();
	const Window*		NonActiveWindow = NonActiveFiler->getActiveWindow();

	string activeFilename = ActiveWindow->getActiveFilename();

	string ret;
	int parseStart = 0;
	int size = inScript.size();
	if (size <= 0) return inScript;

	const char * aa = inScript.c_str();

	for(int i = 0 ; i < size ; i++)
	{
		if (inScript[i] == '$')
		{
			switch(inScript[i+1])
			{
			//$x 2�o�C�g�X�L�b�v.
			case 'F':	//�J�[�\���ʒu�̃t�@�C�����ɒu���������܂��B
			case 'W':	//�J�[�\���ʒu�̊g���q���������t�@�C�����ɒu���������܂��B
			case 'E':	//�J�[�\���ʒu�̃t�@�C�����̊g���q�ɒu���������܂��B
			case 'P':	//�J�[�\���̂�����̃p�X��
			case 'O':	//�J�[�\���̔��Α��̃p�X��
			case 'w':	//��������
			case 'p':	//��������
			case 'o':	//��������
			case 'K':	//�L���b�V�b�W�̌Œ�(�����ł́A�������܂�)
			case 'Z':	//�}�X�N�̎擾.
			case '$':	//$�Ƀ}�b�`.
			case '0':			case '1':			case '2':			case '3':
			case '4':			case '5':			case '6':			case '7':
			case '8':			case '9':
				{
					ret += inScript.substr(parseStart , i -parseStart );
					switch(inScript[i+1])
					{
					case 'F':	//�J�[�\���ʒu�̃t�@�C�����ɒu���������܂��B
						ret += activeFilename.c_str() ;
						break;
					case 'W':	//�J�[�\���ʒu�̊g���q���������t�@�C�����ɒu���������܂��B
						ret += getFilenameOnly(activeFilename.c_str());
						break;
					case 'E':	//�J�[�\���ʒu�̃t�@�C�����̊g���q�ɒu���������܂��B
						ret += getExtOly(activeFilename.c_str());
						break;
					case 'P':	//�J�[�\���̂�����̃p�X��
						ret += ActiveWindow->getNowPath() ;
						break;
					case 'O':	//�J�[�\���̔��Α��̃p�X��
						ret += NonActiveWindow->getNowPath();
						break;
					case 'w':	//��������
						ret += ConvertSortFilename( getFilenameOnly(activeFilename.c_str()) );
						break;
					case 'p':	//��������
						ret += ConvertSortFilename( ActiveWindow->getNowPath() );
						break;
					case 'o':	//��������
						ret += ConvertSortFilename( NonActiveWindow->getNowPath() );
						break;
					case 'K':	//�L���b�V�b�W�̌Œ�(�����ł́A�������܂�)
						ret += "$K";
						break;
					case '$':	//$�Ƀ}�b�`.
						ret += "$";
						break;
					case 'Z':	//�}�X�N�̎擾.
						ret += ActiveWindow->getMask();
						break;
					case '0':			case '1':			case '2':			case '3':
					case '4':			case '5':			case '6':			case '7':
					case '8':			case '9':
						ret += activeFilename.c_str();
						break;
					}
					i = i + 1;
					parseStart = i+1;
				}
				break;
			//$x 3�o�C�g�X�L�b�v.
			case 'J':	//���ƍ���
				{
					ret += inScript.substr(parseStart , i -parseStart );
					switch(inScript[i+1])
					{
					case 'J':	//���ƍ���
						{
							RECT rc;
							ActiveWindow->GetWindowRect(&rc);
							switch(inScript[i+2])
							{
							case 'W':
								ret += RConv::itoa( rc.right - rc.left );
								break;
							case 'H':
								ret += RConv::itoa( rc.bottom- rc.top );
								break;
							default:
								throw RParseException("�}�N���W�J�G���[ %s �� %d ������:J�I�v�V�����̂��Ƃ� W / H ���w�肵�Ă�������",inScript.c_str() ,i);
								break;
							}
						}
						break;
					}
					i = i + 2;
					parseStart = i+2;
				}
				break;
			default:	//�s��.
				throw RParseException("�}�N���W�J�G���[ %s �� %d ������:����ȃI�v�V��������܂���",inScript.c_str() ,i);
				break;
			}
		}
	}

	ret += inScript.substr(parseStart , i - parseStart );
	return ret;
}

