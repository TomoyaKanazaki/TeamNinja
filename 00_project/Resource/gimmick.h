//============================================================
//
// ギミックヘッダー [gimmick.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"
#include "listManager.h"
#include "player_clone.h"

//************************************************************
//	クラス定義
//************************************************************
// ギミックの親クラス
class CGimmick : public CObject3D
{
public:

	// 種類
	enum EType
	{
		TYPE_JUMPTABLE = 0,		// ジャンプ台
		TYPE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CGimmick();

	// デストラクタ
	virtual ~CGimmick() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

	// セット・ゲット関係
	void SetType(const EType type) { m_type = type; }		// 種類の設定処理
	EType GetType(void) const { return m_type; }			// 種類の取得処理

	// 静的メンバ関数
	static CGimmick* Create(const D3DXVECTOR3& rPos, const EType type);	// 生成
	static CListManager<CGimmick>* GetList(void);	// リスト取得

private:

	// メンバ変数
	EType m_type;		// 種類
	CListManager<CGimmick>::AIterator m_iterator;	// イテレーター

	// 静的メンバ変数
	static CListManager<CGimmick>* m_pList;			// オブジェクトリスト
};

#endif	// _GIMMICK_H_
