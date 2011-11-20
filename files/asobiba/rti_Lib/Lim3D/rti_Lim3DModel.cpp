#include <./Lim3D/rti_Lim3DModel.h>

#include <./Lim3D/rti_Lim3DMaster.h>

//�R���X�g���N�^
TLim3DModel::TLim3DModel()
{
	m_Vec		= new TLim3DVecCaretaker;		//���_���W�σN���X
	m_Polygon	= new TLim3DPolygonCaretaker;	//�|���S�����W�σN���X
	m_Make		= new TLim3DMake;				//���f���e�q�֌W
	m_Point		= new TLim3DPoint;				//�ʒu
	m_Breaking		= false;					//���������󂵂Ă���.

	m_Vec->Mount_form_Model(this);				//���_�֌W�Ɏ������e���Ɠ`����.
	m_Make->Mount_form_Model(this);				//���f���e�q�֌W�Ɏ������e���Ɠ`����.
	m_Polygon->Mount_form_Model(this);			//�|���S���Ǘ��Ɏ������e���Ɠ`����.
}

//�f�X�g���N�^
TLim3DModel::~TLim3DModel()
{
	m_Point->Release();
	m_Polygon->Release();
	m_Vec->Release();
	m_Make->Release();
}

//�K�y�[�W�R���N�V�������폜.
bool	TLim3DModel::Del(TLim3DModel* Model)
{
	//���ݎ���������ɂ���Ȃ�....
	if (m_Breaking == true) return true;	//���������󂵂Ă���̂Ŏq�ǂ��̂��Ƃ͖���.
	//�K�y�[�W�R���N�V�������T���o���폜.
	m_Make->Del( Model );

	return true;
}

//�e���f���̎擾
inline TLim3DModel* TLim3DModel::GetParenceModel()
{
	return  ((TLim3DMake*)(m_Parence))->GetParenceModel();
}

/*****************/
//���[�U�[�̂��߂̍����x�����\�b�h.
/*****************/
//���̃��f���ȉ��̃��f���̊G��.
void	TLim3DModel::Drawing()
{
	ClasOnly();
	DrawOnly();
}

//���̃��f���ȉ��̃��f���̊G��.
void	TLim3DModel::Drawing(TMatrix * Ma)
{
	ClasOnly(Ma);
	DrawOnly();
}

//�j��.
void	TLim3DModel::Release()
{
	//�����������ԂƂ���.
	m_Breaking = true;
	//�e�N���X.
	TLim3DModel*	theModel = GetParenceModel();
	//�e�ɍ폜��`����.
	if (theModel == NULL)
		(TLim3DMaster::GetModelCaretaker())->Del(this);
	else
		theModel->Del(this);
	delete this;
}

//���_���̐ݒ�.
bool			TLim3DModel::SetVec(XYZFixed* _c ,	int Many , bool SizeNotChange)
{
	return m_Vec->Set(_c,Many,SizeNotChange);
}

//���_���̒ǉ�.
bool			TLim3DModel::AddVec(XYZFixed* _c ,	int Many , bool SizeNotChange )
{
	return m_Vec->Add(_c,Many,SizeNotChange);
}

