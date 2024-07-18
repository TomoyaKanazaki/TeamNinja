////============================================================
////
////	追跡敵処理 [enemyStalk.cpp]
////	Author：小原立暉
////
////============================================================
////************************************************************
////	インクルードファイル
////************************************************************
//#include "manager.h"
//#include "enemyAmbush.h"
//#include "renderer.h"
//#include "deltaTime.h"
//
//#include "multiModel.h"
//#include "enemy_item.h"
//#include "enemyChaseRange.h"
//
////************************************************************
////	定数宣言
////************************************************************
//namespace
//{
//	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// セットアップテキスト相対パス
//	const int	BLEND_FRAME_OTHER = 5;		// モーションの基本的なブレンドフレーム
//	const int	BLEND_FRAME_UPSET = 8;		// モーション動揺のブレンドフレーム
//	const int	BLEND_FRAME_LAND = 15;		// モーション着地のブレンドフレーム
//	const int	CAUTIOUS_TRANS_LOOP = 7;	// 警戒モーションに遷移する待機ループ数
//	const float	RADIUS = 20.0f;				// 半径
//	const float HEIGHT = 80.0f;				// 身長
//	const float SPEED = -290.0f;			// 速度
//	const float ROT_REV = 4.0f;				// 向きの補正係数
//	const int UPSET_STATE_COUNT = 3;		// 動揺状態のカウント数
//	const int CAUTION_STATE_COUNT = 180;	// 警戒状態のカウント数
//	const float FADE_ALPHA_TRANS = 0.02f;	// フェードの透明度の遷移定数
//
//	const int ITEM_PART_NUMBER = 8;			// アイテムを持つパーツの番号
//	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// アイテムのオフセット座標
//	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// アイテムの向き
//}
//
////************************************************************
////	子クラス [CEnemyAmbush] のメンバ関数
////************************************************************
////============================================================
////	コンストラクタ
////============================================================
//CEnemyAmbush::CEnemyAmbush() : CEnemyAttack(),
//m_state(STATE_WAIT),
//m_nStateCount(0),
//m_nNumUpsetLoop(0)
//{
//
//}
//
////============================================================
////	デストラクタ
////============================================================
//CEnemyAmbush::~CEnemyAmbush()
//{
//
//}
//
////============================================================
////	初期化処理
////============================================================
//HRESULT CEnemyAmbush::Init(void)
//{
//	// 敵の初期化
//	if (FAILED(CEnemyAttack::Init()))
//	{ // 初期化に失敗した場合
//
//		// 失敗を返す
//		assert(false);
//		return E_FAIL;
//	}
//
//	// キャラクター情報の割当
//	BindCharaData(SETUP_TXT);
//
//	// 成功を返す
//	return S_OK;
//}
//
////============================================================
////	終了処理
////============================================================
//void CEnemyAmbush::Uninit(void)
//{
//	// 敵の終了
//	CEnemyAttack::Uninit();
//}
//
////============================================================
////	更新処理
////============================================================
//void CEnemyAmbush::Update(const float fDeltaTime)
//{
//	// 敵の更新
//	CEnemyAttack::Update(fDeltaTime);
//}
//
////============================================================
////	描画処理
////============================================================
//void CEnemyAmbush::Draw(CShader* pShader)
//{
//	// 敵の描画
//	CEnemyAttack::Draw(pShader);
//}
//
////============================================================
//// 情報の設定処理
////============================================================
//void CEnemyAmbush::SetData(void)
//{
//	// アイテムを設定する
//	SetItem(CEnemyItem::Create
//	(
//		CEnemyItem::TYPE_KATANA,
//		ITEM_OFFSET,
//		ITEM_ROT
//	));
//
//	// 親オブジェクト (持ち手) の設定
//	GetItem()->SetParentObject(GetParts(ITEM_PART_NUMBER));
//}
//
////============================================================
//// 半径の取得処理
////============================================================
//float CEnemyAmbush::GetRadius(void) const
//{
//	// 半径を返す
//	return RADIUS;
//}
//
////============================================================
//// 高さの取得処理
////============================================================
//float CEnemyAmbush::GetHeight(void) const
//{
//	// 身長を返す
//	return HEIGHT;
//}
//
////============================================================
//// 状態の更新処理
////============================================================
//int CEnemyAmbush::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
//{
//	int nCurMotion = MOTION_IDOL;	// 現在のモーション
//	switch (m_state)
//	{
//	case CEnemyAmbush::STATE_WAIT:
//
//		// 待機処理
//		nCurMotion = Wait(pPos, pRot, fDeltaTime);
//
//		break;
//
//	case CEnemyAmbush::STATE_WARNING:
//
//		// 警告処理
//		nCurMotion = Warning(pPos);
//
//		break;
//
//	case CEnemyAmbush::STATE_STALK:
//
//		// 追跡処理
//		nCurMotion = Stalk(pPos, pRot, fDeltaTime);
//
//		break;
//
//	case CEnemyAmbush::STATE_ATTACK:
//
//		// 攻撃処理
//		nCurMotion = Attack(*pPos);
//
//		break;
//
//	case CEnemyAmbush::STATE_UPSET:
//
//		// 動揺処理
//		nCurMotion = Upset();
//
//		break;
//
//	case CEnemyAmbush::STATE_CAUTION:
//
//		// 警戒処理
//		nCurMotion = Caution();
//
//		break;
//
//	case CEnemyAmbush::STATE_FADEOUT:
//
//		// フェードアウト処理
//		nCurMotion = FadeOut(pPos, pRot);
//
//		break;
//
//	case CEnemyAmbush::STATE_FADEIN:
//
//		// フェードイン処理
//		nCurMotion = FadeIn();
//
//		break;
//
//	default:
//
//		// 停止
//		assert(false);
//
//		break;
//	}
//
//	// 当たり判定処理
//	Collision(*pPos);
//
//	// 現在のモーションを返す
//	return nCurMotion;
//}
//
////============================================================
//// モーションの更新処理
////============================================================
//void CEnemyAmbush::UpdateMotion(int nMotion, const float fDeltaTime)
//{
//	// 死んでたら抜ける
//	if (IsDeath()) { return; }
//
//	int nAnimMotion = GetMotionType();	// 現在再生中のモーション
//	if (nMotion != NONE_IDX)
//	{ // モーションが設定されている場合
//
//		if (IsMotionLoop())
//		{ // ループするモーション中の場合
//
//			if (nAnimMotion != nMotion)
//			{ // 現在のモーションが再生中のモーションと一致しない場合
//
//				// 現在のモーションの設定
//				SetMotion(nMotion, BLEND_FRAME_OTHER);
//			}
//		}
//	}
//
//	// オブジェクトキャラクターの更新
//	CObjectChara::Update(fDeltaTime);
//
//	// 現在のモーションの更新
//	switch (GetMotionType())
//	{
//	case CEnemyAmbush::MOTION_IDOL:		// 待機
//		break;
//
//	case CEnemyAmbush::MOTION_WALK:		// 歩行
//		break;
//
//	case CEnemyAmbush::MOTION_FOUND:		// 発見
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_ATTACK:	// 攻撃
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_UPSET:		// 動揺
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 動揺モーションのループ数を加算する
//			m_nNumUpsetLoop++;
//
//			if (m_nNumUpsetLoop < UPSET_STATE_COUNT)
//			{ // 動揺モーションのループ回数が一定数未満の場合
//
//				// 待機モーションの設定
//				SetMotion(MOTION_UPSET, BLEND_FRAME_UPSET);
//			}
//			else
//			{ // 上記以外
//
//				// 待機モーションの設定
//				SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);
//			}
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_FALL:		// 落下
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_LANDING:	// 着地
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	default:
//
//		// 停止
//		assert(false);
//
//		break;
//	}
//}
//
////============================================================
//// 着地の更新処理
////============================================================
//void CEnemyAmbush::UpdateLanding(D3DXVECTOR3* pPos)
//{
//	// 親クラスの着地更新
//	CEnemy::UpdateLanding(pPos);
//
//	// 現在のモーション種類を取得
//	int nCurMotion = GetMotionType();
//
//	//// 落下モーションのフラグを設定
//	//bool bTypeFall = nCurMotion == MOTION_FALL;
//
//	//if (!IsJump())
//	//{ // 空中にいない場合
//
//	//	if (bTypeFall)
//	//	{ // モーションが落下中の場合
//
//	//		// 着地モーションを指定
//	//		SetMotion(MOTION_LANDING);
//	//	}
//	//}
//	//else
//	//{ // 空中にいる場合
//
//	//	// 落下モーションを指定
//	//	SetMotion(MOTION_FALL);
//	//}
//}
//
////============================================================
//// 待機処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Wait(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
//{
//	EMotion motion = MOTION_IDOL;				// モーション
//
//	// 重力の更新
//	UpdateGravity();
//
//	// 敵を落下させる
//	pPos->y += GetMovePosition().y * fDeltaTime;
//
//	// 向きの移動処理
//	RotMove(*pRot, ROT_REV, fDeltaTime);
//
//	// 着地判定
//	UpdateLanding(pPos);
//
//	if (JudgeClone() ||
//		JudgePlayer())
//	{ // 分身かプレイヤーが目に入った場合
//
//		// 警告状態にする
//		m_state = STATE_WARNING;
//
//		// 発見モーションを返す
//		return MOTION_FOUND;
//	}
//
//	// 無対象にする
//	SetTarget(TARGET_NONE);
//
//	// 待機モーションを返す
//	return motion;
//}
//
////============================================================
//// 警告処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Warning(D3DXVECTOR3* pPos)
//{
//	// 重力の更新
//	UpdateGravity();
//
//	// 着地判定
//	UpdateLanding(pPos);
//
//	if (GetMotionType() != MOTION_FOUND)
//	{ // 発見モーションじゃなかった場合
//
//		// 追跡状態にする
//		m_state = STATE_STALK;
//	}
//
//	// 歩行状態を返す
//	return MOTION_WALK;
//}
//
////============================================================
//// 追跡処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
//{
//	if (ShakeOffClone() ||
//		ShakeOffPlayer())
//	{ // 分身かプレイヤーが目に入った場合
//
//		// 目標位置の視認処理
//		LookTarget(*pPos);
//
//		// 攻撃判定を false にする
//		SetEnableAttack(false);
//	}
//	else
//	{ // 上記以外
//
//		// 待機状態にする
//		m_state = STATE_WAIT;
//
//		// 待機モーションを返す
//		return MOTION_IDOL;
//	}
//
//	// 向きの移動処理
//	RotMove(*pRot, ROT_REV, fDeltaTime);
//
//	// 重力の更新
//	UpdateGravity();
//
//	// 移動処理
//	Move(pPos, *pRot, SPEED, fDeltaTime);
//
//	// 着地判定
//	UpdateLanding(pPos);
//
//	if (Approach(*pPos))
//	{ // 接近した場合
//
//		// 攻撃状態にする
//		m_state = STATE_ATTACK;
//
//		// 攻撃モーションを返す
//		return MOTION_ATTACK;
//	}
//
//	if (GetChaseRange() != nullptr &&
//		GetChaseRange()->ChaseRange(pPos))
//	{ // 追跡範囲から出た場合
//
//		// フェードアウト状態にする
//		m_state = STATE_FADEOUT;
//
//		// 移動量をリセットする
//		SetMovePosition(VEC3_ZERO);
//
//		// ターゲットを無対象にする
//		SetTarget(TARGET_NONE);
//	}
//
//	// 歩行モーションを返す
//	return MOTION_WALK;
//}
//
////============================================================
//// 攻撃処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Attack(const D3DXVECTOR3& rPos)
//{
//	switch (GetTarget())
//	{
//	case CEnemyAttack::TARGET_PLAYER:
//
//		// プレイヤーの当たり判定処理
//		HitPlayer(rPos);
//
//		if (GetMotionType() != MOTION_ATTACK)
//		{ // 攻撃モーションじゃない場合
//
//			// 待機状態にする
//			m_state = STATE_WAIT;
//		}
//
//		// 待機モーションにする
//		return MOTION_IDOL;
//
//		break;
//
//	case CEnemyAttack::TARGET_CLONE:
//
//		// 分身の当たり判定処理
//		HitClone(rPos);
//
//		// 動揺状態にする
//		m_state = STATE_UPSET;
//
//		// 動揺モーションにする
//		return MOTION_UPSET;
//
//		break;
//
//	default:		// 例外処理
//		assert(false);
//		break;
//	}
//
//	// 待機モーションを返す
//	return MOTION_IDOL;
//}
//
////============================================================
//// 動揺処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Upset(void)
//{
//	if (m_nNumUpsetLoop >= UPSET_STATE_COUNT)
//	{ // 一定時間経過した場合
//
//		// 警戒状態にする
//		m_state = STATE_CAUTION;
//
//		// 動揺モーションのループ数を0にする
//		m_nNumUpsetLoop = 0;
//
//		// 待機モーションにする
//		return MOTION_IDOL;
//	}
//
//	// 動揺モーションにする
//	return MOTION_UPSET;
//}
//
////============================================================
//// 警戒処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Caution(void)
//{
//	// 状態カウントを加算する
//	m_nStateCount++;
//
//	if (m_nStateCount % CAUTION_STATE_COUNT == 0)
//	{ // 状態カウントが一定数になった場合
//
//		// 状態カウントを0にする
//		m_nStateCount = 0;
//
//		// フェードアウト状態にする
//		m_state = STATE_FADEOUT;
//	}
//
//	if (JudgeClone() ||
//		JudgePlayer())
//	{ // 分身かプレイヤーが目に入った場合
//
//		// 状態カウントを0にする
//		m_nStateCount = 0;
//
//		// 警告状態にする
//		m_state = STATE_WARNING;
//
//		// 発見モーションを返す
//		return MOTION_FOUND;
//	}
//	else
//	{ // 上記以外
//
//		// 無対象にする
//		SetTarget(TARGET_NONE);
//	}
//
//	// TODO：待機モーションを返す
//	return MOTION_IDOL;
//}
//
////============================================================
//// フェードアウト処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
//{
//	// 透明度を取得
//	float fAlpha = GetAlpha();
//
//	// 透明度を減算する
//	fAlpha -= FADE_ALPHA_TRANS;
//
//	if (fAlpha <= 0.0f)
//	{ // 透明度が0以下になった場合
//
//		// フェードイン状態にする
//		m_state = STATE_FADEIN;
//
//		// 位置を設定する
//		*pPos = GetPosInit();
//
//		// 過去の位置を適用する(こうしないと当たり判定に引っかかってしまう)
//		SetOldPosition(*pPos);
//
//		// 向きを設定する
//		*pRot = VEC3_ZERO;
//
//		// 透明度を補正する
//		fAlpha = 0.0f;
//	}
//
//	// 透明度を適用
//	CObjectChara::SetAlpha(fAlpha);
//	CEnemyAttack::SetAlpha(fAlpha);
//
//	// 待機モーションにする
//	return MOTION_IDOL;
//}
//
////============================================================
//// フェードイン処理
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::FadeIn(void)
//{
//	// 透明度を取得
//	float fAlpha = GetAlpha();
//
//	// 透明度を減算する
//	fAlpha += FADE_ALPHA_TRANS;
//
//	if (fAlpha >= 1.0f)
//	{ // 透明度が一定数以上になった場合
//
//		// 待機状態にする
//		m_state = STATE_WAIT;
//
//		// 透明度を補正する
//		fAlpha = 1.0f;
//	}
//
//	// 透明度を適用
//	CObjectChara::SetAlpha(fAlpha);
//	CEnemyAttack::SetAlpha(fAlpha);
//
//	// 待機モーションにする
//	return MOTION_IDOL;
//}
