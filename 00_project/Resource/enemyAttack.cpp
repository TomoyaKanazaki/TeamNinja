//============================================================
//
//	�U���n�G���� [enemyAttack.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyAttack.h"
#include "renderer.h"
#include "deltaTime.h"
#include "sound.h"

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"
#include "collision.h"
#include "enemyChaseRange.h"
#include "actor.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
#include "enemyAmbush.h"
#include "effekseerControl.h"
#include "enemyNavigation.h"
#include "camera.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 ATTACK_COLLUP = D3DXVECTOR3(30.0f, 100.0f, 30.0f);	// �U������(��)
	const D3DXVECTOR3 ATTACK_COLLDOWN = D3DXVECTOR3(30.0f, 0.0f, 30.0f);	// �U������(��)
	const int DODGE_COUNT = 20;					// ����J�E���g��
	const float SHAKEOFF_RANGE = 1000.0f;		// �U��؂�鋗��
	const float DIVERSION_EFFECT_SCALE = 18.0f;	// ���g�Ƃ̐퓬�G�t�F�N�g�̑傫��
	const int REGRESSION_COUNT = 120;			// ��A����J�E���g��

	const int WARNING_COUNT[CEnemyAttack::TYPE_MAX] =	// �x����Ԃ̑J�ڃJ�E���g
	{
		59,			// �������G
		36,			// �T�G
		59,			// �҂������G
	};
	const int ATTACK_COUNT[CEnemyAttack::TYPE_MAX] =	// �U����Ԃ̑J�ڃJ�E���g
	{
		44,			// �������G
		34,			// �T�G
		44,			// �҂������G
	};
	const int BLANKATTACK_COUNT[CEnemyAttack::TYPE_MAX] =		// �󔒍U����Ԃ̑J�ڃJ�E���g
	{
		340,		// �������G
		340,		// �T�G
		340,		// �҂������G
	};
	const int BLANKATTACK_CYCLE_COUNT[CEnemyAttack::TYPE_MAX] =		// �󔒍U����Ԃ̉�]�J�E���g
	{
		18,		// �������G
		18,		// �T�G
		18,		// �҂������G
	};
}