//���ׂĂ̒��_�̔j��
void			TLim3DModel::ClearVec()
{
	m_Vec->Clear();
}
//�|���S�����̐ݒ�.
TLim3DPolygon* 	TLim3DModel::NewPolygon()
{
	return m_Polygon->New();
}
//���f���ɑ��݂��邷�ׂẴ|���S���̔j��.
void			TLim3DModel::ClearPolygon()
{
	m_Polygon->Clear();
}
//�|���S�������J�n
TLim3DPolygon*	TLim3DModel::FindFirstPolygon()
{
	return m_Polygon->FindFirst();
}
//�|���S���������s.
TLim3DPolygon*	TLim3DModel::FindNextPolygon()
{
	return m_Polygon->FindNext();
}
//���f���̉�]�p�x���w��.
void			TLim3DModel::SetRotate(Angle x,Angle y,Angle z)
{
	m_Point->SetAngleX(x);
	m_Point->SetAngleY(y);
	m_Point->SetAngleZ(z);
}
//���f���̏ꏊ���w��.
void			TLim3DModel::SetPos(Fixed x,Fixed y,Fixed z)
{
	m_Point->SetPosX(x);
	m_Point->SetPosY(y);
	m_Point->SetPosZ(z);
}
//���f���̉�]�p�x���擾.
void			TLim3DModel::GetRotate(Angle *x,Angle *y,Angle *z)
{
	*x = m_Point->GetAngleX();
	*y = m_Point->GetAngleY();
	*z = m_Point->GetAngleZ();
}
//���f���̏ꏊ���擾.
void			TLim3DModel::GetPos(Fixed *x,Fixed *y,Fixed *z)
{
	*x = m_Point->GetPosX();
	*y = m_Point->GetPosY();
	*z = m_Point->GetPosZ();
}
//���f���̉�]�p�x�𑝂₷.
void			TLim3DModel::AddRotate(int x,int y,int z)
{
	m_Point->SetAngleX(m_Point->GetAngleX() + x);
	m_Point->SetAngleY(m_Point->GetAngleY() + y);
	m_Point->SetAngleZ(m_Point->GetAngleZ() + z);
}
//���f���̏ꏊ���ړ�����.
void			TLim3DModel::AddPos(Fixed x,Fixed y,Fixed z)
{
	m_Point->SetPosX(m_Point->GetPosX() + x);
	m_Point->SetPosY(m_Point->GetPosY() + y);
	m_Point->SetPosZ(m_Point->GetPosZ() + z);
}
//�q���f�����쐬����.
TLim3DModel*	TLim3DModel::NewModel()
{
	return m_Make->New();
}
//���ׂĂ̎q���f�����폜
void			TLim3DModel::ClearModel()
{
	m_Make->Clear();
}
//���f�������J�n
TLim3DModel*	TLim3DModel::FindFirstModel()
{
	return m_Make->FindFirst();
}
//���f���������s.
TLim3DModel*	TLim3DModel::FindNextModel()
{
	return m_Make->FindNext();
}

//�d�����_�̐؂藣��(����댯����ˁ[)
void			TLim3DModel::Optimize()
{
	//���_���N���X�ւ̃x�^�A�N�Z�X
	TLim3DVecCaretaker*		theVec = GetVecCaretaker();
	//�|���S�����N���X�ւ̃x�^�A�N�Z�X
	TLim3DPolygonCaretaker*	thePoly = GetPolygonCaretaker();

	//�i�[���ꂵ�f�[�^.
	VecWorldEye* theSrc  = theVec->GetStart();
	//���ׂẴf�[�^�̐�.
	int theEnd = theVec->GetCount();
	//�O�̂��߁A���݃f�[�^������̂��ǂ����`�F�b�N.
	if (theEnd <= 1) return ;
	//���[�v�J�E���^�[.
	int			 theI , theL ;
	//�I���W�i���f�[�^���i�[����o�b�t�@.
	XYZFixed* theOrignalDataTemp = NULL;
	//�d�����Ă����� true ��������t���O.
	bool*     theDuplicationBuffer = NULL;

	try
	{
		//�I���W�i���f�[�^���i�[����o�b�t�@.
		theOrignalDataTemp = new XYZFixed[ theEnd ];
		//�d�����Ă����� true ��������t���O.
		theDuplicationBuffer = new bool[ theEnd ];
		//�O�̂��ߊm�ۂł������`�F�b�N. 
		if ( theOrignalDataTemp == NULL || theDuplicationBuffer == NULL) throw ;
		//�d���t���O�� false �ŃN���A���܂�.
		memset(theDuplicationBuffer , 0 ,sizeof(bool)*theEnd );
		//�I���W�i���f�[�^�̓Y�����ł�.
		int			 theOrignalDataNum = 0;
	
		for( theI = 0 ; theI < theEnd ; theI++)
		{//theI �ɑ΂��Ă̏d���f�[�^���Ȃ����`�F�b�N���܂�.
			//�d���t���O�������Ă�����A���̃f�[�^�͔�΂��܂�.
			if (theDuplicationBuffer[theI] == true) continue;

			XYZFixed * theSrcVec = &(theSrc[theI].Vec);
			for( theL = theI ; theL < theEnd ; theL++)
			{
				XYZFixed * theDestVec = &(theSrc[theL].Vec);
				if ( theSrcVec->x == theDestVec->x && 
					 theSrcVec->y == theDestVec->y && 
					 theSrcVec->z == theDestVec->z )
				{//���� TheL �́A theI �̃f�[�^�Əd�����Ă��܂�.
					//�d���t���O�𗧂Ă܂�.
					theDuplicationBuffer[theL] = true;
					//���� TheL ���g�p���Ă���|���S���� Link�f�[�^�� theOrignalDataNum�ɕt���ւ��܂�.
					//OptimizePolygonLinkChange(theL , theOrignalDataNum);
					OptimizePolygonLinkChange( 
						&(theSrc[theL]) , &(theSrc[theOrignalDataNum]) );
				}
			}
			//�I���W�i���f�[�^�Ƃ��ċL�^���܂�.
			theOrignalDataTemp[theOrignalDataNum++] = *theSrcVec;
		}
		//�d�����Ă��Ȃ��f�[�^���Z�b�g.
		theVec->Set( theOrignalDataTemp , theOrignalDataNum	 , true);
		//DEBUG3("�œK���ɂ���� %d ���_�� %d �ɂȂ�܂���",theEnd , theOrignalDataNum);
	}
	catch(...)
	{
	}
	//�d���t���O�̔j��.
	delete [] theDuplicationBuffer;
	//�d�����Ă��Ȃ��f�[�^��j��.
	delete [] theOrignalDataTemp;
}


