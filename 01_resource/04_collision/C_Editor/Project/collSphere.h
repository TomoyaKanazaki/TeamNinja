//============================================================
//
//	円判定ヘッダー [collSphere.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLL_SPHERE_H_
#define _COLL_SPHERE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include <vector>

//************************************************************
//	前方宣言
//************************************************************
class CObject;	// オブジェクトクラス

#if _DEBUG
class CDebugCollSphere;	// 球体判定デバッグ表示クラス
#endif	// _DEBUG

//************************************************************
//	クラス定義
//************************************************************
// 円判定クラス
class CCollSphere
{
public:
	// コンストラクタ
	CCollSphere();

	// デストラクタ
	~CCollSphere();

	// 判定情報構造体
	struct SInfo
	{
		D3DXVECTOR3 offset;	// 判定位置オフセット
		float fRadius;		// 判定半径

#if _DEBUG
		CDebugCollSphere *pVisual;	// 判定表示
#endif	// _DEBUG
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetInfo(const SInfo& rInfo, const int nID);	// 判定情報設定
	SInfo GetInfo(const int nID) const;			// 判定情報取得
	std::vector<SInfo> GetVector(void) const;	// 配列取得

	void AddColl	// 判定追加
	( // 引数
		const D3DXVECTOR3& rOffset,	// 判定位置オフセット
		const float fRadius			// 判定半径
	);
	void SubColl(const int nID);	// 判定削除

	// 静的メンバ関数
	static CCollSphere *Create(CObject *pParent);		// 生成
	static void Release(CCollSphere *&prCollSphere);	// 破棄

private:
	// メンバ変数
	CObject *m_pParent;	// 親オブジェクト
	std::vector<SInfo> m_coll;	// 判定情報
};

#endif	// _COLL_SPHERE_H_
