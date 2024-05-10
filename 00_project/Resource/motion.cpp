//============================================================
//
//	モーション処理 [motion.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "motion.h"
#include "multiModel.h"
#include "objectChara.h"

//************************************************************
//	親クラス [CMotion] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMotion::CMotion() :
	m_ppModel	(nullptr),	// モデル情報
	m_pChara	(nullptr),	// オブジェクトキャラクター情報
	m_bUpdate	(true)		// 更新状況
{

}

//============================================================
//	デストラクタ
//============================================================
CMotion::~CMotion()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMotion::Init(void)
{
	// メンバ変数をクリア
	m_ppModel	= nullptr;	// モデル情報
	m_pChara	= nullptr;	// オブジェクトキャラクター情報
	m_bUpdate	= true;		// 更新状況

	// モーションを終了状態にする
	m_info.bFinish = true;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMotion::Uninit(void)
{
	for (auto& rMotionInfo : m_info.vecMotionInfo)
	{ // モーション情報の要素数分繰り返す

		for (auto& rKeyInfo : rMotionInfo.vecKeyInfo)
		{ // キー情報の要素数分繰り返す

			// キーをクリア
			rKeyInfo.vecKey.clear();
		}

		// キー情報をクリア
		rMotionInfo.vecKeyInfo.clear();
	}

	// モーション情報をクリア
	m_info.vecMotionInfo.clear();
}

//============================================================
//	更新処理
//============================================================
void CMotion::Update(const float fDeltaTime)
{
	if (!m_bUpdate) { return; }	// 更新しない
	if (m_info.vecMotionInfo[m_info.nType].GetNumKey() <= 0) { return; }	// キー数未設定

	if (m_blend.nFrame > 0)
	{ // ブレンドフレームが設定されている場合

		// ブレンドの更新
		UpdateBlend();
	}
	else
	{ // ブレンドフレームが設定されていない場合

		// モーションの更新
		UpdateMotion();
	}

	// 移動の更新
	UpdateMove();
}

//============================================================
//	パーツ情報の設定処理
//============================================================
void CMotion::BindPartsData(CMultiModel **ppModel)
{
	// 引数のモデル情報を設定
	m_ppModel = ppModel;
}

//============================================================
//	モーション情報全設定処理
//============================================================
void CMotion::SetAllInfo(const SInfo& rInfo)
{
	for (auto& rVec : rInfo.vecMotionInfo)
	{ // 読み込んだモーション数分繰り返す

		// モーション情報の追加
		AddInfo(rVec);
	}
}

//============================================================
//	モーション情報の追加処理
//============================================================
void CMotion::AddInfo(const SMotionInfo& rInfo)
{
	int nSetMotionID = m_info.GetNumMotion();	// モーションを設定する配列番号

	// 空の要素を最後尾に追加
	m_info.vecMotionInfo.emplace_back();

	// 引数のモーション情報を設定
	m_info.vecMotionInfo[nSetMotionID] = rInfo;

	// モーション全体フレーム数を設定
	int nSubKey = (m_info.vecMotionInfo[nSetMotionID].bLoop) ? 0 : 1;		// ループしない場合最後のキーは含まない
	int nLoop = m_info.vecMotionInfo[nSetMotionID].GetNumKey() - nSubKey;	// 繰り返し数を求める
	for (int nCntKey = 0; nCntKey < nLoop; nCntKey++)
	{ // キーの総数分繰り返す

		// キーのフレーム数を加算
		m_info.vecMotionInfo[nSetMotionID].nWholeFrame += m_info.vecMotionInfo[nSetMotionID].vecKeyInfo[nCntKey].nFrame;
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CMotion::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	パーツ数設定の設定処理
//============================================================
void CMotion::SetNumParts(const int nNumParts)
{
	// キーパーツ原点情報をクリア
	m_info.vecOriginKey.clear();

	// キーパーツ原点を生成
	m_info.vecOriginKey.resize((size_t)nNumParts);

	// ブレンド開始パーツ情報をクリア
	m_blend.vecKey.clear();

	// ブレンド開始パーツを生成
	m_blend.vecKey.resize((size_t)nNumParts);
}

//============================================================
//	設定処理
//============================================================
void CMotion::Set(const int nType, const int nBlendFrame)
{
	// 引数のモーションの種類を設定
	m_info.nType = nType;

	// 引数のブレンドフレームを設定
	m_blend.nFrame = nBlendFrame;

	// モーション情報を初期化
	m_info.nKey			 = 0;		// モーションキー番号
	m_info.nKeyCounter	 = 0;		// モーションキーカウンター
	m_info.nWholeCounter = 0;		// モーション全体カウンター
	m_info.bFinish		 = false;	// モーション終了状況

	// ブレンド情報を初期化
	m_blend.nWholeCounter = 0;		// ブレンド全体カウンター

	// パーツの位置の初期化
	if (m_blend.nFrame > 0)
	{ // ブレンドフレームが設定されている場合

		for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
		{ // パーツ数分繰り返す

			// 現在位置と現在向きを保存
			m_blend.vecKey[nCntKey].pos = m_ppModel[nCntKey]->GetVec3Position();
			m_blend.vecKey[nCntKey].rot = m_ppModel[nCntKey]->GetVec3Rotation();
		}
	}
	else
	{ // ブレンドフレームが設定されていない場合

		for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
		{ // パーツ数分繰り返す

			// 初期位置と初期向きを設定
			m_ppModel[nCntKey]->SetVec3Position(m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].vecKey[nCntKey].pos);
			m_ppModel[nCntKey]->SetVec3Rotation(m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].vecKey[nCntKey].rot);
		}
	}
}

//============================================================
//	原点位置の設定処理
//============================================================
void CMotion::SetOriginPosition(const D3DXVECTOR3& rPos, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // 使用可能なインデックスの場合

		// 原点位置を設定
		m_info.vecOriginKey[nParts].pos = rPos;
	}
	else { assert(false); }	// 使用不可
}

