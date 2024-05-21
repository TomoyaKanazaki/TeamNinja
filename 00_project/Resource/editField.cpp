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
#define KEY_TYPE		(DIK_2)	// 種類変更キー
#define NAME_TYPE		("2")	// 種類変更表示

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
	const D3DXVECTOR2 INIT_SIZE = D3DXVECTOR2(100.0f, 100.0f);	// 大きさ
	const float	MAX_SIZE = 10000.0f;	// 最大の大きさ
	const float	INIT_ALPHA = 0.5f;		// 配置前のα値
}

//************************************************************
//	親クラス [CEditField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditField::CEditField(CEditManager *pEditManager) : CEditStage(pEditManager)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pField = nullptr;	// フィールド情報
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
	m_infoCreate.col	 = XCOL_WHITE;			// 色
	m_infoCreate.part	 = GRID2_ONE;			// 分割数
	m_infoCreate.texPart = GRID2_ONE;			// テクスチャ分割数

	// 親クラスの初期化
	if (FAILED(CEditStage::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	CEditManager *pEditManager = GetPtrEditManager();	// エディットマネージャー
	if (pEditManager == nullptr)
	{ // エディットマネージャーが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フィールドの生成
	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	m_pField = CField::Create
	( // 引数
		m_infoCreate.type,		// 種類
		posEdit,				// 位置
		VEC3_ZERO,				// 向き
		m_infoCreate.size,		// 大きさ
		m_infoCreate.col,		// 色
		m_infoCreate.part,		// 分割数
		m_infoCreate.texPart	// テクスチャ分割数
	);
	if (m_pField == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 色を設定
	D3DXCOLOR col = m_pField->GetColor();	// 元の色を取得
	m_pField->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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
	CEditStage::Uninit();

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

	CEditManager *pEditManager = GetPtrEditManager();	// エディットマネージャー
	if (pEditManager == nullptr)
	{ // エディットマネージャーが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// エディットがOFFなら抜ける
	if (!pEditManager->IsEdit()) { return; }

	// 親クラスの更新
	CEditStage::Update();

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

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditField::DrawDebugControl(void)
{
	// 操作表示の描画
	CEditStage::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "大きさ：[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditField::DrawDebugInfo(void)
{
	// 情報表示の描画
	CEditStage::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d：[種類]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f：[大きさ]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d：[分割]\n", m_infoCreate.part.x, m_infoCreate.part.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d：[テクスチャ分割]\n", m_infoCreate.texPart.x, m_infoCreate.texPart.y);
}

//============================================================
//	情報保存処理
//============================================================
void CEditField::SaveInfo(void)
{
	// 現在の情報を保存
	//m_save = m_infoCreate;
}

//============================================================
//	情報読込処理
//============================================================
void CEditField::LoadInfo(void)
{
	// 保存情報を設定
	//m_infoCreate = m_save;
}

//============================================================
//	保存処理
//============================================================
void CEditField::Save(FILE *pFile)
{
#if _DEBUG

#if 0

	if (pFile != nullptr)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	フィールドの配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_BLOCKSET\n\n");

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // 優先順位の総数分繰り返す

			// ポインタを宣言
			CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

			if (pObjectTop != nullptr)
			{ // 先頭が存在する場合

				// ポインタを宣言
				CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

				while (pObjCheck != nullptr)
				{ // オブジェクトが使用されている場合繰り返す

					// ポインタを宣言
					CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

					if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
					{ // オブジェクトラベルがフィールドではない場合

						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;

						// 次の繰り返しに移行
						continue;
					}

					if (pObjCheck == (CObject*)m_pField)
					{ // 同じアドレスだった場合

						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;

						// 次の繰り返しに移行
						continue;
					}

					// フィールドの情報を取得
					D3DXVECTOR3 posField = pObjCheck->GetVec3Position();		// 位置
					D3DXVECTOR3 rotField = pObjCheck->GetVec3Rotation();		// 向き
					D3DXVECTOR3 sizeField = pObjCheck->GetVec3Sizing();		// 大きさ
					D3DXVECTOR2 partTexXField = pObjCheck->GetTexturePatternX();	// テクスチャ分割X
					D3DXVECTOR2 partTexYField = pObjCheck->GetTexturePatternY();	// テクスチャ分割Y
					D3DXVECTOR2 partTexZField = pObjCheck->GetTexturePatternZ();	// テクスチャ分割Z
					int nType = pObjCheck->GetType();	// 種類

					// 情報を書き出し
					fprintf(pFile, "	BLOCKSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posField.x, posField.y, posField.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotField.x, rotField.y, rotField.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeField.x, sizeField.y, sizeField.z);
					fprintf(pFile, "		PARTX = %.2f %.2f \n", partTexXField.x, partTexXField.y);
					fprintf(pFile, "		PARTY = %.2f %.2f \n", partTexYField.x, partTexYField.y);
					fprintf(pFile, "		PARTZ = %.2f %.2f \n", partTexZField.x, partTexZField.y);
					fprintf(pFile, "	END_BLOCKSET\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_BLOCKSET\n\n");
	}

#endif

#endif	// _DEBUG
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
	useful::LimitNum(m_infoCreate.size.x, INIT_SIZE.x, MAX_SIZE);
	useful::LimitNum(m_infoCreate.size.y, INIT_SIZE.y, MAX_SIZE);

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
	CEditManager *pEditManager = GetPtrEditManager();	// エディットマネージャー
	if (pEditManager == nullptr)
	{ // エディットマネージャーが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	D3DXCOLOR colField = XCOL_WHITE;	// 色保存用

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
		pEditManager->UnSave();

		//----------------------------------------------------
		//	新しいフィールドの生成
		//----------------------------------------------------
		// フィールドの生成
		m_pField = CField::Create
		( // 引数
			m_infoCreate.type,		// 種類
			posEdit,				// 位置
			VEC3_ZERO,				// 向き
			m_infoCreate.size,		// 大きさ
			m_infoCreate.col,		// 色
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
	CEditManager *pEditManager = GetPtrEditManager();	// エディットマネージャー
	if (pEditManager == nullptr)
	{ // エディットマネージャーが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return; }				// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報

	D3DXVECTOR3 posEdit = GetVec3Position();	// エディットの位置
	for (auto& rList : listField)
	{ // フィールド数分繰り返す

		// 同じアドレスだった場合次へ
		if (rList == m_pField) { continue; }

		D3DXVECTOR3 posField  = rList->GetVec3Position();	// フィールド位置
		D3DXVECTOR3 sizeThisField = D3DXVECTOR3(0.0f, 25.0f, 0.0f);		// フィールド大きさ
		D3DXVECTOR3 sizeOtherField = D3DXVECTOR3(0.0f, 25.0f, 0.0f);	// フィールド大きさ

		D3DXVECTOR2 sizeThis = m_pField->GetVec2Sizing();
		sizeThisField.x = sizeThis.x;
		sizeThisField.z = sizeThis.y;

		D3DXVECTOR2 sizeOther = rList->GetVec2Sizing();
		sizeOtherField.x = sizeOther.x;
		sizeOtherField.z = sizeOther.y;

		sizeThisField *= 0.5f;	// フィールド大きさ
		sizeOtherField *= 0.5f;	// フィールド大きさ

		// TODO：判定きもいよー
		// 矩形の当たり判定
		if (collision::Box3D
		( // 引数
			posEdit,		// 判定位置
			posField,		// 判定目標位置
			sizeThisField,
			sizeThisField,
			sizeOtherField,
			sizeOtherField
		))
		{ // 判定内だった場合

			if (bRelase)
			{ // 破棄する場合

				// 終了処理
				rList->Uninit();

				// 未保存を設定
				pEditManager->UnSave();
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

		// 通常色を設定
		rList->SetColor(XCOL_WHITE);
	}
}
#endif
