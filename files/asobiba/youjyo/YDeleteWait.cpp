// YDeleteWait.cpp: YDeleteWait �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YDeleteWait.h"
#include "YHttpdConf.h"
#include "YDATReader.h"
#include "YBBSManager.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YDeleteWait::YDeleteWait()
{

}

YDeleteWait::~YDeleteWait()
{
	Clear();
}

//���ׂč폜
void YDeleteWait::Clear()
{
	for(YDELETE_WAIT_DATA::const_iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		delete (*i);
	}
	Pool.clear();

}

void YDeleteWait::Load(const string inBoard) 
{
	Clear();

	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoard + "/delete.txt";

	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		return ;
	}

	char buf[DELETE_TXT_LINE];

	while( !feof(fp) )
	{
		if ( fgets(buf , DELETE_TXT_LINE , fp) == NULL ) break;

		//�폜�˗�����
		const char* deleteRequest = buf;
		//�폜�Ώ�����
		const char* deleteTarget = YDATReader::next(deleteRequest);
		if (deleteTarget == NULL) break;
		//�폜�����s�������t.
		const char* deleteTime = YDATReader::next(deleteTarget);
		if (deleteTime == NULL) break;
		//�폜�Ώ� �ŏ��̈��.
		const char* del = YDATReader::next(deleteTime);
		if (del == NULL) break;

		int targetNo[DELETE_REQUEST_MAX];
		//�폜�Ώۂ�ǂݍ��ݍב�12��.   12-1 �ǂݍ���.
		int i;
		for(i = 0 ; i < DELETE_REQUEST_MAX ; i ++)
		{
			const char* n = YDATReader::next( del );
			if (n == NULL)
			{
				break;
			}
			targetNo[i] = atoi( string(del , 0 , n - del).c_str() );

			del = n;
		}

		Pool.push_back( new YDeleteWaitData(	
					atol( string(deleteRequest , 0 , deleteTarget - buf).c_str() )	,
					atol( string(deleteTarget , 0 , deleteTime - deleteRequest).c_str() )	,
					atol( string(deleteTime , 0 , del - deleteTarget).c_str() )	,
					i ,
					targetNo
			) );
	}
	fclose(fp);

	std::sort(Pool.begin() , Pool.end() ,YDeleteWaitData::Compre);
}

void YDeleteWait::Save(const string inBoard) 
{
	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoard + "/delete.txt";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		return ;
	}


	for(YDELETE_WAIT_DATA::const_iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		string l = (*i)->getCompateLine() + "\n";

		fputs(l.c_str() , fp);
	}
	fclose(fp);
}


/**
 * Check:				���Ԃ������Ă����炠�ځ[�񂷂�.
 *
 * @param outShareBuffer
 * @param inRequestIta 
 * @param inTargetIta 
 * @return void 
 */
void YDeleteWait::Check(char* outShareBuffer,const string inRequestIta , const string inTargetIta)
{
	time_t timeLimit = time(NULL);

	for(YDELETE_WAIT_DATA::iterator i = Pool.begin() ; i != Pool.end() ; )
	{
		//���Ԃ����Ă����炠�ځ[�񂷂�.
		YDeleteWaitData * deleteData = (*i);
		if ( deleteData->isTimeLimit(timeLimit) )
		{
			deleteData->DoAborn(outShareBuffer , inRequestIta , inTargetIta);
			i = Pool.erase(i);
			delete deleteData;
		}
		else		break;	//��ɏ����Ƀ\�[�g����Ă���̂ŁA����ȏ㒲�ׂĂ��Ӗ����Ȃ�.
	}
}



