#pragma once
//=========================================
//
//  デバッグ専用オブジェクト(絶対消す) [debug_object.h]
//  Author：Tomoya Kanazaki
//
//=========================================
#include "object3D.h"
#include "listManager.h"

//=========================================
//  クラス定義
//=========================================
class CDebugObject : public CObject3D
{
public:

	// コンストラクタ
	CDebugObject();

	// デストラクタ
	virtual ~CDebugObject() override;

	// オーバーライド関数
	virtual HRESULT Init() override; // 初期化
	virtual void Uninit() override; // 終了
	virtual void Update(const float fDeltaTime) override; // 更新
	virtual void Draw(CShader* pShader = nullptr) override; // 描画

	bool Hit(const D3DXVECTOR3& rPos);

	// 静的メンバ関数
	static CDebugObject* Create(); // 生成処理
	static CListManager<CDebugObject>* GetList(); // リスト取得

private:

	// メンバ変数
	CListManager<CDebugObject>::AIterator m_iterator; // イテレーター

	// 静的メンバ変数
	static CListManager<CDebugObject>* m_pList; // オブジェクトリスト
};
