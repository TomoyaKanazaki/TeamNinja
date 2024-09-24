//============================================================
//
//	タッチアクター処理 [touchActor.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "touchActor.h"

#include "touchActorStone.h"
#include "touchActorBird.h"
#include "touchActorBranch.h"

#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL[] =	// モデルのパス
	{
		"data\\MODEL\\TouchActor\\TouchStone.x",	// 石
		"data\\MODEL\\TouchActor\\TouchBird.x",		// 鳥
		"data\\MODEL\\TouchActor\\TouchBranch.x",	// 枝
	};
	const int PRIORITY = 4;			// アクターの優先順位
	const float SUB_ALPHA = 1.0f;	// 透明度の減算量
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CTouchActor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CTouchActor>* CTouchActor::m_pList = nullptr;		// リスト構造

//************************************************************
//	子クラス [CTouchActor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchActor::CTouchActor() : CObjectModel(CObject::LABEL_TOUCHACTOR, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_posInit(VEC3_ZERO),	// 初期位置
m_posOld(VEC3_ZERO),	// 前回の位置
m_move(VEC3_ZERO),		// 移動量
m_type(TYPE_STONE),		// 種類
m_state(STATE_NONE),	// 状態
m_nStateCount(0)		// 状態カウント
{

}

//============================================================
//	デストラクタ
//============================================================
CTouchActor::~CTouchActor()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTouchActor::Init(void)
{
	// 値を初期化
	m_state = STATE_NONE;		// 状態

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CTouchActor>::Create();

		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTouchActor::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTouchActor::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// 前回の位置を保存する
	m_posOld = GetVec3Position();

	// 状態カウントを加算する
	m_nStateCount++;

	switch (m_state)
	{
	case CTouchActor::STATE_NONE:

		// 通常状態更新処理
		UpdateNone(fDeltaTime);

		break;

	case CTouchActor::STATE_ACT:

		// アクション状態更新処理
		UpdateAct(fDeltaTime);

		if (Fade(fDeltaTime))
		{ // 透明になったら

			// 終了処理
			Uninit();

			// この関数を抜ける
			return;
		}

		break;

	default:
		assert(false);
		break;
	}

	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTouchActor::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CTouchActor::SetData
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 拡大率
)
{
	// 情報の設定
	SetVec3Position(rPos);		// 位置
	SetVec3Rotation(rRot);		// 向き
	SetVec3Scaling(rScale);		// 拡大率
	m_type = type;				// 種類
	m_posInit = rPos;			// 初期位置

	// モデルの割り当て処理
	BindModel(MODEL[type]);
}

//============================================================
//	生成処理
//============================================================
CTouchActor* CTouchActor::Create
( // 引数
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 拡大率
)
{
	// モデルUIの生成
	CTouchActor* pActor = nullptr;

	switch (type)
	{
	case CTouchActor::TYPE_STONE:

		pActor = new CTouchStone;

		break;

	case CTouchActor::TYPE_BIRD:

		pActor = new CTouchBird;

		break;

	case CTouchActor::TYPE_BRANCH:

		pActor = new CTouchBranch;

		break;

	default:
		assert(false);
		break;
	}

	// アクターの初期化
	if (FAILED(pActor->Init()))
	{ // 初期化に失敗した場合

		// アクターの破棄
		SAFE_DELETE(pActor);
		return nullptr;
	}

	// 情報の設定処理
	pActor->SetData
	(
		type,
		rPos,
		rRot,
		rScale
	);

	// 確保したアドレスを返す
	return pActor;
}

//============================================================
// リスト構造の取得処理
//============================================================
CListManager<CTouchActor>* CTouchActor::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
// 床との当たり判定
//============================================================
bool CTouchActor::CollisionFieid(D3DXVECTOR3& rPos)
{
	// フィールドとの当たり判定
	return GET_STAGE->LandFieldPosition(rPos, m_posOld, m_move);
}

//============================================================
// フェード処理
//============================================================
bool CTouchActor::Fade(const float fDeltaTime)
{
	float fAlpha = GetAlpha();	// 透明度
	bool bDelete = false;		// 消去状況

	// 透明度を下げる
	fAlpha -= SUB_ALPHA * fDeltaTime;

	if (fAlpha <= 0.0f)
	{ // 透明になったら

		// 消去状況を true にする
		bDelete = true;
	}

	// 透明度を反映
	SetAlpha(fAlpha);

	// 消去状況を返す
	return bDelete;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CTouchActor::LoadSetup(const char* pPass)
{
	int nType = 0;					// 種類の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "タッチアクターセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "STAGE_TOUCH_ACTORSET")
		{
			do
			{ // END_STAGE_ACTORSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "TOUCH_ACTORSET")
				{
					do
					{ // END_TOUCH_ACTORSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str == "TYPE")
						{
							file >> str;	// ＝を読込

							// 種類を読込
							file >> nType;
						}
						else if (str == "POS")
						{
							file >> str;	// ＝を読込

							// 位置を読込
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
					} while (str != "END_TOUCH_ACTORSET");	// END_TOUCH_ACTORSETを読み込むまでループ

					// アクターオブジェクトの生成
					if (CTouchActor::Create((EType)nType, pos) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
			} while (str != "END_STAGE_TOUCH_ACTORSET");	// END_STAGE_TOUCH_ACTORSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}