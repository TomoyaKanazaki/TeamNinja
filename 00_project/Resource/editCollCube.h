#if 1
//============================================================
//
//	エディットコリジョンキューブヘッダー [editCollisionCube.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLLISION_CUBE_H_
#define _EDIT_COLLISION_CUBE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editor.h"
#include "editorObject.h"
#include "collisionCube.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットキューブクラス
class CEditCollisionCube : public CEditorObject
{
public:

	// キューブ情報構造体
	struct SInfo
	{
		CEditor::EAngle angle;	// 向きの種類
		D3DXVECTOR3 size;		// 大きさ
		float fLength;			// 長さ
		float fAngle;			// 方向
	};

	// コンストラクタ
	CEditCollisionCube();

	// デストラクタ
	~CEditCollisionCube() override;

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
	void UpdateSizing(void);	// 大きさ更新
	void CreateCube(void);		// キューブ生成
	void ReleaseCube(void);		// キューブ破棄
	void DeleteCollisionCube(const bool bRelase);	// キューブの削除判定
	void InitAllColorCube(void);					// キューブの色全初期化

	// メンバ変数
	CCollisionCube* m_pCube;	// キューブ情報
	SInfo m_infoCreate;			// キューブ配置情報
	bool m_bSave;				// 保存状況
};

#endif	// _EDIT_FIELD_H_
#endif
