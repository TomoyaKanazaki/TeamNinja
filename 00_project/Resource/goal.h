//==========================================
//
//  ゴール(goal.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "objectModel.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//==========================================
//  クラス定義
//==========================================
class CGoal : public CObjectModel
{
public:

	// メンバ関数
	CGoal();
	~CGoal();

	HRESULT Init() override; // 初期化
	void Uninit() override; // 終了
	void Update(const float fDeltaTime) override; // 更新
	void Draw(CShader* pShader = nullptr) override; // 描画

	bool GetClear() const { return m_bClear; }// クリア状態の取得

	// 静的メンバ関数
	static CGoal* Create(const D3DXVECTOR3& rPos); // 生成処理
	static CGoal* GetGoal(void);					// リスト取得
	static HRESULT LoadSetup(const char* pPass);	// セットアップ

private:

	// メンバ関数
	void CollisionPlayer();	// プレイヤーとの当たり判定

	// メンバ変数
	CListManager<CGoal>::AIterator m_iterator;	// イテレーター
	bool m_bClear; // クリアフラグ
	CEffekseer::CEffectData* m_pEffectdata; //保持するエフェクト情報

	// 静的メンバ変数
	static CListManager<CGoal>* m_pList;	// オブジェクトリスト
};
