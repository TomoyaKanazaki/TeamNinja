//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "motion.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CCollSphere;	// �~����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�L�����N�^�[�N���X
class CObjectChara : public CObject
{
public:
	// �R���X�g���N�^
	explicit CObjectChara(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectChara() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetEnableUpdate(const bool bUpdate) override;		// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }	// �ʒu�擾
	D3DXVECTOR3 GetVec3Rotation(void) const override { return m_rot; }	// �����擾

	// �ÓI�����o�֐�
	static CObjectChara *Create(const D3DXVECTOR3 &rPos, const D3DXVECTOR3 &rRot = VEC3_ZERO);	// ����

	// ���z�֐�
	virtual void SetMotion(const int nType, const int nBlendFrame = 0);	// ���[�V�����ݒ�

	// �����o�֐�
	void SetPartsInfo	// �p�[�c���ݒ�
	( // ����
		const int nID,				// �p�[�c�C���f�b�N�X
		const int nParentID,		// �e�C���f�b�N�X
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const char *pFileName		// �t�@�C����
	);
	void SetMtxWorld(const D3DXMATRIX &rMtxWorld);	// �}�g���b�N�X�ݒ�
	void SetPartsPosition(const int nPartsID, const D3DXVECTOR3& rPos);	// �p�[�c�ʒu�ݒ�
	void SetPartsRotation(const int nPartsID, const D3DXVECTOR3& rRot);	// �p�[�c�����ݒ�
	D3DXVECTOR3 GetPartsPosition(const int nPartsID) const;		// �p�[�c�ʒu�擾
	D3DXVECTOR3 GetPartsRotation(const int nPartsID) const;		// �p�[�c�����擾
	CMultiModel *GetMultiModel(const int nPartsID) const;		// �}���`���f���擾
	CMotion *GetMotion(void) const;								// ���[�V�����擾
	D3DXMATRIX *GetPtrMtxWorld(void)	{ return &m_mtxWorld; }	// �}�g���b�N�X�|�C���^�擾
	D3DXMATRIX GetMtxWorld(void) const	{ return m_mtxWorld; }	// �}�g���b�N�X�擾
	int GetNumModel(void) const			{ return m_nNumModel; }	// �p�[�c�����擾

	void SetMaterial(const D3DXMATERIAL& rMat, const int nPartsID, const int nMatID);	// �}�e���A���ݒ�
	void SetAllMaterial(const D3DXMATERIAL& rMat);	// �}�e���A���S�ݒ�
	void ResetMaterial(void);			// �}�e���A���Đݒ�
	void SetAlpha(const float fAlpha);	// �����x�ݒ�
	float GetAlpha(void) const;			// �����x�擾
	float GetMaxAlpha(void) const;		// �ő哧���x�擾

	void SetModelInfo(void)							{ return m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel); }	// ���f�����ݒ�
	void SetMotionInfo(CMotion::SMotionInfo info)	{ return m_pMotion->SetInfo(info); }				// ���[�V�������ݒ�
	void SetEnableMotionUpdate(const bool bUpdate)	{ return m_pMotion->SetEnableUpdate(bUpdate); }		// �X�V�󋵐ݒ�
	int GetMotionType(void) const			{ return m_pMotion->GetType(); }							// ���[�V������ގ擾
	int GetMotionNumType(void) const		{ return m_pMotion->GetNumType(); }							// ���[�V������ޑ����擾
	int GetMotionKey(void) const			{ return m_pMotion->GetKey();}								// ���[�V�����L�[�ԍ��擾
	int GetMotionNumKey(void) const			{ return m_pMotion->GetNumKey(m_pMotion->GetType()); }		// ���[�V�����L�[�����擾
	int GetMotionKeyCounter(void) const		{ return m_pMotion->GetKeyCounter(); }						// ���[�V�����L�[�J�E���^�[�擾
	int GetMotionWholeCounter(void) const	{ return m_pMotion->GetWholeCounter(); }					// ���[�V�����S�̃J�E���^�[�擾
	int GetMotionWholeFrame(void) const		{ return m_pMotion->GetWholeFrame(m_pMotion->GetType()); }	// ���[�V�����S�̃t���[�����擾
	int GetMotionCancelFrame(void) const	{ return m_pMotion->GetCancelFrame(m_pMotion->GetType()); }	// ���[�V�����L�����Z���t���[���擾
	int GetMotionComboFrame(void) const		{ return m_pMotion->GetComboFrame(m_pMotion->GetType()); }	// ���[�V�����R���{�t���[���擾
	bool IsMotionFinish(void) const			{ return m_pMotion->IsFinish(); }							// ���[�V�����I���擾
	bool IsMotionLoop(void) const			{ return m_pMotion->IsLoop(m_pMotion->GetType()); }			// ���[�V�������[�v�擾
	bool IsMotionCancel(void) const			{ return m_pMotion->IsCancel(m_pMotion->GetType()); }		// ���[�V�����L�����Z���擾
	bool IsMotionCombo(void) const			{ return m_pMotion->IsCombo(m_pMotion->GetType()); }		// ���[�V�����R���{�擾
	bool IsWeaponDisp(void) const			{ return m_pMotion->IsWeaponDisp(m_pMotion->GetType()); }	// ���[�V��������\���擾
	bool IsLeftWeaponCollision(void)		{ return m_pMotion->IsLeftWeaponCollision(); }				// ���̍U������t���O�擾
	bool IsRightWeaponCollision(void)		{ return m_pMotion->IsRightWeaponCollision(); }				// �E�̍U������t���O�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	CMultiModel	*m_apMultiModel[motion::MAX_PARTS];	// ���f���̏��
	CCollSphere	*m_apCollision[motion::MAX_PARTS];	// �~����N���X
	CMotion		*m_pMotion;		// ���[�V�����̏��
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// �ʒu
	D3DXVECTOR3	m_rot;			// ����
	int			m_nNumModel;	// �p�[�c�̑���
};

#endif	// _OBJECTCHARA_H_
