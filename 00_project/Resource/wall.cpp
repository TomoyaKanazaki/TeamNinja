//============================================================
//
//	�Ǐ��� [wall.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wall.h"
#include "manager.h"
#include "collision.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"
#include "camera.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\FIELD\\soil005.png",	// �y�e�N�X�`��
		"data\\TEXTURE\\FIELD\\soil005.png",	// ���y�e�N�X�`��
		"data\\TEXTURE\\FIELD\\soil002.png",	// ���e�N�X�`��
		"data\\TEXTURE\\FIELD\\water.png",		// ���e�N�X�`��
		"data\\TEXTURE\\FIELD\\plaster00.jpg",	// ��̎���e�N�X�`��
		"data\\TEXTURE\\FIELD\\OldWood001.jpg",	// �؃e�N�X�`��
	};

	const int PRIORITY = 4;	// �ǂ̗D�揇��
	const float INVISIBLE_MIN = 300.0f; // �������ŏ��͈�
	const float INVISIBLE_MAX = 500.0f; // �������ŏ��͈�
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CWall::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CWall> *CWall::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CWall] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_SOIL),	// ���
	m_posVtx(nullptr)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CWall::~CWall()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWall::Init(void)
{
	// �����o�ϐ���������
	m_type = TYPE_SOIL;	// ���
	m_posVtx = nullptr;

	// �I�u�W�F�N�g���b�V���E�H�[���̏�����
	if (FAILED(CObjectMeshWall::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CWall>::Create();
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
void CWall::Uninit(void)
{
	// ���_���̔j��
	if (m_posVtx != nullptr)
	{
		delete[] m_posVtx;
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���b�V���E�H�[���̏I��
	CObjectMeshWall::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWall::Update(const float fDeltaTime)
{
	// ���_���̐ݒ�
	//Invisible();

	// �I�u�W�F�N�g���b�V���E�H�[���̍X�V
	CObjectMeshWall::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CWall::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̕`��
	CObjectMeshWall::Draw(pShader);
}

//============================================================
// �����蔻�菈��
//============================================================
bool CWall::Collision
(
	D3DXVECTOR3& rPos,		// �ʒu
	D3DXVECTOR3& rPosOld,	// �O��̈ʒu
	const float fRadius,	// ���a
	const float fHeight,	// ����
	D3DXVECTOR3& rMove,		// �ړ���
	bool* pJump				// �W�����v��
)
{
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);		// ����
	D3DXVECTOR3 PlayerUp = D3DXVECTOR3(fRadius, fHeight, fRadius);	// �v���C���[�̃T�C�Y(��)
	D3DXVECTOR3 PlayerDown = D3DXVECTOR3(fRadius, 0.0f, fRadius);	// �v���C���[�̃T�C�Y(��)
	D3DXVECTOR3 sizeUp = VEC3_ZERO;			// �v���C���[�̃T�C�Y(��)
	D3DXVECTOR3 sizeDown = VEC3_ZERO;		// �v���C���[�̃T�C�Y(��)
	bool bUp = false;		// ���
	bool bHit = false;		// �q�b�g��

	// �T�C�Y��ݒ�
	sizeUp.x = GetVec2Sizing().x * 0.5f;
	sizeUp.y = GetVec2Sizing().y;
	sizeUp.z = 0.0f;
	sizeDown.x = GetVec2Sizing().x * 0.5f;
	sizeDown.y = 0.0f;
	sizeDown.z = 0.0f;

	// �����蔻�菈��(�����̗񋓔������)
	bHit = collision::ResponseBox3D
	(
		rPos,				// �v���C���[�̈ʒu
		rPosOld,			// �v���C���[�̑O��̈ʒu
		GetVec3Position(),	// �ǂ̈ʒu
		PlayerUp,			// �v���C���[�̃T�C�Y(��)
		PlayerDown,			// �v���C���[�̃T�C�Y(��)
		sizeUp,				// �ǂ̃T�C�Y(��)
		sizeDown,			// �ǂ̃T�C�Y(��)
		angle,				// ����
		&rMove,				// �ړ���
		&bUp				// ���
	);

	if (bUp &&
		pJump != nullptr)
	{ // ��ɏ�������A�W�����v�󋵂� NULL ����Ȃ��ꍇ

		// �W�����v�󋵂�ݒ肷��
		*pJump = false;
	}

	// �q�b�g�󋵂�Ԃ�
	return bHit;
}

//============================================================
//	��������
//============================================================
CWall *CWall::Create
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart	// �e�N�X�`��������
)
{
	// �ǂ̐���
	CWall *pWall = new CWall;
	if (pWall == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �ǂ̏�����
		if (FAILED(pWall->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �ǂ̔j��
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// ��ނ�ݒ�
		pWall->SetType(type);

		// �ʒu��ݒ�
		pWall->SetVec3Position(rPos);

		// ������ݒ�
		pWall->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pWall->SetVec2Sizing(rSize);

		// �F��ݒ�
		pWall->SetColor(rCol);

		// ��������ݒ�
		if (FAILED(pWall->SetPattern(rTexPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �ǂ̔j��
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pWall->SetTexPattern(rTexPart);

		// ���_�����v�Z���s��
		pWall->CalcVtx();

		// �m�ۂ����A�h���X��Ԃ�
		return pWall;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CWall> *CWall::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CWall::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		// ��ނ�ۑ�
		m_type = type;

		// �e�N�X�`����o�^�E����
		BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));
	}
	else { assert(false); }	// �͈͊O
}

//==========================================
//  ���_���̐ݒ�
//==========================================
void CWall::Invisible()
{
	// �J�������̎擾
	CCamera* pCamera = GET_CAMERA;

	// ��荞�ݏ�ԂłȂ���Ί֐��𔲂���
	if (pCamera->GetState() != CCamera::STATE_AROUND) { return; }

	// ���g�̏����擾����
	POSGRID2 part = GetPattern();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == nullptr) { return; }
	SMeshWall meshWall = GetMeshWall();
	D3DXCOLOR col = meshWall.col;

	// �J�����̏����擾����
	D3DXVECTOR3 posV = pCamera->GetPositionV();
	float fRotCamera = pCamera->GetRotation().y;

	// �J�����ƕǂ̕�������v���Ȃ��ꍇ�֐��𔲂���
	EAngle angleCamera = useful::RotToFourDire(fRotCamera);
	EAngle angleWall = useful::RotToFourDire(meshWall.rot.y);
	if ((int)angleCamera + (int)angleWall % 2)
	{
		return;
	}

	// �v���C���[�̍��W���擾
	D3DXVECTOR3 posPlayerScreen = pCamera->CalcPlayerPos();
	D3DXVECTOR3 posPlayerWorld = GET_PLAYER->GetCenterPos();

	// ���_���ւ̃|�C���^�錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < part.y + 1; nCntHeight++) // �c
	{
		for (int nCntWidth = 0; nCntWidth < part.x + 1; nCntWidth++) // ��
		{
			// xz���ʏ�ɂ����鎋�_�Ƃ̋������Z�o
			D3DXVECTOR3 vecPlayer = posPlayerWorld - posV;
			D3DXVECTOR3 vecVtx = m_posVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))] - posV;

			// ���_���W���v���C���[���������ʒu�ɂ���ꍇ1.0��Ԃ�
			if (vecPlayer.x * vecPlayer.x + vecPlayer.z * vecPlayer.z < vecVtx.x * vecVtx.x + vecVtx.z * vecVtx.z)
			{
				col.a = 1.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// ���_�̃X�N���[�����W���Z�o
			D3DXVECTOR3 pos = VEC3_ZERO;
			if (!pCamera->OnScreen(m_posVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))], pos))
			{
				// ��ʊO�̏ꍇ1.0��Ԃ�
				col.a = 1.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// �v���C���[�������ɕ`�悳���ꍇ1.0��Ԃ�
			if (pos.z >= posPlayerScreen.z)
			{
				col.a = 1.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// �v���C���[�Ƃ̋���^2���Z�o
			D3DXVECTOR3 vec = posPlayerScreen - pos;
			float fLength = sqrtf(vec.x * vec.x + vec.y * vec.y);

			// �v���C���[�Ƃ̋������߂��ꍇ��0��Ԃ�
			if (fLength < INVISIBLE_MIN)
			{
				col.a = 0.0f;
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
				continue;
			}

			// ���ߔ͈͓��̊������Z�o���Ԃ�
			col.a = (fLength - INVISIBLE_MIN) / (INVISIBLE_MAX - INVISIBLE_MIN);
			pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].col = col;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//==========================================
//  ���_���W�̌v�Z����
//==========================================
void CWall::CalcVtx()
{
	// ���g�̏����擾
	SMeshWall meshWall = GetMeshWall();
	POSGRID2 part = GetPattern();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == nullptr) { return; }

	// ���_�����̃��������m�ۂ���
	m_posVtx = new D3DXVECTOR3[(part.x + 1) * (part.y + 1)];

	//  �v�Z�p�}�g���b�N�X�̐錾
	D3DXMATRIX mtxRot, mtxTrans;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// �|�C���^��錾
	VERTEX_3D* pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < part.y + 1; nCntHeight++) // �c
	{
		for (int nCntWidth = 0; nCntWidth < part.x + 1; nCntWidth++) // ��
		{ 
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&meshWall.mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, meshWall.rot.y, meshWall.rot.x, meshWall.rot.z);
			D3DXMatrixMultiply(&meshWall.mtxWorld, &meshWall.mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].pos.x + meshWall.pos.x,
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].pos.y + meshWall.pos.y,
				pVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))].pos.z + meshWall.pos.z
			);
			D3DXMatrixMultiply(&meshWall.mtxWorld, &meshWall.mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &meshWall.mtxWorld);

			// �}�g���b�N�X���璸�_���W�𒊏o
			m_posVtx[nCntHeight + nCntWidth + (nCntHeight * (part.x))] = D3DXVECTOR3(meshWall.mtxWorld._41, meshWall.mtxWorld._42, meshWall.mtxWorld._43);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}
