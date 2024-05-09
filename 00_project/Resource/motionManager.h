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

	// パーツ情報構造体
	struct SParts
	{
		SParts()
		{
			strPass.clear();
			pos = VEC3_ZERO;
			rot = VEC3_ZERO;
			nParentID = NONE_IDX;
		}

		std::string strPass;	// モデルパス
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		int nParentID;			// 親インデックス
	};

	// パーツ構造体
	struct SPartsInfo
	{
		SPartsInfo()
		{
			pInfo = nullptr;
			nNumParts = 0;
		}

		SParts *pInfo;	// パーツ情報
		int nNumParts;	// パーツ数
	};

	// キャラクター情報構造体
	struct SCharaData
	{
		SCharaData()
		{

		}

		// メンバ変数
		CMotion::SInfo infoMotion;	// モーション情報
		SPartsInfo infoParts;		// パーツ情報
	};

	// メンバ関数
	HRESULT Init(void);	// モーションマネージャー初期化
	void Uninit(void);	// モーションマネージャー終了
	SCharaData Regist(const char *pTextPass);	// キャラクター情報登録

	// 静的メンバ関数
	static CMotionManager *Create(void);	// 生成
	static void Release(CMotionManager *&prMotionManager);	// 破棄

private:
	// 静的メンバ関数
	static HRESULT LoadSetup(SCharaData *pInfoChara, const char *pTextPass);	// セットアップ

	// メンバ変数
	std::map<std::string, SCharaData> m_mapMotion;	// モーション連想配列
};

#endif	// _MOTION_MANAGER_H_
