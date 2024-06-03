#if 1
//============================================================
//
//	エディットコリジョンシリンダーヘッダー [editCollCylinder.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLL_CYLINDER_H_
#define _EDIT_COLL_CYLINDER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorCollShape.h"
#include "collisionCylinder.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットコリジョンシリンダークラス
class CEditCollCylinder : public CEditorCollShape
{
public:
	// コンストラクタ
	CEditCollCylinder(const int nIdx);

	// デストラクタ
	~CEditCollCylinder() override;

	// シリンダー情報構造体
	struct SInfo
	{
		float fRadius;		// 半径
		float fHeight;		// 高さ
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
	void Create(void) override;		// 生成処理

	// セット・ゲット関係
	CCollisionCylinder* GetCylinder(void) const { return m_pCylinder; }		// シリンダーの取得処理

private:
	// オーバーライド関数
	void UpdateOffset(void) override;		// オフセット座標更新

	// メンバ関数
	void UpdateSizing(void);	// 大きさ更新

	// メンバ変数
	CCollisionCylinder* m_pCylinder;	// シリンダー情報
	SInfo m_infoCreate;	// シリンダー配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_COLL_CYLINDER_H_
#endif
