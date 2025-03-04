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
#include "actor.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditManager;			// エディットマネージャークラス
class CEditorCollShape;		// エディターコリジョンシェイプクラス
class CEditCollCube;		// キューブ当たり判定
class CEditCollCylinder;	// シリンダー当たり判定
class CEditCollSphere;		// スフィア当たり判定
class CEditCollPolygon;		// ポリゴン当たり判定

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
		TYPE_CUBE = 0,		// キューブ
		TYPE_CYLINDER,		// シリンダー
		TYPE_SPHERE,		// スフィア
		TYPE_POLYGON,		// ポリゴン
		TYPE_MAX			// この列挙型の総数
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
	void DispObject(const bool bDisp);		// オブジェクトの描画関係処理
	void ChangeObjectType(void);			// オブジェクトタイプ変更
	void ChangeActorType(void);				// アクタータイプ変更
	void Create(void);						// 生成処理
	void Delete(void);						// 消去処理
	void ManagerSave(void);					// マネージャーのセーブ処理

	void InitAllColorCollCube(void);				// キューブの色全初期化
	void InitAllColorCollCylinder(void);			// シリンダーの色全初期化
	void InitAllColorCollSphere(void);				// スフィアの色全初期化
	void InitAllColorCollPolygon(void);				// ポリゴンの色全初期化
	void DeleteCollCube(const bool bRelease);		// キューブの消去処理
	void DeleteCollCylinder(const bool bRelease);	// シリンダーの消去処理
	void DeleteCollSphere(const bool bRelease);		// スフィアの消去処理
	void DeleteCollPolygon(const bool bRelease);	// ポリゴンの消去処理

	// メンバ変数
	CEditorCollShape* m_pEditor;	// エディター情報
	CActor* m_pActor;				// アクターの情報
	CEditCollision::EType m_type;	// コリジョンタイプ
	CActor::EType m_actorType;		// アクタータイプ
	std::vector<CEditCollCube> m_cube;			// キューブの可変長配列
	std::vector<CEditCollCylinder> m_cylinder;	// シリンダーの可変長配列
	std::vector<CEditCollSphere> m_sphere;		// スフィアの可変長配列
	std::vector<CEditCollPolygon> m_polygon;	// ポリゴンの可変長配列

};

#endif	// _EDIT_STAGE_H_
