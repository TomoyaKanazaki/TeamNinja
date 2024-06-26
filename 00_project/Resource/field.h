//============================================================
//
//	�n�ʃw�b�_�[ [field.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshField.h"

//************************************************************
//  �O���錾
//************************************************************
class CPlayerClone;	// �v���C���[�̕��g�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �n�ʃN���X
class CField : public CObjectMeshField
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_SOIL = 0,	// �y�e�N�X�`��
		TYPE_WEED_SOIL,	// ���y�e�N�X�`��
		TYPE_WEED,		// ���e�N�X�`��
		TYPE_FLOOR,		// ���e�N�X�`��
		TYPE_GRAVEL,	// ������
		TYPE_BOOB,		// �D
		TYPE_CLEEN,		// �|����
		TYPE_FALL,		// ���Ƃ���
		TYPE_DECAYED,	// ��������
		TYPE_WATER,		// ��
		TYPE_GUTTER,	// �h�u
		TYPE_BRIDGE,	// ��
		TYPE_MAX,		// ���̗񋓌^�̑���
		TYPE_NONE		// ���݂��Ȃ����
	};

	// �n�`��
	enum ETerrain
	{
		TERRAIN_120x120 = 0,	// 120x120�������̒n�`
		TERRAIN_MAX				// ���̗񋓌^�̑���
	};

	// �t�B�[���h�
	enum EZ
	{
		Z_MIDDLE = 0, // ���S
		Z_FRONT, // ��O
		Z_BACK, // ��
		Z_MAX
	};

	// �R���X�g���N�^
	CField();

	// �f�X�g���N�^
	~CField() override;

	// �n�`���\����
	struct STerrainInfo
	{
		D3DXVECTOR3 *pPosGap;	// ���_���W�̂����
		POSGRID2 part;			// ������
	};

	// ���z�֐�
	virtual void Hit(CPlayerClone* pClone);		// ���g�ɓ������Ă������̏���
	virtual void Miss(CPlayerClone* pClone);	// ���g�ɓ������Ă��Ȃ����̏���

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CField *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart	// �e�N�X�`��������
	);
	static CListManager<CField> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	void SetTerrain(const ETerrain terrain);	// �n�`�ݒ�
	void SetType(const EType type);				// ��ސݒ�
	ETerrain GetTerrain(void) const	{ return m_terrain; }	// �n�`�擾
	EType GetType(void) const		{ return m_type; }		// ��ގ擾
	virtual bool IsFall() const { return bool(); }			// ��������̎擾
	const char GetFlag() const;								// �t���O�擾

	// �ÓI�����o�֐�
	static EZ CalcNearLine(); // �v���C���[�ɍł��߂�����̎Z�o

	// �ÓI�����o�֐�
	static const char GetFlag(EType type);		// �t���O�擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CField> *m_pList;	// �I�u�W�F�N�g���X�g
	static STerrainInfo m_aTerrainInfo[TERRAIN_MAX];	// �n�`���
	static EZ m_eNear; // �ł��߂����

	// �����o�ϐ�
	CListManager<CField>::AIterator m_iterator;	// �C�e���[�^�[
	ETerrain m_terrain;	// �n�`
	EType m_type;		// ���
};

#endif	// _FIELD_H_
