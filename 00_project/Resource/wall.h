//============================================================
//
//	�ǃw�b�_�[ [wall.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WALL_H_
#define _WALL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshWall.h"

//==========================================
//  �O���錾
//==========================================
class CCamera;

//************************************************************
//	�N���X��`
//************************************************************
// �ǃN���X
class CWall : public CObjectMeshWall
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_SOIL = 0,	// �y�e�N�X�`��
		TYPE_WEED_SOIL,	// ���y�e�N�X�`��
		TYPE_WEED,		// ���e�N�X�`��
		TYPE_WATER,		// ���e�N�X�`��
		TYPE_PLASTER,	// ��̎���e�N�X�`��
		TYPE_WOOD,		// �؂̃e�N�X�`��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CWall();

	// �f�X�g���N�^
	~CWall() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �����o�֐�(�����ǉ�)
	bool Collision				// �����蔻�菈��
	(
		D3DXVECTOR3& rPos,			// �ʒu
		D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,		// ���a
		const float fHeight,		// ����
		D3DXVECTOR3& rMove,			// �ړ���
		bool* pJump = nullptr		// �W�����v��
	);

	// �ÓI�����o�֐�
	static CWall *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart	// �e�N�X�`��������
	);
	static CListManager<CWall> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	void SetType(const EType type);					// ��ސݒ�
	EType GetType(void) const { return m_type; }	// ��ގ擾

private:

	// �����o�֐�
	void Invisible(); // ����������
	float InvisibleVtx(const D3DXVECTOR3& posVtx, const D3DXVECTOR3& posPlayer, const D3DXVECTOR3& posPlayerWorld, SMeshWall meshWall, CCamera* pCamera); // ���_�v�Z����

	// �ÓI�����o�ϐ�
	static CListManager<CWall> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CWall>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;	// ���
};

#endif	// _WALL_H_
