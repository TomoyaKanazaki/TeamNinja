//============================================================
//
//	神器処理 [godItem.cpp]
//	Author：小原立暉
//	Adder ：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "godItem.h"
#include "renderer.h"
#include "collision.h"
#include "effekseerControl.h"
#include "effekseerManager.h"
#include "sound.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\GODITEM\\Magatama.x";		// モデル
	const char* SETUP_TXT = "data\\TXT\\goditem.txt";			// セットアップテキスト相対パス
	const int PRIORITY = 4;		// 神器の優先順位
	const D3DXVECTOR3 EFFECT_OFFSET = D3DXVECTOR3(0.0f, 36.0f, 0.0f);	// エフェクト用オフセット
	const float COLLISION_RADIUS = 70.0f;						// 当たり判定用の半径

	const float ADD_ROT = 0.03f;		// 向きの追加量
	const float ADD_HEIGHT = 25.0f;		// 高さの追加量
	const D3DXVECTOR3 ROLL_DEST_POS = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// 回転状態の移動目標位置
	const D3DXVECTOR3 ROLL_ROT = D3DXVECTOR3(0.0f, 4.5f, 0.0f);			// 回転状態の向き
}

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CGodItem>* CGodItem::m_pList = nullptr;		// リスト構造
bool CGodItem::m_aGet[TYPE_MAX] = {};						// 取得状況

//************************************************************
//	子クラス [CGodItem] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGodItem::CGodItem() : CObjectModel(CObject::LABEL_GODITEM, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_pEffectBody(nullptr),	// 本体エフェクト情報
m_pEffectLand(nullptr),	// 着地エフェクト情報
m_fPosInitY(0.0f),		// 初期位置(Y軸)
m_type(TYPE_RED),		// 種類
m_state(STATE_ITEM),	// 状態
m_fHeightRot(0.0f)		// 高さの向き
{

}

