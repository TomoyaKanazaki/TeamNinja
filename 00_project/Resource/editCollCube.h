#if 1
//============================================================
//
//	エディットコリジョンキューブヘッダー [editCollCube.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLL_CUBE_H_
#define _EDIT_COLL_CUBE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorCollShape.h"
#include "collisionCube.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットコリジョンキューブクラス
class CEditCollCube : public CEditorCollShape
{
public:
	// コンストラクタ
	CEditCollCube(const int nIdx);

	// デストラクタ
	~CEditCollCube() override;

	// キューブ情報構造体
	struct SInfo
	{
		float fWidth;		// 幅
		float fHeight;		// 高さ
		float fDepth;		// 奥行
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
	CCollisionCube* GetCube(void) const { return m_pCube; }		// キューブの取得処理
	SInfo GetInfo(void) const { return m_infoCreate; }			// 情報の取得処理

private:
	// オーバーライド関数
	void UpdateOffset(void) override;		// オフセット座標更新

	// メンバ関数
	void UpdateSize(void);	// 大きさ更新

	// メンバ変数
	CCollisionCube* m_pCube;	// キューブ情報
	SInfo m_infoCreate;	// キューブ配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_COLL_CUBE_H_
#endif
