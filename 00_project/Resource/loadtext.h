//============================================================
//
//	�e�L�X�g�Ǎ��֐��w�b�_�[ [loadtext.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LOAD_TEXT_H_
#define _LOAD_TEXT_H_

//************************************************************
//	�O���錾
//************************************************************
class CText2D;		// �e�L�X�g2D�N���X 
class CString2D;	// ������2D�N���X 

//************************************************************
//	���O��Ԑ錾
//************************************************************
// �e�L�X�g�Ǎ����
namespace loadtext
{
	void LoadVector(std::ifstream *pFile, std::vector<std::wstring> *pVecStr);			// ������z��̓Ǎ�
	std::vector<std::wstring> LoadText(const char *pFilePass, const char *pTextStr);	// �e�L�X�g�Ǎ� (������)
	std::vector<std::wstring> LoadText(const char *pFilePass, const int nTextID);		// �e�L�X�g�Ǎ� (�C���f�b�N�X)
	void BindText(CText2D *pText2D, const std::vector<std::wstring> &rVecStr);			// �e�L�X�g����
	void BindString(CString2D *pString2D, const std::vector<std::wstring> &rVecStr);	// �����񊄓�
}

#endif	// _LOAD_TEXT_H_