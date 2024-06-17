#if 1
//============================================================
//
//	エディットアクターヘッダー [editActor.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_ACTOR_H_
#define _EDIT_ACTOR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "actor.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットアクタークラス
class CEditActor : public CEditorObject
{
public:
	// コンストラクタ
	CEditActor(CEditStage* pEditor);

	// デストラクタ
	~CEditActor() override;

	// アクター情報構造体
	struct SInfo
	{
		CActor::EType type;	// 種類
		D3DXVECTOR3 scale;	// 大きさ
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
	void UpdatePosition(void) override;		// 位置更新
	void UpdateRotation(void) override;		// 向き更新

	// メンバ関数
	void UpdateScaling(void);	// 大きさ更新
	void ChangeType(void);		// 種類更新
	void CreateActor(void);		// アクター生成
	void ReleaseActor(void);	// アクター破棄
	void DeleteCollisionActor(const bool bRelase);	// アクターの削除判定
	void InitAllColorActor(void);					// アクターの色全初期化

	// メンバ変数
	CActor* m_pActor;	// アクター情報
	SInfo m_infoCreate;	// アクター配置情報
	bool m_bSave;		// 保存状況
};

#endif	// _EDIT_ACTOR_H_
#endif
