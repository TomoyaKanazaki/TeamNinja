//===========================================
//
//  �A���Ǘ��N���X(multi_plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "multi_plant.h"
#include "plant.h"
#include "manager.h"
#include "camera.h"
//===========================================
//  �萔��`
//===========================================
namespace
{
	// TODO �����Ƃ����A���̃e�N�X�`���p�ӂ���
	const char* TEXTURE_FILE[] = // �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\Flower001.png",	// �`���[���b�v
		"data\\TEXTURE\\Flower001.png",	// �e�X�g�e�N�X�`��
		"data\\TEXTURE\\Flower001.png",	// �e�X�g�e�N�X�`��
		"data\\TEXTURE\\Flower001.png",	// �e�X�g�e�N�X�`��
		"data\\TEXTURE\\Flower001.png",	// �e�X�g�e�N�X�`��
		"data\\TEXTURE\\Flower001.png",	// �e�X�g�e�N�X�`��
	};

	const float PERMISSION = 0.05f; // �d�Ȃ�̋��e�͈�
	const float SIZE_SCALE = 0.3f; // �����͈͂̔{��

	const float FRAME_SIZE = 2.0f; // �����
}

//===========================================
//  �ÓI�x������
//===========================================
static_assert(NUM_ARRAY(TEXTURE_FILE) == CGimmick::TYPE_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  �R���X�g���N�^
//===========================================
CMultiPlant::CMultiPlant() : CObject3D(),
m_Type(CGimmick::TYPE_MAX),
m_nNum(0),
m_bGrow(false)
{
	memset(&m_pFlame[0], 0, sizeof(m_pFlame));
}

//===========================================
//  �f�X�g���N�^
//===========================================
CMultiPlant::~CMultiPlant()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CMultiPlant::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CObject3D::Init())){ assert(false); return E_FAIL; }

	// ���x���̕ύX
	SetLabel(LABEL_GIMMICK);

	// �`��ݒ�̕ύX
	SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CMultiPlant::Uninit(void)
{
	// �e�N���X�̏I��
	CObject3D::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CMultiPlant::Update(const float fDeltaTime)
{
	// �����̐F���X�V
	FrameAlpha();

	// �e�N���X�̍X�V
	CObject3D::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CMultiPlant::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObject3D::Draw(pShader);
}

//===========================================
//  ��������
//===========================================
CMultiPlant* CMultiPlant::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const CGimmick::EType type, int nNum)
{
	// �M�~�b�N�̐���
	CMultiPlant* pPlant = new CMultiPlant;

	// �������̊m�ۂɎ��s�����ꍇ�֐��𔲂���
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// ������
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// �ʒu��ݒ�
	pPlant->SetVec3Position(rPos);

	// �T�C�Y��ݒ�
	pPlant->SetVec3Sizing(rSize);

	// �^�C�v�̐ݒ�
	pPlant->m_Type = type;

	// �������̐ݒ�
	pPlant->m_nNum = nNum;

	// �����̐���
	pPlant->FrameCreate();

	// �Ԃ̐���
	pPlant->Grow();

	// �m�ۂ����A�h���X��Ԃ�
	return pPlant;
}

//===========================================
//  �����̓�����
//===========================================
void CMultiPlant::FrameAlpha()
{
	// ���g�̃X�N���[�����W���擾
	D3DXVECTOR3 posScreen = VEC3_ZERO;
	GET_CAMERA->OnScreen(GetVec3Position(), posScreen);

	// �X�N���[���̒��S�ƌ��ԃx�N�g���̑傫�� ^ 2���Z�o
	D3DXVECTOR3 vecCenter = SCREEN_CENT - posScreen;
	float fScalar = vecCenter.x * vecCenter.x + vecCenter.y * vecCenter.y;

	// �X�N���[���̍��� * 0.5�ɑ΂��銄���̎Z�o
	float fScale = fScalar / (SCREEN_CENT.y * SCREEN_CENT.y);

	// �����̓����x���X�V
	for (int i = 0; i < 4; ++i)
	{
		// ��������Ă��Ȃ��ꍇ���ɐi��
		if (m_pFlame[i] == nullptr) { continue; }

		// Scale��0�ȉ��̏ꍇ�`�悵�Ȃ�
		if (fScale <= 0.0) { m_pFlame[i]->SetEnableDraw(false); continue; }
		else { m_pFlame[i]->SetEnableDraw(true); }

		// �����x���X�V����
		m_pFlame[i]->SetAlpha(1.0f - fScale);
	}
}

