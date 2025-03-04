//============================================================
//
//	便利関数処理 [useful.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "useful.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "fog.h"
#include "camera.h"
#include <regex>

//************************************************************
//	通常関数
//************************************************************
//	便利関数空間
//************************************************************
//============================================================
//	数値の桁数ごとの分解
//============================================================
//	スコア表示の際などに使用
//============================================================
void useful::DivideDigitNum
(
	int *pNumDivide,	// 分解結果の格納配列
	const int nNum,		// 分解する数値
	const int nMaxDigit	// 分解する数字の桁数
)
{
	// 変数を宣言
	int nDigit = 1;	// 各桁の数値の計算用

	for (int nCntDigit = 0; nCntDigit < nMaxDigit; nCntDigit++)
	{ // 桁の最大数分繰り返す

		// 桁数分乗算する
		nDigit *= 10;
	}

	for (int nCntTex = nMaxDigit - 1; nCntTex >= 0; nCntTex--, pNumDivide++)
	{ // 桁数の最大から桁数の最小まで繰り返す

		// 桁数ごとの数値を求める
		*pNumDivide = (nNum % nDigit) / (nDigit / 10);

		// 除算して次の桁数に合わせる
		nDigit /= 10;
	}
}

//============================================================
//	法線の正規化
//============================================================
void useful::NormalizeNormal
(
	const D3DXVECTOR3& rPosLeft,	// 左位置
	const D3DXVECTOR3& rPosCenter,	// 中心位置
	const D3DXVECTOR3& rPosRight,	// 右位置
	D3DXVECTOR3& rNor				// 法線
)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine[2];	// 辺ベクトル

	// 辺ベクトルを求める
	vecLine[0] = rPosLeft - rPosCenter;
	vecLine[1] = rPosRight - rPosCenter;

	// 法線を計算
	rNor.x = (vecLine[0].y * vecLine[1].z) - (vecLine[0].z * vecLine[1].y);
	rNor.y = (vecLine[0].z * vecLine[1].x) - (vecLine[0].x * vecLine[1].z);
	rNor.z = (vecLine[0].x * vecLine[1].y) - (vecLine[0].y * vecLine[1].x);

	// 法線を正規化
	D3DXVec3Normalize(&rNor, &rNor);
}

//============================================================
//	文字列内の連続文字の置換
//============================================================
void useful::ReplaceConsecChar
(
	std::string *pDestStr,		// 置き換えを行う文字列
	const char cRepChar,		// 検出する文字
	const std::string& rRepStr,	// 置き換える文字列
	const int nRepStrLength		// 置き換える文字列の長さ (通常はstringのsize)

)
{
	int nStartID = 0;	// 文字検出の開始インデックス
	while (1)
	{ // 区切り文字が見つかった場合

		// 文字列内から引数の文字を検出
		int nFindID = pDestStr->find(cRepChar, nStartID);	// 検出インデックス

		// 引数の文字が検出されなかった場合ループを抜ける
		if ((size_t)nFindID == std::string::npos) { break; }

		// 置き換えの先頭まで文字列を削除する
		std::string sFindStr = *pDestStr;	// 置き換え文字列を代入
		sFindStr.erase(0, nFindID);			// 検出インデックスまで文字列削除

		int nCntConsec = 0;	// 文字連続カウンター
		for (char cChar : sFindStr)
		{ // 文字列の長さ分繰り返す

			if (cChar != cRepChar)
			{ // 検出文字と別の文字になった場合

				// 連続文字をすべて削除し、置き換え文字列を代入
				pDestStr->replace(nFindID, nCntConsec, rRepStr);

				// 文字検出の開始インデックスを設定
				nStartID = nFindID + nRepStrLength;

				break;
			}

			// 文字連続数を加算
			nCntConsec++;
		}
	}
}

