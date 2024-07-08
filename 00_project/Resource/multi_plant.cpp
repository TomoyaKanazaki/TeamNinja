//===========================================
//
//  �A���Ǘ��N���X(multi_plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "multi_plant.h"
#include "plant.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	// TODO �����Ƃ����A���̃e�N�X�`���p�ӂ���
	const char* TEXTURE_FILE[] = // �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\test.png", // �e�X�g�e�N�X�`��
	};

	const float PERMISSION = 0.02f; // �d�Ȃ�̋��e�͈�
}

//===========================================
//  �ÓI�x������
//===========================================
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMultiPlant::TYPE_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  �R���X�g���N�^
//===========================================
CMultiPlant::CMultiPlant() : CObject3D(),
m_Type(CMultiPlant::TYPE_MAX),
m_pos(VEC3_ZERO),
m_size(VEC3_ZERO),
m_nNum(0),
m_bGrow(false)
{
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
	// �A���̐���
	if (!m_bGrow) { Grow(); }

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
CMultiPlant* CMultiPlant::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EType type, int nNum)
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

	// �m�ۂ����A�h���X��Ԃ�
	return pPlant;
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
				if (pos[j].x - fTemp < PERMISSION)
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
				if (pos[j].y - fTemp < PERMISSION)
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
		D3DXVECTOR3 posPlant = D3DXVECTOR3
		(
			m_pos.x + (m_size.x * pos[i].x),
			m_pos.y,
			m_pos.z + (m_size.z * pos[i].y)
		);

		// �A���𐶐�
		CPlant::Create(posPlant, TEXTURE_FILE[m_Type]);
	}

	// �������̉��
	delete[] pos;
}
