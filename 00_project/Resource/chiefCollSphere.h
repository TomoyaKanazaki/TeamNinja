#if 1
//============================================================
//
//	円判定統括ヘッダー [chiefCollSphere.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CHIEF_COLL_SPHERE_H_
#define _CHIEF_COLL_SPHERE_H_

//************************************************************
//	前方宣言
//************************************************************
class CCollisionSphere;	// スフィアの当たり判定クラス

//************************************************************
//	クラス定義
//************************************************************
// 円判定統括クラス
class CChiefCollSphere
{
public:
	// コンストラクタ
	CChiefCollSphere();

	// デストラクタ
	~CChiefCollSphere();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void OffSet(const D3DXMATRIX& rMtx);	// オフセット更新
	D3DXVECTOR3 GetWorldPosition(const int nCollID) const;	// 判定位置取得

#if 0
	void SetInfo(const SInfo& rInfo, const int nID);	// 判定情報設定
	SInfo GetInfo(const int nID) const;					// 判定情報取得
	void SetVector(const std::vector<SInfo>& rVector);	// 配列設定
	std::vector<SInfo> GetVector(void) const;			// 配列取得
#endif

	HRESULT AddColl	// 判定追加
	( // 引数
		const D3DXVECTOR3& rOffset,	// 判定位置オフセット
		const float fRadius			// 判定半径
	);
	void SubColl(const int nCollID);	// 判定削除

	// 静的メンバ関数
	static CChiefCollSphere *Create(void);	// 生成
	static void Release(CChiefCollSphere *&prChiefCollSphere);	// 破棄

private:
	// メンバ変数
	std::vector<CCollisionSphere*> m_vecColl;	// 判定情報配列
};

#endif	// _CHIEF_COLL_SPHERE_H_
#endif
