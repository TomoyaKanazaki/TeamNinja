//#if 1
////============================================================
////
////	エディット攻撃敵ヘッダー [editEnemyAttack.h]
////	Author：小原立暉
////
////============================================================
////************************************************************
////	二重インクルード防止
////************************************************************
//#ifndef _EDIT_ENEMY_ATTACK_H_
//#define _EDIT_ENEMY_ATTACK_H_
//
////************************************************************
////	インクルードファイル
////************************************************************
//#include "editorObject.h"
//#include "enemyAttack.h"
//
////************************************************************
////	クラス定義
////************************************************************
//// エディット攻撃敵クラス
//class CEditEnemyAttack : public CEditorObject
//{
//public:
//
//	// ナビゲーションの種類
//	enum ENavType
//	{
//		NAVTYPE_RANDOM = 0,	// ランダムナビ
//		NAVTYPE_STREET,		// 巡回ナビ
//		NAVTYPE_MAX			// この列挙型の総数
//	};
//
//	// コンストラクタ
//	CEditEnemyAttack(CEditStage* pEditor);
//
//	// デストラクタ
//	~CEditEnemyAttack() override;
//
//	// 敵情報構造体
//	struct SInfo
//	{
//		CEnemyAttack::EType type;	// 種類
//		float fMoveWidth;			// 移動範囲幅
//		float fMoveDepth;			// 移動範囲奥行
//		float fChaseWidth;			// 追跡範囲幅
//		float fChaseDepth;			// 追跡範囲奥行
//	};
//
//	// オーバーライド関数
//	HRESULT Init(void) override;	// 初期化
//	void Uninit(void) override;		// 終了
//	void Update(void) override;		// 更新
//	HRESULT Save(void) override;	// 保存
//	bool IsSave(void) override;		// 保存状況取得
//	void SaveInfo(void) override;	// 情報保存
//	void LoadInfo(void) override;	// 情報読込
//	void DrawDebugControl(void) override;	// 操作表示描画
//	void DrawDebugInfo(void) override;		// 情報表示描画
//
//private:
//
//	// メンバ関数
//	void ChangeType(void);		// 種類更新
//	void CreateActor(void);		// 敵生成
//	void ReleaseActor(void);	// 敵破棄
//	void DeleteCollisionActor(const bool bRelase);	// 敵の削除判定
//	void InitAllColorActor(void);					// 敵の色全初期化
//
//	// メンバ変数
//	CEnemyAttack* m_pEnemy;	// 敵の情報
//	SInfo m_infoCreate;		// 敵配置情報
//	bool m_bSave;			// 保存状況
//};
//
//#endif	// _EDIT_ACTOR_H_
//#endif
