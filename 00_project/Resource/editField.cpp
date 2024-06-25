#if 1
//============================================================
//
//	エディットフィールド処理 [editField.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editField.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
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
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_field.txt";	// セーブテキストパス

	const D3DXVECTOR2 INIT_SIZE = D3DXVECTOR2(editstage::SIZE, editstage::SIZE);	// 大きさ
	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
	const int DIGIT_FLOAT = 2;		// 小数点以下の桁数
}

//************************************************************
//	親クラス [CEditField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditField::CEditField(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pField = nullptr;	// フィールド情報
	m_bSave = false;	// 保存状況
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// フィールド配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditField::~CEditField()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditField::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pField			 = nullptr;				// 情報
	m_infoCreate.type	 = (CField::EType)0;	// 種類
	m_infoCreate.size	 = INIT_SIZE;			// 大きさ
	m_infoCreate.part	 = GRID2_ONE;			// 分割数
	m_infoCreate.texPart = GRID2_ONE;			// テクスチャ分割数
	m_bSave				 = false;				// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorObject::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フィールドの生成
	D3DXCOLOR colField = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 地面色
	m_pField = CField::Create
	( // 引数
		m_infoCreate.type,		// 種類
		GetVec3Position(),		// 位置
		GetVec3Rotation(),		// 向き
		m_infoCreate.size,		// 大きさ
		colField,				// 色
		m_infoCreate.part,		// 分割数
		m_infoCreate.texPart	// テクスチャ分割数
	);
	if (m_pField == nullptr)
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
void CEditField::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorObject::Uninit();

	// フィールドの色の全初期化
	InitAllColorField();

	// フィールドの終了
	SAFE_UNINIT(m_pField);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditField::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorObject::Update();

	// 大きさの更新
	UpdateSizing();

	// テクスチャ分割の更新
	UpdateTexPart();

	// 種類の変更
	ChangeType();

	// フィールドの生成
	CreateField();

	// フィールドの破棄
	ReleaseField();

	// 位置を反映
	m_pField->SetVec3Position(GetVec3Position());

	// 向きを反映
	m_pField->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditField::IsSave(void)
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
void CEditField::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditField::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditField::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "大きさ：[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditField::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[種類]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f：[大きさ]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d：[分割]\n", m_infoCreate.part.x, m_infoCreate.part.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d：[テクスチャ分割]\n", m_infoCreate.texPart.x, m_infoCreate.texPart.y);

#endif	// _DEBUG
}

//============================================================
//	向き更新処理
//============================================================
void CEditField::UpdateRotation(void)
{
	// 向きの更新
	CEditorObject::UpdateRotation();

	// 向きを反映
	m_pField->SetVec3Rotation(GetVec3Rotation());
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditField::UpdateSizing(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

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
			m_infoCreate.size.y += INIT_SIZE.y;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.y -= INIT_SIZE.y;
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
			m_infoCreate.size.y += INIT_SIZE.y;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.y -= INIT_SIZE.y;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);

	// 大きさを反映
	m_pField->SetVec2Sizing(m_infoCreate.size);
}

//============================================================
//	テクスチャ分割の更新処理
//============================================================
void CEditField::UpdateTexPart(void)
{
	// テクスチャ分割数を設定
	m_infoCreate.texPart.x = (int)(m_infoCreate.size.x / INIT_SIZE.x);
	m_infoCreate.texPart.y = (int)(m_infoCreate.size.y / INIT_SIZE.y);

	// テクスチャ分割数を設定
	m_pField->SetTexPattern(m_infoCreate.texPart);
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditField::ChangeType(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 種類を変更
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CField::EType)((m_infoCreate.type + 1) % CField::TYPE_MAX);
	}

	// 種類を反映
	m_pField->SetType(m_infoCreate.type);
}

