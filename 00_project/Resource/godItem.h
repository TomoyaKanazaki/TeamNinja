//============================================================
//
//	神器ヘッダー [godItem.h]
//	Author：小原立暉
//	Adder ：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GODITEM_H_
#define _GODITEM_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	クラス定義
//************************************************************
// 神器クラス
class CGodItem : public CObjectModel
{
public:

	// 種類
	enum EType
	{
		TYPE_RED = 0,	// 赤
		TYPE_GREEN,		// 緑
		TYPE_BLUE,		// 青
		TYPE_MAX		// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_ITEM = 0,	// アイテム状態
		STATE_ROLL,		// 回転移動状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CGodItem();

	// デストラクタ
	~CGodItem() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	bool Collision		// 当たり判定処理
	(
		const D3DXVECTOR3& rPos,	// 位置
		const float fRadius			// 半径
	);

	// セット・ゲット関係
	float GetInitPosY(void) const;		// 初期位置(Y軸)の取得処理
	void SetType(const EType type);		// 種類の設定処理
	EType GetType(void) const;			// 種類の取得処理

	// 静的メンバ関数
	static CGodItem* Create(const D3DXVECTOR3& rPos, const EType type);		// 生成処理
	static CListManager<CGodItem>* GetList(void);	// リスト構造の取得処理
	static void SetRollPosition(void);				// 回転移動状態の目標位置設定
	static bool IsGet(const EType type);			// 取得状況取得
	static void InitGet(void);						// 取得状況初期化
	static void Delete(const EType type);			// 削除
	static HRESULT LoadSetup(const char* pPass);	// セットアップ
	static HRESULT LoadPossess(const char* pPass, bool* pGet);	// 獲得状況読込
	static HRESULT SavePossess(const char* pPass, bool* pGet);	// 獲得状況保存

private:

	// メンバ関数
	void Cycle(void);		// 向き処理
	void Height(void);		// 高さ設定処理
	void UpdateRoll(const float fDeltaTime);	// 回転移動の更新

	// メンバ変数
	CListManager<CGodItem>::AIterator m_iterator;	// イテレーター
	CEffekseer::CEffectData* m_pEffectBody;			// 本体エフェクト情報
	CEffekseer::CEffectData* m_pEffectLand;			// 着地エフェクト情報
	float m_fPosInitY;	// 初期位置(Y軸)
	EType m_type;		// 種類
	EState m_state;		// 状態
	float m_fHeightRot;	// 高さの向き

	// 静的メンバ関数
	static bool DuplicationCheck(const EType type);	// 重複チェック処理

	// 静的メンバ変数
	static CListManager<CGodItem>* m_pList;	// リスト構造
	static bool m_aGet[TYPE_MAX];			// 取得状況
};

#endif	// _COIN_H_