/*****************/
//���[�U�[�̂��߂̒჌�x�����\�b�h.
/*****************/

//�v�Z�̂�(���_�v�Z)
void	TLim3DModel::ClasOnly()
{
	//�܂��A��]�s����쐬���āA���邭�邭��[���Ċ����Ń��[���h�ֈڍs���܂�.
	TMatrix		Mat;			//�s��
	//��]�s��쐬.
	Mat.MakeTrans(m_Point->GetAngleX() , m_Point->GetAngleY() , m_Point->GetAngleZ() ,
					m_Point->GetPosX() , m_Point->GetPosY() , m_Point->GetPosZ());
	//���ׂĂ̒��_���ړ������邼!
	VecWorldEye* it  = m_Vec->GetStart();
	VecWorldEye* end = m_Vec->GetEnd();
	for ( ; it != end ; it++)
	{
		//it->World = it->Vec;
		Mat.VectorMul( &(it->World) , &(it->Vec) );
		//����ŁA ���[���h�͂��Ƃ܂�܂���.

		//���͎��_���W�ɕϊ����܂�.
		(TLim3DMaster::GetSelectEye())->EyeConbert( &(it->Eye) , &(it->World) );
		//���_���W�ɂȂ�܂���.
	}
	//�q���f�����G�悵�܂�.
	TLim3DModel*	theChildModel ;
	for( theChildModel = FindFirstModel()  ;
				theChildModel != NULL ; theChildModel = FindNextModel() )
	{
		theChildModel->Drawing( &Mat );
	}
}

//�v�Z�̂�(���_�v�Z)
void	TLim3DModel::ClasOnly(TMatrix * inParenceMatrix)
{
	//�܂��A��]�s����쐬���āA���邭�邭��[���Ċ����Ń��[���h�ֈڍs���܂�.
	TMatrix		Mat;			//�s��
	//��]�s��쐬.
	Mat.MakeTrans(m_Point->GetAngleX() , m_Point->GetAngleY() , m_Point->GetAngleZ() ,
					m_Point->GetPosX() , m_Point->GetPosY() , m_Point->GetPosZ());
	//�e�̍s��������܂�.
	Mat.Synthesis(inParenceMatrix);

	//���ׂĂ̒��_���ړ������邼!
	VecWorldEye* it  = m_Vec->GetStart();
	VecWorldEye* end = m_Vec->GetEnd();
	for ( ; it != end ; it++)
	{
		//it->World = it->Vec;
		Mat.VectorMul( &(it->World) , &(it->Vec) );
		//����ŁA ���[���h�͂��Ƃ܂�܂���.

		//���͎��_���W�ɕϊ����܂�.
		(TLim3DMaster::GetSelectEye())->EyeConbert( &(it->Eye) , &(it->World) );
		//���_���W�ɂȂ�܂���.
	}
	//�q���f�����G�悵�܂�.
	TLim3DModel*	theChildModel ;
	for( theChildModel = FindFirstModel()  ;
				theChildModel != NULL ; theChildModel = FindNextModel() )
	{
		theChildModel->Drawing( &Mat );
	}
}

