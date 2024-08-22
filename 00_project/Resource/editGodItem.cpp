#if 1
//============================================================
//
//	エディット神器処理 [editGodItem.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editGodItem.h"
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
	const char* MODEL = "data\\MODEL\\GODITEM\\Magatama.x";			// モデル
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_goditem.txt";	// セーブテキストパス

	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
}

//************************************************************
//	親クラス [CEditGodItem] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditGodItem::CEditGodItem(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pGodItem = nullptr;	// 神器情報
	m_bSave = false;	// 保存状況
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// 神器配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditGodItem::~CEditGodItem()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditGodItem::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pGodItem = nullptr;					// 情報
	m_infoCreate.type = (CGodItem::EType)0;	// 種類
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ゴールポイントの生成
	m_pGodItem = CObjectModel::Create
	( // 引数
		GetVec3Position(),		// 位置
		VEC3_ZERO				// 向き
	);
	if (m_pGodItem == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを割り当てる
	m_pGodItem->BindModel(MODEL);

	// マテリアルの割り当て
	switch (m_infoCreate.type)
	{
	case CGodItem::TYPE_RED:
		m_pGodItem->SetAllMaterial(material::Red());	// 赤色
		break;

	case CGodItem::TYPE_GREEN:
		m_pGodItem->SetAllMaterial(material::Green());	// 緑色
		break;

	case CGodItem::TYPE_BLUE:
		m_pGodItem->SetAllMaterial(material::Blue());	// 青色
		break;

	default:
		assert(false);
		break;
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
void CEditGodItem::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// 神器の終了
	SAFE_UNINIT(m_pGodItem);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditGodItem::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// 種類の変更
	ChangeType();

	// 神器の生成
	CreateGodItem();

	// 神器の破棄
	ReleaseGodItem();

	// 位置を反映
	m_pGodItem->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pGodItem->SetVec3Rotation(VEC3_ZERO);

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditGodItem::IsSave(void)
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
void CEditGodItem::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditGodItem::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditGodItem::DrawDebugControl(void)
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
void CEditGodItem::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[種類]\n", m_infoCreate.type);

#endif	// _DEBUG
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditGodItem::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CGodItem::EType)((m_infoCreate.type + 1) % CGodItem::TYPE_MAX);

		// マテリアルの割り当て
		switch (m_infoCreate.type)
		{
		case CGodItem::TYPE_RED:
			m_pGodItem->SetAllMaterial(material::Red());	// 赤色
			break;

		case CGodItem::TYPE_GREEN:
			m_pGodItem->SetAllMaterial(material::Green());	// 緑色
			break;

		case CGodItem::TYPE_BLUE:
			m_pGodItem->SetAllMaterial(material::Blue());	// 青色
			break;

		default:
			assert(false);
			break;
		}
	}
}

//============================================================
//	神器の生成処理
//============================================================
void CEditGodItem::CreateGodItem(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	if (CGodItem::GetList() != nullptr)
	{ // リストがある場合

		for (auto& item : CGodItem::GetList()->GetList())
		{
			if (item->GetType() == m_infoCreate.type)
			{ // 現在選択している神器が存在している場合

				// この先の処理を行わない
				return;
			}
		}
	}

	// 神器を配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		// ゴールポイントの生成
		CGodItem::Create
		(
			GetVec3Position(),
			m_infoCreate.type
		);

		// 未保存を設定
		m_bSave = false;
	}
}

//============================================================
//	神器の破棄処理
//============================================================
void CEditGodItem::ReleaseGodItem(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// 神器を削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// 神器の削除判定
	DeleteCollisionGodItem(bRelease);
}

//============================================================
//	神器の削除判定
//============================================================
void CEditGodItem::DeleteCollisionGodItem(const bool bRelase)
{
	// 削除判定が false の場合抜ける
	if (!bRelase) { return; }

	CListManager<CGodItem>* pListManager = CGodItem::GetList();	// 神器リストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CGodItem*> listGodItem = pListManager->GetList();	// 神器リスト情報

	for (auto& rList : listGodItem)
	{ // 神器数分繰り返す

		if (rList->GetType() == m_infoCreate.type)
		{ // 種類が一致した場合

			// 終了処理
			rList->Uninit();
		}
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditGodItem::Save(void)
{
#if _DEBUG

	// 神器のリストを取得
	CListManager<CGodItem>* pListManager = CGodItem::GetList();	// リストマネージャー
	std::list<CGodItem*> listGodItem;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listGodItem = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "神器配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	神器配置のセーブデータ [save_goditem.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [goditem.txt] に張り付け\n" << std::endl;

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_GODITEMSET\n" << std::endl;

	for (const auto& rList : listGodItem)
	{ // 地面の総数分繰り返す

		// 書き出す情報を取得
		CGodItem::EType type = rList->GetType();		// 種類
		D3DXVECTOR3 pos = rList->GetVec3Position();		// 位置
		float fPosInitY = rList->GetInitPosY();			// 初期位置(Y軸)

		// 情報を書き出し
		file << "	GODITEMSET" << std::endl;
		file << "		POS		= " << pos.x << " " << fPosInitY << " " << pos.z << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		" << std::endl;
		file << "		TEXT_" << type << std::endl;
		file << "			STR = これが" << type << "つ目の勾玉" << std::endl;
		file << "		END_TEXT" << std::endl;
		file << "	END_GODITEMSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_GODITEMSET" << std::endl;

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
