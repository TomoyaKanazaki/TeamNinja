#pragma once
//==========================================
//
//  目標に向かって移動するエフェクトのクラス(effect_move.h)
//  Author : Tomoya Kanazaki 
//
//==========================================
#include "object.h"
#include "effekseerControl.h"

//==========================================
//  クラス定義
//==========================================
class CMoveEffect : public CObject
{
public:

	// メンバ関数
	CMoveEffect();
	~CMoveEffect();

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime);
	void Draw(CShader* pShader = nullptr);

	//静的メンバ関数
	static CMoveEffect* Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, const D3DXVECTOR3& posTarget, float fDistanceDelete); // 生成処理
	static CMoveEffect* Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, CObject* pObj, float fDistanceDelete); // 生成処理

private:

	// メンバ関数
	void Release(void) override { CObject::Release(); }	// 破棄
	void CreateEffect(std::string path); // エフェクトの生成
	void Move(const float fDeltaTime); // 移動処理

	// メンバ変数
	D3DXVECTOR3 m_pos; // 自身の座標
	float m_fDeleteDistance; // 削除範囲
	float m_fSpeed; // 移動量
	CObject* m_pTargetObj; // 目標になるオブジェクト
	D3DXVECTOR3 m_posTarget; // 目標地点
	CEffekseer::CEffectData* m_pEffect; // エフェクト情報(見た目)

};
