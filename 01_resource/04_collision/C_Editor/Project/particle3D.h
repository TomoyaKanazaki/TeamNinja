//============================================================
//
//	パーティクル3Dヘッダー [particle3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PARTICLE3D_H_
#define _PARTICLE3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// パーティクル3Dクラス
class CParticle3D : public CObject
{
public:
	// コンストラクタ
	CParticle3D();

	// デストラクタ
	~CParticle3D() override;

	// 種類列挙
	enum EType
	{
		TYPE_NONE = 0,			// なし
		TYPE_DAMAGE,			// ダメージ
		TYPE_HEAL,				// 回復
		TYPE_FIRE,				// 炎
		TYPE_SMALL_EXPLOSION,	// 小爆発
		TYPE_BIG_EXPLOSION,		// 大爆発
		TYPE_PLAYER_DAMAGE,		// プレイヤーダメージ
		TYPE_MAX				// この列挙型の総数
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetType(const int nType) override;					// 種類設定
	int GetType(void) const override;						// 種類取得
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得

	// 静的メンバ関数
	static CParticle3D *Create	// 生成
	( // 引数
		const EType type,					// 種類
		const D3DXVECTOR3& rPos,			// 位置
		const D3DXCOLOR& rCol = XCOL_WHITE	// 色
	);

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);		// ダメージ
	void Heal(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);			// 回復

	void Fire(const D3DXVECTOR3& rPos);				// 炎
	void SmallExplosion(const D3DXVECTOR3& rPos);	// 小爆発
	void BigExplosion(const D3DXVECTOR3& rPos);		// 大爆発
	void PlayerDamage(const D3DXVECTOR3& rPos);		// プレイヤーダメージ

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXCOLOR m_col;	// 色
	EType m_type;		// 種類
	int  m_nLife;		// 寿命
};

#endif	// _PARTICLE3D_H_
