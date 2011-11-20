// RfilesSMBLogon.cpp: RfilesSMBLogon �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesSMBLogon.h"
#include "MultiString.h"
#include "ROutOfRangeException.h"

//�񋓂ł���l�b�g���[�N���\�[�X�̍ő�l
const int MAX_NETWORKRESOURCE	=	1024;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//�������ł��Ȃ������ꍇ�� RNotSupportException �𓊂���.
RfilesSMBLogon::RfilesSMBLogon(__RFILETYPE inRfileType , const string inPath) 
{
	RfileType = inRfileType;
	Path = inPath;

	switch(RfileType)
	{
	case __RFILETYPE_SMBLOGON_REPORT:
	case __RFILETYPE_SMBLOGON_BIGICON:
		break;
	default:
		throw RNotSupportException("RfilesSMBLogon::�w�肳�ꂽ �t�@�C���`�� %d �� �T�|�[�g���Ă��܂���B",RfileType);
		break;
	}
}
//final
RfilesSMBLogon::~RfilesSMBLogon()
{
}

//�ꗗ�擾
void RfilesSMBLogon::ls()
{
	RTable * rt = getWritebleTable();
	rt->clear();
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	string Provider ,RemoteName;

	NETRESOURCE netResouce = {0};
	netResouce.dwScope = RESOURCE_GLOBALNET;
	netResouce.dwType = RESOURCETYPE_ANY;
	netResouce.dwUsage = RESOURCEUSAGE_CONTAINER;

	NETRESOURCE *nr = &netResouce;

	try
	{
		if (Path.substr(0,6) == "smb://")
		{
			int dilimiter[MAX_DIMLIMITERES] ;
			int maxDilimiter = MultiString::setPathDelimiteres(Path.c_str() , '\\' , dilimiter ,MAX_DIMLIMITERES);
			
			ASSERT(maxDilimiter <= MAX_DIMLIMITERES);
			switch(maxDilimiter)
			{
			case 1:	// smb://\\  //�T�[�r�X�v���o�C�_�̎擾.
				nr = NULL;
				break;
			case 2:	// smb://\\Microsoft Windows Network\\		//�h���C���̌���.
				Provider = Path.substr( dilimiter[0]+1 , dilimiter[1]-(dilimiter[0]+1) );
				netResouce.lpProvider = (char*) Provider.c_str();
				netResouce.lpRemoteName = NULL;
				break;
			case 3:	// smb://\\Microsoft Windows Network\\DOMAIN\\		//�}�V���̌���
				Provider = Path.substr( dilimiter[0]+1 , dilimiter[1]-(dilimiter[0]+1) );
				netResouce.lpProvider = (char*) Provider.c_str();
				RemoteName = Path.substr( dilimiter[1]+1 , dilimiter[2]-(dilimiter[1]+1) );
				netResouce.lpRemoteName = (char*) RemoteName.c_str();
				break;
			default:
				throw ROutOfRangeException("�l�b�g���[�N���\�[�X�̃p�X�̉��߂ɖ�肪����܂�");
			}
			netResouce.dwDisplayType = RESOURCEDISPLAYTYPE_DOMAIN;
		}
		else
		{	// \\\\�}�V���� �̕\�L

			//�Ō�� \\ �����.
			RemoteName = Path.substr( 0 , Path.size() - 1);

			netResouce.dwType = RESOURCETYPE_ANY;
			netResouce.dwDisplayType = RESOURCEDISPLAYTYPE_SERVER;
			netResouce.dwUsage = RESOURCEUSAGE_ALL;
			netResouce.lpProvider = "Microsoft Windows Network";
			netResouce.lpRemoteName = (char*) RemoteName.c_str();
		}

		//�񋓂̊J�n.
		HANDLE hEnum;
		DWORD ret=WNetOpenEnum(RESOURCE_GLOBALNET,RESOURCETYPE_ANY,0, nr,&hEnum);
		if(ret!=NO_ERROR)
		{
			throw RWin32Exception("�l�b�g���[�N���\�[�X�̎擾�Ɏ��s���܂���.");
		}

		NETRESOURCE Array[MAX_NETWORKRESOURCE];
		DWORD Entries = 0xFFFFFFFF;
		DWORD ArraySize = ArraySize=sizeof(Array);
		ret=WNetEnumResource(hEnum,&Entries,Array,&ArraySize);
		if(ret==NO_ERROR)
		{
			DWORD i;
			for(i=0;i<Entries;++i)
			{
				if(Array[i].dwUsage & RESOURCEUSAGE_CONTAINER || Array[i].dwUsage & RESOURCEUSAGE_CONNECTABLE)
				{ // ���\�[�X���擾���܂����B
					RfileNetResourceBigIcon* p = (RfileNetResourceBigIcon*) RfileFactory::New(RfileType);

					string name;
					name = Array[i].lpRemoteName;
					if (Array[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER)
					{	//�T�[�o�[ \\�}�V���� �Ƃ��Ƃł�̂Ő擪�� \\ ���폜
						if (name[0] == '\\' ) name = name.substr(2);
					}
					else if (Array[i].dwDisplayType == RESOURCEDISPLAYTYPE_SHARE)
					{	//���L���� \\�}�V����\\���L �ƂȂ�̂� \\�}�V����\\ �܂ō폜
						name = name.substr( MultiString::getLastDirectoryPos(name)+1 );
					}

					p->Attach(name.c_str() ,Array[i].lpComment , Array[i].dwDisplayType );
					p->SortAttach( sortkey );
					rt->add( p );
				}
			}
		}
		else if(ret!=ERROR_NO_MORE_ITEMS)
		{
			WNetCloseEnum(hEnum);
			throw RWin32Exception("�l�b�g���[�N���\�[�X�̗񋓒��ɃG���[���������܂���.");
		}

		WNetCloseEnum(hEnum);
	}
	catch(RException e)
	{
		TRACE("�擾���� RException ���������܂��� ���R:%s\n" , e.getMessage() );
	}

	rt->Sort();
	rt->createAllList( );
}
//�ꗗ�擾
void RfilesSMBLogon::ls(string inFilter)
{
	//��ʂ��܂�?
	ls();
}

//�폜
void RfilesSMBLogon::rm(string inFilename)
{
}
//���l�[��
void RfilesSMBLogon::rename(string inSrcFilename,string inDestFilename)
{
}
//��ނ�Ԃ�.
__RFILETYPE		RfilesSMBLogon::getRfileType()	const
{
	return RfileType;
}
//�p�X�̎擾
string RfilesSMBLogon::getPath() const 
{
	return Path;
}
//�p�X�̐ݒ�
void		RfilesSMBLogon::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTable���쐬���܂��B
//ls ���g���O�ɌĂ�ł��������B
//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
void RfilesSMBLogon::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	switch(inKey)
	{
	case __FILECOLUMN_TYPE_NAME:
		setTable( new RTableEasy(inKey,IsAscending) );
		break;
	case __FILECOLUMN_TYPE_COMMENT:
		setTable( new RTableMuchDuplication<string>(inKey,IsAscending) );
		break;
	default:
		throw RNotSupportException("�w�肳�ꂽ�^�C�v�̃J���� %d �̓T�|�[�g����Ă��܂���",inKey);
		break;
	}
	return;
}

//�e�X�g.
void RfilesSMBLogon::test()
{
}
