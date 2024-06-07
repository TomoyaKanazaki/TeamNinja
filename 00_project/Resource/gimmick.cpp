//============================================================
//
//	�M�~�b�N���� [gimmick.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick.h"

#include "gimmick_jumptable.h"
#include "gimmick_step.h"
#include "gimmick_fall.h"
#include "gimmick_post.h"
#include "gimmick_malti.h"
#include "gimmick_decayed.h"
#include "gimmick_jumpoff.h"
#include "gimmick_heavydoor.h"

#include "gimmick_gravel.h"
#include "gimmick_boob.h"
#include "gimmick_water.h"
#include "gimmick_cleen.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 2;				// �M�~�b�N�͈̓|���S���̗D�揇��
	const int INIT_NUM_ACTIVE = -1;		// �����\�l���̏����l
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CGimmick>* CGimmick::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CGimmick] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmick::CGimmick() : CObject3D(CObject::LABEL_GIMMICK, CObject::DIM_3D, PRIORITY),
m_type(TYPE_JUMPTABLE),			// ���
m_nNumActive(INIT_NUM_ACTIVE)	// �����\�ȕ��g�̐�
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmick::~CGimmick()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmick::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CGimmick>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGimmick::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmick::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmick::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CGimmick* CGimmick::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSize,		// �T�C�Y
	const EType type,				// ���
	const int nNumActive			// �����\�ȕ��g�̐�
)
{
	// ���g��UI�̐���
	CGimmick* pGimmick = nullptr;

	switch (type)
	{
	case CGimmick::TYPE_JUMPTABLE:		// �W�����v��

		pGimmick = new CGimmickJumpTable;

		break;

	case CGimmick::TYPE_STEP:			// ���ݑ�

		pGimmick = new CGimmickStep;

		break;

	case CGimmick::TYPE_FALL:			// ���Ƃ���

		pGimmick = new CGimmickFall;

		break;

	case CGimmick::TYPE_POST:			// �ݒu

		pGimmick = new CGimmickPost;

		break;

	case CGimmick::TYPE_MALTI:			// �����Ǘ�

		pGimmick = new CGimmickMalti;

		break;

	case CGimmick::TYPE_DECAED:			// ��������

		pGimmick = new CGimmickDecaed;

		break;

	case CGimmick::TYPE_GRAVEL:			// ������

		pGimmick = new CGimmickGravel;

		break;

	case CGimmick::TYPE_BOOB:			// �h�u

		pGimmick = new CGimmickBoob;

		break;

	case CGimmick::TYPE_WATER:			// ����

		pGimmick = new CGimmickWater;

		break;

	case CGimmick::TYPE_JUMPOFF:			// ��э~��

		pGimmick = new CGimmickJumpOff;

		break;

	case CGimmick::TYPE_HEAVYDOOR:			// �d����

		pGimmick = new CGimmickHeavyDoor;

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (pGimmick == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���g��UI�̏�����
		if (FAILED(pGimmick->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���g��UI�̔j��
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// �ʒu��ݒ�
		pGimmick->SetVec3Position(rPos);

		// �T�C�Y��ݒ�
		pGimmick->SetVec3Sizing(rSize);

		// ��ނ�ݒ�
		pGimmick->m_type = type;

		// �����\�ȕ��g�̐���ݒ�
		pGimmick->m_nNumActive = nNumActive;

#ifdef _DEBUG

		// �F�̐ݒ�
		pGimmick->SetColor(XCOL_YELLOW);

#else

		// �F�̐ݒ�
		pGimmick->SetColor(XCOL_AWHITE);

#endif // _DEBUG


		// �m�ۂ����A�h���X��Ԃ�
		return pGimmick;
	}
}

//============================================================
// ���X�g�擾
//============================================================
CListManager<CGimmick>* CGimmick::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}
