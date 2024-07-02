#if 1
//============================================================
//
//	エディットコリジョンスフィアヘッダー [editCollSphere.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLL_SPHERE_H_
#define _EDIT_COLL_SPHERE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorCollShape.h"
#include "collisionSphere.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットコリジョンスフィアクラス
class CEditCollSphere : public CEditorCollShape
{
public:
	// コンストラクタ
	CEditCollSphere(const int nIdx);

	// デストラクタ
	~CEditCollSphere() override;

	// スフィア情報構造体
	struct SInfo
	{
		float fRadius;		// 半径
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	bool IsSave(void) override;		// 保存状況取得
	void SaveInfo(void) override;	// 情報保存
	void LoadInfo(void) override;	// 情報読込
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画
	void Create(void) override;		//生成処理

	// セット・ゲット関係
	CCollisionSphere* GetSphere(void) const { return m_pSphere; }		// スフィアの取得処理
	SInfo GetInfo(void) const { return m_infoCreate; }					// 情報の取得処理

private:
	// オーバーライド関数
	void UpdateOffset(void) override;		// オフセット座標更新

	// メンバ関数
	void UpdateSize(void);		// 大きさ更新

	// メンバ変数
	CCollisionSphere* m_pSphere;	// スフィア情報
	SInfo m_infoCreate;	// スフィア配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_COLL_SPHERE_H_
#endif
