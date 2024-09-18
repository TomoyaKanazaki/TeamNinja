//============================================================
//
//	タッチアクターヘッダー [touchActor.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TOUCH_ACTOR_H_
#define _TOUCH_ACTOR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"
#include "listManager.h"

//************************************************************
//	クラス定義
//************************************************************
// タッチアクタークラス
class CTouchActor : public CObjectModel
{
public:

	// 状態
	enum EState
	{
		STATE_NONE = 0,		// 通常状態
		STATE_ACT,			// アクション状態
		STATE_MAX			// この列挙型の総数
	};

	// 種類列挙
	enum EType
	{
		TYPE_CAN = 0,	// 缶
		TYPE_BIRD,		// 鳥
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTouchActor();

	// デストラクタ
	virtual ~CTouchActor() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

	virtual void SetData				// 情報の設定処理
	(
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rScale	// 拡大率
	);

	// メンバ関数
	virtual bool Collision		// 当たり判定
	(
		const D3DXVECTOR3& rPos,
		const D3DXVECTOR3& rPosOld,
		const float fRadius,
		const float fHeight
	) = 0;

	// セット・ゲット関数
	void SetType(const EType type)		{ m_type = type; }		// 種類の設定処理
	EType GetType(void) const			{ return m_type; }		// 種類の取得処理
	void SetState(const EState state)	{ m_state = state; }	// 状態の設定処理
	EState GetState(void) const			{ return m_state; }		// 状態の取得処理

	// 静的メンバ関数
	static CTouchActor* Create	// 生成
	( // 引数
		const EType type,						// 種類
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 拡大率
	);
	static CListManager<CTouchActor>* GetList(void);	// リスト構造の取得処理

private:

	// オーバーライド関数
	virtual void UpdateNone(const float fDeltaTime) = 0;	// 通常状態更新処理
	virtual void UpdateAct(const float fDeltaTime) = 0;		// アクション状態更新処理

	// メンバ変数
	CListManager<CTouchActor>::AIterator m_iterator;	// イテレーター

	EType m_type;	// 種類
	EState m_state;	// 状態

	// 静的メンバ変数
	static CListManager<CTouchActor>* m_pList;			// リスト構造
};

#endif	// _TOUCH_ACTOR_H_
