//============================================================
//
//	モーションマネージャーヘッダー [motionManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "motion.h"

//************************************************************
//	クラス定義
//************************************************************
// モーションマネージャークラス
class CMotionManager
{
public:
	// コンストラクタ
	CMotionManager();

	// デストラクタ
	~CMotionManager();

	// キー構造体
	struct SKey
	{
		// コンストラクタ
		SKey(const char *pTextPass, const char **ppModelPassArray) :
		pTextPass(pTextPass), ppModelPassArray(ppModelPassArray) {}

		// 比較演算子オーバーロード
		bool operator<(const SKey& rKey) const
		{
			// 情報をまとめて比較
			return std::tie(pTextPass, ppModelPassArray) < std::tie(rKey.pTextPass, rKey.ppModelPassArray);
		}

		// メンバ変数
		const char *pTextPass;			// テキストパス
		const char **ppModelPassArray;	// モデル配列ポインタ
	};

	// パーツ情報構造体
	struct SParts
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		int nParentID;		// 親インデックス
	};

	// パーツ構造体
	struct SPartsInfo
	{
		SParts aInfo[motion::MAX_PARTS];	// パーツ情報
		int nNumParts;	// パーツ数
	};

	// キャラクター情報構造体
	struct SCharaData
	{
		CMotion::SInfo infoMotion;	// モーション情報
		SPartsInfo infoParts;		// パーツ情報
	};

	// メンバ関数
	HRESULT Init(void);	// モーションマネージャー初期化
	void Uninit(void);	// モーションマネージャー終了
	SCharaData Regist	// キャラクター情報登録
	( // 引数
		const char *pTextPass,			// テキストパス
		const char **ppModelPassArray	// モデルパス配列
	);

	// 静的メンバ関数
	static CMotionManager *Create(void);	// 生成
	static void Release(CMotionManager *&prMotionManager);	// 破棄

private:
	// 静的メンバ関数
	static void LoadSetup(SCharaData *pInfoChara, const char *pTextPass, const char **ppModelPassArray);	// セットアップ

	// メンバ変数
	std::map<SKey, SCharaData> m_mapMotion;	// モーション連想配列
};

#endif	// _MOTION_MANAGER_H_