//���ځ[����s.
void YDeleteWaitData::DoAborn(char* outShareBuffer,const string inRequestIta , const string inTargetIta)
{
	try
	{
		//�폜���s
		{
			YBBSManager::getInstance()->Aborn(inTargetIta , this->DeleteTarget , this->DeleteSize , this->Delete);
		}
		/*ChangeLog.txt:2003/02/22	�Ӌ` -> �ًc
		*/
		//�폜�����̏�������.
		{
			POST	post;
			post["bbs"]  = inRequestIta;
			post["FROM"]  = "YoujyoKiller��";
			post["mail"]  = "age";
			post["MESSAGE"]  = "���Ԃ܂ŁA�ًc�̐\���o���Ȃ��������߂ɁA���N�G�X�g����Ă��������͍폜����܂����B";
			post["key"]  = num2str( this->DeleteRequest );
			post["submit"] = "��������";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}
		//�폜�����s���ꂽ���Ƃ��^�[�Q�b�g�X���ɏ�������
		{
			POST	post;
			post["bbs"]  = inTargetIta;
			post["FROM"]  = "YoujyoKiller��";
			post["mail"]  = "age";
			post["MESSAGE"]  = "���Ԃ܂ŁA�ًc�̐\���o���Ȃ��������߂ɁA�폜�˗��͎��s����܂����B";
			post["key"]  = num2str( this->DeleteTarget );
			post["submit"] = "��������";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}
	}
	catch(RException e)
	{
		printf("�폜�G���[���������܂���!! %s.\n�폜�˗� %s / �^�[�Q�b�g %s / �폜�˗�DAT %ld\n" ,e.getMessage() ,inRequestIta.c_str() , inTargetIta.c_str() ,this->DeleteRequest);
		try
		{
			//�폜�����̏�������.
			{
				POST	post;
				post["bbs"]  = inRequestIta;
				post["FROM"]  = "YoujyoKiller��";
				post["mail"]  = "age";
				post["MESSAGE"]  = string("�폜���ɃG���[���������܂����B<br>�폜�͎��s���܂����B<br>�Ǘ��l�ɂ��A����������<br>") + e.getMessage();
				post["key"]  = num2str( this->DeleteRequest );
				post["submit"] = "��������";

				YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
			}
		}
		catch(RException e)
		{
			//�G���[�񍐂��G���[������A�������߂�.
			printf("�폜�G���[�̕񍐂Ɏ��s���܂����A�������߂�!! %s.\n�폜�˗� %s / �^�[�Q�b�g %s / �폜�˗�DAT %ld\n" ,e.getMessage() ,inRequestIta.c_str() , inTargetIta.c_str() ,this->DeleteRequest);
		}
	}
}

//�폜�˗��̒�o.
void YDeleteWait::Presentation(DATNUMBER inDeleteRequest , DATNUMBER inDeleteTarget , 
									time_t inDeleteTime , int inSize ,const int*	inDelete)
{
	Pool.push_back( new YDeleteWaitData(	
				inDeleteRequest	,
				inDeleteTarget	,
				inDeleteTime	,
				inSize ,
				inDelete
		) );
	std::sort(Pool.begin() , Pool.end() ,YDeleteWaitData::Compre);
}

//�폜�˗��̃L�����Z��.
bool YDeleteWait::Cancel(DATNUMBER inDeleteRequest)
{
	for(YDELETE_WAIT_DATA::iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		YDeleteWaitData * deleteData = (*i);
		if (deleteData->IsYourRequest(inDeleteRequest) )
		{
			Pool.erase(i);
			delete deleteData;
			return true;
		}
	}
	//����ȃ��N�G�X�g����܂���.
	return false;
}

//���Ȃ����ێ����Ă���폜�˗��ł���?
DATNUMBER YDeleteWait::IsYourRequest(DATNUMBER inDeleteRequest)
{
	for(YDELETE_WAIT_DATA::iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		YDeleteWaitData * deleteData = (*i);
		if (deleteData->IsYourRequest(inDeleteRequest) )
		{
			return deleteData->getDeleteTarget();
		}
	}
	//����ȃ��N�G�X�g����܂���.
	return 0;
}


void YDeleteWait::test()
{
	int deleted[12];
	string r;
	{
		YDeleteWait d;

		deleted[0] = 1;
		deleted[1] = 2;
		deleted[2] = 3;
		deleted[3] = 4;
		d.Presentation(10, 100 , 1000 , 4 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");
		}
	}
	{
		YDeleteWait d;

		deleted[0] = 1;
		deleted[1] = 2;
		deleted[2] = 3;
		deleted[3] = 4;
		d.Presentation(10, 100 , 1000 , 4 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");
		}

		deleted[0] = 5;
		deleted[1] = 4;
		deleted[2] = 3;
		deleted[3] = 2;
		deleted[4] = 1;
		d.Presentation(510, 5100 , 100 , 5 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "510<>5100<>100<>5<>4<>3<>2<>1<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");
		}

		deleted[0] = 9;
		deleted[1] = 8;
		deleted[2] = 7;
		deleted[3] = 6;
		deleted[4] = 5;
		d.Presentation(810, 8100 , 10000 , 5 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "510<>5100<>100<>5<>4<>3<>2<>1<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "810<>8100<>10000<>9<>8<>7<>6<>5<>");
		}

		d.Save("test");
		d.Load("test");
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "510<>5100<>100<>5<>4<>3<>2<>1<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "810<>8100<>10000<>9<>8<>7<>6<>5<>");
		}
	}
	puts("YDeleteWait::test() OK");
}
