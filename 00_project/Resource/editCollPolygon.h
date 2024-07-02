#if 1
//============================================================
//
//	エディットコリジョンポリゴンヘッダー [editCollPolygon.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLL_POLYGON_H_
#define _EDIT_COLL_POLYGON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorCollShape.h"
#include "collisionPolygon.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットコリジョンポリゴンクラス
class CEditCollPolygon : public CEditorCollShape
{
public:
	// コンストラクタ
	CEditCollPolygon(const int nIdx);

	// デストラクタ
	~CEditCollPolygon() override;

	// ポリゴン情報構造体
	struct SInfo
	{
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 size;	// サイズ
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
	CCollisionPolygon* GetPolygon(void) const { return m_pPolygon; }	// ポリゴンの取得処理
	SInfo GetInfo(void) const { return m_infoCreate; }					// 情報の取得処理

private:
	// オーバーライド関数
	void UpdateOffset(void) override;		// オフセット座標更新

	// メンバ関数
	void UpdateRot(void);			// 向き更新
	void UpdateSize(void);		// 大きさ更新

	// メンバ変数
	CCollisionPolygon* m_pPolygon;	// ポリゴン情報
	SInfo m_infoCreate;	// ポリゴン配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_COLL_SPHERE_H_
#endif