//============================================================
//	経過時間・X移動量・重力から放物線の位置を求める処理
//============================================================
D3DXVECTOR2 useful::CalcPosParabola
(
	const float fGravity,	// 重力
	const float fMoveX,		// X移動量
	const float fDestPosX,	// 最大X座標
	const float fDestPosY,	// 最大Y座標
	const float fTime,		// 経過時間
	float *pMaxTime,		// 最大経過時間
	float *pMaxPosY			// 最大到達Y座標
)
{
	const float fMaxTime	= fDestPosX / fMoveX;	// 最大経過時間
	const float fHalfTime	= fMaxTime * 0.5f;		// 半分の最大経過時間
	const float fMoveY		= -0.5f * fGravity * fMaxTime;	// Y移動量
	const float fMaxPosY	= 0.5f * fGravity * (fHalfTime * fHalfTime) + fMoveY * fHalfTime;	// 最大到達Y座標

	if (pMaxTime != nullptr)
	{
		// 最大経過時間を保存
		*pMaxTime = fMaxTime;
	}
	if (pMaxPosY != nullptr)
	{
		// 最大到達Y座標を保存
		*pMaxPosY = fMaxPosY;
	}

	// 現在時間の位置を求める
	D3DXVECTOR2 posTime = VEC2_ZERO;
	posTime.x = fMoveX * fTime;
	posTime.y = (0.5f * fGravity * (fTime * fTime) + fMoveY * fTime) * (fDestPosY / fMaxPosY);

	// 現在時間の位置を返す
	return posTime;
}

//============================================================
//	経過時間・X移動量・重力から放物線の移動量を求める処理
//============================================================
D3DXVECTOR2 useful::CalcMoveParabola
(
	const float fGravity,	// 重力
	const float fMoveX,		// X移動量
	const float fDestPosX,	// 最大X座標
	const float fDestPosY,	// 最大Y座標
	const float fTime,		// 経過時間
	float *pMaxTime,		// 最大経過時間
	float *pMaxPosY			// 最大到達Y座標
)
{
	const float fMaxTime	= fDestPosX / fMoveX;	// 最大経過時間
	const float fHalfTime	= fMaxTime * 0.5f;		// 半分の最大経過時間
	const float fMoveY		= -0.5f * fGravity * fMaxTime;	// Y移動量
	const float fMaxPosY	= 0.5f * fGravity * (fHalfTime * fHalfTime) + fMoveY * fHalfTime;	// 最大到達Y座標

	if (pMaxTime != nullptr)
	{
		// 最大経過時間を保存
		*pMaxTime = fMaxTime;
	}
	if (pMaxPosY != nullptr)
	{
		// 最大到達Y座標を保存
		*pMaxPosY = fMaxPosY;
	}

	// 現在時間の移動量を求める
	D3DXVECTOR2 moveTime = VEC2_ZERO;
	moveTime.x = fMoveX;
	moveTime.y = (fGravity * fTime + fMoveY) * (fDestPosY / fMaxPosY);

	// 現在時間の移動量を返す
	return moveTime;
}

//============================================================
//	二次曲線処理
//============================================================
float useful::QuadraticCurve
(
	const float fPosX,			// 位置(X軸)
	const float fStart,			// 出発地点
	const float fEnd,			// 終了地点
	const float fHeight			// 最高高度
)
{
	// 変数を宣言
	float fPosY = 0.0f;		// Y座標
	float fMax = (fEnd + fStart) * 0.5f;		// 最高到達点のX座標
	float fMagni = -fHeight / (((fEnd - fStart) * 0.5f) * ((fEnd - fStart) * 0.5f));	// 倍率

	// Y座標を設定する
	fPosY = fMagni * ((fPosX - fMax) * (fPosX - fMax)) + fHeight;

	// Y座標を返す
	return fPosY;
}

//============================================================
//	放物線処理
//============================================================
void useful::Parabola
( // 引数
	const float fVelo,			// 初速
	const float fAdd,			// 重力
	const int nCount,			// 経過時間
	float* pGravity,			// 重力
	float* pHeight				// 高さ
)
{
	// 重力を設定する
	*pGravity = fVelo + fAdd * nCount;

	// 高さを設定する
	*pHeight = (fVelo * nCount) + (0.5f * fAdd * (nCount * nCount));
}

