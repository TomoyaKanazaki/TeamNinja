//============================================================
//
//	神器処理 [godItem.cpp]
//	Author：小原立暉
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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\GODITEM\\Magatama.x";		// モデル
	const char* SETUP_TXT = "data\\TXT\\goditem.txt";			// セットアップテキスト相対パス
	const int PRIORITY = 4;		// 神器の優先順位
	const D3DXVECTOR3 EFFECT_OFFSET = D3DXVECTOR3(0.0f, 80.0f, 0.0f);	// エフェクト用オフセット

	const float ADD_ROT = 0.03f;		// 向きの追加量
	const float ADD_HEIGHT = 25.0f;		// 高さの追加量
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
m_fPosInitY(0.0f),		// 初期位置(Y軸)
m_type(TYPE_RED),		// 種類
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
	// 向き処理
	Cycle();

	// 高さ設定処理
	Height();

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
// 当たり判定処理
//============================================================
bool CGodItem::Collision
(
	const D3DXVECTOR3& rPos,	// 位置
	const float fRadius			// 半径
)
{
	if (collision::Circle3D(rPos, GetVec3Position(), fRadius, GetModelData().fRadius))
	{ // 当たった場合

		// 取得状況を true にする
		m_aGet[m_type] = true;

		// TODO : それっぽいエフェクトにする
		GET_EFFECT->Create("data\\EFFEKSEER\\check.efkefc", rPos + EFFECT_OFFSET, VEC3_ZERO, VEC3_ZERO, 30.0f);

		// 神器取得音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_GETGODITEM_000);

		// 終了処理
		Uninit();

		// true を返す
		return true;
	}

	// false を返す
	return false;
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

					// 神器オブジェクトの生成
					if (CGodItem::Create(pos, (EType)type) == nullptr)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
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
