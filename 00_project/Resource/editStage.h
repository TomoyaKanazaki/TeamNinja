//============================================================
//
//	エディットステージヘッダー [editStage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_STAGE_H_
#define _EDIT_STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editor.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;		// エディットマネージャークラス
class CEditorObject;	// エディターオブジェクトクラス

//************************************************************
//	定数宣言
//************************************************************
namespace editstage
{
	const float SIZE = 50.0f;	// 一ブロックの大きさ
}

//************************************************************
//	クラス定義
//************************************************************
// エディットステージクラス
class CEditStage : public CEditor
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_FIELD = 0,		// 地面
		TYPE_WALL,			// 壁
		TYPE_ACTOR,			// アクター
		TYPE_CHECKPOINT,	// チェックポイント
		TYPE_GOALPOINT,		// ゴールポイント
		TYPE_GIMMICK,		// ギミック
		TYPE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEditStage(CEditManager *pEditManager);

	// デストラクタ
	virtual ~CEditStage();

	// メンバ関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	HRESULT Save(void) override;	// 保存
	bool IsSave(void) override;		// 保存状況取得
	void SaveInfo(void) override;	// 情報保存
	void LoadInfo(void) override;	// 情報読込
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画

	void UpdatePosition(void);	// 位置更新
	void UpdateRotation(void);	// 向き更新
	void SetVec3Position(const D3DXVECTOR3 pos)	{ m_pos = pos; }		// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const		{ return m_pos; }		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3 rot) { m_rot = rot; useful::NormalizeRot(m_rot); }		// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const		{ return m_rot; }		// 向き取得
	void SetAngle(const EAngle angle)			{ m_angle = angle; }	// 方向設定
	EAngle GetAngle(void) const					{ return m_angle; }		// 方向取得

private:
	// メンバ関数
	void ChangeObjectType(void);	// オブジェクトタイプ変更

	// メンバ変数
	CEditorObject *m_pEditor;	// エディター情報
	CEditStage::EType m_type;	// オブジェクトタイプ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	EAngle m_angle;		// 角度
};

#endif	// _EDIT_STAGE_H_
