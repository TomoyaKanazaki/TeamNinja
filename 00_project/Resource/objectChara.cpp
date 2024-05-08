//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[���� [objectChara.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	�q�N���X [CObjectChara] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectChara::CObjectChara(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pMotion	(nullptr),		// ���[�V�����̏��
	m_pos		(VEC3_ZERO),	// �ʒu
	m_rot		(VEC3_ZERO),	// ����
	m_nNumModel	(0)				// �p�[�c�̑���
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	memset(&m_apCollision[0], 0, sizeof(m_apCollision));	// �����蔻��̏��
	D3DXMatrixIdentity(&m_mtxWorld);	// ���[���h�}�g���b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectChara::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	memset(&m_apCollision[0], 0, sizeof(m_apCollision));	// �����蔻��̏��
	D3DXMatrixIdentity(&m_mtxWorld);	// ���[���h�}�g���b�N�X
	m_pMotion	= nullptr;		// ���[�V�����̏��
	m_pos		= VEC3_ZERO;	// �ʒu
	m_rot		= VEC3_ZERO;	// ����
	m_nNumModel	= 0;			// �p�[�c�̑���

	// ���[�V�����̐���
	m_pMotion = CMotion::Create(this);
	if (m_pMotion == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectChara::Uninit(void)
{
	for (int nCntChara = 0; nCntChara < motion::MAX_PARTS; nCntChara++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �}���`���f���̏I��
		SAFE_UNINIT(m_apMultiModel[nCntChara]);
	}

	// ���[�V�����̔j��
	SAFE_REF_RELEASE(m_pMotion);

	// �I�u�W�F�N�g�L�����N�^�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectChara::Update(const float fDeltaTime)
{
	if (m_pMotion != nullptr)
	{ // ���[�V���������݂���ꍇ

		// ���[�V�����̍X�V
		m_pMotion->Update(fDeltaTime);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectChara::Draw(CShader *pShader)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �p�[�c�̕`��
		m_apMultiModel[nCntChara]->Draw(pShader);
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CObjectChara::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);	// ���g
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // �p�[�c�̑������J��Ԃ�

		m_apMultiModel[nCntChara]->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CObjectChara::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // �p�[�c�̑������J��Ԃ�

		m_apMultiModel[nCntChara]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectChara::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot);
}