//============================================================
//	ランダム向き取得
//============================================================
float useful::RandomRot(void)
{
	// ランダム向きを返す
	return (float)(rand() % 629 - 314) * 0.01f;
}

//============================================================
//	向きの四方向変換
//============================================================
EAngle useful::RotToFourDire(float fRot)
{
	// 向きを0〜2πにする
	fRot += D3DX_PI;

	if (fRot >= QRTR_PI && fRot <= D3DX_PI - QRTR_PI)
	{
		// 右を返す
		return ANGLE_90;
	}
	else if (fRot >= D3DX_PI - QRTR_PI && fRot <= D3DX_PI + QRTR_PI)
	{
		// 手前を返す
		return ANGLE_0;
	}
	else if (fRot >= D3DX_PI + QRTR_PI && fRot <= (D3DX_PI * 2) - QRTR_PI)
	{
		// 左を返す
		return ANGLE_270;
	}
	else if (fRot >= (D3DX_PI * 2) - QRTR_PI && fRot <= (D3DX_PI * 2) || fRot >= 0.0f && fRot <= QRTR_PI)
	{
		// 奥を返す
		return ANGLE_180;
	}

	// 初期値を返す
	assert(false);	// 向きの正規化ミス
	return (EAngle)0;
}

//============================================================
//	アングルの向き変換
//============================================================
float useful::FourDireToRot(const EAngle angle)
{
	// 角度を取得
	float fRot = 0.0f;

	switch (angle)
	{
	case EAngle::ANGLE_0: // 0
		fRot = D3DX_PI * 0.0f;
		break;

	case EAngle::ANGLE_90: // 1.57
		fRot = D3DX_PI * 0.5f;
		break;

	case EAngle::ANGLE_180: // 3.14
		fRot = D3DX_PI * 1.0f;
		break;

	case EAngle::ANGLE_270: // 4.71
		fRot = D3DX_PI * 1.5f;
		break;

	default:
		assert(false);
		break;
	}

	// 向きを返す
	return fRot;
}

//============================================================
//	向きの正規化
//============================================================
void useful::NormalizeRot(float& rRot)
{
	if (rRot > D3DX_PI)
	{ // 向きが 3.14 を超えた場合

		// 向きの正規化
		rRot -= D3DX_PI * 2.0f;
	}
	else if (rRot < -D3DX_PI)
	{ // 向きが -3.14 を超えた場合

		// 向きの正規化
		rRot += D3DX_PI * 2.0f;
	}
}

//============================================================
//	三軸向きの正規化
//============================================================
void useful::NormalizeRot(D3DXVECTOR3& rRot)
{
	// 全ての向きを正規化
	NormalizeRot(rRot.x);
	NormalizeRot(rRot.y);
	NormalizeRot(rRot.z);
}

//============================================================
//	パスのベースネーム変換
//============================================================
void useful::PathToBaseName(std::string *pPath)
{
	std::filesystem::path fsPath(*pPath);	// パス

	// パスからベースネームを取得
	*pPath = fsPath.stem().string();
}

//============================================================
//	パス区切りの標準化
//============================================================
void useful::StandardizePathPart(std::string *pPath)
{
	// スラッシュを置換
	ReplaceConsecChar
	( // 引数
		pPath,	// 置き換えを行う文字列
		'/',	// 検出する文字
		"\\",	// 置き換える文字列
		1		// 置き換える文字列の長さ (通常文字と違い\は二つで一文字となる)
	);

	// バックスラッシュを置換
	ReplaceConsecChar
	( // 引数
		pPath,	// 置き換えを行う文字列
		'\\',	// 検出する文字
		"\\",	// 置き換える文字列
		1		// 置き換える文字列の長さ (通常文字と違い\は二つで一文字となる)
	);
}