// ���֌W�萔
namespace sound
{
	const int WARNING_COUNT[CEnemyAttack::TYPE_MAX] =		// �x�����̖�J�E���g
	{
		37,			// �������G
		2,			// �T�G
		37,			// �҂������G
	};
	const CSound::ELabel WARNING_LABEL[CEnemyAttack::TYPE_MAX] =	// �x�����̃��x��
	{
		CSound::LABEL_SE_STALKFOUND_000,	// �������G
		CSound::LABEL_SE_WOLFFOUND_000,		// �T�G
		CSound::LABEL_SE_STALKFOUND_000,	// �҂������G
	};
	const CSound::ELabel ATTACK_LABEL[CEnemyAttack::TYPE_MAX] =		// �U�����̃��x��
	{
		CSound::LABEL_SE_STALKATTACK_000,	// �������G
		CSound::LABEL_SE_WOLFATTACK_000,	// �T�G
		CSound::LABEL_SE_STALKATTACK_000,	// �҂������G
	};
	const CSound::ELabel BLANK_ATTACK_LABEL[CEnemyAttack::TYPE_MAX] =	// �󔒍U�����̃��x��
	{
		CSound::LABEL_SE_STALKATTACK_001,	// �������G
		CSound::LABEL_SE_WOLFATTACK_001,	// �T�G
		CSound::LABEL_SE_STALKATTACK_001,	// �҂������G
	};
	const int UPSET_COUNT[CEnemyAttack::TYPE_MAX] =			// ���h���̖�J�E���g
	{
		200,		// �������G
		80,			// �T�G
		200,		// �҂������G
	};
	const CSound::ELabel UPSET_LABEL[CEnemyAttack::TYPE_MAX] =		// ���h���̃��x��
	{
		CSound::LABEL_SE_STALKUPSET_000,	// �������G
		CSound::LABEL_SE_WOLFUPSET_000,		// �T�G
		CSound::LABEL_SE_STALKUPSET_000,	// �҂������G
	};
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CEnemyAttack>* CEnemyAttack::m_pList = nullptr;			// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CEnemyAttack] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack::CEnemyAttack() : CEnemy(),
m_pChaseRange(nullptr),		// �ǐՔ͈͂̏��
m_pClone(nullptr),			// ���g�̏��
m_posTarget(VEC3_ZERO),		// �ڕW�̈ʒu
m_target(TARGET_NONE),		// �W�I
m_state(STATE_ORIGIN),		// ���
m_nStateCount(0),			// ��ԃJ�E���g
m_nAttackCount(0),			// �U���J�E���g
m_nRegressionCount(0),		// ��A�J�E���g
m_type(TYPE_STALK),			// ���
m_bDodge(false)				// �����t�t���O
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack::Init(void)
{
	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CEnemyAttack>::Create();
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
void CEnemyAttack::Uninit(void)
{
	// �ǐՔ͈͂̏I������
	SAFE_UNINIT(m_pChaseRange);

	// ��������
	m_actor.clear();

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyAttack::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyAttack::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CEnemyAttack::SetData(void)
{
	// �A�N�^�[�̓o�^����
	ActorRegist();
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CEnemyAttack>* CEnemyAttack::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CEnemyAttack::LoadSetup(const char* pPass)
{
	int nType = 0;					// ��ނ̑���p
	int nNavType = 0;				// �i�r�̎�ނ̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	float fMoveWidth = 0.0f;		// �ړ���
	float fMoveDepth = 0.0f;		// �ړ����s
	std::vector<D3DXVECTOR3> route;	// ���H
	float fChaseWidth = 0.0f;		// �ǐՕ�
	float fChaseDepth = 0.0f;		// �ǐՉ��s

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��

	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�G�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_ENEMYSET")
		{
			do
			{ // END_STAGE_ENEMYSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "ENEMYSET")
				{
					do
					{ // END_ENEMYSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str == "TYPE")
						{
							file >> str;	// ����Ǎ�

							// ��ނ�Ǎ�
							file >> nType;
						}
						else if (str == "NAVTYPE")
						{
							file >> str;	// ����Ǎ�

							// �i�r�̎�ނ�Ǎ�
							file >> nNavType;
						}
						else if (str == "POS")
						{
							file >> str;	// ����Ǎ�

							// �ʒu��Ǎ�
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
						else if (str == "ROT")
						{
							file >> str;	// ����Ǎ�

							// ������Ǎ�
							file >> rot.x;
							file >> rot.y;
							file >> rot.z;
						}
						else if (str == "MOVESIZE")
						{
							// �����_������Ȃ������ꍇ�A��~
							assert(nNavType == CEnemyNav::TYPE_RANDOM);

							file >> str;	// ����Ǎ�

							// �ړ��͈͂�Ǎ�
							file >> fMoveWidth;
							file >> fMoveDepth;
						}
						else if (str == "ROUTE")
						{
							// ���[�g���񂶂�Ȃ������ꍇ�A��~
							assert(nNavType == CEnemyNav::TYPE_STREET);

							int nNumRoute = 0;
							D3DXVECTOR3 posRoute = VEC3_ZERO;

							// ���H�̑�����Ǎ�
							file >> nNumRoute;

							for (int nCnt = 0; nCnt < nNumRoute; nCnt++)
							{
								// ���H��Ǎ�
								file >> posRoute.x;
								file >> posRoute.y;
								file >> posRoute.z;

								// ���H��ǉ�����
								route.push_back(posRoute);
							}
						}
						else if (str == "CHASESIZE")
						{
							file >> str;	// ����Ǎ�

							// �ǐՔ͈͂�Ǎ�
							file >> fChaseWidth;
							file >> fChaseDepth;
						}
					} while (str != "END_ENEMYSET");	// END_ENEMYSET��ǂݍ��ނ܂Ń��[�v

					switch (nType)
					{
					case CEnemyAttack::TYPE_STALK:

						switch (nNavType)
						{
						case CEnemyNav::TYPE_RANDOM:

							// �ǐՓG�̐���
							CEnemyStalk::Create
							(
								pos,			// �ʒu
								rot,			// ����
								fMoveWidth,		// �ړ���
								fMoveDepth,		// �ړ����s
								fChaseWidth,	// �ǐՕ�
								fChaseDepth		// �ǐՉ��s
							);

							break;

						case CEnemyNav::TYPE_STREET:

							// �ǐՓG�̐���
							CEnemyStalk::Create
							(
								pos,			// �ʒu
								rot,			// ����
								route,			// ���[�g
								fChaseWidth,	// �ǐՕ�
								fChaseDepth		// �ǐՉ��s
							);

							break;

						default:

							// ��~
							assert(false);

							break;
						}

						break;

					case CEnemyAttack::TYPE_WOLF:

						switch (nNavType)
						{
						case CEnemyNav::TYPE_RANDOM:

							// �T�G�̐���
							CEnemyWolf::Create
							(
								pos,			// �ʒu
								rot,			// ����
								fMoveWidth,		// �ړ���
								fMoveDepth,		// �ړ����s
								fChaseWidth,	// �ǐՕ�
								fChaseDepth		// �ǐՉ��s
							);

							break;

						case CEnemyNav::TYPE_STREET:

							// �T�G�̐���
							CEnemyWolf::Create
							(
								pos,			// �ʒu
								rot,			// ����
								route,			// ���[�g
								fChaseWidth,	// �ǐՕ�
								fChaseDepth		// �ǐՉ��s
							);

							break;

						default:

							// ��~
							assert(false);

							break;
						}

						break;

					case CEnemyAttack::TYPE_AMBUSH:

						// �҂������G�̐���
						CEnemyAmbush::Create
						(
							pos,			// �ʒu
							rot,			// ����
							fChaseWidth,	// �ǐՕ�
							fChaseDepth		// �ǐՉ��s
						);

						break;

					default:

						// ��~
						assert(false);

						break;
					}
				}
			} while (str != "END_STAGE_ENEMYSET");	// END_STAGE_ENEMYSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
// �ړ�����
//============================================================
void CEnemyAttack::Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(rRot.y) * fSpeed * fDeltaTime;
	move.z = cosf(rRot.y) * fSpeed * fDeltaTime;

	// �ʒu���ړ�����
	*pPos += move;

	// ����K�p
	SetMovePosition(move);
}

//============================================================
// �����̈ړ�����
//============================================================
void CEnemyAttack::RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = destRot.y - rRot.y;
	useful::NormalizeRot(fDiffRot);	// ���������̐��K��

	// �����̍X�V
	rRot.y += fDiffRot * fDeltaTime * fRevRota;
	useful::NormalizeRot(rRot.y);	// �����̐��K��
}

//============================================================
//	�ڕW�ʒu�̎��F����
//============================================================
void CEnemyAttack::LookTarget(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����

	// �ڕW���������߂�
	destRot.y = atan2f(rPos.x - GetTargetPos().x, rPos.z - GetTargetPos().z);

	SetDestRotation(destRot);	// �ڕW�����𔽉f
}

//============================================================
// �ڋߏ���
//============================================================
bool CEnemyAttack::Approach(const D3DXVECTOR3& rPos, const float fDis)
{
	float fDistance = 0.0f;					// ����

	// �����𑪂�
	fDistance = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));

	if (fDistance <= fDis)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}

//============================================================
// �v���C���[�̒T������
//============================================================
bool CEnemyAttack::JudgePlayer(void)
{
	// �v���C���[��������Ȃ������ꍇ false ��Ԃ�
	if (!SearchPlayer(&m_posTarget)) { return false; }

	// �W�I���v���C���[�ɂ���
	m_target = TARGET_PLAYER;

	// true ��Ԃ�
	return true;
}

//============================================================
// ���g�̒T������
//============================================================
bool CEnemyAttack::JudgeClone(void)
{
	// ���g��������Ȃ������ꍇ false ��Ԃ�
	if (!SearchClone(&m_posTarget, &m_pClone)) { return false; }

	// �W�I�𕪐g�ɂ���
	m_target = TARGET_CLONE;

	// true ��Ԃ�
	return true;
}

//============================================================
// �v���C���[�i������
//============================================================
bool CEnemyAttack::PlayerIngress(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;				// �ʒu

	// �v���C���[���̎擾
	CPlayer* pPlayer = GET_PLAYER;

	// �v���C���[�����Ȃ��ꍇ�A������
	if (pPlayer == nullptr) { return false; }

	// �ʒu���擾����
	pos = pPlayer->GetVec3Position();

	// �͈͓��ɂ��Ȃ������ꍇ�Afalse ��Ԃ�
	if (!m_pChaseRange->InsideTargetPos(GetPosInit(), pos)) { return false; }

	// �ړI���v���C���[�ɂ���
	m_target = TARGET_PLAYER;

	// �ړI�̈ʒu��ݒ肷��
	m_posTarget = pos;

	// true ��Ԃ�
	return true;
}

//============================================================
// ���g�i������
//============================================================
bool CEnemyAttack::CloneIngress(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// �ʒu

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // ���g�̃��X�g�������ꍇ

		// false��Ԃ�
		return false;
	}

	for (auto& clone : CPlayerClone::GetList()->GetList())
	{
		// ���s��Ԃ���Ȃ��ꍇ�A���ɐi��
		if (clone->GetAction() != CPlayerClone::ACTION_MOVE) { continue; }

		// ���g�̈ʒu���擾����
		pos = clone->GetVec3Position();

		// �͈͓��i���󋵂�Ԃ�
		if (!m_pChaseRange->InsideTargetPos(GetPosInit(), pos)) { continue; }

		// ���g�̏���ݒ肷��
		m_pClone = clone;

		// �ړI�𕪐g�ɂ���
		m_target = TARGET_CLONE;

		// �ړI�̈ʒu��ݒ肷��
		m_posTarget = pos;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//====================================================================================================================================================================================
// TODO�F�������牺�͂��Z���J�X���肾�����ŏC��
//====================================================================================================================================================================================

//============================================================
// �v���C���[�̃q�b�g����
//============================================================
bool CEnemyAttack::HitPlayer(const D3DXVECTOR3& rPos)
{
	// �����t�t���O�� false �̏ꍇ�A������
	if (m_bDodge == false) { return false; }

	// �q�b�g����
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();
	float fRadius = CScene::GetPlayer()->GetRadius();
	D3DXVECTOR3 sizeUpPlayer =				// �v���C���[�̔���(�E�E��E��)
	{
		fRadius,
		CScene::GetPlayer()->GetHeight(),
		fRadius
	};
	D3DXVECTOR3 sizeDownPlayer =			// �v���C���[�̔���(���E���E�O)
	{
		fRadius,
		0.0f,
		fRadius
	};

	// ����J�E���g�����Z����
	m_nAttackCount++;

	if (m_nAttackCount > DODGE_COUNT)
	{ // ����J�E���g���߂����ꍇ

		bool bHit = false;	// �q�b�g��

		// �{�b�N�X�̓����蔻��
		if (collision::Box3D
		(
			rPos,				// ����ʒu
			posPlayer,			// ����ڕW�ʒu
			ATTACK_COLLUP,		// ����T�C�Y(�E�E��E��)
			ATTACK_COLLDOWN,	// ����T�C�Y(���E���E�O)
			sizeUpPlayer,		// ����ڕW�T�C�Y(�E�E��E��)
			sizeDownPlayer		// ����ڕW�T�C�Y(���E���E�O)
		))
		{ // �������ĂȂ������ꍇ

			// ����J�E���g������������
			m_nAttackCount = 0;

			// ���g�ƃv���C���[�����ԃx�N�g�����Z�o
			D3DXVECTOR3 vec = posPlayer - rPos;

			// �q�b�g����
			CScene::GetPlayer()->HitKnockBack(500, vec);

			// �q�b�g�󋵂� true �ɂ���
			bHit = true;
		}

		// �����t�t���O�� false �ɂ���
		m_bDodge = false;

		// �q�b�g�� ��Ԃ�
		return bHit;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// ���g�̃q�b�g����
//============================================================
bool CEnemyAttack::HitClone(const D3DXVECTOR3& rPos)
{
	// ���g�̏�񂪑��݂��Ȃ��ꍇ������
	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr ||
		m_pClone == nullptr)
	{
		return false;
	}

	CPlayerClone* pClone = nullptr;	// ���g�̏��

	for (auto& rClone : CPlayerClone::GetList()->GetList())
	{
		// ���g�̏�񂶂ᖳ�������ꍇ����
		if (m_pClone != rClone) { continue; }

		// ���g�̏���ݒ�
		pClone = rClone;

		// for���𔲂���
		break;
	}

	// ���g�� NULL �̏ꍇ������
	if (pClone == nullptr || pClone->GetAction() != CPlayerClone::ACTION_MOVE) { return false; }

	// �q�b�g����
	D3DXVECTOR3 sizeUpClone =
	{
		pClone->GetRadius(),
		pClone->GetHeight(),
		pClone->GetRadius()
	};
	D3DXVECTOR3 sizeDownClone =
	{
		pClone->GetRadius(),
		0.0f,
		pClone->GetRadius()
	};

	// �{�b�N�X�̓����蔻��
	if (!collision::Box3D
	(
		rPos,						// ����ʒu
		pClone->GetVec3Position(),	// ����ڕW�ʒu
		ATTACK_COLLUP,				// ����T�C�Y(�E�E��E��)
		ATTACK_COLLDOWN,			// ����T�C�Y(���E���E�O)
		sizeUpClone,				// ����ڕW�T�C�Y(�E�E��E��)
		sizeDownClone				// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // �������ĂȂ������甲����

		return false;
	}

	// �q�b�g����
	pClone->Hit(20);

	// ���g�Ƃ̐퓬�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\diversion.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, DIVERSION_EFFECT_SCALE);

	// �����t�t���O�� false �ɂ���
	m_bDodge = false;

	// true ��Ԃ�
	return true;
}

//===========================================
//  �����蔻��̎擾
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackUp()
{
	return ATTACK_COLLUP;
}

//===========================================
//  �����蔻��̎擾
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackDown()
{
	return ATTACK_COLLDOWN;
}

//===========================================
// ���̈ʒu�ɖ߂鏈��
//===========================================
bool CEnemyAttack::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (pCamera->OnScreen(*pPos) ||
		pCamera->OnScreen(D3DXVECTOR3(pPos->x, pPos->y + fHeight, pPos->z)) ||
		pCamera->OnScreen(GetPosInit()))
	{ // ��ʓ��ɂ���ꍇ

		// ��A�J�E���g�����Z�b�g����
		m_nRegressionCount = 0;

		// false ��Ԃ�
		return false;
	}

	// ��A�J�E���g�����Z����
	m_nRegressionCount++;

	// ��A�J�E���g����萔�ȉ��̏ꍇ�A������
	if (m_nRegressionCount < REGRESSION_COUNT) { return false; }

	// �ʒu��ݒ肷��
	*pPos = GetPosInit();

	// ������ݒ肷��
	*pRot = GetRotInit();

	// �ߋ��̈ʒu��K�p����(�������Ȃ��Ɠ����蔻��Ɉ����������Ă��܂�)
	SetOldPosition(*pPos);

	// �ړI�̌�����ݒ肷��(������ɖ��Ӗ��Ɍ����Ă��܂�����)
	SetDestRotation(*pRot);

	// �����x��1.0f�ɂ���
	SetAlpha(1.0f);

	// �ړ��ʂ����Z�b�g����
	SetMovePosition(VEC3_ZERO);

	// �^�[�Q�b�g�𖳑Ώۂɂ���
	SetTarget(TARGET_NONE);

	// true ��Ԃ�
	return true;
}

//===========================================
// �A�N�^�[�̓����蔻�菈��
//===========================================
void CEnemyAttack::CollisionActor(D3DXVECTOR3& rPos, bool& bHit)
{
	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (m_actor.empty()) { return; }

	D3DXVECTOR3 move = GetMovePosition();
	bool bJump = IsJump();

	for (auto& actor : m_actor)
	{
		// �����蔻�菈��
		actor->Collision
		(
			rPos,				// �ʒu
			GetOldPosition(),	// �O��̈ʒu
			GetRadius(),		// ���a
			GetHeight(),		// ����
			move,				// �ړ���
			bJump,				// �W�����v��
			bHit				// �q�b�g��
		);
	}

	// �ړ��ʂƃW�����v�󋵂𔽉f
	SetMovePosition(move);
	SetEnableJump(bJump);
}

//===========================================
// �x������
//===========================================
int CEnemyAttack::Warning
(
	D3DXVECTOR3* pPos,		// �ʒu
	D3DXVECTOR3* pRot,		// ����
	const float fDeltaTime,	// �f���^�^�C��
	const float fRotRev		// �����̕␳��
)
{
	switch (m_target)
	{
	case CEnemyAttack::TARGET_PLAYER:

		// �v���C���[�̒T������
		JudgePlayer();

		break;

	case CEnemyAttack::TARGET_CLONE:

		// �N���[���̒T������
		JudgeClone();

		break;

	default:
		// ��~
		assert(false);
		break;
	}

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount == sound::WARNING_COUNT[m_type])
	{ // ��莞�Ԍo�߂����ꍇ

		// ��������炷
		PLAY_SOUND(sound::WARNING_LABEL[m_type]);
	}

	// �ڕW�ʒu�̎��F����
	LookTarget(*pPos);

	// �����̈ړ�����
	RotMove(*pRot, fRotRev, fDeltaTime);

	if (m_nStateCount % WARNING_COUNT[m_type] == 0)
	{ // ��莞�Ԍo�߂����ꍇ

		// �ǐՏ�Ԃɂ���
		SetState(STATE_STALK);
	}

	// ��Ԃ�Ԃ�(�Ӗ��Ȃ�)
	return m_state;
}

//===========================================
// �ǐՏ���
//===========================================
int CEnemyAttack::Stalk
(
	D3DXVECTOR3* pPos,		// �ʒu
	D3DXVECTOR3* pRot, 		// ����
	const float fDeltaTime,	// �f���^�^�C��
	const float fRotRev		// �����̕␳��
)
{
	// ���s�J�E���g�����Z����
	m_nStateCount++;

	if (!CloneIngress() &&
		!PlayerIngress())
	{ // ���g���v���C���[���͈͓��ɂ��Ȃ��ꍇ

		// �Ǝ���Ԃɂ���
		SetState(STATE_ORIGIN);

		// �W�I�𖢐ݒ�ɂ���
		SetTarget(TARGET_NONE);

		// �Ǝ���Ԃ�Ԃ�
		return m_state;
	}

	// �ڕW�ʒu�̎��F����
	LookTarget(*pPos);

	// �����t�t���O�� false �ɂ���
	SetEnableDodge(false);

	// �U���J�E���g�����Z�b�g����
	SetAttackCount(0);

	// �����̈ړ�����
	RotMove(*pRot, fRotRev, fDeltaTime);

	// �ړ�����
	Move(pPos, *pRot, GetSpeed(), fDeltaTime);

	if (Approach(*pPos))
	{ // �ڋ߂����ꍇ

		if (GetTarget() == CEnemyAttack::TARGET_PLAYER)
		{ // �ڕW���v���C���[�̏ꍇ

			// �����t�t���O�� true �ɂ���
			SetEnableDodge(true);

			// �U���J�E���g�����Z�b�g����
			SetAttackCount(0);
		}

		// �U����Ԃɂ���
		SetState(STATE_ATTACK);

		// �U����Ԃ�Ԃ�
		return m_state;
	}

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(GetPosInit(), pPos))
	{ // �ǐՔ͈͂���o���ꍇ

		// �\����Ԃɂ���
		SetState(STATE_STANCE);

		// �\����Ԃ�Ԃ�
		return m_state;
	}

	// �ǐՏ�Ԃ�Ԃ�
	return m_state;
}

//===========================================
// �U������
//===========================================
int CEnemyAttack::Attack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{
	case CEnemyAttack::TARGET_PLAYER:

		if (HitPlayer(rPos))
		{ // �v���C���[�ɓ��������ꍇ

			// �U������炷
			PLAY_SOUND(sound::ATTACK_LABEL[m_type]);
		}

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % ATTACK_COUNT[m_type] == 0)
		{ // ���J�E���g�o�߂����ꍇ

			// �Ǝ���Ԃɂ���
			SetState(STATE_ORIGIN);
		}

		// �Ǝ���Ԃ�Ԃ�
		return m_state;

	case CEnemyAttack::TARGET_CLONE:

		if (HitClone(rPos))
		{ // ���g�ɓ��������ꍇ

			// �󔒍U����Ԃɂ���
			SetState(STATE_BLANKATTACK);

			// ���g�U������炷
			PLAY_SOUND(sound::BLANK_ATTACK_LABEL[m_type]);

			// �󔒍U����Ԃ�Ԃ�
			return m_state;
		}
		else
		{ // ��L�ȊO

			// �Ǝ���Ԃɂ���
			SetState(STATE_ORIGIN);
		}

		// �Ǝ���Ԃ�Ԃ�
		return m_state;

	default:		// ��O����
		assert(false);
		break;
	}

	// �Ǝ���Ԃ�Ԃ�
	return m_state;
}

//===========================================
// �󔒍U������
//===========================================
int CEnemyAttack::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	// �����̈ړ�����
	RotMove(*pRot, fRotRev, fDeltaTime);

