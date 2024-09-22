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

#include "touchActorStone.h"
#include "touchActorBird.h"
#include "touchActorBranch.h"

#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL[] =	// ���f���̃p�X
	{
		"data\\MODEL\\TouchActor\\TouchStone.x",	// ��
		"data\\MODEL\\TouchActor\\TouchBird.x",		// ��
		"data\\MODEL\\TouchActor\\TouchBranch.x",	// �}
	};
	const int PRIORITY = 4;			// �A�N�^�[�̗D�揇��
	const float SUB_ALPHA = 1.0f;	// �����x�̌��Z��
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
m_posInit(VEC3_ZERO),	// �����ʒu
m_posOld(VEC3_ZERO),	// �O��̈ʒu
m_move(VEC3_ZERO),		// �ړ���
m_type(TYPE_STONE),		// ���
m_state(STATE_NONE),	// ���
m_nStateCount(0)		// ��ԃJ�E���g
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

	// �O��̈ʒu��ۑ�����
	m_posOld = GetVec3Position();

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	switch (m_state)
	{
	case CTouchActor::STATE_NONE:

		// �ʏ��ԍX�V����
		UpdateNone(fDeltaTime);

		break;

	case CTouchActor::STATE_ACT:

		// �A�N�V������ԍX�V����
		UpdateAct(fDeltaTime);

		if (Fade(fDeltaTime))
		{ // �����ɂȂ�����

			// �I������
			Uninit();

			// ���̊֐��𔲂���
			return;
		}

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
	m_posInit = rPos;			// �����ʒu

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
	case CTouchActor::TYPE_STONE:

		pActor = new CTouchStone;

		break;

	case CTouchActor::TYPE_BIRD:

		pActor = new CTouchBird;

		break;

	case CTouchActor::TYPE_BRANCH:

		pActor = new CTouchBranch;

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

//============================================================
// ���Ƃ̓����蔻��
//============================================================
bool CTouchActor::CollisionFieid(D3DXVECTOR3& rPos)
{
	// �t�B�[���h�Ƃ̓����蔻��
	return GET_STAGE->LandFieldPosition(rPos, m_posOld, m_move);
}

//============================================================
// �t�F�[�h����
//============================================================
bool CTouchActor::Fade(const float fDeltaTime)
{
	float fAlpha = GetAlpha();	// �����x
	bool bDelete = false;		// ������

	// �����x��������
	fAlpha -= SUB_ALPHA * fDeltaTime;

	if (fAlpha <= 0.0f)
	{ // �����ɂȂ�����

		// �����󋵂� true �ɂ���
		bDelete = true;
	}

	// �����x�𔽉f
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bDelete;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CTouchActor::LoadSetup(const char* pPass)
{
	int nType = 0;					// ��ނ̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�^�b�`�A�N�^�[�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (file >> str)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (str.front() == '#')
		{ // �R�����g�A�E�g����Ă���ꍇ

			// ��s�S�ēǂݍ���
			std::getline(file, str);
		}
		else if (str == "STAGE_TOUCH_ACTORSET")
		{
			do
			{ // END_STAGE_ACTORSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "TOUCH_ACTORSET")
				{
					do
					{ // END_TOUCH_ACTORSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str == "TYPE")
						{
							file >> str;	// ����Ǎ�

							// ��ނ�Ǎ�
							file >> nType;
						}
						else if (str == "POS")
						{
							file >> str;	// ����Ǎ�

							// �ʒu��Ǎ�
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
					} while (str != "END_TOUCH_ACTORSET");	// END_TOUCH_ACTORSET��ǂݍ��ނ܂Ń��[�v

					// �A�N�^�[�I�u�W�F�N�g�̐���
					if (CTouchActor::Create((EType)nType, pos) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_TOUCH_ACTORSET");	// END_STAGE_TOUCH_ACTORSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}