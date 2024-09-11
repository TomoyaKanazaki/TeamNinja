#pragma once
//===========================================
//
//  チュートリアルクラス(tutorial.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectBillboard.h"

//===========================================
//  クラス定義
//===========================================
class CTutorial : public CObjectBillboard
{
public:

	enum EType
	{
		TYPE_MOVE = 0, // 歩行
		TYPE_CLONE, // 分身
		TYPE_GIMMICK, // ギミック
		TYPE_MAX, // 最大数
	};

	// メンバ関数
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(void) override; // 初期化処理
	virtual void Uninit(void) override; // 終了処理
	virtual void Update(const float fDeltaTime) override; // 更新処理
	virtual void Draw(CShader* pShader = nullptr) override; // 描画処理
	void BindTexture(const char* pTexturePass) override;	// テクスチャ割当 (パス)

	// 静的メンバ関数
	static CTutorial* Create(const D3DXVECTOR3& rPos, const EType type);
	static CListManager<CTutorial>* GetList(void); // リスト取得

private:

	// 静的メンバ変数
	static CListManager<CTutorial>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CTutorial>::AIterator m_iterator; // イテレーター
	float m_fGapRate; // 揺れの範囲

};
