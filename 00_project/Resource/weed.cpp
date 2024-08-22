//============================================================
//
//	������ [weed.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "weed.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY		= 6;	// ���\���̗D�揇��
	const int ALPHA_NUMREF	= 120;	// ���e�X�g�̎Q�ƒl
}

//************************************************************
//	�e�N���X [CWeed] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWeed::CWeed() : CObject3D(CObject::LABEL_WEED, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_fCurLength	(0.0f),	// ���݂̋���
	m_fDestLength	(0.0f),	// �ڕW�̋���
	m_fCurAngle		(0.0f),	// ���݂̊p�x
	m_fDestAngle	(0.0f)	// �ڕW�̊p�x
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CWeed::~CWeed()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWeed::Init(void)
{
	// �����o�ϐ���������
	m_fCurLength	= 0.0f;	// ���݂̋���
	m_fDestLength	= 0.0f;	// �ڕW�̋���
	m_fCurAngle		= 0.0f;	// ���݂̊p�x
	m_fDestAngle	= 0.0f;	// �ڕW�̊p�x

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���̃e�N�X�`��������
	BindTexture("data\\TEXTURE\\grass000.png");

	// ���_�����ɂ���
	SetOrigin(CObject3D::ORIGIN_DOWN);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// �J�����O���[�h�𗼖ʕ\���ɂ���
	pRenderState->SetCulling(D3DCULL_NONE);

	// ���C�e�B���O�̉e�����󂯂Ȃ��悤�ɂ���
	pRenderState->SetLighting(false);

	// �傫����ݒ�
	SetVec3Sizing(D3DXVECTOR3(100.0f, 100.0f, 0.0f));	// TODO

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CWeed::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWeed::Update(const float fDeltaTime)
{
	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CWeed::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CWeed *CWeed::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// ���̐���
	CWeed *pWeed = new CWeed;
	if (pWeed == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���̏�����
		if (FAILED(pWeed->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���̔j��
			SAFE_DELETE(pWeed);
			return nullptr;
		}

		// �ʒu��ݒ�
		pWeed->SetVec3Position(rPos);

		// ������ݒ�
		pWeed->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pWeed;
	}
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
void CWeed::CollisionPlayer(void)
{
	CPlayer* pPlayer = GET_PLAYER;	// �v���C���[���

	// �v���C���[�����Ȃ��ꍇ������
	if (pPlayer == nullptr) { return; }

	// �v���C���[�Ƃ̐ڐG����
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 posWeed = GetVec3Position();			// ���ʒu
	float fRadPlayer = pPlayer->GetRadius();			// �v���C���[���a
	float fRadWeed = GetVec3Sizing().x;					// �����a
	if (collision::Circle3D(posPlayer, posWeed, fRadPlayer, fRadWeed))
	{ // ����ł����ꍇ

		// �v���C���[�Ԃ̋��������߂�
		float fDistance = sqrtf((posPlayer.x - posWeed.x) * (posPlayer.x - posWeed.x) + (posPlayer.z - posWeed.z) * (posPlayer.z - posWeed.z));

		// ���̐�[�����炷���������߂�
		m_fDestLength = fabsf(fDistance - (fRadPlayer + fRadWeed));
		useful::LimitMaxNum(m_fDestLength, (fRadPlayer + fRadWeed) - 60.0f);

		// �v���C���[���������߂�
		m_fDestAngle = atan2f(posWeed.x - posPlayer.x, posWeed.z - posPlayer.z);
	}
	else
	{ // ����ł��Ȃ��ꍇ

		// ���܂�Ă��Ȃ��̂ŏ����l�ɖ߂�
		m_fDestLength = 0.0f;	// �ڕW�̋���
		m_fDestAngle = 0.0f;	// �ڕW�̊p�x
	}

	// ���݂̋��������߂�
	float fDiffLength = m_fDestLength - m_fCurLength;	// �����̋���
	m_fCurLength += fDiffLength * 0.5f;					// ������ڕW�ɋ߂Â���

	// ���݂̊p�x�����߂�
	float fDiffAngle = m_fDestAngle - m_fCurAngle;		// �����̊p�x
	m_fCurAngle += fDiffAngle * 0.5f;					// ������ڕW�ɋ߂Â���

	// �㒸�_�ړ��I�t�Z�b�g�����߂�
	D3DXVECTOR3 offset = D3DXVECTOR3(m_fCurLength * sinf(m_fCurAngle), 0.0f, m_fCurLength * cosf(m_fCurAngle));

	// ���W�̂��炷�ʂ�ݒ�
	SetGapPosition(0, offset);
	SetGapPosition(1, offset);
}
