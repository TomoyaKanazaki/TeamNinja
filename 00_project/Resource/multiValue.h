//============================================================
//
//	マルチ数字ヘッダー [multiValue.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MULTIVALUE_H_
#define _MULTIVALUE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "value.h"

//************************************************************
//	クラス定義
//************************************************************
// マルチ数字クラス
class CMultiValue : public CObject
{
public:
	// コンストラクタ
	CMultiValue();

	// デストラクタ
	~CMultiValue() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// 大きさ取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得
	void SetPriority(const int nPriority) override;			// 優先順位設定

	// 静的メンバ関数
	static CMultiValue *Create	// 生成
	( // 引数
		const CValue::EType type,	// 数字種類
		const int nNum,				// 数字
		const int nDigit,			// 桁数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize,	// 大きさ
		const D3DXVECTOR3& rSpace,	// 列間
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void AddNum(const int nNum);		// 数値加算
	void SetNum(const int nNum);		// 数値設定
	HRESULT SetDigit(const int nDigit);	// 桁数設定
	void SetMin(const int nMin);		// 最小値設定
	void SetMax(const int nMax);		// 最大値設定
	int GetNum(void) const;				// 数値取得
	int GetDigit(void) const;			// 桁数取得
	int GetMin(void) const;				// 最小値取得
	int GetMax(void) const;				// 最大値取得
	void SetSpace(const D3DXVECTOR3& rSpace);	// 列間設定
	D3DXVECTOR3 GetSpace(void) const;			// 列間取得
	void SetType(const CValue::EType type);		// 種類設定

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定
	void SetTexNum(void);	// 数字のテクスチャ座標設定

	// メンバ変数
	std::list<CValue*> m_listValue;	// 数字リスト
	D3DXVECTOR3 m_pos;		// 原点位置
	D3DXVECTOR3 m_rot;		// 原点向き
	D3DXVECTOR3 m_size;		// 大きさ
	D3DXVECTOR3 m_space;	// 列間
	D3DXCOLOR m_col;		// 色
	int m_nNum;	// 数字
	int m_nMin;	// 最小値
	int m_nMax;	// 最大値
};

#endif	// _MULTIVALUE_H_
