#if 1
//============================================================
//
//	エディットウォールヘッダー [editWall.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_WALL_H_
#define _EDIT_WALL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "wall.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットウォールクラス
class CEditWall : public CEditorObject
{
public:
	// コンストラクタ
	CEditWall(CEditStage* pEditor);

	// デストラクタ
	~CEditWall() override;

	// ウォール情報構造体
	struct SInfo
	{
		CWall::EType type;	// 種類
		D3DXVECTOR2 size;	// 大きさ
		POSGRID2 part;		// 分割数
		POSGRID2 texPart;	// テクスチャ分割数
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	HRESULT Save(void) override;	// 保存
	bool IsSave(void) override;		// 保存状況取得
	void SaveInfo(void) override;	// 情報保存
	void LoadInfo(void) override;	// 情報読込
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画

private:
	// オーバーライド関数
	void UpdateRotation(void) override;	// 向き更新

	// メンバ関数
	void UpdateSizing(void);	// 大きさ更新
	void UpdateTexPart(void);	// テクスチャ分割更新
	void ChangeType(void);		// 種類変更
	void CreateWall(void);		// ウォール生成
	void ReleaseWall(void);		// ウォール破棄
	void DeleteCollisionWall(const bool bRelase);	// ウォールの削除判定
	void InitAllColorWall(void);					// ウォールの色全初期化

	// メンバ変数
	CWall *m_pWall;		// ウォール情報
	SInfo m_infoCreate;	// ウォール配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_WALL_H_
#endif