//============================================================
//	原点向きの設定処理
//============================================================
void CMotion::SetOriginRotation(const D3DXVECTOR3& rRot, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // 使用可能なインデックスの場合

		// 原点向きを設定
		m_info.vecOriginKey[nParts].rot = rRot;
	}
	else { assert(false); }	// 使用不可
}

//============================================================
//	種類取得処理
//============================================================
int CMotion::GetType(void) const
{
	// 現在のモーションの種類を返す
	return m_info.nType;
}

//============================================================
//	種類の総数取得処理
//============================================================
int CMotion::GetNumType(void)
{
	// モーションの種類の総数を返す
	return m_info.GetNumMotion();
}

//============================================================
//	キー番号取得処理
//============================================================
int CMotion::GetKey(void) const
{
	// 現在のキー番号を返す
	return m_info.nKey;
}

//============================================================
//	キーの総数取得処理
//============================================================
int CMotion::GetNumKey(const int nType)
{
	// 引数モーションのキーの総数を返す
	int nSubKey = (m_info.vecMotionInfo[m_info.GetNumMotion()].bLoop) ? 0 : 1;	// ループしない場合最後のキーは含まない
	return m_info.vecMotionInfo[nType].GetNumKey() - nSubKey;
}

//============================================================
//	モーションキーカウンター取得処理
//============================================================
int CMotion::GetKeyCounter(void) const
{
	// 現在のモーションキーカウンターを返す
	return m_info.nKeyCounter;
}

//============================================================
//	モーション全体カウンター取得処理
//============================================================
int CMotion::GetWholeCounter(void) const
{
	// 現在のモーション全体カウンターを返す
	return m_info.nWholeCounter;
}

//============================================================
//	モーション全体フレーム数取得処理
//============================================================
int CMotion::GetWholeFrame(const int nType) const
{
	// 引数モーションの全体フレーム数を返す
	return m_info.vecMotionInfo[nType].nWholeFrame;
}

//============================================================
//	モーションキャンセルフレーム取得処理
//============================================================
int CMotion::GetCancelFrame(const int nType) const
{
	// 引数モーションのキャンセルフレームを返す
	return m_info.vecMotionInfo[nType].nCancelFrame;
}

//============================================================
//	モーションコンボフレーム取得処理
//============================================================
int CMotion::GetComboFrame(const int nType) const
{
	// 引数モーションのコンボフレームを返す
	return m_info.vecMotionInfo[nType].nComboFrame;
}

//============================================================
//	終了取得処理
//============================================================
bool CMotion::IsFinish(void) const
{
	// 現在のモーションの終了状況を返す
	return m_info.bFinish;
}

//============================================================
//	ループ取得処理
//============================================================
bool CMotion::IsLoop(const int nType) const
{
	// 引数モーションのループのON/OFF状況を返す
	return m_info.vecMotionInfo[nType].bLoop;
}

//============================================================
//	キャンセル取得処理
//============================================================
bool CMotion::IsCancel(const int nType) const
{
	if (m_info.vecMotionInfo[nType].nCancelFrame != NONE_IDX)
	{ // キャンセルフレームが設定されている場合

		// 引数モーションのキャンセル状況を返す
		return (m_info.nWholeCounter >= m_info.vecMotionInfo[nType].nCancelFrame);
	}
	else
	{ // キャンセルフレームが設定されていない場合

		// キャンセル不可を返す
		return false;
	}
}

