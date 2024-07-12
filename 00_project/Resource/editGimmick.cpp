#if 1
//============================================================
//
//	エディットギミック処理 [editGimmick.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editGimmick.h"
#include "editManager.h"
#include "manager.h"
#include "stage.h"

#include "collision.h"
#include "object3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE				(DIK_0)	// 生成キー
#define NAME_CREATE				("0")	// 生成表示
#define KEY_RELEASE				(DIK_9)	// 破棄キー
#define NAME_RELEASE			("9")	// 破棄表示
#define KEY_TYPE				(DIK_4)	// 種類変更キー
#define NAME_TYPE				("4")	// 種類変更表示
#define KEY_UP_NUM_ACTIVE		(DIK_5)	// 発動可能人数の増加キー
#define NAME_UP_NUM_ACTIVE		("5")	// 発動可能人数の増加表示
#define KEY_DOWN_NUM_ACTIVE		(DIK_6)	// 発動可能人数の減少キー
#define NAME_DOWN_NUM_ACTIVE	("6")	// 発動可能人数の減少表示

#define KEY_UP_SIZE_X		(DIK_T)	// X軸拡大キー
#define NAME_UP_SIZE_X		("T")	// X軸拡大表示
#define KEY_DOWN_SIZE_X		(DIK_G)	// X軸縮小キー
#define NAME_DOWN_SIZE_X	("G")	// X軸縮小表示
#define KEY_UP_SIZE_Y		(DIK_Y)	// Y軸拡大キー
#define NAME_UP_SIZE_Y		("Y")	// Y軸拡大表示
#define KEY_DOWN_SIZE_Y		(DIK_H)	// Y軸縮小キー
#define NAME_DOWN_SIZE_Y	("H")	// Y軸縮小表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_gimmick.txt";	// セーブテキストパス
	const char* ANGLE_TEXTURE = "data\\TEXTURE\\EDITOR\\Angle.png";	// 方向のテクスチャ

	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(editstage::SIZE, 0.0f, editstage::SIZE);	// 大きさ
	const float	INIT_ALPHA = 0.5f;		// 配置前のα値
	const int DIGIT_FLOAT = 2;			// 小数点以下の桁数
	const D3DXVECTOR3 ANGLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);	// 矢印のサイズ
	const D3DXVECTOR3 ANGLE_SHIFT = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// 矢印の高さ

	const char* TYPE_NAME[] =	// オブジェクトタイプ名
	{
		"ジャンプ台",
		"踏み台",
		"設置",
		"飛び降り",
		"重い扉",
		"橋",
		"吹っ飛ばすやつ",
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CGimmick::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	親クラス [CEditGimmick] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditGimmick::CEditGimmick(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pGimmick = nullptr;	// ギミック情報
	m_pAngleSign = nullptr;	// 矢印の情報
	m_bSave = false;		// 保存状況
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// ギミック配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditGimmick::~CEditGimmick()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditGimmick::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pGimmick = nullptr;					// ギミック
	m_infoCreate.type = CGimmick::EType(0);	// 種類
	m_infoCreate.size = INIT_SIZE;			// 大きさ
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ギミックの生成
	D3DXCOLOR colGimmick = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 地面色
	m_pGimmick = CGimmick::Create
	( // 引数
		GetVec3Position(),
		GetAngle(),
		m_infoCreate.size,
		m_infoCreate.type,
		m_infoCreate.nNumActive
	);
	if (m_pGimmick == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 方向を生成
	m_pAngleSign = CObject3D::Create
	(
		GetVec3Position() + ANGLE_SHIFT,
		ANGLE_SIZE,
		GetVec3Rotation()
	);
	if (m_pAngleSign == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャの割り当て処理
	m_pAngleSign->BindTexture(ANGLE_TEXTURE);

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditGimmick::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// ギミックの色の全初期化
	InitAllColorGimmick();

	// ギミックの終了
	SAFE_UNINIT(m_pGimmick);

	// 方向の終了
	SAFE_UNINIT(m_pAngleSign);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditGimmick::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// 大きさの更新
	UpdateSize();

	// 種類の変更
	ChangeType();

	// 発動可能人数の更新
	UpdateNumActive();

	// ギミックの生成
	CreateGimmick();

	// ギミックの破棄
	ReleaseGimmick();

	// 位置を反映
	m_pGimmick->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pGimmick->SetVec3Rotation(GetVec3Rotation());

	// 矢印の位置を反映
	m_pAngleSign->SetVec3Position(GetVec3Position() + ANGLE_SHIFT);

	// 矢印の向きを反映
	m_pAngleSign->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditGimmick::IsSave(void)
{
#if _DEBUG

	// 保存状況を返す
	return m_bSave;

#else	// NDEBUG

	// falseを返す
	return false;

#endif	// _DEBUG
}

//============================================================
//	情報保存処理
//============================================================
void CEditGimmick::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditGimmick::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditGimmick::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "大きさ：[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "発動可能人数：[%s/%s]\n", NAME_UP_NUM_ACTIVE, NAME_DOWN_NUM_ACTIVE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditGimmick::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[種類]\n", TYPE_NAME[m_infoCreate.type]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f：[大きさ]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[発動可能人数]\n", m_infoCreate.nNumActive);

#endif	// _DEBUG
}

//============================================================
//	向き更新処理
//============================================================
void CEditGimmick::UpdateRotation(void)
{
	// 向きの更新
	CEditorObject::UpdateRotation();

	// 向きを反映
	m_pGimmick->SetVec3Rotation(GetVec3Rotation());
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditGimmick::UpdateSize(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 大きさを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE_X))
		{
			m_infoCreate.size.x += INIT_SIZE.x;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_X))
		{
			m_infoCreate.size.x -= INIT_SIZE.x;
		}
		if (pKeyboard->IsPress(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_X))
		{
			m_infoCreate.size.x += INIT_SIZE.x;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_X))
		{
			m_infoCreate.size.x -= INIT_SIZE.x;
		}
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.z, INIT_SIZE.z);

	// 大きさを反映
	m_pGimmick->SetVec3Sizing(m_infoCreate.size);
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditGimmick::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CGimmick::EType)((m_infoCreate.type + 1) % CGimmick::TYPE_MAX);

		// 終了処理
		m_pGimmick->Uninit();

		// ギミックを再生成する
		m_pGimmick = CGimmick::Create
		( // 引数
			GetVec3Position(),			// 位置
			GetAngle(),					// 方向
			m_infoCreate.size,			// サイズ
			m_infoCreate.type,			// 種類
			m_infoCreate.nNumActive		// 発動可能人数
		);
	}
}

