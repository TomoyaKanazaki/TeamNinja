//============================================================
//
//	プレイヤーの回避エフェクトヘッダー [player_dodge.h]
//	Author：小原立暉
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_DODGE_H_
#define _PLAYER_DODGE_H_

//************************************************************
// インクルードファイル
//************************************************************
#include "effekseerControl.h"

//************************************************************
//	クラス定義
//************************************************************
// プレイヤーの回避エフェクトクラス
class CPlayerDodge
{
public:
	// コンストラクタ
	CPlayerDodge();

	// デストラクタ
	~CPlayerDodge();

	// オーバーライド関数
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(const D3DXVECTOR3& rPos);	// 更新

	// 登録処理
	void Regist(CEffekseer::CEffectData* pEffect);

	// 静的メンバ関数
	static CPlayerDodge* Create(void);		// 生成処理

private:

	// メンバ変数
	std::vector<CEffekseer::CEffectData*> m_effectData;	// エフェクト情報
};

#endif	// _PLAYER_TITLE_H_