//�G��̂�(���_�v�Z�����łɂ���Ă��邱�Ƃ�����)
void	TLim3DModel::DrawOnly()
{
	TLim3DPolygon* Poly = m_Polygon->FindFirst();
	if (Poly == NULL)	return ;
	do
	{
		Poly->Draw();		//�|���S���ɊG�施�ߔ��s.
	}
	while( Poly = m_Polygon->FindNext() );
}


//���_���N���X�ւ̃x�^�A�N�Z�X
TLim3DVecCaretaker*		TLim3DModel::GetVecCaretaker()
{
	return m_Vec;
}

//�|���S�����N���X�ւ̃x�^�A�N�Z�X
TLim3DPolygonCaretaker*	TLim3DModel::GetPolygonCaretaker()
{
	return m_Polygon;
}

//�e�q���N���X�ւ̃x�^�A�N�Z�X
TLim3DMake*				TLim3DModel::GetMake()
{
	return m_Make;
}

//�ꏊ���N���X�ւ̃x�^�A�N�Z�X
TLim3DPoint*			TLim3DModel::GetPoint()
{
	return m_Point;
}


//�|���S���̎w�肳�ꂽ�Ȃ���f�[�^(inTarget)��V�����f�[�^(inNew)�ɕύX���܂�.
void	TLim3DModel::OptimizePolygonLinkChange(VecWorldEye* inTarget ,VecWorldEye* inNew)
{
	TLim3DPolygon* Poly = m_Polygon->FindFirst();
	if (Poly == NULL)	return ;
	do
	{	//�Ȃ���f�[�^�@�\�ւ� �ׂ��A�N�Z�X�擾.
		TLim3DLinkData* theLinkData = Poly->GetLinkData();
		//�|���S���̃����N�f�[�^�ɃA�N�Z�X���āA�����������s���܂�.
		PolygonDataEx* thePolygonLinkDataIt = theLinkData->GetStart();
		PolygonDataEx* thePolygonLinkDataEnd= theLinkData->GetEnd();
		
		for( ; thePolygonLinkDataIt != thePolygonLinkDataEnd ; thePolygonLinkDataIt ++)
		{
			if ( thePolygonLinkDataIt->LinkVecPointer == inTarget)
			{	//�^�[�Q�b�g�𔭌����܂���. ���������܂�.
				thePolygonLinkDataIt->LinkVecPointer = inNew;
			}
		}
	}
	while( Poly = m_Polygon->FindNext() );
}