	if (m_nStateCount <= BLANKATTACK_COUNT[m_type])
	{ // ���J�E���g�ȉ��̏ꍇ

		if (m_nStateCount % BLANKATTACK_CYCLE_COUNT[m_type] == 0)
		{ // ���J�E���g���Ƃ�

			// �ړI�̌������擾
			D3DXVECTOR3 rotDest = GetDestRotation();

			// �ړI�̌�����ݒ肷��
			rotDest.y = useful::RandomRot();

			// �ړI�̌�����K�p
			SetDestRotation(rotDest);
		}
	}
	else
	{ // ��L�ȊO

		// ���h��Ԃɂ���
		SetState(STATE_UPSET);

		// ���h��Ԃ�Ԃ�
		return m_state;
	}

	// �󔒍U����Ԃɂ���
	return m_state;
}

//===========================================
// ���h����
//===========================================
int CEnemyAttack::Upset(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount == sound::UPSET_COUNT[m_type])
	{ // ��ԃJ�E���g����萔�ɂȂ����Ƃ�

		// ���h����炷
		PLAY_SOUND(sound::UPSET_LABEL[m_type]);
	}

	// ���h��Ԃ�Ԃ�
	return m_state;
}

//===========================================
// �\������
//===========================================
int CEnemyAttack::Stance(void)
{
	// ���g�̔�������
	JudgeClone();

	if (PlayerIngress())
	{ // �͈͓��ɓ������ꍇ

		// �x����Ԃɂ���
		SetState(STATE_WARNING);

		// �x�����[�V������Ԃ�
		return m_state;
	}

	// �\����Ԃ�Ԃ�
	return m_state;
}

//===========================================
// ��Ԃ̐ݒ菈��
//===========================================
void CEnemyAttack::SetState(const EState state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;

	// ��ԃJ�E���g��0�ɂ���
	m_nStateCount = 0;
}

//===========================================
// �A�N�^�[�̓o�^����
//===========================================
void CEnemyAttack::ActorRegist(void)
{
	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CActor*> list = CActor::GetList()->GetList();
	D3DXVECTOR3 pos = GetPosInit();
	D3DXVECTOR3 vtxChase = D3DXVECTOR3(m_pChaseRange->GetWidth(), 0.0f, m_pChaseRange->GetDepth());

	for (auto actor : list)
	{
		assert(actor != nullptr);

		// ���f�����ǐՔ͈͓��ɂȂ��ꍇ�A���ɐi��
		if (!collision::Box2D(pos, actor->GetVec3Position(), vtxChase, vtxChase, actor->GetModelData().vtxMax, -actor->GetModelData().vtxMin)) { continue; }

		// �A�N�^�[��o�^����
		m_actor.push_back(actor);
	}
}