//============================================================
// 発動可能人数更新
//============================================================
void CEditGimmick::UpdateNumActive(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 発動可能人数を変更
	if (pKeyboard->IsTrigger(KEY_UP_NUM_ACTIVE))
	{
		m_infoCreate.nNumActive++;
	}
	if (pKeyboard->IsTrigger(KEY_DOWN_NUM_ACTIVE))
	{
		m_infoCreate.nNumActive--;
	}

	// 発動可能人数を反映
	m_pGimmick->SetNumActive(m_infoCreate.nNumActive);
}

//============================================================
//	ギミックの生成処理
//============================================================
void CEditGimmick::CreateGimmick(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	D3DXCOLOR colGimmick = XCOL_WHITE;			// 色保存用

	// ギミックを配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	ギミックの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pGimmick->SetEnableUpdate(true);
		m_pGimmick->SetEnableDraw(true);

		// 色を設定
		colGimmick = m_pGimmick->GetColor();	// 元の色を取得
		m_pGimmick->SetColor(D3DXCOLOR(colGimmick.r, colGimmick.g, colGimmick.b, 1.0f));

		// 未保存を設定
		m_bSave = false;

		//----------------------------------------------------
		//	新しいギミックの生成
		//----------------------------------------------------
		// ギミックの生成
		m_pGimmick = CGimmick::Create
		( // 引数
			GetVec3Position(),			// 位置
			GetAngle(),					// 方向
			m_infoCreate.size,			// サイズ
			m_infoCreate.type,			// 種類
			m_infoCreate.nNumActive		// 発動可能人数
		);
		assert(m_pGimmick != nullptr);

		// 色を設定
		colGimmick = m_pGimmick->GetColor();	// 元の色を取得
		m_pGimmick->SetColor(D3DXCOLOR(colGimmick.r, colGimmick.g, colGimmick.b, INIT_ALPHA));
	}
}