//===========================================
//  �����̕\��
//===========================================
void CMultiPlant::FrameCreate()
{
	// �e���_�̍��W���擾
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 posVtx[4] =
	{
		GetVertexPosition(0) + pos,
		GetVertexPosition(1) + pos,
		GetVertexPosition(2) + pos,
		GetVertexPosition(3) + pos
	};

	// �e���_�����ԃx�N�g�����Z�o
	D3DXVECTOR3 vecVtx[4] =
	{
		(posVtx[1] - posVtx[0]),
		(posVtx[3] - posVtx[1]),
		(posVtx[0] - posVtx[2]),
		(posVtx[2] - posVtx[3])
	};

	// ���𐶐�
	for (int i = 0; i < 4; ++i)
	{
		// �ӂ̏���ݒ�
		D3DXVECTOR3 size = vecVtx[i];
		if (size.x == 0.0f) { size.x = FRAME_SIZE; size.z = fabsf(size.z); }
		if (size.z == 0.0f) { size.z = FRAME_SIZE; size.x = fabsf(size.x); }

		// �ӂ𐶐�
		m_pFlame[i] = CObject3D::Create(posVtx[i] + (vecVtx[i] * 0.5f), size);
		m_pFlame[i]->SetColor(XCOL_CYAN);
		m_pFlame[i]->SetLabel(CObject::LABEL_GIMMICK);
	}
}

//===========================================
//  �Ԃ̐���
//===========================================
void CMultiPlant::Grow()
{
	// �����t���O�𗧂Ă�
	m_bGrow = true;

	// �I�u�W�F�N�g�̐������W�ۑ��p�ϐ�
	D3DXVECTOR2* pos = new D3DXVECTOR2[m_nNum];
	for (int i = 0; i < m_nNum; ++i) { pos[i] = VEC2_ZERO; }

	// �K�v�Ȑ���������
	for (int i = 0; i < m_nNum; ++i)
	{
		// x���W��ݒ�
		while (1)
		{
			// ��������
			float fTemp = ((float)rand() / (RAND_MAX * 0.5f)) - 1.0f;

			// �d�Ȃ�Ȃ��悤�ɂ���
			bool bHit = false;
			for (int j = 0; j < m_nNum; ++j)
			{
				// �d�Ȃ肷�����ꍇ���[�v�𔲂���
				if (fabsf(pos[j].x - fTemp) < PERMISSION)
				{
					bHit = true;
					break;
				}
			}

			// �d�Ȃ��Ă����ꍇ������x
			if (bHit) { continue; }

			// ������ۑ�����
			pos[i].x = fTemp;

			// ���[�v�𔲂���
			break;
		}

		// y���W��ݒ�
		while (1)
		{
			// ��������
			float fTemp = ((float)rand() / (RAND_MAX * 0.5f)) - 1.0f;

			// �d�Ȃ�Ȃ��悤�ɂ���
			bool bHit = false;
			for (int j = 0; j < m_nNum; ++j)
			{
				// �d�Ȃ肷�����ꍇ���[�v�𔲂���
				if (fabsf(pos[j].y - fTemp) < PERMISSION)
				{
					bHit = true;
					break;
				}
			}

			// �d�Ȃ��Ă����ꍇ������x
			if (bHit) { continue; }

			// ������ۑ�����
			pos[i].y = fTemp;

			// ���[�v�𔲂���
			break;
		}

		// ������������������W���Z�o
		D3DXVECTOR3 posThis = GetVec3Position();
		D3DXVECTOR3 sizeThis = GetVec3Sizing() * SIZE_SCALE;
		D3DXVECTOR3 posPlant = D3DXVECTOR3
		(
			posThis.x + (sizeThis.x * pos[i].x),
			posThis.y,
			posThis.z + (sizeThis.z * pos[i].y)
		);

		// �A���𐶐�
		CPlant::Create(posPlant, TEXTURE_FILE[m_Type]);
	}

	// �������̉��
	delete[] pos;
}
