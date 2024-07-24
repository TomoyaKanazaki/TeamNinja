#if 1
//============================================================
//
//	エディットコイン処理 [editCoin.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editCoin.h"
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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_coin.txt";	// セーブテキストパス

	const D3DXVECTOR3 SCALING = D3DXVECTOR3(0.1f, 0.1f, 0.1f);		// 拡縮率
	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
}

//************************************************************
//	親クラス [CEditCoin] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditCoin::CEditCoin(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pCoin = nullptr;	// コイン情報
	m_bSave = false;	// 保存状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditCoin::~CEditCoin()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditCoin::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pCoin = nullptr;						// 情報
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// コインの生成
	m_pCoin = CCoin::Create(GetVec3Position());
	if (m_pCoin == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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
void CEditCoin::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// コインの色の全初期化
	InitAllColorCoin();

	// コインの終了
	SAFE_UNINIT(m_pCoin);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditCoin::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// コインの生成
	CreateCoin();

	// コインの破棄
	ReleaseCoin();

	// 位置を反映
	m_pCoin->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pCoin->SetVec3Rotation(VEC3_ZERO);

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditCoin::IsSave(void)
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
void CEditCoin::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditCoin::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditCoin::DrawDebugControl(void)
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
void CEditCoin::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
// 位置更新
//============================================================
void CEditCoin::UpdatePosition(void)
{
	// 位置の更新
	CEditorObject::UpdatePosition();

	// 位置を反映
	m_pCoin->SetVec3Position(GetVec3Position());
}

//============================================================
//	コインの生成処理
//============================================================
void CEditCoin::CreateCoin(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// コインを配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	コインの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pCoin->SetEnableUpdate(true);
		m_pCoin->SetEnableDraw(true);

		// 未保存を設定
		m_bSave = false;

		//----------------------------------------------------
		//	新しいコインの生成
		//----------------------------------------------------
		// コインの生成
		m_pCoin = CCoin::Create(GetVec3Position());
		assert(m_pCoin != nullptr);
	}
}

//============================================================
//	コインの破棄処理
//============================================================
void CEditCoin::ReleaseCoin(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// コインを削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// コインの削除判定
	DeleteCollisionCoin(bRelease);
}

//============================================================
//	コインの削除判定
//============================================================
void CEditCoin::DeleteCollisionCoin(const bool bRelase)
{
	CListManager<CCoin>* pListManager = CCoin::GetList();	// コインリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CCoin*> listCoin = pListManager->GetList();	// コインリスト情報

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	for (auto& rList : listCoin)
	{ // コイン数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pCoin) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
		D3DXVECTOR3 scaleThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 scaleOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		D3DXVECTOR3 scaleThisCoin = m_pCoin->GetVec3Scaling();	// 自身の地面の大きさ
		scaleThis.x = scaleThisCoin.x;	// 判定サイズXを設定
		scaleThis.y = scaleThisCoin.y;	// 判定サイズYを設定
		scaleThis.z = scaleThisCoin.z;	// 判定サイズZを設定
		scaleThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		D3DXVECTOR3 scaleOtherCoin = rList->GetVec3Scaling();		// 対象の地面の大きさ
		scaleOther.x = scaleOtherCoin.x;	// 判定サイズXを設定
		scaleOther.y = scaleOtherCoin.y;	// 判定サイズYを設定
		scaleOther.z = scaleOtherCoin.z;	// 判定サイズZを設定
		scaleOther *= 0.5f;				// 判定サイズを半分に

		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,	// 判定位置
			posOther,	// 判定目標位置
			scaleThis,	// 判定サイズ(右・上・後)
			scaleThis,	// 判定サイズ(左・下・前)
			scaleOther,	// 判定目標サイズ(右・上・後)
			scaleOther	// 判定目標サイズ(左・下・前)
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
//	コインの色の全初期化処理
//============================================================
void CEditCoin::InitAllColorCoin(void)
{
	CListManager<CCoin>* pListManager = CCoin::GetList();	// コインリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CCoin*> listCoin = pListManager->GetList();	// コインリスト情報

	for (auto& rList : listCoin)
	{ // コイン数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pCoin) { continue; }

		// 通常色を設定
		rList->ResetMaterial();
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditCoin::Save(void)
{
#if _DEBUG

	// 地面のリストを取得
	CListManager<CCoin>* pListManager = CCoin::GetList();	// リストマネージャー
	std::list<CCoin*> listCoin;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listCoin = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "コイン配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	コイン配置のセーブデータ [save_coin.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [coin.txt] に張り付け\n" << std::endl;

	// コインの色の全初期化
	InitAllColorCoin();

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_COINSET\n" << std::endl;

	for (const auto& rList : listCoin)
	{ // 地面の総数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pCoin) { continue; }

		// 書き出す情報を取得
		D3DXVECTOR3 pos = rList->GetVec3Position();		// 位置

		// 情報を書き出し
		file << "	COINSET	= " << pos.x << " " << pos.y << " " << pos.z  << "\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_COINSET" << std::endl;

	// コインの削除判定
	DeleteCollisionCoin(false);

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
