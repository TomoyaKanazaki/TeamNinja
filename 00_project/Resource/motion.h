//============================================================
//
//	モーションヘッダー [motion.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//************************************************************
//	定数宣言
//************************************************************
namespace motion
{
	const int MAX_PARTS	= 32;	// パーツの最大数
}

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス
class CObjectChara;	// オブジェクトキャラクタークラス

//************************************************************
//	クラス定義
//************************************************************
// モーションクラス
class CMotion
{
public:
	// コンストラクタ
	CMotion();

	// デストラクタ
	~CMotion();

	// 判定カウント管理構造体
	struct SCollTime
	{
		SCollTime()
		{
			nMin = 0;
			nMax = 0;
		}

		int nMin;	// 攻撃判定の開始カウント
		int nMax;	// 攻撃判定の終了カウント
	};

	// キー情報構造体
	struct SKey
	{
		SKey()
		{
			pos = VEC3_ZERO;
			rot = VEC3_ZERO;
		}

		D3DXVECTOR3 pos;	// モデル位置
		D3DXVECTOR3 rot;	// モデル向き
	};

	// キー管理構造体
	struct SKeyInfo
	{
		SKeyInfo()
		{
			vecKey.clear();
			move = VEC3_ZERO;
			nFrame = 0;
		}

		std::vector<SKey> vecKey;	// キーパーツ情報
		D3DXVECTOR3 move;	// キー移動量
		int nFrame;			// キー再生フレーム数
	};

	// モーション管理構造体
	struct SMotionInfo
	{
		SMotionInfo()
		{
			vecKeyInfo.clear();
			nNumKey = 0;
			nWholeFrame = 0;
			nCancelFrame = 0;
			nComboFrame = 0;
			bLoop = false;
			bWeaponDisp = false;
		}

		std::vector<SKeyInfo> vecKeyInfo;	// キー情報
		SCollTime collLeft;		// 左攻撃判定のカウント
		SCollTime collRight;	// 右攻撃判定のカウント
		int  nNumKey;			// キー総数
		int  nWholeFrame;		// モーション全体フレーム数
		int  nCancelFrame;		// キャンセル可能フレーム
		int  nComboFrame;		// コンボ可能フレーム
		bool bLoop;				// ループ ON/OFF
		bool bWeaponDisp;		// 武器表示 ON/OFF
	};

	// モーション情報構造体
	struct SInfo
	{
		SInfo()
		{
			vecMotionInfo.clear();
			vecOriginKey.clear();
			nNumType		= 0;
			nType			= 0;
			nKey			= 0;
			nKeyCounter		= 0;
			nWholeCounter	= 0;
			bFinish			= false;
		}

		std::vector<SMotionInfo> vecMotionInfo;	// モーション情報
		std::vector<SKey> vecOriginKey;			// キーパーツ原点情報
		int  nNumType;		// モーション種類総数
		int  nType;			// モーション種類
		int  nKey;			// モーションキー番号
		int  nKeyCounter;	// モーションキーカウンター
		int  nWholeCounter;	// モーション全体カウンター
		bool bFinish;		// モーション終了状況
	};

	// ブレンド情報構造体
	struct SBlend
	{
		SBlend()
		{
			vecKey.clear();
			nFrame = 0;
			nWholeCounter = 0;
		}

		std::vector<SKey> vecKey;	// ブレンド開始パーツ情報
		int nFrame;			// ブレンド再生フレーム数
		int nWholeCounter;	// ブレンド全体カウンター
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);		// 更新
	void BindPartsData(CMultiModel **ppModel);	// パーツ情報設定
	void SetAllInfo(const SInfo& rInfo);		// モーション情報全設定
	void AddInfo(const SMotionInfo& rInfo);		// モーション情報追加
	void SetEnableUpdate(const bool bUpdate);	// 更新状況設定
	void SetNumParts(const int nNumParts);		// パーツ数設定
	void Set(const int nType, const int nBlendFrame = 0);				// 設定
	void SetOriginPosition(const D3DXVECTOR3& rPos, const int nParts);	// 原点位置の設定
	void SetOriginRotation(const D3DXVECTOR3& rRot, const int nParts);	// 原点向きの設定

	int  GetType(void) const;					// 種類取得
	int  GetNumType(void) const;				// 種類総数取得
	int  GetKey(void) const;					// キー番号取得
	int  GetNumKey(const int nType) const;		// キー総数取得
	int  GetKeyCounter(void) const;				// モーションキーカウンター取得
	int  GetWholeCounter(void) const;			// モーション全体カウンター取得
	int  GetWholeFrame(const int nType) const;	// モーション全体フレーム数取得
	int  GetCancelFrame(const int nType) const;	// モーションキャンセルフレーム取得
	int  GetComboFrame(const int nType) const;	// モーションコンボフレーム取得
	bool IsFinish(void) const;					// 終了取得
	bool IsLoop(const int nType) const;			// ループ取得
	bool IsCancel(const int nType) const;		// キャンセル取得
	bool IsCombo(const int nType) const;		// コンボ取得
	bool IsWeaponDisp(const int nType) const;	// 武器表示取得
	bool IsLeftWeaponCollision(void);			// 左の攻撃判定フラグ取得
	bool IsRightWeaponCollision(void);			// 右の攻撃判定フラグ取得
	D3DXVECTOR3 GetOriginPosition(const int nParts) const;	// 原点位置の取得
	D3DXVECTOR3 GetOriginRotation(const int nParts) const;	// 原点向きの取得

	// 静的メンバ関数
	static CMotion *Create(CObjectChara *pChara);	// 生成
	static void Release(CMotion *&prMotion);		// 破棄

private:
	// メンバ関数
	void UpdateMove(void);		// 移動更新
	void UpdateMotion(void);	// モーション更新
	void UpdateBlend(void);		// ブレンド更新

	// メンバ変数
	CMultiModel **m_ppModel;	// モデル情報
	CObjectChara *m_pChara;		// オブジェクトキャラクター情報
	SInfo  m_info;		// モーション情報
	SBlend m_blend; 	// ブレンド情報
	int  m_nNumParts;	// パーツ数
	bool m_bUpdate;		// 更新状況
};

#endif	// _MOTION_H_
