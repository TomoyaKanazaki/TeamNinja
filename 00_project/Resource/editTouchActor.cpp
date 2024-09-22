#if 1
//============================================================
//
//	エディットタッチアクター処理 [editTouchActor.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editTouchActor.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_RELEASE		(DIK_9)	// 破棄キー
#define NAME_RELEASE	("9")	// 破棄表示
#define KEY_TYPE		(DIK_4)	// 種類変更キー
#define NAME_TYPE		("4")	// 種類変更表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_touchActor.txt";	// セーブテキストパス

	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
}

//************************************************************
//	親クラス [CEditTouchActor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditTouchActor::CEditTouchActor(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pActor = nullptr;					// タッチアクター情報
	m_type = CTouchActor::TYPE_STONE;	// 種類
	m_bSave = false;					// 保存状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditTouchActor::~CEditTouchActor()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditTouchActor::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pActor = nullptr;						// 情報
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タッチアクターの生成
	m_pActor = CTouchActor::Create(CTouchActor::TYPE_STONE, GetVec3Position());
	if (m_pActor == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}
	// 更新が通らないようにする
	m_pActor->SetLabel(CObject::LABEL_NONE);

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
void CEditTouchActor::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// タッチアクターの色の全初期化
	InitAllColorTouchActor();

	// タッチアクターの終了
	SAFE_UNINIT(m_pActor);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditTouchActor::Update(void)
{
#if _DEBUG

	// 種類変更
	ChangeType();

	// 親クラスの更新
	CEditorObject::Update();

	// タッチアクターの生成
	CreateTouchActor();

	// タッチアクターの破棄
	ReleaseTouchActor();

	// 位置を反映
	m_pActor->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pActor->SetVec3Rotation(VEC3_ZERO);

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditTouchActor::IsSave(void)
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
void CEditTouchActor::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditTouchActor::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditTouchActor::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditTouchActor::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditTouchActor::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		// 種類を1つ進める
		m_type = (CTouchActor::EType)((m_type + 1) % CTouchActor::TYPE_MAX);

		// 終了処理
		m_pActor->Uninit();

		// ギミックを再生成する
		m_pActor = CTouchActor::Create
		( // 引数
			m_type,				// 種類
			GetVec3Position()	// 位置
		);
	}
}

//============================================================
//	タッチアクターの生成処理
//============================================================
void CEditTouchActor::CreateTouchActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// タッチアクターを配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	タッチアクターの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pActor->SetEnableUpdate(true);
		m_pActor->SetEnableDraw(true);
		m_pActor->SetLabel(CObject::LABEL_TOUCHACTOR);

#ifdef _DEBUG

		// 初期位置を設定する
		m_pActor->SetVec3PosInit(m_pActor->GetVec3Position());

#endif // _DEBUG

		// 未保存を設定
		m_bSave = false;

		//----------------------------------------------------
		//	新しいタッチアクターの生成
		//----------------------------------------------------
		// タッチアクターの生成
		m_pActor = CTouchActor::Create(m_type, GetVec3Position());
		assert(m_pActor != nullptr);
		// 更新が通らないようにする
		m_pActor->SetLabel(CObject::LABEL_NONE);
	}
}

//============================================================
//	タッチアクターの破棄処理
//============================================================
void CEditTouchActor::ReleaseTouchActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// タッチアクターを削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// タッチアクターの削除判定
	DeleteCollisionTouchActor(bRelease);
}

//============================================================
//	タッチアクターの削除判定
//============================================================
void CEditTouchActor::DeleteCollisionTouchActor(const bool bRelase)
{
	CListManager<CTouchActor>* pListManager = CTouchActor::GetList();	// タッチアクターリストマネージャー
	if (pListManager == nullptr) { return; }							// リスト未使用の場合抜ける
	std::list<CTouchActor*> listTouchActor = pListManager->GetList();	// タッチアクターリスト情報

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	for (auto& rList : listTouchActor)
	{ // タッチアクター数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pActor) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();				// 対象の地面位置
		D3DXVECTOR3 scaleThisMax = m_pActor->GetModelData().vtxMax;		// 自身の最大値
		D3DXVECTOR3 scaleThisMin = -m_pActor->GetModelData().vtxMin;	// 自身の最小値
		D3DXVECTOR3 scaleOtherMax = rList->GetModelData().vtxMax;		// 対象の最大値
		D3DXVECTOR3 scaleOtherMin = -rList->GetModelData().vtxMin;		// 対象の最小値

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,		// 判定位置
			posOther,		// 判定目標位置
			scaleThisMax,	// 判定サイズ(右・上・後)
			scaleThisMin,	// 判定サイズ(左・下・前)
			scaleOtherMax,	// 判定目標サイズ(右・上・後)
			scaleOtherMin	// 判定目標サイズ(左・下・前)
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
				rList->SetAllMaterial(material::Red());
			}
		}
		else
		{ // 判定外だった場合

			// 通常色を設定
			rList->ResetMaterial();
		}
	}
}

//============================================================
//	タッチアクターの色の全初期化処理
//============================================================
void CEditTouchActor::InitAllColorTouchActor(void)
{
	CListManager<CTouchActor>* pListManager = CTouchActor::GetList();	// タッチアクターリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CTouchActor*> listTouchActor = pListManager->GetList();	// タッチアクターリスト情報

	for (auto& rList : listTouchActor)
	{ // タッチアクター数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pActor) { continue; }

		// 通常色を設定
		rList->ResetMaterial();
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditTouchActor::Save(void)
{
#if _DEBUG

	// 神器のリストを取得
	CListManager<CTouchActor>* pListManager = CTouchActor::GetList();	// リストマネージャー
	std::list<CTouchActor*> listTouchActor;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listTouchActor = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "タッチアクター配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	タッチアクター配置のセーブデータ [save_touchactor.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [touchactor.txt] に張り付け\n" << std::endl;

	// タッチアクターの色の全初期化
	InitAllColorTouchActor();

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_TOUCH_ACTORSET\n" << std::endl;

	for (const auto& rList : listTouchActor)
	{ // 地面の総数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pActor) { continue; }

		// 書き出す情報を取得
		CTouchActor::EType type = rList->GetType();		// 種類
		D3DXVECTOR3 pos = rList->GetVec3PosInit();		// 初期位置

		// 情報を書き出し
		file << "	TOUCH_ACTORSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "	END_TOUCH_ACTORSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_TOUCH_ACTORSET" << std::endl;

	// タッチアクターの削除判定
	DeleteCollisionTouchActor(false);

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
