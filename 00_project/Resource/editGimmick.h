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
//	クラス定義
//************************************************************
// エディットギミッククラス
class CEditGimmick : public CEditorObject
{
public:
	// コンストラクタ
	CEditGimmick();

	// デストラクタ
	~CEditGimmick() override;

	// ギミック情報構造体
	struct SInfo
	{
		CGimmick::EType type;	// 種類
		D3DXVECTOR3 size;		// 大きさ
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
	void UpdateRotation(void) override;		// 向き更新

	// メンバ関数
	void UpdateSize(void);		// 大きさ更新
	void ChangeType(void);		// 種類更新
	void CreateGimmick(void);		// ギミック生成
	void ReleaseGimmick(void);	// ギミック破棄
	void DeleteCollisionGimmick(const bool bRelase);	// ギミックの削除判定
	void InitAllColorGimmick(void);					// ギミックの色全初期化

	// メンバ変数
	CGimmick* m_pGimmick;	// ギミック情報
	SInfo m_infoCreate;		// ギミック配置情報
	bool m_bSave;			// 保存状況
};

#endif	// _EDIT_ACTOR_H_
#endif