//============================================================
//	ギミックの破棄処理
//============================================================
void CEditGimmick::ReleaseGimmick(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// ギミックを削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// ギミックの削除判定
	DeleteCollisionGimmick(bRelease);
}

//============================================================
//	ギミックの削除判定
//============================================================
void CEditGimmick::DeleteCollisionGimmick(const bool bRelase)
{
	CListManager<CGimmick>* pListManager = CGimmick::GetList();	// ギミックリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CGimmick*> listGimmick = pListManager->GetList();	// ギミックリスト情報

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	for (auto& rList : listGimmick)
	{ // ギミック数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pGimmick) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		D3DXVECTOR3 sizeThisGimmick = m_pGimmick->GetVec3Sizing();	// 自身の地面の大きさ
		sizeThis.x = sizeThisGimmick.x;	// 判定サイズXを設定
		sizeThis.y = editstage::SIZE;	// 判定サイズYを設定
		sizeThis.z = sizeThisGimmick.y;	// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		D3DXVECTOR3 sizeOtherGimmick = rList->GetVec3Sizing();	// 対象の地面の大きさ
		sizeOther.x = sizeOtherGimmick.x;	// 判定サイズXを設定
		sizeOther.y = editstage::SIZE;	// 判定サイズYを設定
		sizeOther.z = sizeOtherGimmick.y;	// 判定サイズZを設定
		sizeOther *= 0.5f;				// 判定サイズを半分に

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			sizeThis,	// 判定サイズ(右・上・後)
			sizeThis,	// 判定サイズ(左・下・前)
			sizeOther,	// 判定目標サイズ(右・上・後)
			sizeOther,	// 判定目標サイズ(左・下・前)
			useful::RotToFourDire(GetVec3Rotation().y),			// 判定方向列挙
			useful::RotToFourDire(rList->GetVec3Rotation().y)	// 判定目標方向列挙
		))
		{ // 判定内だった場合

			if (bRelase)
			{ // 破棄する場合

				// 終了処理
				rList->Uninit();

				// 未保存を設定
				m_bSave = false;
			}
			else
			{ // 破棄しない場合

				// 赤を設定
				rList->SetColor(XCOL_RED);
			}
		}
		else
		{ // 判定外だった場合

			// 通常色を設定
			rList->SetColor(XCOL_WHITE);
		}
	}
}

//============================================================
//	ギミックの色の全初期化処理
//============================================================
void CEditGimmick::InitAllColorGimmick(void)
{
	CListManager<CGimmick>* pListManager = CGimmick::GetList();	// ギミックリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CGimmick*> listGimmick = pListManager->GetList();	// ギミックリスト情報

	for (auto& rList : listGimmick)
	{ // ギミック数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pGimmick) { continue; }

		// 通常色を設定
		rList->SetColor(XCOL_WHITE);
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditGimmick::Save(void)
{
#if _DEBUG

	// 地面のリストを取得
	CListManager<CGimmick>* pListManager = CGimmick::GetList();	// リストマネージャー
	std::list<CGimmick*> listGimmick;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listGimmick = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ギミック配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	ギミック配置のセーブデータ [save_gimmick.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [stage.txt] に張り付け\n" << std::endl;

	// ギミックの色の全初期化
	InitAllColorGimmick();

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_GIMMICKSET\n" << std::endl;

	for (const auto& rList : listGimmick)
	{ // 地面の総数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pGimmick) { continue; }

		// 書き出す情報を取得
		D3DXVECTOR3 pos = rList->GetVec3Position();	// 位置
		EAngle angle = rList->GetAngle();			// 方向
		D3DXVECTOR3 size = rList->GetVec3Sizing();	// サイズ
		CGimmick::EType type = rList->GetType();	// 種類
		int nNumActive = rList->GetNumActive();		// 発動可能人数

		// 情報を書き出し
		file << "	GIMMICKSET" << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ANGLE	= " << angle << std::endl;
		file << "		SIZE	= " << size.x << " " << size.z << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		NUMACT	= " << nNumActive << std::endl;
		file << "	END_GIMMICKSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_GIMMICKSET" << std::endl;

	// ギミックの削除判定
	DeleteCollisionGimmick(false);

	// 保存済みにする
	m_bSave = true;

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}
#endif
