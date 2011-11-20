// XLException.h: XLException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLEXCEPTION_H__D2331AFF_2532_48BB_AEF0_A3AD924595E1__INCLUDED_)
#define AFX_XLEXCEPTION_H__D2331AFF_2532_48BB_AEF0_A3AD924595E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * class XLException:	��O�N���X�̑匳
 *
 * @note				�Ȃ��A std::exception ���g��Ȃ��̂�?
 *						std::exception �́A�@�\�Ȃ���.
 *						string ���炢�n���Ȃ��悤�ł͍���.
 *						�܂��A����std::exception���p�����č�����Ƃ��Ă��A
 *						���̉��b�͂����������Ƃ��Ȃ��B
 */
class XLException  
{
public:
	/**
	 * XLException:		�R���X�g���N�^
	 *
	 */
	XLException()
	{
	}

	/**
	 * ~XLException:	�f�X�g���N�^
	 *
	 */
	virtual ~XLException()
	{
	}

	/**
	 * XLException:				�R���X�g���N�^
	 *
	 * @param inErrorMessage	�Z�b�g����G���[
	 *
	 */
	XLException(const string & inErrorMessage)
	{
		setError(inErrorMessage);
	}

	/**
	 * getMessage:				�G���[���b�Z�[�W�̎擾
	 *
	 * @return const char*		�G���[���b�Z�[�W
	 */
	const char * getMessage() const
	{
		return this->ErrorMessage.c_str();
	}
protected:
	/**
	 * setError:				�G���[���b�Z�[�W�̐ݒ�
	 *
	 * @param inErrorMessage	�ݒ肷��G���[���b�Z�[�W
	 */
	void setError(string inErrorMessage)
	{
		this->ErrorMessage = inErrorMessage;
	}
private:
	string	ErrorMessage;
};

#endif // !defined(AFX_XLEXCEPTION_H__D2331AFF_2532_48BB_AEF0_A3AD924595E1__INCLUDED_)