//============================================================
//	マルチバイト文字列のワイド文字列変換
//============================================================
std::wstring useful::MultiByteToWide(const std::string &rSrcStr)
{
	int nSrcSize = (int)rSrcStr.size();	// 変換前の文字列のサイズ
	if (nSrcSize <= 0) { return L""; }	// 文字列がない場合抜ける

	// 文字列を変換
	std::wstring wsDest(nSrcSize, L'\0');	// 変換後の文字列
	int nDestSize = MultiByteToWideChar
	( // 引数
		CP_ACP,				// 変換コードページ
		0,					// 変換フラグ
		&rSrcStr.front(),	// 変換前文字列の先頭アドレス
		nSrcSize,			// 変換前文字列のサイズ
		&wsDest.front(),	// 変換後文字列の先頭アドレス
		(int)wsDest.size()	// 変換後文字列のサイズ
	);

	// 文字列サイズを修正
	wsDest.resize(nDestSize);

	// 変換後の文字列を返す
	return wsDest;
}

//============================================================
//	ワイド文字列のマルチバイト文字列変換
//============================================================
std::string useful::WideToMultiByte(const std::wstring &rSrcStr)
{
	int nSrcSize = (int)rSrcStr.size();	// 変換前の文字列のサイズ
	if (nSrcSize <= 0) { return ""; }	// 文字列がない場合抜ける

	// 変換後の文字列サイズを取得
	int nDestSize = WideCharToMultiByte
	( // 引数
		CP_ACP,				// 変換コードページ
		0,					// 変換フラグ
		&rSrcStr.front(),	// 変換前文字列の先頭アドレス
		nSrcSize,			// 変換前文字列のサイズ
		nullptr,			// 変換後文字列の先頭アドレス
		0,					// 変換後文字列のサイズ
		nullptr,			// 変換不可時の置換文字
		nullptr				// 変換不可な文字が存在したか
	);

	// 文字列を変換
	std::string sDest(nDestSize, '\0');	// 変換後の文字列
	WideCharToMultiByte
	( // 引数
		CP_ACP,				// 変換コードページ
		0,					// 変換フラグ
		&rSrcStr.front(),	// 変換前文字列の先頭アドレス
		nSrcSize,			// 変換前文字列のサイズ
		&sDest.front(),		// 変換後文字列の先頭アドレス
		(int)sDest.size(),	// 変換後文字列のサイズ
		nullptr,			// 変換不可時の置換文字
		nullptr				// 変換不可な文字が存在したか
	);

	// 変換後の文字列を返す
	return sDest;
}

//============================================================
//	ベクトルの向き変換
//============================================================
void useful::VecToRot(const D3DXVECTOR3& rVec, float *pPhi, float *pTheta)
{
	// 方位角の計算
	*pPhi = atan2f(rVec.y, rVec.x);

	// 仰角の計算
	*pTheta = atan2f(sqrtf((rVec.x * rVec.x) + (rVec.y * rVec.y)), rVec.z);
}

//============================================================
//	ベクトルの向き変換(丹野)
//============================================================
D3DXVECTOR3 useful::VectorToAngles(const D3DXVECTOR3& vector)
{
	D3DXVECTOR3 angles;

	// Yaw（ヨー）を計算
	angles.y = atan2f(vector.x, vector.z);

	// Pitch（ピッチ）を計算
	angles.x = atan2f(vector.y, sqrt(vector.x * vector.x + vector.z * vector.z));

	// Roll（ロール）は0度に設定
	angles.z = 0.0f;

	return angles;
}

//============================================================
//	向きのベクトル変換
//============================================================
void useful::RotToVec(const float fPhi, const float fTheta, D3DXVECTOR3 *pVec)
{
	// ベクトルの計算
	pVec->x = sinf(fTheta) * cosf(fPhi);
	pVec->y = sinf(fTheta) * sinf(fPhi);
	pVec->z = cosf(fTheta);
}

