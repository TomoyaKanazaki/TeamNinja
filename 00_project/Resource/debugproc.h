//==========================================================
//
//�f�o�b�O�\������ [debugproc.h]
//Author Ibuki Okusada
// Added by Tomoya Kanazaki
//
//==========================================================
#ifndef _DEBUGPROC_H_		//���̃}�N������`����Ă��Ȃ��ꍇ
#define _DEBUGPROC_H_

//**********************************************************
//�l�[���X�y�[�X�̒�`
//**********************************************************
namespace DebugProc
{
	// �f�o�b�O�ʒu��
	enum EPoint
	{
		POINT_CENTER = 0,	// ����
		POINT_LEFT,			// ��
		POINT_RIGHT,		// �E
		POINT_MAX			// ���̗񋓌^�̑���
	};

	// �ϐ��錾
	static LPD3DXFONT m_pFont;	// �t�H���g�ւ̃|�C���^

	// �֐��錾
	void Init(void); // ����������
	void Uninit(void); // �I������
	void Update(void); // �X�V����
	void Draw(void); // �`�揈��
	void Print(const EPoint point, const char *fmt, ...); // ���������
};

#endif