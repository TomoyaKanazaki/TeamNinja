#if 1
//============================================================
//
//	エディットゴールポイント処理 [editGoalPoint.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editManager.h"
#include "editGoalPoint.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"

#include "goal.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_RELEASE		(DIK_9)	// 破棄キー
#define NAME_RELEASE	("9")	// 破棄表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_goalpoint.txt";	// セーブテキストパス

	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
}

//************************************************************
//	親クラス [CEditGoalPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditGoalPoint::CEditGoalPoint(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pPoint = nullptr;	// ポイント
	m_bSave = false;	// 保存状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditGoalPoint::~CEditGoalPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditGoalPoint::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pPoint = nullptr;		// ポイント
	m_bSave = false;		// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ゴールポイントの生成
	m_pPoint = CObjectModel::Create
	( // 引数
		GetVec3Position(),		// 位置
		VEC3_ZERO				// 向き
	);
	if (m_pPoint == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを割り当てる
	m_pPoint->BindModel("data\\MODEL\\ENEMY\\BOSS_DRAGON\\03_head.x");

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
void CEditGoalPoint::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// ゴールポイントの終了
	SAFE_UNINIT(m_pPoint);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditGoalPoint::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// ゴールポイントの生成
	CreateCheckPoint();

	// ゴールポイントの破棄
	ReleaseCheckPoint();

	// 位置を反映
	m_pPoint->SetVec3Position(GetVec3Position());

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditGoalPoint::IsSave(void)
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
void CEditGoalPoint::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditGoalPoint::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditGoalPoint::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditGoalPoint::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	if (CGoal::GetGoal() == nullptr)
	{ // ゴールが NULL の場合

		DebugProc::Print(DebugProc::POINT_RIGHT, "設置可能\n");
	}
	else
	{ // 上記以外

		DebugProc::Print(DebugProc::POINT_RIGHT, "設置不可能\n");
	}

#endif	// _DEBUG
}

//============================================================
//	ゴールポイントの生成処理
//============================================================
void CEditGoalPoint::CreateCheckPoint(void)
{
	CGoal* pGoal = CGoal::GetGoal();			// ゴールの情報を取得
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置

	// ゴールポイントを配置
	if (pKeyboard->IsTrigger(KEY_CREATE) &&
		pGoal == nullptr)
	{
		//----------------------------------------------------
		//	ゴールポイントの生成
		//----------------------------------------------------
		// ゴールポイントの生成
		CGoal::Create(GetVec3Position());
		assert(m_pPoint != nullptr);

		// 未保存を設定
		m_bSave = false;
	}
}

//============================================================
//	ゴールポイントの破棄処理
//============================================================
void CEditGoalPoint::ReleaseCheckPoint(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// ゴールポイントを削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// ゴールポイントの削除判定
	DeleteCollisionCheckPoint(bRelease);
}

//============================================================
//	ゴールポイントの削除判定
//============================================================
void CEditGoalPoint::DeleteCollisionCheckPoint(const bool bRelase)
{
	// ゴールを取得
	CGoal* pGoal = CGoal::GetGoal();

	if (pGoal != nullptr)
	{ // ゴールが NULL じゃない場合

		if (bRelase == true)
		{ // 削除する場合

			// ゴールを終了する
			pGoal->Uninit();
		}
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditGoalPoint::Save(void)
{
#if _DEBUG

	// ゴールの情報を取得
	CGoal* pGoal = CGoal::GetGoal();

	// ゴールが NULL の場合、抜ける
	if (pGoal == nullptr) { return S_OK; }

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ゴールポイントの書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	ゴールポイントのセーブデータ [save_goalpoint.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [Point.txt] に張り付け\n" << std::endl;

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_GOALSET\n" << std::endl;

	// 位置を書き出し
	D3DXVECTOR3 pos = pGoal->GetVec3Position();	// 位置
	file << "	POS = " << pos.x << " " << pos.y << " " << pos.z << "\n" << std::endl;

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_GOALSET" << std::endl;

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