//============================================================
//	テクスチャのアスペクト比の取得処理
//============================================================
D3DXVECTOR3 useful::GetTexAspect(const int nTexID)
{
	// テクスチャアスペクト比を取得
	D3DXVECTOR2 aspect = GET_MANAGER->GetTexture()->GetInfo(nTexID).aspect;

	// アスペクト比を3D座標に変換
	D3DXVECTOR3 aspect3 = D3DXVECTOR3(aspect.x, aspect.y, 0.0f);

	// アスペクト比から計算した横幅を返す
	return aspect3;
}

//============================================================
//	縦幅からテクスチャのアスペクト比を考慮した横幅の取得処理
//============================================================
float useful::GetTexWidthFromAspect(const float fHeight, const int nTexID)
{
	// テクスチャアスペクト比を取得
	D3DXVECTOR2 aspect = GET_MANAGER->GetTexture()->GetInfo(nTexID).aspect;

	// アスペクト比から計算した横幅を返す
	return fHeight * aspect.x;
}

//============================================================
//	横幅からテクスチャのアスペクト比を考慮した縦幅の取得処理
//============================================================
float useful::GetTexHeightFromAspect(const float fWidth, const int nTexID)
{
	// テクスチャアスペクト比を取得
	D3DXVECTOR2 aspect = GET_MANAGER->GetTexture()->GetInfo(nTexID).aspect;

	// アスペクト比から計算した縦幅を返す
	return fWidth * aspect.y;
}