//============================================================
//	フィールドの生成処理
//============================================================
void CEditField::CreateField(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	D3DXCOLOR colField = XCOL_WHITE;			// 色保存用

	// フィールドを配置
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	フィールドの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pField->SetEnableUpdate(true);
		m_pField->SetEnableDraw(true);

		// 色を設定
		colField = m_pField->GetColor();	// 元の色を取得
		m_pField->SetColor(D3DXCOLOR(colField.r, colField.g, colField.b, 1.0f));

		// 未保存を設定
		m_bSave = false;

		//----------------------------------------------------
		//	新しいフィールドの生成
		//----------------------------------------------------
		// フィールドの生成
		m_pField = CField::Create
		( // 引数
			m_infoCreate.type,		// 種類
			GetVec3Position(),		// 位置
			GetVec3Rotation(),		// 向き
			m_infoCreate.size,		// 大きさ
			colField,				// 色
			m_infoCreate.part,		// 分割数
			m_infoCreate.texPart	// テクスチャ分割数
		);
		assert(m_pField != nullptr);

		// 色を設定
		colField = m_pField->GetColor();	// 元の色を取得
		m_pField->SetColor(D3DXCOLOR(colField.r, colField.g, colField.b, INIT_ALPHA));
	}
}

//============================================================
//	フィールドの破棄処理
//============================================================
void CEditField::ReleaseField(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	bool bRelease = false;	// 破棄状況

	// フィールドを削除
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// フィールドの削除判定
	DeleteCollisionField(bRelease);
}

//============================================================
//	フィールドの削除判定
//============================================================
void CEditField::DeleteCollisionField(const bool bRelase)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	for (auto& rList : listField)
	{ // フィールド数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pField) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// 対象の地面位置
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// 自身の大きさ
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// 対象の大きさ

		// 自身の大きさを設定
		D3DXVECTOR2 sizeThisField = m_pField->GetVec2Sizing();	// 自身の地面の大きさ
		sizeThis.x = sizeThisField.x;	// 判定サイズXを設定
		sizeThis.y = editstage::SIZE;	// 判定サイズYを設定
		sizeThis.z = sizeThisField.y;	// 判定サイズZを設定
		sizeThis *= 0.5f;				// 判定サイズを半分に

		// 対象の大きさを設定
		D3DXVECTOR2 sizeOtherField = rList->GetVec2Sizing();	// 対象の地面の大きさ
		sizeOther.x = sizeOtherField.x;	// 判定サイズXを設定
		sizeOther.y = editstage::SIZE;	// 判定サイズYを設定
		sizeOther.z = sizeOtherField.y;	// 判定サイズZを設定
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
//	フィールドの色の全初期化処理
//============================================================
void CEditField::InitAllColorField(void)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報

	for (auto& rList : listField)
	{ // フィールド数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pField) { continue; }

		// 通常色を設定
		rList->SetColor(XCOL_WHITE);
	}
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditField::Save(void)
{
#if _DEBUG

	// 地面のリストを取得
	CListManager<CField> *pListManager = CField::GetList();	// リストマネージャー
	std::list<CField*> listField;	// 地面リスト
	if (pListManager != nullptr)
	{ // リストマネージャーが生成されている場合

		// リストを取得
		listField = pListManager->GetList();
	}

	// ファイルを開く
	std::ofstream  file(SAVE_PASS);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ステージ地面配置の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 見出しを書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	ステージ地面配置のセーブデータ [save_field.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# この行から下をコピーし [stage.txt] に張り付け\n" << std::endl;

	// フィールドの色の全初期化
	InitAllColorField();

	// 小数点書き出しの方法を指定
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// 読み込み開始文字列を書き出し
	file << "STAGE_FIELDSET\n" << std::endl;

	for (const auto& rList : listField)
	{ // 地面の総数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pField) { continue; }

		// 書き出す情報を取得
		CField::EType type	= rList->GetType();			// 種類
		D3DXVECTOR3 pos		= rList->GetVec3Position();	// 位置
		D3DXVECTOR3 rot		= rList->GetVec3Rotation();	// 向き
		D3DXVECTOR2 size	= rList->GetVec2Sizing();	// 大きさ
		D3DXCOLOR col		= rList->GetColor();		// 色
		POSGRID2 part		= rList->GetPattern();		// 分割数
		POSGRID2 texPart	= rList->GetTexPattern();	// テクスチャ分割数

		// 向きを360度に変換
		rot = D3DXToDegree(rot);

		// 情報を書き出し
		file << "	FIELDSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
		file << "		SIZE	= " << size.x << " " << size.y << std::endl;
		file << "		COL		= " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
		file << "		PART	= " << part.x << " " << part.y << std::endl;
		file << "		TEXPART	= " << texPart.x << " " << texPart.y << std::endl;
		file << "	END_FIELDSET\n" << std::endl;
	}

	// 読み込み終了文字列を書き出し
	file << "END_STAGE_FIELDSET" << std::endl;

	// フィールドの削除判定
	DeleteCollisionField(false);

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
