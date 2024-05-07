//============================================================
//
//	プレイヤーの分身ヘッダー [player_avatar.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_AVATAR_H_
#define _PLAYER_AVATAR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"

//************************************************************
//	前方宣言
//************************************************************
class CShadow;	// 影クラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤーの分身クラス
class CPlayerAvatar : public CObjectChara
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_WAIST = 0,	// 腰
		MODEL_BODY,			// 体
		MODEL_HEAD,			// 頭
		MODEL_ARMUL,		// 左上腕
		MODEL_ARMUR,		// 右上腕
		MODEL_ARMDL,		// 左下腕
		MODEL_ARMDR,		// 右下腕
		MODEL_HANDL,		// 左手
		MODEL_HANDR,		// 右手
		MODEL_LEGUL,		// 左太もも
		MODEL_LEGUR,		// 右太もも
		MODEL_LEGDL,		// 左脛
		MODEL_LEGDR,		// 右脛
		MODEL_FOOTL,		// 左足
		MODEL_FOOTR,		// 右足
		MODEL_SWORDL,		// 左剣
		MODEL_SWORDR,		// 右剣
		MODEL_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPlayerAvatar();

	// デストラクタ
	~CPlayerAvatar() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定

	// 静的メンバ関数
	static CPlayerAvatar* Create(void);					// 生成
	static void Delete(const int nNum);					// 消去処理
	static CListManager<CPlayerAvatar>* GetList(void);	// リスト取得

private:
	// メンバ関数
	void LoadSetup(void);			// セットアップ
	EMotion UpdateNormal(void);		// 通常状態時の更新

	void UpdateControl(void);		// 操作処理
	void UpdateMotion(int nMotion, const float fDeltaTime);	// モーション・オブジェクトキャラクターの更新
	bool UpdateFadeOut(const float fAdd);				// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);				// フェードイン状態時の更新

	// 静的メンバ変数
	static CListManager<CPlayerAvatar>* m_pList;		// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayerAvatar>::AIterator m_iterator;	// イテレーター
	CShadow* m_pShadow;			// 影の情報
};

#endif	// _PLAYER_H_
