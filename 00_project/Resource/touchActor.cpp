//============================================================
//
//	�^�b�`�A�N�^�[���� [touchActor.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "touchActor.h"

#include "touchActorCan.h"
#include "touchActorBird.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL[] =	// ���f���̃p�X
	{
		"data\\MODEL\\TouchActor\\TouchCan.x",		// ��
		"data\\MODEL\\LilyPad\\LilyPad000.x",		// ��
	};
	const int PRIORITY = 4;	// �A�N�^�[�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CTouchActor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// �ÓI�����o�ϐ��錾
//************************************************************
CListManager<CTouchActor>* CTouchActor::m_pList = nullptr;		// ���X�g�\��

//************************************************************
//	�q�N���X [CTouchActor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTouchActor::CTouchActor() : CObjectModel(CObject::LABEL_TOUCHACTOR, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_CAN),		// ���
m_state(STATE_NONE)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTouchActor::~CTouchActor()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTouchActor::Init(void)
{
	// �l��������
	m_state = STATE_NONE;		// ���

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CTouchActor>::Create();

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
void CTouchActor::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTouchActor::Update(const float fDeltaTime)
{
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	switch (m_state)
	{
	case CTouchActor::STATE_NONE:

		// �ʏ��ԍX�V����
		UpdateNone(fDeltaTime);

		break;

	case CTouchActor::STATE_ACT:

		// �A�N�V������ԍX�V����
		UpdateAct(fDeltaTime);

		break;

	default:
		assert(false);
		break;
	}

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTouchActor::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw(pShader);
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CTouchActor::SetData
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �g�嗦
)
{
	// ���̐ݒ�
	SetVec3Position(rPos);		// �ʒu
	SetVec3Rotation(rRot);		// ����
	SetVec3Scaling(rScale);		// �g�嗦
	m_type = type;				// ���

	// ���f���̊��蓖�ď���
	BindModel(MODEL[type]);
}

//============================================================
//	��������
//============================================================
CTouchActor* CTouchActor::Create
( // ����
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �g�嗦
)
{
	// ���f��UI�̐���
	CTouchActor* pActor = nullptr;

	switch (type)
	{
	case CTouchActor::TYPE_CAN:

		pActor = new CTouchCan;

		break;

	case CTouchActor::TYPE_BIRD:

		pActor = new CTouchBird;

		break;
	default:
		assert(false);
		break;
	}

	// �A�N�^�[�̏�����
	if (FAILED(pActor->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �A�N�^�[�̔j��
		SAFE_DELETE(pActor);
		return nullptr;
	}

	// ���̐ݒ菈��
	pActor->SetData
	(
		type,
		rPos,
		rRot,
		rScale
	);

	// �m�ۂ����A�h���X��Ԃ�
	return pActor;
}

//============================================================
// ���X�g�\���̎擾����
//============================================================
CListManager<CTouchActor>* CTouchActor::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}