//============================================================
//	��������
//============================================================
CObjectChara *CObjectChara::Create(const D3DXVECTOR3 &rPos, const D3DXVECTOR3 &rRot)
{
	// �I�u�W�F�N�g�L�����N�^�[�̐���
	CObjectChara *pObjectChara = new CObjectChara;
	if (pObjectChara == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�L�����N�^�[�̏�����
		if (FAILED(pObjectChara->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�L�����N�^�[�̔j��
			SAFE_DELETE(pObjectChara);
			return nullptr;
		}

		// �ʒu��ݒ�
		pObjectChara->SetVec3Position(rPos);

		// ������ݒ�
		pObjectChara->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectChara;
	}
}

//============================================================
//	���[�V�����̐ݒ菈��
//============================================================
void CObjectChara::SetMotion(const int nType, const int nBlendFrame)
{
	// ���[�V�����̐ݒ�
	m_pMotion->Set(nType, nBlendFrame);
}

//============================================================
//	�p�[�c���̐ݒ菈��
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nID,				// �p�[�c�C���f�b�N�X
	const int nParentID,		// �e�C���f�b�N�X
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const char *pFileName		// �t�@�C����
)
{
	if (nID < motion::MAX_PARTS && nParentID < motion::MAX_PARTS && pFileName != nullptr)
	{ // �p�[�c�E�e�C���f�b�N�X���z��͈͓����A�t�@�C���������݂���ꍇ

		// ���f���̐���
		m_apMultiModel[nID] = CMultiModel::Create(rPos, rRot);

		// ���f���̌��_�ʒu�E������ݒ�
		m_pMotion->SetOriginPosition(rPos, nID);
		m_pMotion->SetOriginRotation(rRot, nID);

		// ���f��������
		m_apMultiModel[nID]->BindModel(pFileName);

		// �e���f���̐ݒ�
		if (nParentID == NONE_IDX)
		{ // �e���Ȃ��ꍇ

			// nullptr��ݒ�
			m_apMultiModel[nID]->SetParentModel(nullptr);
		}
		else
		{ // �e������ꍇ

			// �e�̃A�h���X��ݒ�
			m_apMultiModel[nID]->SetParentModel(m_apMultiModel[nParentID]);
		}

		// �p�[�c�̑��������Z
		m_nNumModel++;
	}
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectChara::SetMtxWorld(const D3DXMATRIX &rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	�p�[�c�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetPartsPosition(const int nPartsID, const D3DXVECTOR3& rPos)
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̈ʒu��ݒ�
		m_apMultiModel[nPartsID]->SetVec3Position(rPos);
	}
	else { assert(false); }
}

//============================================================
//	�p�[�c�����̐ݒ菈��
//============================================================
void CObjectChara::SetPartsRotation(const int nPartsID, const D3DXVECTOR3& rRot)
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̌�����ݒ�
		m_apMultiModel[nPartsID]->SetVec3Rotation(rRot);
	}
	else { assert(false); }
}

//============================================================
//	�p�[�c�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsPosition(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����C���f�b�N�X�̃p�[�c�̈ʒu��Ԃ�
		return m_apMultiModel[nPartsID]->GetVec3Position();
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	�p�[�c�����擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsRotation(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����C���f�b�N�X�̃p�[�c�̌�����Ԃ�
		return m_apMultiModel[nPartsID]->GetVec3Rotation();
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	�}���`���f���擾����
//============================================================
CMultiModel *CObjectChara::GetMultiModel(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �}���`���f���̏���Ԃ�
		return m_apMultiModel[nPartsID];
	}
	else { assert(false); return m_apMultiModel[0]; }
}

//============================================================
//	���[�V�����擾����
//============================================================
CMotion *CObjectChara::GetMotion(void) const
{
	// �C���X�^���X���g�p
	assert(m_pMotion != nullptr);

	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}

//============================================================
//	�}�e���A���̐ݒ菈��
//============================================================
void CObjectChara::SetMaterial(const D3DXMATERIAL& rMat, const int nPartsID, const int nMatID)
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����̃}�e���A����ݒ�
		m_apMultiModel[nPartsID]->SetMaterial(rMat, nMatID);
	}
	else { assert(false); }
}

//============================================================
//	�}�e���A���S�ݒ菈��
//============================================================
void CObjectChara::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �����̃}�e���A����S�}�e���A���ɐݒ�
		m_apMultiModel[nCntChara]->SetAllMaterial(rMat);
	}
}

//============================================================
//	�}�e���A���Đݒ菈��
//============================================================
void CObjectChara::ResetMaterial(void)
{
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �S�}�e���A���ɏ����}�e���A�����Đݒ�
		m_apMultiModel[nCntChara]->ResetMaterial();
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CObjectChara::SetAlpha(const float fAlpha)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �����̓����x��ݒ�
		m_apMultiModel[nCntParts]->SetAlpha(fAlpha);
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CObjectChara::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		float fCurAlpha = m_apMultiModel[nCntParts]->GetAlpha();	// ���݂̃��f���̓����x
		if (fCurAlpha > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = fCurAlpha;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CObjectChara::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (m_apMultiModel[nCntParts]->GetMaxAlpha() > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_apMultiModel[nCntParts]->GetMaxAlpha();
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CObjectChara::LoadSetup(const char *pTextPass, const char **ppModelPass)
{
	// �ϐ���錾
	CMotion::SMotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nParentID	= 0;	// �e�C���f�b�N�X�̑���p
	int nNowPose	= 0;	// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;	// ���݂̃L�[�ԍ�
	int nLoop		= 0;	// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �|�[�Y����p�̕ϐ���������
	memset(&info, 0, sizeof(info));

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(pTextPass, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

						// �p�[�c���̐ݒ�
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, ppModelPass[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// ���������𐳋K��
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "���[�V�����Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
