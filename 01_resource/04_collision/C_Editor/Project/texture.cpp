//============================================================
//
//	�e�N�X�`������ [texture.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
int CTexture::m_nNumAll = 0;	// �e�N�X�`���̑���

//************************************************************
//	�e�N���X [CTexture] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTexture::CTexture()
{
	// �e�N�X�`���ւ̃|�C���^���N���A
	memset(&m_apTexture[0], 0, sizeof(m_apTexture));

	// �S�t�@�C�������폜
	m_sFileName.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTexture::~CTexture()
{

}

//============================================================
//	�e�N�X�`����������
//============================================================
HRESULT CTexture::Load(void)
{
	// �e�N�X�`���ւ̃|�C���^��������
	for (int nCntTexture = 0; nCntTexture < texture::MAX_NUM; nCntTexture++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// null����
		m_apTexture[nCntTexture] = nullptr;
	}

	// �S�t�@�C�������폜
	m_sFileName.clear();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�e�N�X�`���j������
//============================================================
void CTexture::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < texture::MAX_NUM; nCntTexture++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �e�N�X�`���̔j��
		SAFE_RELEASE(m_apTexture[nCntTexture]);
	}

	// �S�t�@�C�������폜
	m_sFileName.clear();
}

//============================================================
//	�e�N�X�`���o�^���� (����)
//============================================================
int CTexture::Regist(const SInfo info)
{
	// �ϐ���錾
	HRESULT hr;				// �ُ�I���̊m�F�p
	int nID = m_nNumAll;	// �e�N�X�`���Ǎ��ԍ�

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	if (m_nNumAll >= texture::MAX_NUM)
	{ // �e�N�X�`���I�[�o�[�̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}

	// ��̃e�N�X�`���𐶐�
	hr = D3DXCreateTexture
	( // ����
		pDevice,			// Direct3D�f�o�C�X
		info.Width,			// �e�N�X�`������
		info.Height,		// �e�N�X�`���c��
		info.MipLevels,		// �~�b�v�}�b�v���x��
		info.Usage,			// �����E�m�ۃI�v�V����
		info.Format,		// �s�N�Z���t�H�[�}�b�g
		info.Pool,			// �i�[������
		&m_apTexture[nID]	// �e�N�X�`���ւ̃|�C���^
	);
	if (FAILED(hr))
	{ // �e�N�X�`���̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}

	// �e�N�X�`���t�@�C�����Ȃ���ۑ�
	m_sFileName.push_back(NONE_STRING);

	// �e�N�X�`���̑��������Z
	m_nNumAll++;

	// �ǂݍ��񂾃e�N�X�`���̔z��ԍ���Ԃ�
	return nID;
}

//============================================================
//	�e�N�X�`���o�^���� (�p�X)
//============================================================
int CTexture::Regist(const char *pFileName)
{
	// �ϐ���錾
	int nID = m_nNumAll;	// �e�N�X�`���Ǎ��ԍ�

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	if (pFileName != nullptr)
	{ // �|�C���^���g�p����Ă���ꍇ

		int nCntTexture = 0;	// �e�N�X�`���ԍ�
		for (auto sFileName : m_sFileName)
		{ // �Ō���܂ŌJ��Ԃ�

			if (sFileName.compare(pFileName) == 0)
			{ // �����񂪈�v�����ꍇ

				// ���łɓǂݍ���ł���e�N�X�`���̔z��ԍ���Ԃ�
				return nCntTexture;
			}

			// ���̃e�N�X�`���ԍ��ɂ���
			nCntTexture++;
		}

		if (m_nNumAll >= texture::MAX_NUM)
		{ // �e�N�X�`���I�[�o�[�̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice, pFileName, &m_apTexture[nID])))
		{ // �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �e�N�X�`���̃t�@�C������ۑ�
		m_sFileName.push_back(pFileName);

		// �e�N�X�`���̑��������Z
		m_nNumAll++;

		// �ǂݍ��񂾃e�N�X�`���̔z��ԍ���Ԃ�
		return nID;
	}
	else
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// �e�N�X�`����g�p��Ԃ�
		return NONE_IDX;
	}
}

//============================================================
//	�e�N�X�`�����̎擾����
//============================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nID)
{
	if (nID >= 0 && nID < m_nNumAll)
	{ // �����̃C���f�b�N�X���͈͓��̏ꍇ

		// �����̃e�N�X�`���|�C���^��Ԃ�
		return m_apTexture[nID];
	}
	else if (nID == NONE_IDX)
	{ // �����̃C���f�b�N�X�� -1�̏ꍇ

		// nullptr��Ԃ�
		return nullptr;
	}
	else
	{ // �����̃C���f�b�N�X���g�p�s�̏ꍇ

		// nullptr��Ԃ�
		assert(false);
		return nullptr;
	}
}

//============================================================
//	��������
//============================================================
CTexture *CTexture::Create(void)
{
	// �e�N�X�`���̐���
	CTexture *pTexture = new CTexture;
	if (pTexture == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�N�X�`���̓Ǎ�
		if (FAILED(pTexture->Load()))
		{ // �e�N�X�`���Ǎ��Ɏ��s�����ꍇ

			// �e�N�X�`���̔j��
			SAFE_DELETE(pTexture);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTexture;
	}
}

//============================================================
//	�j������
//============================================================
void CTexture::Release(CTexture *&prTexture)
{
	// �e�N�X�`���̔j��
	assert(prTexture != nullptr);
	prTexture->Unload();

	// �������J��
	SAFE_DELETE(prTexture);
}
