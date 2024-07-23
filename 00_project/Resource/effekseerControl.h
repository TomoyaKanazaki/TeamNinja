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

class CEffekseer
{
public:
	//�N���X���N���X
	class CEffectData//�G�t�F�N�g���
	{
	public:

		CEffectData();
		~CEffectData();
	
		::Effekseer::Vector3D m_pos;	//���W
		::Effekseer::Vector3D m_rot;	//����
		::Effekseer::Vector3D m_move;	//�ړ���
		Effekseer::Handle handle;		//�{��
		Effekseer::EffectRef efcRef;	//�Q�Ə��
		float m_fScale;					//�X�P�[��
		bool m_bLoop;					//���[�v
	};

	CEffekseer();
	~CEffekseer();
	void Init();
	void Update();
	void Draw();

	void Uninit();
	Effekseer::EffectRef Loading(std::string path);
	CEffectData* Create(std::string path, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fScale = 1.0f, bool bLoop = false);
	static CEffekseer* GetInstance() {
		if (pInstance == NULL) { pInstance = new CEffekseer; return pInstance; }
		else
		{
			return pInstance;
		}
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

//�G�t�F�N�g�f�[�^���O���Ŏg�p���邽�߂̃N���X
class CEFFECTDATA
{
public:
	CEFFECTDATA();
	~CEFFECTDATA();
	CEFFECTDATA& operator=(const CEffekseer::CEffectData* o)
	{
		*(this->m_ppEffect) = (CEffekseer::CEffectData*)o;
	}
	
	bool IsExists() { return(!*m_ppEffect == NULL); }

	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetMove(D3DXVECTOR3 move);
	void SetScale(float fscale);

	D3DXVECTOR3 GetPos(){	
							D3DXVECTOR3 Vec;
							Vec.x = (*m_ppEffect)->m_pos.X;
							Vec.y = (*m_ppEffect)->m_pos.Y;
							Vec.z = (*m_ppEffect)->m_pos.Z;
							return Vec;
						}
	D3DXVECTOR3 GetRot(){
							D3DXVECTOR3 Vec;
							Vec.x = (*m_ppEffect)->m_rot.X;
							Vec.y = (*m_ppEffect)->m_rot.Y;
							Vec.z = (*m_ppEffect)->m_rot.Z;
							return Vec;
						}
	D3DXVECTOR3 GetMove(){
							D3DXVECTOR3 Vec;
							Vec.x = (*m_ppEffect)->m_move.X;
							Vec.y = (*m_ppEffect)->m_move.Y;
							Vec.z = (*m_ppEffect)->m_move.Z;
							return Vec;
						}
	float GetScale() { return (*m_ppEffect)->m_fScale; }
private:
	CEffekseer::CEffectData** m_ppEffect;
};


#endif // ! _EFFEKSEERCONTROL_H_