//============================================================
//	コンボ取得処理
//============================================================
bool CMotion::IsCombo(const int nType) const
{
	if (m_info.vecMotionInfo[nType].nComboFrame != NONE_IDX)
	{ // コンボフレームが設定されている場合

		// 引数モーションのコンボ状況を返す
		return (m_info.nWholeCounter >= m_info.vecMotionInfo[nType].nComboFrame);
	}
	else
	{ // コンボフレームが設定されていない場合

		// コンボ不可を返す
		return false;
	}
}

//============================================================
//	武器表示の取得処理
//============================================================
bool CMotion::IsWeaponDisp(const int nType) const
{
	// 引数モーションの武器表示のON/OFF状況を返す
	return m_info.vecMotionInfo[nType].bWeaponDisp;
}

//============================================================
//	左の攻撃判定フラグの取得処理
//============================================================
bool CMotion::IsLeftWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.vecMotionInfo[m_info.nType];	// 現在のモーション情報
	if (pMotionInfo->collLeft.nMin == NONE_IDX) { return false; }	// 開始カウント未設定
	if (pMotionInfo->collLeft.nMax == NONE_IDX) { return false; }	// 終了カウント未設定

	if (m_info.nWholeCounter >= pMotionInfo->collLeft.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collLeft.nMax)
	{ // カウンターが開始と終了の範囲内の場合

		return true;
	}

	return false;
}

//============================================================
//	右の攻撃判定フラグの取得処理
//============================================================
bool CMotion::IsRightWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.vecMotionInfo[m_info.nType];	// 現在のモーション情報
	if (pMotionInfo->collRight.nMin == NONE_IDX) { return false; }	// 開始カウント未設定
	if (pMotionInfo->collRight.nMax == NONE_IDX) { return false; }	// 終了カウント未設定

	if (m_info.nWholeCounter >= pMotionInfo->collRight.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collRight.nMax)
	{ // カウンターが開始と終了の範囲内の場合

		return true;
	}

	return false;
}

//============================================================
//	原点位置の取得処理
//============================================================
D3DXVECTOR3 CMotion::GetOriginPosition(const int nParts) const
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // 使用可能なインデックスの場合

		// 原点位置を返す
		return m_info.vecOriginKey[nParts].pos;
	}

	// インデックスエラー
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	原点向きの取得処理
//============================================================
D3DXVECTOR3 CMotion::GetOriginRotation(const int nParts) const
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // 使用可能なインデックスの場合

		// 原点向きを返す
		return m_info.vecOriginKey[nParts].rot;
	}

	// インデックスエラー
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	生成処理
//============================================================
CMotion *CMotion::Create(CObjectChara *pChara)
{
	// モーションの生成
	CMotion *pMotion = new CMotion;
	if (pMotion == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// モーションの初期化
		if (FAILED(pMotion->Init()))
		{ // 初期化に失敗した場合

			// モーションの破棄
			SAFE_DELETE(pMotion);
			return nullptr;
		}

		// オブジェクトキャラクターを設定
		pMotion->m_pChara = pChara;

		// 確保したアドレスを返す
		return pMotion;
	}
}

//============================================================
//	破棄処理
//============================================================
void CMotion::Release(CMotion *&prMotion)
{
	// モーションの終了
	assert(prMotion != nullptr);
	prMotion->Uninit();

	// メモリ開放
	SAFE_DELETE(prMotion);
}

//============================================================
//	移動の更新処理
//============================================================
void CMotion::UpdateMove(void)
{
	if (m_pChara == nullptr) { return; }	// オブジェクトキャラクター未設定

	// 変数を宣言
	D3DXMATRIX  mtxChara	= m_pChara->GetMtxWorld();				// キャラマトリックス
	D3DXVECTOR3 posSetChara	= m_pChara->GetVec3Position();			// キャラ設定位置
	D3DXVECTOR3 posOldChara	= useful::GetMatrixPosition(mtxChara);	// キャラ過去位置
	D3DXVECTOR3 posCurChara	= VEC3_ZERO;							// キャラ現在位置

	// 移動量を求める
	float fRate = 1.0f / (float)m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].nFrame;	// キーフレーム割合
	D3DXVECTOR3 moveRate = m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].move * fRate;	// フレーム移動量

	if (m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].nFrame > 0)
	{ // フレームが設定されている場合

		// 移動量をマトリックスに反映
		D3DXMATRIX mtxMove;	// マトリックス計算用
		D3DXMatrixTranslation(&mtxMove, moveRate.x, moveRate.y, moveRate.z);
		D3DXMatrixMultiply(&mtxChara, &mtxMove, &mtxChara);

		// 移動量を与えたマトリックスのワールド座標を求める
		posCurChara = useful::GetMatrixPosition(mtxChara);

		// 過去と現在の位置から移動量を求め、位置に与える
		posSetChara += posOldChara - posCurChara;

		// 位置を反映
		m_pChara->SetVec3Position(posSetChara);
	}
}