//==========================================
//  マトリックスを考慮した頂点の取得
//==========================================
D3DXVECTOR3 useful::GetMatrixPoint(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// 変数を宣言
	D3DXMATRIX mtx = rMtx; // ワールドマトリックス
	D3DXMATRIX  mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, rScale.x, rScale.y, rScale.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rRot.y, rRot.x, rRot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, rPos.x, rPos.y, rPos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点情報を抽出して返す
	return D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

//============================================================
//	マトリックスの位置取得
//============================================================
D3DXVECTOR3 useful::GetMatrixPosition(const D3DXMATRIX&rMtx)
{
	// マトリックスの位置を返す
	return D3DXVECTOR3(rMtx._41, rMtx._42, rMtx._43);
}

//============================================================
//	マトリックスの向き取得
//============================================================
D3DXVECTOR3 useful::GetMatrixRotation(const D3DXMATRIX& rMtx)
{
	float fYaw, fPitch, fRoll;	// 計算結果の保存用
	float fCosPitch;			// 向き計算用

	// マトリックスからPitchを求める
	fPitch = asinf(-rMtx._32);

	// マトリックスからYaw・Rollを求める
	fCosPitch = cosf(fPitch);
	if (fabs(fCosPitch) > 0.0001f)
	{ // Pitchの角度が計算に問題ない場合

		fYaw  = atan2f(rMtx._31 / fCosPitch, rMtx._33 / fCosPitch);
		fRoll = atan2f(rMtx._12 / fCosPitch, rMtx._22 / fCosPitch);
	}
	else
	{ // Pitchが90度または-90度の場合

		fYaw  = 0.0f;	// 正確な値が取れないので0.0fとする
		fRoll = atan2f(rMtx._21, rMtx._11);
	}

	// マトリックスの向きを返す
	return D3DXVECTOR3(fPitch, fYaw, fRoll);
}

//============================================================
//	マトリックスの拡大率取得
//============================================================
D3DXVECTOR3 useful::GetMatrixScaling(const D3DXMATRIX& rMtx)
{
	D3DXVECTOR3 scale;	// 計算結果の保存用

	// マトリックスから拡大率を求める
	scale.x = sqrtf(rMtx._11 * rMtx._11 + rMtx._12 * rMtx._12 + rMtx._13 * rMtx._13);
	scale.y = sqrtf(rMtx._21 * rMtx._21 + rMtx._22 * rMtx._22 + rMtx._23 * rMtx._23);
	scale.z = sqrtf(rMtx._31 * rMtx._31 + rMtx._32 * rMtx._32 + rMtx._33 * rMtx._33);

	// マトリックスの拡大率を返す
	return scale;
}

//==========================================
//  プレイヤーとの距離が遠い判定
//==========================================
bool useful::IsNearPosR(const D3DXVECTOR3& pos)
{
	// プレイヤー座標を取得
	D3DXVECTOR3 posPlayer = GET_CAMERA->GetPositionR();

	// フォグの距離を取得
	float fFar = Fog::GetEnd();

	// プレイヤーとの距離を算出
	D3DXVECTOR3 vec = pos - posPlayer;

	return fFar * fFar > vec.x * vec.x + vec.z * vec.z;
}

//************************************************************
//	マテリアル空間
//************************************************************
//============================================================
//	白色マテリアル取得
//============================================================
D3DXMATERIAL material::White(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を白色にする
	mat.MatD3D.Diffuse = XCOL_WHITE;

	// マテリアルを返す
	return mat;
}

//============================================================
//	赤色マテリアル取得
//============================================================
D3DXMATERIAL material::Red(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を赤色にする
	mat.MatD3D.Diffuse = XCOL_RED;

	// マテリアルを返す
	return mat;
}

//============================================================
//	緑色マテリアル取得
//============================================================
D3DXMATERIAL material::Green(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を緑色にする
	mat.MatD3D.Diffuse = XCOL_GREEN;

	// マテリアルを返す
	return mat;
}

//============================================================
//	青色マテリアル取得
//============================================================
D3DXMATERIAL material::Blue(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を青色にする
	mat.MatD3D.Diffuse = XCOL_BLUE;

	// マテリアルを返す
	return mat;
}

//============================================================
//	黄色マテリアル取得
//============================================================
D3DXMATERIAL material::Yellow(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を黄色にする
	mat.MatD3D.Diffuse = XCOL_YELLOW;

	// マテリアルを返す
	return mat;
}

//============================================================
//	水色マテリアル取得
//============================================================
D3DXMATERIAL material::Cyan(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を水色にする
	mat.MatD3D.Diffuse = XCOL_CYAN;

	// マテリアルを返す
	return mat;
}

//============================================================
//	灰色マテリアル取得
//============================================================
D3DXMATERIAL material::Gray(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を灰色にする
	mat.MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	// マテリアルを返す
	return mat;
}

//============================================================
//	ダメージ赤色マテリアル取得
//============================================================
D3DXMATERIAL material::DamageRed(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を設定
	mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.85f);

	// マテリアルの放射光を設定
	mat.MatD3D.Emissive = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

	// マテリアルを返す
	return mat;
}

//============================================================
//	発光緑色マテリアル取得
//============================================================
D3DXMATERIAL material::GlowGreen(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を緑色にする
	mat.MatD3D.Diffuse = XCOL_GREEN;

	// マテリアルの放射光を緑色にする
	mat.MatD3D.Emissive = XCOL_GREEN;

	// マテリアルを返す
	return mat;
}

//============================================================
//	発光水色マテリアル取得
//============================================================
D3DXMATERIAL material::GlowCyan(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を水色にする
	mat.MatD3D.Diffuse = XCOL_CYAN;

	// マテリアルの放射光を水色にする
	mat.MatD3D.Emissive = XCOL_CYAN;

	// マテリアルを返す
	return mat;
}

