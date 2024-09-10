//====================================
//�G�t�F�N�V�A�R���g���[���N���X
//Author: �O�� ���V��
//====================================
#ifndef  _EFFEKSEERCONTROL_H_//��d�C���N���[�h�h�~�̃}�N��
#define _EFFEKSEERCONTROL_H_
//�C���N���[�h
#include "main.h"
#include <vector>
#include <functional>
#include <string>
class CEffekseer
{
public:
	//�N���X���N���X
	class CEffectData//�G�t�F�N�g���
	{
	public:

		CEffectData();
		~CEffectData();
		void Erase();
		bool GetExist();
		::Effekseer::Vector3D m_pos;	//���W
		::Effekseer::Vector3D m_rot;	//����
		::Effekseer::Vector3D m_move;	//�ړ���
		Effekseer::Handle handle;		//�{��
		Effekseer::EffectRef efcRef;	//�Q�Ə��
		std::string Path;				//�p�X
		float m_fScale;					//�X�P�[��
		bool m_bLoop;					//���[�v
		bool m_bAutoDelete;
	};

	CEffekseer();
	~CEffekseer();
	void Init();
	void Update();
	void Draw();

	void Uninit();

	void AllClear();
	Effekseer::EffectRef Loading(std::string path);
	CEffectData* Create(std::string path, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fScale = 1.0f, bool bLoop = false, bool bAutoDelete = true);
	static CEffekseer* GetInstance() {
		if (pInstance == NULL) { pInstance = new CEffekseer; return pInstance; }
		else
		{
			return pInstance;
		}
	}
	static void Release()
	{
			SAFE_DELETE(pInstance)
	}
	Effekseer::ManagerRef GetManager() { return m_EfkManager; }
	std::vector<CEffectData*> GetList() { return m_vEffect; }
protected:
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

private:
	static CEffekseer* pInstance;						//�R���g���[���N���X�{��
	std::vector<CEffectData *> m_vEffect;				//�G�t�F�N�g�v�[��
	EffekseerRendererDX9::RendererRef m_efkRenderer;	//�����_���[
	Effekseer::ManagerRef m_EfkManager;					//�}�l�[�W��
	int32_t time = 0;									//����
};


#endif // ! _EFFEKSEERCONTROL_H_