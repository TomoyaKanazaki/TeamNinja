//============================================================
//
//	�}���`�������� [multiValue.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "multiValue.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �}���`�����̗D�揇��
}

//************************************************************
//	�q�N���X [CMultiValue] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMultiValue::CMultiValue() : CObject(CObject::LABEL_UI, CObject::DIM_2D, object::DEFAULT_PRIO),
	m_pos		(VEC3_ZERO),	// ���_�ʒu
	m_rot		(VEC3_ZERO),	// ���_����
	m_size		(VEC3_ZERO),	// �傫��
	m_space		(VEC3_ZERO),	// ���
	m_col		(XCOL_WHITE),	// �F
	m_nNum		(0),			// ����
	m_nMin		(0),			// �ŏ��l
	m_nMax		(0)				// �ő�l
{
	// �������X�g���N���A
	m_listValue.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMultiValue::~CMultiValue()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMultiValue::Init(void)
{
	// �����o�ϐ���������
	m_pos	= VEC3_ZERO;	// ���_�ʒu
	m_rot	= VEC3_ZERO;	// ���_����
	m_size	= VEC3_ZERO;	// �傫��
	m_space	= VEC3_ZERO;	// ���
	m_col	= XCOL_WHITE;	// �F
	m_nNum	= 0;			// ����
	m_nMin	= 0;			// �ŏ��l
	m_nMax	= 0;			// �ő�l

	// �������X�g��������
	m_listValue.clear();

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMultiValue::Uninit(void)
{
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����̏I��
		SAFE_UNINIT(rList);
	}

	// �������X�g���N���A
	m_listValue.clear();

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CMultiValue::Update(void)
{
	// TODO
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.z += 0.01f;
	SetVec3Rotation(rot);

	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����̍X�V
		assert(rList != nullptr);
		rList->Update();
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�`�揈��
//============================================================
void CMultiValue::Draw(CShader *pShader)
{
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����̕`��
		assert(rList != nullptr);
		rList->Draw(pShader);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMultiValue::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMultiValue::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �ݒ肷�������ۑ�
	m_rot = rRot;

	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// ���������̐ݒ�
		assert(rList != nullptr);
		rList->SetVec3Rotation(rRot);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Rotation(void) const
{
	// �ۑ����ꂽ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CMultiValue::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �ݒ肷��傫����ۑ�
	m_size = rSize;

	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����傫���̐ݒ�
		assert(rList != nullptr);
		rList->SetVec3Sizing(rSize);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Sizing(void) const
{
	// �ۑ����ꂽ�傫����Ԃ�
	return m_size;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CMultiValue::SetColor(const D3DXCOLOR& rCol)
{
	// �ݒ肷��F��ۑ�
	m_col = rCol;

	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����F�̐ݒ�
		assert(rList != nullptr);
		rList->SetColor(rCol);
	}
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CMultiValue::GetColor(void) const
{
	// �ۑ����ꂽ�F��Ԃ�
	return m_col;
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CMultiValue::SetPriority(const int nPriority)
{
	// �����̗D�揇�ʂ�ݒ�
	CObject::SetPriority(nPriority);	// ���g
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����̗D�揇�ʂ̐ݒ�
		assert(rList != nullptr);
		rList->SetPriority(nPriority);
	}
}

//============================================================
//	��������
//============================================================
CMultiValue *CMultiValue::Create
(
	const CValue::ETexture texture,	// �e�N�X�`��
	const int nNum,					// ����
	const int nDigit,				// ����
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSize,		// �傫��
	const D3DXVECTOR3& rSpace,		// ���
	const D3DXVECTOR3& rRot,		// ����
	const D3DXCOLOR& rCol			// �F
)
{
	// �}���`�����̐���
	CMultiValue *pMultiValue = new CMultiValue;
	if (pMultiValue == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �}���`�����̏�����
		if (FAILED(pMultiValue->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �}���`�����̔j��
			SAFE_DELETE(pMultiValue);
			return nullptr;
		}

		// ������ݒ�
		pMultiValue->SetDigit(nDigit);

		// ������ݒ�
		pMultiValue->SetNum(nNum);

		// �e�N�X�`����ݒ�
		pMultiValue->SetTexture(texture);

		// �ʒu��ݒ�
		pMultiValue->SetVec3Position(rPos);

		// ������ݒ�
		pMultiValue->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pMultiValue->SetVec3Sizing(rSize);

		// �F��ݒ�
		pMultiValue->SetColor(rCol);

		// ��Ԃ�ݒ�
		pMultiValue->SetSpace(rSpace);

		// �m�ۂ����A�h���X��Ԃ�
		return pMultiValue;
	}
}

//============================================================
//	���l�̉��Z����
//============================================================
void CMultiValue::AddNum(const int nNum)
{
	// ���������������Z
	m_nNum += nNum;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	���l�̐ݒ菈��
//============================================================
void CMultiValue::SetNum(const int nNum)
{
	// �����������̒l�ɐݒ�
	m_nNum = nNum;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
HRESULT CMultiValue::SetDigit(const int nDigit)
{
	//--------------------------------------------------------
	//	�����I�u�W�F�N�g�̔j���E����
	//--------------------------------------------------------
	// �����I�u�W�F�N�g�̔j��
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����̏I��
		SAFE_UNINIT(rList);
	}

	// �������X�g���N���A
	m_listValue.clear();

	// �����I�u�W�F�N�g�̐���
	for (int nCntValue = 0; nCntValue < nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̐���
		CValue *pValue = CValue::Create(CValue::TEXTURE_NORMAL);
		if (pValue == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����X�V�E�����`���OFF�ɂ���
		pValue->SetEnableUpdate(false);
		pValue->SetEnableDraw(false);

		// ���X�g�ɐ����I�u�W�F�N�g��ǉ�
		m_listValue.push_back(pValue);
	}

	//--------------------------------------------------------
	//	��{���̍Đݒ�
	//--------------------------------------------------------
	// ������ݒ�
	SetVec3Rotation(m_rot);

	// �傫����ݒ�
	SetVec3Sizing(m_size);

	// �F��ݒ�
	SetColor(m_col);

	//--------------------------------------------------------
	//	�����l�̐ݒ�
	//--------------------------------------------------------
	int nLimit = 1;	// �ő�l�̌v�Z�p
	for (int i = 0; i < nDigit; i++)
	{ // �����̌������J��Ԃ�

		// �����𑝂₷
		nLimit *= 10;
	}

	// �ŏ��l�̐ݒ�
	SetMin(0);

	// �ő�l�̐ݒ�
	SetMax(nLimit - 1);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�ŏ��l�̐ݒ菈��
//============================================================
void CMultiValue::SetMin(const int nMin)
{
	// ��O����
	assert(nMin >= 0 && nMin <= m_nMax);

	// �����̍ŏ��l��ݒ�
	m_nMin = nMin;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);
}

//============================================================
//	�ő�l�̐ݒ菈��
//============================================================
void CMultiValue::SetMax(const int nMax)
{
#if _DEBUG	// �ő�l�����K���`�F�b�N

	int nLimit = 1;	// �ő�l�̌v�Z�p
	int nDigit = (int)m_listValue.size();	// ����
	for (int i = 0; i < nDigit; i++)
	{ // �����̌������J��Ԃ�

		// �����𑝂₷
		nLimit *= 10;
	}

	// ��O����
	assert(nMax <= nLimit - 1 && nMax >= m_nMin);

#endif	// _DEBUG

	// �����̍ő�l��ݒ�
	m_nMax = nMax;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);
}

//============================================================
//	���l�擾����
//============================================================
int CMultiValue::GetNum(void) const
{
	// �����̒l��Ԃ�
	return m_nNum;
}

//============================================================
//	�����擾����
//============================================================
int CMultiValue::GetDigit(void) const
{
	// ������Ԃ�
	return (int)m_listValue.size();
}

//============================================================
//	�ŏ��l�擾����
//============================================================
int CMultiValue::GetMin(void) const
{
	// �ŏ��l��Ԃ�
	return m_nMin;
}

//============================================================
//	�ő�l�擾����
//============================================================
int CMultiValue::GetMax(void) const
{
	// �ő�l��Ԃ�
	return m_nMax;
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CMultiValue::SetSpace(const D3DXVECTOR3& rSpace)
{
	// �����̗�Ԃ�ݒ�
	m_space = rSpace;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��Ԏ擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetSpace(void) const
{
	// ��Ԃ�Ԃ�
	return m_space;
}

//============================================================
//	�e�N�X�`���̐ݒ菈��
//============================================================
void CMultiValue::SetTexture(const CValue::ETexture texture)
{
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����e�N�X�`���̐ݒ�
		assert(rList != nullptr);
		rList->SetTexture(texture);

		// TODO
		//rList->BindTexture(-1);
	}
}

//============================================================
//	�j������
//============================================================
void CMultiValue::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CMultiValue::SetPositionRelative(void)
{
	// TODO�F���Έʒu�̐ݒ�
#if 0
	int nCntDigit = 0;	// �����C���f�b�N�X
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// ���_�̈ʒu���琔���̈ʒu��ݒ�
		rList->SetVec3Position(m_pos + (m_space * (float)nCntDigit));

		// ���������Z
		nCntDigit++;
	}
#else
	// �������Ȃ��ꍇ������
	if ((int)m_listValue.size() <= 0) { return; }

	//float fTextHeight	= GetTextHeight() * 0.5f;	// �e�L�X�g�S�̂̏c��
	//float fFrontHeight	= m_listString.front()->GetHeight() * 0.5f;	// �擪������̏c��
	//float fStartOffset	= -fTextHeight + fFrontHeight - (fTextHeight * (m_alignY - 1));	// ������̊J�n�ʒu�I�t�Z�b�g

	D3DXVECTOR3 posOffset = VEC3_ZERO;	// �����̊J�n�I�t�Z�b�g
	//posOffset.x = ;
	//posOffset.y = ;

	D3DXVECTOR3 rotStart = VEC3_ZERO;	// �����̊J�n����
	rotStart.x = m_rot.z + HALF_PI;		// �������̌���
	rotStart.y = m_rot.z;				// �c�����̌���

	D3DXVECTOR3 posStart = VEC3_ZERO;	// �����̊J�n�ʒu
	posStart.x = m_pos.x + sinf(rotStart.x) * posOffset.x + sinf(rotStart.y) * posOffset.y;	// �J�n�ʒuX
	posStart.y = m_pos.y + cosf(rotStart.x) * posOffset.x + cosf(rotStart.y) * posOffset.y;	// �J�n�ʒuY

	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �ʒu�𔽉f
		assert(rList != nullptr);
		rList->SetVec3Position(posStart);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		posStart.x += sinf(rotStart.x) * m_space.x + sinf(rotStart.y) * m_space.y;
		posStart.y += cosf(rotStart.x) * m_space.x + cosf(rotStart.y) * m_space.y;
	}
#endif
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CMultiValue::SetTexNum(void)
{
	// �������������Ƃɕ���
	int nDigit = (int)m_listValue.size();	// ���l�̌���
	int *pNumDivide = new int[nDigit];		// ���l�̕���p�z��
	useful::ZeroClear(pNumDivide);			// �z��̃������N���A
	useful::DivideDigitNum
	( // ����
		pNumDivide,	// �������ʂ̊i�[�z��
		m_nNum,		// �������鐔�l
		nDigit - 1	// �������鐔���̌���
	);

	// �����������l�𔽉f
	int nCntDigit = 0;	// �����C���f�b�N�X
	for (auto& rList : m_listValue)
	{ // �����̌������J��Ԃ�

		// �����̐ݒ�
		assert(rList != nullptr);
		rList->SetNumber(pNumDivide[nCntDigit]);

		// ���������Z
		nCntDigit++;
	}

	// ���l�̕���p�z���j��
	SAFE_DEL_ARRAY(pNumDivide);
}