//============================================================
//	モーションの更新処理
//============================================================
void CMotion::UpdateMotion(void)
{
	int nType = m_info.nType;	// モーション種類
	int nKey  = m_info.nKey;	// モーションキー番号

	// 次のモーションキー番号を求める
	int nNextKey = (nKey + 1) % m_info.vecMotionInfo[nType].GetNumKey();

	// パーツの位置の更新
	for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
	{ // パーツ数分繰り返す

		// 位置・向きの差分を求める
		D3DXVECTOR3 diffPos = m_info.vecMotionInfo[nType].vecKeyInfo[nNextKey].vecKey[nCntKey].pos - m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].pos;
		D3DXVECTOR3 diffRot = m_info.vecMotionInfo[nType].vecKeyInfo[nNextKey].vecKey[nCntKey].rot - m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].rot;
		useful::NormalizeRot(diffRot);	// 差分向きの正規化

		// 現在のパーツの位置・向きを更新
		float fRate = (float)m_info.nKeyCounter / (float)m_info.vecMotionInfo[nType].vecKeyInfo[nKey].nFrame;	// キーフレーム割合
		m_ppModel[nCntKey]->SetVec3Position(m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.vecMotionInfo[nType].vecKeyInfo[nKey].vecKey[nCntKey].rot + diffRot * fRate);
	}

	// モーションの遷移の更新
	if (m_info.nKeyCounter < m_info.vecMotionInfo[nType].vecKeyInfo[nKey].nFrame)
	{ // 現在のキーの再生が終了していない場合

		// カウンターを加算
		m_info.nKeyCounter++;
		m_info.nWholeCounter++;
	}
	else
	{ // 現在のキーの再生が終了した場合

		// 次のキーに移行
		if (m_info.vecMotionInfo[nType].bLoop)
		{ // モーションがループする場合

			// キーカウンターを初期化
			m_info.nKeyCounter = 0;

			// キーカウントを加算
			m_info.nKey = (m_info.nKey + 1) % m_info.vecMotionInfo[nType].GetNumKey();	// 最大値で0に戻す

			if (m_info.nKey == 0)
			{ // キーが最初に戻った場合

				// 全体カウンターを初期化
				m_info.nWholeCounter = 0;
			}
		}
		else
		{ // モーションがループしない場合

			if (m_info.nKey < m_info.vecMotionInfo[nType].GetNumKey() - 2)
			{ // 現在のキーが最終のキーではない場合

				// キーカウンターを初期化
				m_info.nKeyCounter = 0;

				// キーカウントを加算
				m_info.nKey++;
			}
			else
			{ // 現在のキーが最終のキーの場合

				// モーションを終了状態にする
				m_info.bFinish = true;
			}
		}
	}
}

//============================================================
//	ブレンド更新処理
//============================================================
void CMotion::UpdateBlend(void)
{
	// パーツの位置の更新
	for (int nCntKey = 0; nCntKey < m_info.vecMotionInfo[m_info.nType].vecKeyInfo[m_info.nKey].GetNumParts(); nCntKey++)
	{ // パーツ数分繰り返す

		// 位置・向きの差分を求める
		D3DXVECTOR3 diffPos = m_info.vecMotionInfo[m_info.nType].vecKeyInfo[0].vecKey[nCntKey].pos - m_blend.vecKey[nCntKey].pos;
		D3DXVECTOR3 diffRot = m_info.vecMotionInfo[m_info.nType].vecKeyInfo[0].vecKey[nCntKey].rot - m_blend.vecKey[nCntKey].rot;
		useful::NormalizeRot(diffRot);	// 差分向きの正規化

		// 現在のパーツの位置・向きを更新
		float fRate = (float)m_blend.nWholeCounter / (float)m_blend.nFrame;	// キーフレーム割合
		m_ppModel[nCntKey]->SetVec3Position(m_blend.vecKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_blend.vecKey[nCntKey].rot + diffRot * fRate);
	}

	// モーションの遷移の更新
	if (m_blend.nWholeCounter < m_blend.nFrame)
	{ // ブレンドの再生が終了していない場合

		// カウンターを加算
		m_blend.nWholeCounter++;
	}
	else
	{ // ブレンドの再生が終了した場合

		// ブレンド再生フレーム数を初期化
		m_blend.nFrame = 0;

		// ブレンド全体カウンターを初期化
		m_blend.nWholeCounter = 0;
	}
}
