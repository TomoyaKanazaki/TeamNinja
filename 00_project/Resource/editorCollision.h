//============================================================
//
//	エディットコリジョンヘッダー [editCollision.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_COLLISION_H_
#define _EDIT_COLLISION_H_

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
//	クラス定義
//************************************************************
// エディットステージクラス
class CEditCollision : public CEditor
{
public:

	// 種類列挙
	enum EType
	{
		TYPE_CUBE = 0,	// キューブ
		TYPE_CYLINDER,	// 円柱
		TYPE_SPHERE,	// 球
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEditCollision(CEditManager* pEditManager);

	// デストラクタ
	virtual ~CEditCollision();

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

private:
	// メンバ関数
	void ChangeObjectType(void);	// オブジェクトタイプ変更

	// メンバ変数
	CEditorObject* m_pEditor;		// エディター情報
	CEditCollision::EType m_type;	// オブジェクトタイプ
};

#endif	// _EDIT_COLLISION_H_