//============================================================
//	デストラクタ
//============================================================
CGodItem::~CGodItem()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGodItem::Init(void)
{
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
		m_pList = CListManager<CGodItem>::Create();

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
void CGodItem::Uninit(void)
{
	// 本体エフェクトの削除
	SAFE_DELETE(m_pEffectBody);

	// 着地エフェクトの削除
	SAFE_DELETE(m_pEffectLand);

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
void CGodItem::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_ITEM:

		// 向き処理
		Cycle();

		// 高さ設定処理
		Height();

		break;

	case STATE_ROLL:

		// 回転移動の更新
		UpdateRoll(fDeltaTime);
		break;
	}

	if (m_pEffectBody != nullptr)
	{
		// エフェクト位置を反映
		m_pEffectBody->m_pos = GetVec3Position() + EFFECT_OFFSET;
	}

	if (m_pEffectLand != nullptr)
	{
		D3DXVECTOR3 posThis = GetVec3Position();

		// 地面の着地位置の取得
		posThis.y = GET_STAGE->GetFieldDownPositionHeight(posThis);

		// エフェクト位置を反映
		m_pEffectLand->m_pos = posThis;
	}

	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGodItem::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CGodItem* CGodItem::Create(const D3DXVECTOR3& rPos, const EType type)
{
	// 種類の重複チェック処理
	assert(DuplicationCheck(type));

	// モデルUIの生成
	CGodItem* pItem = new CGodItem;

	if (pItem == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 神器の初期化
		if (FAILED(pItem->Init()))
		{ // 初期化に失敗した場合

			// 神器の破棄
			SAFE_DELETE(pItem);
			return nullptr;
		}

		// 位置を設定
		pItem->SetVec3Position(rPos);

		// 初期位置を設定
		pItem->m_fPosInitY = rPos.y;

		// 向きを設定
		pItem->SetVec3Rotation(VEC3_ZERO);

		// 拡大率を設定
		pItem->SetVec3Scaling(VEC3_ONE);

		// モデルの割り当て処理
		pItem->BindModel(MODEL);

		// マテリアルの割り当て
		switch (type)
		{
		case CGodItem::TYPE_RED:
			pItem->SetAllMaterial(material::Red());		// 赤色
			break;

		case CGodItem::TYPE_GREEN:
			pItem->SetAllMaterial(material::Green());	// 緑色
			break;

		case CGodItem::TYPE_BLUE:
			pItem->SetAllMaterial(material::Blue());	// 青色
			break;

		default:
			assert(false);
			break;
		}

		// 種類を設定する
		pItem->m_type = type;

		// 取得状況を false にする
		m_aGet[pItem->m_type] = false;

		// 本体から発する光のエフェクトを割当
		pItem->m_pEffectBody = GET_EFFECT->Create("data\\EFFEKSEER\\magatama_light.efkefc", rPos + EFFECT_OFFSET, VEC3_ZERO, VEC3_ZERO, 30.0f, false, false);

		// 地面から発する光のエフェクトを割当
		D3DXVECTOR3 posThis = rPos;
		posThis.y = GET_STAGE->GetFieldDownPositionHeight(posThis);	// 地面の着地位置の取得
		pItem->m_pEffectLand = GET_EFFECT->Create("data\\EFFEKSEER\\aura.efkefc", posThis, VEC3_ZERO, VEC3_ZERO, 50.0f, false, false);

		// 確保したアドレスを返す
		return pItem;
	}
}

//============================================================
// リスト構造の取得処理
//============================================================
CListManager<CGodItem>* CGodItem::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
//	回転移動状態の目標位置設定処理
//============================================================
void CGodItem::SetRollPosition(void)
{
	// 総数が全種類以上の場合エラー
	if (m_pList->GetNumAll() > TYPE_MAX) { assert(false); }

	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 posDest = posPlayer + ROLL_DEST_POS;		// 移動目標位置

	for (auto& item : m_pList->GetList())
	{
		if (item->m_state == STATE_ROLL)
		{ // 回転状態の場合

			// 現在位置を反映
			item->SetVec3Position(posDest);

			if (item->m_pEffectBody != nullptr)
			{
				// エフェクト位置を反映
				item->m_pEffectBody->m_pos = item->GetVec3Position() + EFFECT_OFFSET;
			}
		}
	}
}

//============================================================
// 当たり判定処理
//============================================================
bool CGodItem::Collision
(
	const D3DXVECTOR3& rPos,	// 位置
	const float fRadius			// 半径
)
{
	// アイテム状態ではない場合抜ける
	if (m_state != STATE_ITEM) { return false; }

	if (collision::Circle3D(rPos, GetVec3Position(), fRadius, COLLISION_RADIUS))
	{ // 当たった場合

		// 取得状況を true にする
		m_aGet[m_type] = true;

		// 神器取得音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_GETGODITEM_000);

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲームシーンの場合

			// ゲームマネージャーを神器獲得状態にする
			CSceneGame::GetGameManager()->PossessGodItem(m_type);
		}

		// 着地エフェクトの削除
		SAFE_DELETE(m_pEffectLand);

		// 回転移動状態にする
		m_state = STATE_ROLL;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// 初期位置(Y軸)の取得処理
//============================================================
float CGodItem::GetInitPosY(void) const
{
	// 初期位置を返す
	return m_fPosInitY;
}

//============================================================
// 種類の設定処理
//============================================================
void CGodItem::SetType(const EType type)
{
	// 種類を設定する
	m_type = type;
}

//============================================================
// 種類の取得処理
//============================================================
CGodItem::EType CGodItem::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CGodItem::LoadSetup(const char* pPass)
{
	int type = 0;					// 種類
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "神器セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

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
		else if (str == "STAGE_GODITEMSET")
		{
			do
			{ // END_STAGE_GODITEMSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "GODITEMSET")
				{
					do
					{ // END_GODITEMSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str == "POS")
						{
							file >> str;	// ＝を読込

							// 位置を読込
							file >> pos.x;
							file >> pos.y;
							file >> pos.z;
						}
						else if (str == "TYPE")
						{
							file >> str;	// ＝を読込

							// 種類を読込
							file >> type;
						}
					} while (str != "END_GODITEMSET");	// END_GODITEMSETを読み込むまでループ

					if (!m_aGet[type])
					{ // 勾玉を取得していない場合

						// 神器オブジェクトの生成
						if (CGodItem::Create(pos, (EType)type) == nullptr)
						{ // 確保に失敗した場合

							// 失敗を返す
							assert(false);
							return E_FAIL;
						}
					}
				}
			} while (str != "END_STAGE_GODITEMSET");	// END_STAGE_GODITEMSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
// 獲得状況の読込処理
//============================================================
HRESULT CGodItem::LoadPossess(const char* pPass, bool* pGet)
{
	// ファイルを開く
	std::ifstream file(pPass, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

#ifdef _DEBUG
		// エラーメッセージボックス
		MessageBox(nullptr, "神器獲得状況の読み込みに失敗！", "警告！", MB_ICONWARNING);
#endif

		// 獲得状況の保存
		bool aGet[TYPE_MAX] = {};
		SavePossess(pPass, &aGet[0]);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を書き出し
	file.read((char*)pGet, sizeof(bool) * TYPE_MAX);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
// 獲得状況の保存処理
//============================================================
HRESULT CGodItem::SavePossess(const char* pPass, bool* pGet)
{
	// ファイルを開く
	std::ofstream file(pPass, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "神器獲得状況の書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を書き出し
	file.write((char*)pGet, sizeof(bool) * TYPE_MAX);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
// 重複チェック処理
//============================================================
bool CGodItem::DuplicationCheck(const EType type)
{
	if (m_pList == nullptr)
	{ // 神器が一つも存在していない場合

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		if (m_pList->GetNumAll() >= TYPE_MAX)
		{ // 総数が全種類以上の場合

			// false を返す
			return false;
		}

		for (auto& item : m_pList->GetList())
		{
			if (item->m_type == type)
			{ // 種類が重複していた場合

				// false を返す
				return false;
			}
		}

		// true を返す
		return true;
	}
}

//============================================================
// 取得状況取得
//============================================================
bool CGodItem::IsGet(const EType type)
{
	// 取得状況を返す
	return m_aGet[type];
}

//============================================================
// 取得状況の初期化
//============================================================
void CGodItem::InitGet(void)
{
	for (int i = 0; i < TYPE_MAX; i++)
	{ // 種類の総数分繰り返す

		// 取得をOFFにする
		m_aGet[i] = false;
	}
}

//============================================================
//	神器の削除処理
//============================================================
void CGodItem::Delete(const EType type)
{
	// 総数が全種類以上の場合エラー
	if (m_pList->GetNumAll() > TYPE_MAX) { assert(false); }

	for (auto& item : m_pList->GetList())
	{
		if (item->m_type == type)
		{ // 種類が重複していた場合

			// 終了処理
			item->Uninit();
		}
	}
}

//============================================================
// 向き処理
//============================================================
void CGodItem::Cycle(void)
{
	// 向きを取得
	D3DXVECTOR3 rot = GetVec3Rotation();

	// 回転させる
	rot.y += ADD_ROT;

	// 向きの正規化
	useful::NormalizeRot(rot.y);

	// 向きを適用
	SetVec3Rotation(rot);
}

//============================================================
// 高さ設定処理
//============================================================
void CGodItem::Height(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 向きを加算する
	m_fHeightRot += ADD_ROT;

	// 向きの正規化
	useful::NormalizeRot(m_fHeightRot);

	// 高さを設定する
	pos.y = m_fPosInitY + (cosf(m_fHeightRot) * -ADD_HEIGHT) + ADD_HEIGHT;

	// 位置を適用
	SetVec3Position(pos);
}

//============================================================
//	回転移動の更新処理
//============================================================
void CGodItem::UpdateRoll(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GET_PLAYER->GetVec3Rotation();	// プレイヤー向き
	D3DXVECTOR3 posCur = GetVec3Position();	// 現在位置

	//--------------------------------------------------------
	//	位置の設定
	//--------------------------------------------------------
	// 目標位置を計算
	D3DXVECTOR3 posDest = posPlayer + D3DXVECTOR3
	(
		23.0f * sinf(rotPlayer.y - HALF_PI) + 14.0f * sinf(rotPlayer.y - D3DX_PI),
		118.0f,
		23.0f * cosf(rotPlayer.y - HALF_PI) + 14.0f * cosf(rotPlayer.y - D3DX_PI)
	);

	// 差分位置を計算
	D3DXVECTOR3 posDiff = posDest - posCur;

	// 現在位置を更新
	posCur += posDiff * 4.0f * fDeltaTime;

	// 現在位置を反映
	SetVec3Position(posCur);

	//--------------------------------------------------------
	//	拡大率の設定
	//--------------------------------------------------------
	D3DXVECTOR3 scaleCur = GetVec3Scaling();	// 現在拡大率

	// 差分拡大率を計算
	D3DXVECTOR3 scaleDiff = D3DXVECTOR3(0.75f, 0.75f, 0.75f) - scaleCur;

	// 現在拡大率を更新
	scaleCur += scaleDiff * 2.0f * fDeltaTime;

	// 現在拡大率を反映
	SetVec3Scaling(scaleCur);

	//--------------------------------------------------------
	//	向きの設定
	//--------------------------------------------------------
	// 向きを回転
	SetVec3Rotation(GetVec3Rotation() + ROLL_ROT * fDeltaTime);
}