//************************************************************
//	2整数座標の構造体メンバ関数
//************************************************************
//============================================================
//	演算子のオーバーロード (比較演算子 ==)
//============================================================
bool POSGRID2::operator==(const POSGRID2& grid) const
{
	if (grid.x == x && grid.y == y)
	{ // 方眼座標がすべて同じ場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	演算子のオーバーロード (比較演算子 !=)
//============================================================
bool POSGRID2::operator!=(const POSGRID2& grid) const
{
	if (grid.x != x || grid.y != y)
	{ // 方眼座標が一つでも違う場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	演算子のオーバーロード (加算演算子 +)
//============================================================
POSGRID2 POSGRID2::operator+(const POSGRID2& grid) const
{
	POSGRID2 tempGrid = *this;	// 加算結果の代入用

	// 引数の整数座標を加算
	tempGrid.x += grid.x;
	tempGrid.y += grid.y;

	// 加算結果を返す
	return tempGrid;
}

//============================================================
//	演算子のオーバーロード (減算演算子 -)
//============================================================
POSGRID2 POSGRID2::operator-(const POSGRID2& grid) const
{
	POSGRID2 tempGrid = *this;	// 減算結果の代入用

	// 引数の整数座標を減算
	tempGrid.x -= grid.x;
	tempGrid.y -= grid.y;

	// 減算結果を返す
	return tempGrid;
}

//============================================================
//	演算子のオーバーロード (乗算演算子 *)
//============================================================
POSGRID2 POSGRID2::operator*(const int nVal) const
{
	POSGRID2 tempGrid = *this;	// 乗算結果の代入用

	// 引数の整数座標を乗算
	tempGrid.x *= nVal;
	tempGrid.y *= nVal;

	// 乗算結果を返す
	return tempGrid;
}

//============================================================
//	演算子のオーバーロード (除算演算子 /)
//============================================================
POSGRID2 POSGRID2::operator/(const int nVal) const
{
	POSGRID2 tempGrid = *this;	// 除算結果の代入用

	// 引数の整数座標を除算
	tempGrid.x /= nVal;
	tempGrid.y /= nVal;

	// 除算結果を返す
	return tempGrid;
}

//************************************************************
//	3整数座標の構造体メンバ関数
//************************************************************
//============================================================
//	演算子のオーバーロード (比較演算子 ==)
//============================================================
bool POSGRID3::operator==(const POSGRID3& grid) const
{
	if (grid.x == x && grid.y == y && grid.z == z)
	{ // 方眼座標がすべて同じ場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	演算子のオーバーロード (比較演算子 !=)
//============================================================
bool POSGRID3::operator!=(const POSGRID3& grid) const
{
	if (grid.x != x || grid.y != y || grid.z != z)
	{ // 方眼座標が一つでも違う場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	演算子のオーバーロード (加算演算子 +)
//============================================================
POSGRID3 POSGRID3::operator+(const POSGRID3& grid) const
{
	POSGRID3 tempGrid = *this;	// 加算結果の代入用

	// 引数の整数座標を加算
	tempGrid.x += grid.x;
	tempGrid.y += grid.y;
	tempGrid.z += grid.z;

	// 加算結果を返す
	return tempGrid;
}

//============================================================
//	演算子のオーバーロード (減算演算子 -)
//============================================================
POSGRID3 POSGRID3::operator-(const POSGRID3& grid) const
{
	POSGRID3 tempGrid = *this;	// 減算結果の代入用

	// 引数の整数座標を減算
	tempGrid.x -= grid.x;
	tempGrid.y -= grid.y;
	tempGrid.z -= grid.z;

	// 減算結果を返す
	return tempGrid;
}

//============================================================
//	演算子のオーバーロード (乗算演算子 *)
//============================================================
POSGRID3 POSGRID3::operator*(const int nVal) const
{
	POSGRID3 tempGrid = *this;	// 乗算結果の代入用

	// 引数の整数座標を乗算
	tempGrid.x *= nVal;
	tempGrid.y *= nVal;
	tempGrid.z *= nVal;

	// 乗算結果を返す
	return tempGrid;
}

//============================================================
//	演算子のオーバーロード (除算演算子 /)
//============================================================
POSGRID3 POSGRID3::operator/(const int nVal) const
{
	POSGRID3 tempGrid = *this;	// 除算結果の代入用

	// 引数の整数座標を除算
	tempGrid.x /= nVal;
	tempGrid.y /= nVal;
	tempGrid.z /= nVal;

	// 除算結果を返す
	return tempGrid;
}
