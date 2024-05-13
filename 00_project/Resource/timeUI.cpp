//============================================================
//
//	�^�C��UI���� [timeUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timeUI.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "value.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\timePart000.png",	// �ʏ��؂�
	};

	const int PRIORITY = 6;	// �^�C�}�[�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CValue::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CTimeUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimeUI::CTimeUI() : CObject(CObject::LABEL_UI),
	m_type			(CValue::TYPE_NORMAL),	// �������
	m_pos			(VEC3_ZERO),			// �ʒu
	m_sizeValue		(VEC3_ZERO),			// �����̑傫��
	m_sizePart		(VEC3_ZERO),			// ��؂�̑傫��
	m_spaceValue	(VEC3_ZERO),			// �����̋�
	m_spacePart		(VEC3_ZERO),			// ��؂�̋�
	m_col			(XCOL_WHITE)			// �F
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// ��؂�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimeUI::~CTimeUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimeUI::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// ��؂�̏��
	m_type = CValue::TYPE_NORMAL;	// �������
	m_pos			= VEC3_ZERO;	// �ʒu
	m_sizeValue		= VEC3_ZERO;	// �����̑傫��
	m_sizePart		= VEC3_ZERO;	// ��؂�̑傫��
	m_spaceValue	= VEC3_ZERO;	// �����̋�
	m_spacePart		= VEC3_ZERO;	// ��؂�̋�
	m_col			= XCOL_WHITE;	// �F

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̐���
		m_apValue[nCntValue] = CValue::Create();
		if (m_apValue[nCntValue] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̐���
		m_apPart[nCntPart] = CObject2D::Create(VEC3_ZERO);
		if (m_apPart[nCntPart] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimeUI::Uninit(void)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̏I��
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̏I��
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CTimeUI::Update(const float fDeltaTime)
{
	// TODO
#if 1
	if (GET_INPUTKEY->IsPress(DIK_W))
	{
		m_pos.y -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_S))
	{
		m_pos.y += 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_A))
	{
		m_pos.x -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_D))
	{
		m_pos.x += 1.0f;
	}