//���_�f�[�^�Ǘ��N���X�ŁA���_�f�[�^�̍ăA���P�[�V���������s���ꂽ.
void	TLim3DModel::ReAllocMemoryVec(VecWorldEye* OldMemoryAddress ,
							VecWorldEye* NewMemoryAddress ,  int OldUseSize)
{
	VecWorldEye*	TempData;
	VecWorldEye*	OldMemoryAddressEnd = OldMemoryAddress+OldUseSize;
	TLim3DPolygon* Poly = m_Polygon->FindFirst();
	if (Poly == NULL)	return ;
	do
	{	//�Ȃ���f�[�^�@�\�ւ� �ׂ��A�N�Z�X�擾.
		TLim3DLinkData* theLinkData = Poly->GetLinkData();
		//�|���S���̃����N�f�[�^�ɃA�N�Z�X���āA�����������s���܂�.
		PolygonDataEx* thePolygonLinkDataIt = theLinkData->GetStart();
		PolygonDataEx* thePolygonLinkDataEnd= theLinkData->GetEnd();

		for( ; thePolygonLinkDataIt != thePolygonLinkDataEnd ; thePolygonLinkDataIt ++)
		{
			//�������炵���Ȃ�̂ŁA�����Ώۂ̃f�[�^���ʂ��Ă���.
			TempData = thePolygonLinkDataIt->LinkVecPointer;
			if ( TempData >= OldMemoryAddress && TempData < OldMemoryAddressEnd)
			{	//�^�[�Q�b�g�𔭌����܂���. ���������܂�.
				thePolygonLinkDataIt->LinkVecPointer =
					(VecWorldEye*)((DWORD)TempData -
							(DWORD)OldMemoryAddress + (DWORD)NewMemoryAddress);
			}
		}
	}
	while( Poly = m_Polygon->FindNext() );

	//�q���f���ɂ���āA���L����Ă��邩������Ȃ��̂ŁA
	//�O�̂��߂��ׂĂ̎q������.
	TLim3DModel*	theChildModel ;
	for( theChildModel = FindFirstModel()  ;
				theChildModel != NULL ; theChildModel = FindNextModel() )
	{
		theChildModel->ReAllocMemoryVec
					(OldMemoryAddress , NewMemoryAddress , OldUseSize);
	}
}


//�w�肵�����f���̃R�s�[�Ɏ������Ȃ�.
void	TLim3DModel::CopyForm(TLim3DModel * inModel)
{
	m_Point->CopyForm( inModel->GetPoint() );
//	m_Polygon->CopyForm( inModel->GetPolygonCaretaker() );
//	m_Vec->CopyForm( inModel->GetVecCaretaker() );
//	m_Make->CopyForm( inModel->GetMake() );
}


//�w�肳�ꂽ���f���������B��������@�e�q�֌W�Ō����u����c�v�ɂ����邩�ǂ������ׂ�.
bool TLim3DModel::IsForefathers(TLim3DModel * inModel)
{
	if ( this == inModel) return true;

	TLim3DModel * theParenceModel = GetParenceModel();
	if (theParenceModel == NULL) return false;
	return theParenceModel->IsForefathers(inModel);
}



//�����̐e�ɑ΂��ẴX�L�����������s����.
void TLim3DModel::Skin()
{
	TLim3DModel * theParenceModel = GetParenceModel();
	//�e�����Ȃ��ꍇ�͖v.
	if (theParenceModel == NULL) return ;

	//�����̐e�̒��_�ւ̃A�N�Z�X�o�H������.
	TLim3DVecCaretaker* theParenceVec = theParenceModel->GetVecCaretaker();

	//���[�N�G���A.
	VecWorldEye *it , *end , *ParenceIt , *ParenceEnd;
	//�܂��A��]�s����쐬���āA���邭�邭��[���Ċ����Ń��[���h�ֈڍs���܂�.
	TMatrix		Mat;			//�s��
	//��]�s��쐬.
	Mat.MakeTrans(m_Point->GetAngleX() , m_Point->GetAngleY() , m_Point->GetAngleZ() ,
					m_Point->GetPosX() , m_Point->GetPosY() , m_Point->GetPosZ());

	//���ׂĂ̒��_���ړ������邼!
	it  = m_Vec->GetStart();
	end  = m_Vec->GetEnd();
	for ( ; it != end ; it++)
	{
		Mat.VectorMul( &(it->World) , &(it->Vec) );
	}

	//���L�ł��钸�_���T�[�`���܂�.
	ParenceIt  = theParenceVec->GetStart();
	ParenceEnd = theParenceVec->GetEnd();
	for( ; ParenceIt != ParenceEnd ; ParenceIt++)
	{
		it  = m_Vec->GetStart();
		end  = m_Vec->GetEnd();
		for ( ; it != end ; it++)
		{
			//���� World �́A�e���W�ł̘b�ł�.
			if ( VectorCmp( &ParenceIt->Vec , &it->World , IF(3) ) )
			{	//�ύX�ł������Ȃ̂ŁA�|���S���N�ɂ�����.
				OptimizePolygonLinkChange( it , ParenceIt);
			}
		}
	}
}