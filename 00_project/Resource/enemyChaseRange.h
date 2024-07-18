//============================================================
//
//	�G�̒ǐՔ͈̓w�b�_�[ [enemyChaseRange.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_CHASE_RANGE_H_
#define _ENEMY_CHASE_RANGE_H_

//************************************************************
// �O���錾
//************************************************************
class CObjectMeshCube;		// ���b�V���L���[�u

//************************************************************
//	�N���X��`
//************************************************************
// �G�̒ǐՔ͈̓N���X
class CEnemyChaseRange
{
public:

	// �R���X�g���N�^
	CEnemyChaseRange();

	// �f�X�g���N�^
	~CEnemyChaseRange();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��

	// �����o�֐�
	bool ChaseRange(const D3DXVECTOR3& rInitPos, D3DXVECTOR3* pPos);					// �ǐՔ͈͏���
	bool InsideTargetPos(const D3DXVECTOR3& rInitPos, const D3DXVECTOR3& rTargetPos);	// �^�[�Q�b�g���͈͓��ɂ��邩�̏���

	// �ÓI�����o�֐�
	static CEnemyChaseRange* Create(const D3DXVECTOR3& rPos, const float fWidth, const float fDepth);		// ��������

private:

	// �����o�ϐ�
	float m_fWidth;		// ��
	float m_fDepth;		// ���s

#ifdef _DEBUG

	CObjectMeshCube* m_pRangeCube;		// �͈̓L���[�u

#endif // _DEBUG
};

#endif	// _ENEMY_CHASE_RANGE_H_
