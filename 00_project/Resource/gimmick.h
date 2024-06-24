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

//************************************************************
// 前方宣言
//************************************************************
class CPlayerClone;		// プレイヤーの分身

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
		TYPE_STEP,				// 踏み台
		TYPE_POST,				// 設置
		TYPE_JUMPOFF,			// 飛び降り
		TYPE_HEAVYDOOR,			// 重い扉
		TYPE_BRIDGE,			// 橋
		TYPE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CGimmick();

	// デストラクタ
	virtual ~CGimmick() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;		// 更新
	virtual void Draw(CShader* pShader = nullptr) override;		// 描画

	// セット・ゲット関係
	void SetType(const EType type) { m_type = type; }			// 種類の設定処理
	EType GetType(void) const { return m_type; }				// 種類の取得処理
	void SetNumActive(const int nNum) { m_nNumActive = nNum; }	// 発動可能人数の設定処理
	int GetNumActive(void) const { return m_nNumActive; }		// 発動可能人数の取得処理
	virtual bool IsSet() { return bool(); }						// 設置ギミックのフラグ取得用

	// 当たり判定
	virtual void CollisionClone() {}							// クローンとの当たり判定
	virtual bool CollisionPlayer() { return false; }			// プレイヤーとの当たり判定

	// 静的メンバ関数
	static CGimmick* Create				// 生成
	(
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rSize,		// サイズ
		const EType type,				// 種類
		const int nNumActive			// 発動可能な分身の数
	);
	static CListManager<CGimmick>* GetList(void);	// リスト取得

private:

	// メンバ変数
	CListManager<CGimmick>::AIterator m_iterator;	// イテレーター
	EType m_type;		// 種類
	int m_nNumActive;	// 発動可能な数

	// 静的メンバ変数
	static CListManager<CGimmick>* m_pList;			// オブジェクトリスト
};

#endif	// _GIMMICK_H_
