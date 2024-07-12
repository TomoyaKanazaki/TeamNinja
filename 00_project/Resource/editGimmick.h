#if 1
//============================================================
//
//	エディットギミックヘッダー [editGimmick.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_GIMMICK_H_
#define _EDIT_GIMMICK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "gimmick.h"

//************************************************************
// 前方宣言
//************************************************************
class CObject3D;			// 3Dポリゴン

//************************************************************
//	クラス定義
//************************************************************
// エディットギミッククラス
class CEditGimmick : public CEditorObject
{
public:
	// コンストラクタ
	CEditGimmick(CEditStage* pEditor);

	// デストラクタ
	~CEditGimmick() override;

	// フィールド情報構造体
	struct SInfo
	{
		CGimmick::EType type;	// 種類
		D3DXVECTOR3 size;		// 大きさ
		int nNumActive;			// 発動に可能な人数
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
	void UpdateSize(void);		// 大きさ更新
	void ChangeType(void);		// 種類変更
	void UpdateNumActive(void);	// 発動可能人数更新
	void CreateGimmick(void);	// フィールド生成
	void ReleaseGimmick(void);	// フィールド破棄
	void DeleteCollisionGimmick(const bool bRelase);	// フィールドの削除判定
	void InitAllColorGimmick(void);					// フィールドの色全初期化

	// メンバ変数
	CGimmick* m_pGimmick;		// フィールド情報
	CObject3D* m_pAngleSign;	// 矢印の情報
	SInfo m_infoCreate;	// フィールド配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_GIMMICK_H_
#endif
