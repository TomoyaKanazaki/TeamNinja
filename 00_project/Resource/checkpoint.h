//==========================================
//
//  チェックポイント(checkpoint.h)
//  Author : Tomoya Kanazaki
//  Adder：Ritsuki Obara
//
//==========================================
#pragma once
#include "object.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//==========================================
//  クラス定義
//==========================================
class CCheckPoint : public CObject
{
public:

	// メンバ関数
	CCheckPoint();
	~CCheckPoint();

	HRESULT Init() override; // 初期化
	void Uninit() override; // 終了
	void Update(const float fDeltaTime) override; // 更新
	void Draw(CShader* pShader = nullptr) override; // 描画
	float GetRadius(void) const;		// 半径取得処理

	int GetSaveTension() const { return m_nSaveTension; }	// セーブ時の士気力取得
	void SetVec3Position(const D3DXVECTOR3& rPos) override { m_pos = rPos; }	// 位置設定処理
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }			// 位置取得処理

	// 静的メンバ関数
	static CCheckPoint* Create(const D3DXVECTOR3& rPos);	// 生成処理
	static CListManager<CCheckPoint>* GetList(void);		// リスト取得
	static HRESULT LoadSetup(const char* pPass);			// セットアップ

private:

	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ関数
	void CollisionPlayer();	// プレイヤーとの当たり判定

	// メンバ変数
	CListManager<CCheckPoint>::AIterator m_iterator;	// イテレーター
	D3DXVECTOR3 m_pos; // 位置
	bool m_bSave; // セーブフラグ
	int m_nSaveTension; // セーブ時の士気力
	CEffekseer::CEffectData* m_pEffectdata; //保持するエフェクト情報

	// 静的メンバ変数
	static CListManager<CCheckPoint>* m_pList;	// オブジェクトリスト
};
