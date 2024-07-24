//============================================================
//
//	�J�ڃ|�C���g���� [transpoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "transpoint.h"
#include "collision.h"
#include "manager.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char*	MODEL_PASS	= "data\\MODEL\\GATE\\gate001.x";	// ���f���t�@�C��
	const int	PRIORITY	= 3;		// �J�ڃ|�C���g�̗D�揇��
	const float	RADIUS		= 120.0f;	// �J�ڃ|�C���g�ɐG����锼�a
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CTransPoint>* CTransPoint::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CTransPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_TRANSPOINT, CObject::DIM_3D, PRIORITY),
	m_sTransMapPass	(pPass)	// �J�ڐ�}�b�v�p�X
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTransPoint::~CTransPoint()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTransPoint::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �J�ڃ|�C���g���f��������
	BindModel(MODEL_PASS);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CTransPoint>::Create();
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
void CTransPoint::Uninit(void)
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
void CTransPoint::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTransPoint::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pShader);
	}
}

//============================================================
//	��������
//============================================================
CTransPoint *CTransPoint::Create(const char* pPass, const D3DXVECTOR3& rPos)
{
	// �J�ڃ|�C���g�̐���
	CTransPoint *pTransPoint = new CTransPoint(pPass);
	if (pTransPoint == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �J�ڃ|�C���g�̏�����
		if (FAILED(pTransPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �J�ڃ|�C���g�̔j��
			SAFE_DELETE(pTransPoint);
			return nullptr;
		}

		// �ʒu��ݒ�
		pTransPoint->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pTransPoint;
	}
}

//============================================================
//	�J�ڃ|�C���g�Ƃ̓����蔻��
//============================================================
CTransPoint* CTransPoint::Collision(const D3DXVECTOR3& rPos, const float fRadius)
{
	// �J�ڃ|�C���g���Ȃ��ꍇ������
	if (m_pList == nullptr) { return nullptr; }

	std::list<CTransPoint*> list = m_pList->GetList();	// �������X�g
	for (const auto& rList : list)
	{ // �v�f�����J��Ԃ�

		D3DXVECTOR3 posTrans = rList->GetVec3Position();	// �J�ڃ|�C���g�ʒu

		// XZ���ʂ̉~�̓����蔻��
		bool bHit = collision::Circle2D
		(
			rPos,		// ����ʒu
			posTrans,	// ����ڕW�ʒu
			fRadius,	// ���蔼�a
			RADIUS		// ����ڕW���a
		);
		if (bHit)
		{ // �������Ă���ꍇ

			// �������Ă���J�ڃ|�C���g��Ԃ�
			return rList;
		}
	}

	// ����������Ă��Ȃ��ꍇnullptr��Ԃ�
	return nullptr;
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CTransPoint> *CTransPoint::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CTransPoint::LoadSetup(const char* pPass)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	std::string sTrans;	// �J�ڐ�}�b�v�p�X�̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�J�ڃ|�C���g�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
		else if (str == "STAGE_TRANSSET")
		{
			do
			{ // END_STAGE_TRANSSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#')
				{ // �R�����g�A�E�g����Ă���ꍇ

					// ��s�S�ēǂݍ���
					std::getline(file, str);
				}
				else if (str == "TRANSSET")
				{
					do
					{ // END_TRANSSET��ǂݍ��ނ܂Ń��[�v

						// �������ǂݍ���
						file >> str;

						if (str == "TRANS_PASS")
						{
							file >> str;	// ����Ǎ�
							file >> sTrans;	// �J�ڐ�}�b�v�p�X��Ǎ�
						}
						else if (str == "POS")
						{
							file >> str;	// ����Ǎ�

							// �ʒu��Ǎ�
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
					} while (str != "END_TRANSSET");	// END_TRANSSET��ǂݍ��ނ܂Ń��[�v

					// �J�ڃ|�C���g�̐���
					if (CTransPoint::Create(sTrans.c_str(), pos) == nullptr)
					{ // �m�ۂɎ��s�����ꍇ

						// ���s��Ԃ�
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_TRANSSET");	// END_STAGE_TRANSSET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
