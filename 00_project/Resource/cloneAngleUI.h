//============================================================
//
// 分身を出す方向のUIヘッダー [cloneAngleUI.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CLONE_ANGLE_UI_H_
#define _CLONE_ANGLE_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 分身方向UIクラス
class CCloneAngleUI : public CObject3D
{
public:

	// コンストラクタ
	CCloneAngleUI();

	// デストラクタ
	~CCloneAngleUI() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CCloneAngleUI* Create(const D3DXVECTOR3& rPos);	// 生成

private:

};

#endif	// _CLONE_ANGLE_H_