#endif

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̍X�V
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->Update(fDeltaTime);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̍X�V
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->Update(fDeltaTime);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CTimeUI::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTimeUI::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CTimeUI::SetPriority(const int nPriority)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̗D�揇�ʂ�ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetPriority(nPriority);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̗D�揇�ʂ�ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetPriority(nPriority);
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CTimeUI::SetEnableUpdate(const bool bUpdate)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̍X�V�󋵂�ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableUpdate(bUpdate);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̍X�V�󋵂�ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTimeUI::SetEnableDraw(const bool bDraw)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̕`��󋵂�ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableDraw(bDraw);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̕`��󋵂�ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	��������
//============================================================
CTimeUI *CTimeUI::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSizeValue,	// �����̑傫��
	const D3DXVECTOR3& rSizePart,	// ��؂�̑傫��
	const D3DXVECTOR3& rSpaceValue,	// �����̋�
	const D3DXVECTOR3& rSpacePart,	// ��؂�̋�
	const CValue::EType type,		// �������
	const D3DXCOLOR& rCol			// �F
)
{
	// �^�C��UI�̐���
	CTimeUI *pTimeUI = new CTimeUI;
	if (pTimeUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C��UI�̏�����
		if (FAILED(pTimeUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C��UI�̔j��
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// ������ނ�ݒ�
		pTimeUI->SetValueType(type);

		// �����̑傫����ݒ�
		pTimeUI->SetSizingValue(rSizeValue);

		// ��؂�̑傫����ݒ�
		pTimeUI->SetSizingPart(rSizePart);

		// �����̋󔒂�ݒ�
		pTimeUI->SetSpaceValue(rSpaceValue);

		// ��؂�̋󔒂�ݒ�
		pTimeUI->SetSpacePart(rSpacePart);

		// �F��ݒ�
		pTimeUI->SetColor(rCol);

		// �ʒu��ݒ�
		pTimeUI->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimeUI;
	}
}

//============================================================
//	�����̎�ނ̐ݒ菈��
//============================================================
void CTimeUI::SetValueType(const CValue::EType type)
{
	// �ݒ肳�ꂽ�����̎�ނ�ۑ�
	m_type = type;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̎�ނ�ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetType(type);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// �e�N�X�`��������
		assert(m_apPart[nCntPart] != nullptr);
		//m_apPart[nCntPart]->BindTexture(TEXTURE_FILE[(int)type]);	// TODO
	}
}

//============================================================
//	�����̑傫���̐ݒ菈��
//============================================================
void CTimeUI::SetSizingValue(const D3DXVECTOR3& rSize)
{
	// �����̐����̑傫����ݒ�
	m_sizeValue = rSize;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��؂�̑傫���̐ݒ菈��
//============================================================
void CTimeUI::SetSizingPart(const D3DXVECTOR3& rSize)
{
	// �����̋�؂�̑傫����ݒ�
	m_sizePart = rSize;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����̋󔒂̐ݒ菈��
//============================================================
void CTimeUI::SetSpaceValue(const D3DXVECTOR3& rSpace)
{
	// �����̐����̋󔒂�ݒ�
	m_spaceValue = rSpace;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��؂�̋󔒂̐ݒ菈��
//============================================================
void CTimeUI::SetSpacePart(const D3DXVECTOR3& rSpace)
{
	// �����̋�؂�̋󔒂�ݒ�
	m_spacePart = rSpace;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CTimeUI::SetColor(const D3DXCOLOR& rCol)
{
	// �ݒ肳�ꂽ�F��ۑ�
	m_col = rCol;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̐F��ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetColor(rCol);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̐F��ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetColor(rCol);
	}
}

//============================================================
//	�����S�̂̉����擾����
//============================================================
float CTimeUI::GetTimeWidth(void) const
{
#if 0
	// �������Ȃ��ꍇ������
	if ((int)m_listValue.size() <= 0) { assert(false); return 0.0f; }

	float fValueWidth = 0.0f;	// �����S�̂̏c��
	int nEndNumID = (int)m_listValue.size() - 1;	// �I�[�����̃C���f�b�N�X
	for (int i = 0; i < nEndNumID; i++)
	{ // �I�[�����𔲂����������J��Ԃ�

		// ���̐����܂ł̗�Ԃ����Z
		fValueWidth += m_space.x;
	}

	// �擪�ƏI�[�̐����̖������ꂽ�T�C�Y�����Z
	fValueWidth += m_listValue.front()->GetVec3Sizing().x * 0.5f;	// �擪�����̌��_���T�C�Y
	fValueWidth += m_listValue.back()->GetVec3Sizing().x * 0.5f;	// �I�[�����̌��_�E�T�C�Y

	// �����S�̂̏c����Ԃ�
	return fValueWidth;
#else
	return 100.0f;
#endif
}

//============================================================
//	�����S�̂̏c���擾����
//============================================================
float CTimeUI::GetTimeHeight(void) const
{
#if 0
	// �������Ȃ��ꍇ������
	if ((int)m_listValue.size() <= 0) { assert(false); return 0.0f; }

	float fValueHeight = 0.0f;	// �����S�̂̏c��
	int nEndNumID = (int)m_listValue.size() - 1;	// �I�[�����̃C���f�b�N�X
	for (int i = 0; i < nEndNumID; i++)
	{ // �I�[�����𔲂����������J��Ԃ�

		// ���̐����܂ł̍s�Ԃ����Z
		fValueHeight += m_space.y;
	}

	// �擪�ƏI�[�̐����̖������ꂽ�T�C�Y�����Z
	fValueHeight += m_listValue.front()->GetVec3Sizing().y * 0.5f;	// �擪�����̌��_��T�C�Y
	fValueHeight += m_listValue.back()->GetVec3Sizing().y * 0.5f;	// �I�[�����̌��_���T�C�Y

	// �����S�̂̏c����Ԃ�
	return fValueHeight;
#else
	return 100.0f;
#endif
}

//============================================================
//	�����S�̂̑傫���擾����
//============================================================
D3DXVECTOR3 CTimeUI::GetTimeSize(void) const
{
	// �����S�̂̑傫����Ԃ�
	return D3DXVECTOR3(GetTimeWidth(), GetTimeHeight(), 0.0f);
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CTimeUI::SetPositionRelative(void)
{
	D3DXVECTOR3 spaceValue = m_spaceValue * 0.5f;	// �����̋�
	D3DXVECTOR3 spacePart = m_spacePart * 0.5f;		// ��؂�̋�
	D3DXVECTOR3 posPoly = m_pos - spaceValue;		// �|���S�������ʒu
	int nNumValue = 0;	// �����̐�����
	int nNumPart = 0;	// ��؂�̐�����

	for (int nCntTimer = 0; nCntTimer < timeUI::MAX_DIGIT + timeUI::MAX_PART; nCntTimer++)
	{ // �����̐� + ��؂�̐����J��Ԃ�

		if (nCntTimer == timeUI::MAX_MIN || nCntTimer == timeUI::MAX_MIN + timeUI::MAX_SEC + 1)
		{ // ��؂�^�C�~���O�̏ꍇ

			assert(m_apPart[nNumValue] != nullptr);

			// �|���S�������ʒu�����炷
			posPoly += spacePart;

			// ��؂�̈ʒu��ݒ�
			m_apPart[nNumValue]->SetVec3Position(posPoly);

			// ��؂�̑傫����ݒ�
			m_apPart[nNumValue]->SetVec3Sizing(m_sizePart);

			// ��؂萶���������Z
			nNumValue++;

			// �|���S�������ʒu�����炷
			posPoly += spacePart;
		}
		else
		{ // �����^�C�~���O�̏ꍇ

			assert(m_apValue[nNumPart] != nullptr);

			// �|���S�������ʒu�����炷
			posPoly += spaceValue;

			// �����̈ʒu��ݒ�
			m_apValue[nNumPart]->SetVec3Position(posPoly);

			// �����̑傫����ݒ�
			m_apValue[nNumPart]->SetVec3Sizing(m_sizeValue);

			// ���������������Z
			nNumPart++;

			// �|���S�������ʒu�����炷
			posPoly += spaceValue;
		}
	}
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CTimeUI::SetTexNum(void)
{
	int aNumDivide[timeUI::MAX_DIGIT];	// ���l����p

#if 0
	// �����������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), timeUI::MAX_MIN);

	// �b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN], GetSec(), timeUI::MAX_SEC);

	// �~���b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN + timeUI::MAX_SEC], GetMSec(), timeUI::MAX_MSEC);
#else
	// �����������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[0], 0, timeUI::MAX_MIN);

	// �b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN], 0, timeUI::MAX_SEC);

	// �~���b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN + timeUI::MAX_SEC], 0, timeUI::MAX_MSEC);
#endif

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̐ݒ�
		m_apValue[nCntValue]->SetNumber(aNumDivide[nCntValue]);
	}
}
