//============================================================
//
//	入力処理 [input.cpp]
//	Author：藤田勇一
//	adder : 金崎朋弥
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "input.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	namespace mouse
	{
		const int MAX_KEY = 4;	// マウスキーの最大数
	}

	namespace vibration
	{
		// 振動継続時間
		const int TIME[] =
		{
			0, // 何もしない状態
			100, // 死亡
			10, // 被弾
			30, // 橋
			5, // ジャンプ
			50, // ボタン
			1, // 分身
		};

		// 振動レベル
		const WORD LEVEL[] =
		{
			0, // 何もしない状態
			(WORD)(USHRT_MAX), // 死亡
			(WORD)(USHRT_MAX * 0.5f), // 被弾
			(WORD)(USHRT_MAX), // 橋
			(WORD)(USHRT_MAX * 0.5f), // ジャンプ
			(WORD)(USHRT_MAX * 0.7f), // ボタン
			(WORD)(USHRT_MAX * 0.1f), // 分身
		};
	}

	namespace stick
	{
		const int TRIGGER_DEAD = 5000; // トリガー取得の閾値
		const int INIT_VALUE = 1000; // スティック入力の初期値
	}
}

//===========================================
//  静的警告処理
//===========================================
static_assert(NUM_ARRAY(vibration::TIME) == CInputPad::TYPE_MAX, "ERROR : Type Count Mismatch");
static_assert(NUM_ARRAY(vibration::LEVEL) == CInputPad::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	静的メンバ変数宣言
//************************************************************
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInputオブジェクト
int CInput::m_nNumAll = 0;	// 入力デバイスの総数

//************************************************************
//	親クラス [CInput] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CInput::CInput() :
	m_pDevice(nullptr)	// DirectInputデバイス
{
	// 入力デバイスの総数を加算
	m_nNumAll++;
}

//============================================================
//	デストラクタ
//============================================================
CInput::~CInput()
{
	// 入力デバイスの総数を減算
	m_nNumAll--;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND /*hWnd*/)
{
	// メンバ変数を初期化
	m_pDevice = nullptr;	// DirectInputデバイス

	if (m_pInput == nullptr)
	{ // 入力デバイスが使用されていない場合

		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CInput::Uninit(void)
{
	// DirectInputデバイスの破棄
	if (m_pDevice != nullptr)
	{ // DirectInputデバイスが使用中の場合

		// アクセス権開放
		m_pDevice->Unacquire();

		// メモリ開放
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if (m_nNumAll <= 1)
	{ // 最後のDirectInputデバイスが破棄された場合

		// DirectInputオブジェクトの破棄
		SAFE_RELEASE(m_pInput);
	}
}

//************************************************************
//	子クラス [CInputKeyboard] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CInputKeyboard::CInputKeyboard()
{
	// メンバ変数をクリア
	memset(&m_aKeyStatePress[0],   0, sizeof(m_aKeyStatePress));	// マウスのプレス情報
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// マウスのトリガー情報
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// マウスのリリース情報
}

//============================================================
//	デストラクタ
//============================================================
CInputKeyboard::~CInputKeyboard()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// メンバ変数を初期化
	memset(&m_aKeyStatePress[0],   0, sizeof(m_aKeyStatePress));	// マウスのプレス情報
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// マウスのトリガー情報
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// マウスのリリース情報

	// 入力の初期化
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// DirectInputデバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{ // 設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{ // 設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CInputKeyboard::Uninit(void)
{
	// 入力の終了
	CInput::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CInputKeyboard::Update(void)
{
	// 変数配列を宣言
	BYTE aKeyState[keyboard::MAX_KEY];	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // 取得に成功した場合

		for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
		{ // キーの最大数分繰り返す

			// キーボードのリリース情報を保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// キーボードのトリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// キーボードのプレス情報を保存
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{ // 取得に失敗した場合

		// キーボードへのアクセス権を再度獲得
		m_pDevice->Acquire();
	}
}

//============================================================
//	プレス初期化処理
//============================================================
void CInputKeyboard::InitPress(void)
{
	// メモリをクリア
	memset(&m_aKeyStatePress, 0, sizeof(m_aKeyStatePress));
}

//============================================================
//	トリガー初期化処理
//============================================================
void CInputKeyboard::InitTrigger(void)
{
	// メモリをクリア
	memset(&m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
}

//============================================================
//	リリース初期化処理
//============================================================
void CInputKeyboard::InitRelease(void)
{
	// メモリをクリア
	memset(&m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//============================================================
//	プレス取得処理
//============================================================
bool CInputKeyboard::IsPress(int nKey)
{
	// 入力情報を返す
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//============================================================
//	トリガー取得処理
//============================================================
bool CInputKeyboard::IsTrigger(int nKey)
{
	// 入力情報を返す
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//============================================================
//	リリース取得処理
//============================================================
bool CInputKeyboard::IsRelease(int nKey)
{
	// 入力情報を返す
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//============================================================
//	全プレス取得処理
//============================================================
bool CInputKeyboard::IsAnyPress(void)
{
	for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
	{ // キーの最大数分繰り返す

		if (m_aKeyStatePress[nCntKey] & 0x80)
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	全トリガー取得処理
//============================================================
bool CInputKeyboard::IsAnyTrigger(void)
{
	for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
	{ // キーの最大数分繰り返す

		if (m_aKeyStateTrigger[nCntKey] & 0x80)
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	全リリース取得処理
//============================================================
bool CInputKeyboard::IsAnyRelease(void)
{
	for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
	{ // キーの最大数分繰り返す

		if (m_aKeyStateRelease[nCntKey] & 0x80)
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	生成処理
//============================================================
CInputKeyboard *CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// キーボードの生成
	CInputKeyboard *pKeyboard = new CInputKeyboard;
	if (pKeyboard == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// キーボードの初期化
		if (FAILED(pKeyboard->Init(hInstance, hWnd)))
		{ // 初期化に失敗した場合

			// キーボードの破棄
			SAFE_DELETE(pKeyboard);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pKeyboard;
	}
}

//============================================================
//	破棄処理
//============================================================
void CInputKeyboard::Release(CInputKeyboard *&prKeyboard)
{
	// キーボードの終了
	assert(prKeyboard != nullptr);
	prKeyboard->Uninit();

	// メモリ開放
	SAFE_DELETE(prKeyboard);
}

//************************************************************
//	子クラス [CInputMouse] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CInputMouse::CInputMouse()
{
	// メンバ変数をクリア
	memset(&m_keyStatePress,   0, sizeof(m_keyStatePress));		// プレス情報
	memset(&m_keyStateTrigger, 0, sizeof(m_keyStateTrigger));	// トリガー情報
	memset(&m_keyStateRelease, 0, sizeof(m_keyStateRelease));	// リリース情報
}

//============================================================
//	デストラクタ
//============================================================
CInputMouse::~CInputMouse()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 変数を宣言
	DIPROPDWORD diprop;	// デバイスの設定用

	// メンバ変数を初期化
	memset(&m_keyStatePress,   0, sizeof(m_keyStatePress));		// プレス情報
	memset(&m_keyStateTrigger, 0, sizeof(m_keyStateTrigger));	// トリガー情報
	memset(&m_keyStateRelease, 0, sizeof(m_keyStateRelease));	// リリース情報

	// 入力の初期化
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// DirectInputデバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{ // 設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{ // 設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// デバイスの設定
	diprop.diph.dwSize			= sizeof(diprop);
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph);
	diprop.diph.dwObj	= 0;
	diprop.diph.dwHow	= DIPH_DEVICE;
	diprop.dwData		= DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{ // デバイスの設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	m_pDevice->Acquire();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CInputMouse::Uninit(void)
{
	// 入力の終了
	CInput::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CInputMouse::Update(void)
{
	// 変数を宣言
	DIMOUSESTATE keyState;	// マウスの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(keyState), &keyState)))
	{ // 取得に成功した場合

		for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
		{ // マウスキーの最大数分繰り返す

			// マウスのリリース情報を保存
			m_keyStateRelease.rgbButtons[nCntMouKey] = (m_keyStatePress.rgbButtons[nCntMouKey] | keyState.rgbButtons[nCntMouKey]) ^ keyState.rgbButtons[nCntMouKey];

			// マウスのトリガー情報を保存
			m_keyStateTrigger.rgbButtons[nCntMouKey] = (m_keyStatePress.rgbButtons[nCntMouKey] ^ keyState.rgbButtons[nCntMouKey]) & keyState.rgbButtons[nCntMouKey];
		}

		// マウスのプレス情報を保存
		m_keyStatePress = keyState;
	}
	else
	{ // 取得に失敗した場合

		// マウスへのアクセス権を再度獲得
		m_pDevice->Acquire();
	}
}

//============================================================
//	プレス取得処理
//============================================================
bool CInputMouse::IsPress(EKey mouKey)
{
	// 入力情報を返す
	return (m_keyStatePress.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	トリガー取得処理
//============================================================
bool CInputMouse::IsTrigger(EKey mouKey)
{
	// 入力情報を返す
	return (m_keyStateTrigger.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	リリース取得処理
//============================================================
bool CInputMouse::IsRelease(EKey mouKey)
{
	// 入力情報を返す
	return (m_keyStateRelease.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	全プレス取得処理
//============================================================
bool CInputMouse::IsAnyPress(void)
{
	for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
	{ // マウスキーの最大数分繰り返す

		if (m_keyStatePress.rgbButtons[nCntMouKey] & 0x80)
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	全トリガー取得処理
//============================================================
bool CInputMouse::IsAnyTrigger(void)
{
	for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
	{ // マウスキーの最大数分繰り返す

		if (m_keyStateTrigger.rgbButtons[nCntMouKey] & 0x80)
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	全リリース取得処理
//============================================================
bool CInputMouse::IsAnyRelease(void)
{
	for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
	{ // マウスキーの最大数分繰り返す

		if (m_keyStateRelease.rgbButtons[nCntMouKey] & 0x80)
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	移動量取得処理
//============================================================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	// マウスの移動量を返す
	return D3DXVECTOR3
	(
		(float)m_keyStatePress.lX,	// 横移動量
		(float)m_keyStatePress.lY,	// 縦移動量
		(float)m_keyStatePress.lZ	// ホイール移動量
	);
}

//============================================================
//	生成処理
//============================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// マウスの生成
	CInputMouse *pMouse = new CInputMouse;
	if (pMouse == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// マウスの初期化
		if (FAILED(pMouse->Init(hInstance, hWnd)))
		{ // 初期化に失敗した場合

			// マウスの破棄
			SAFE_DELETE(pMouse);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pMouse;
	}
}

//============================================================
//	破棄処理
//============================================================
void CInputMouse::Release(CInputMouse *&prMouse)
{
	// マウスの終了
	assert(prMouse != nullptr);
	prMouse->Uninit();

	// メモリ開放
	SAFE_DELETE(prMouse);
}

//************************************************************
//	親クラス [CInputPad] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CInputPad::CInputPad()
{
	// メンバ変数をクリア
	memset(&m_aVibration[0],		0, sizeof(m_aVibration));		// バイブ情報
	memset(&m_aKeyStatePress[0],	0, sizeof(m_aKeyStatePress));	// プレス情報
	memset(&m_aKeyStateTrigger[0],	0, sizeof(m_aKeyStateTrigger));	// トリガー情報
	memset(&m_aKeyStateRelease[0],	0, sizeof(m_aKeyStateRelease));	// リリース情報
	memset(&m_nStickAngleL[0],		0, sizeof(m_nStickAngleL));		// スティック角度情報
	memset(&m_nStickAngleR[0],		0, sizeof(m_nStickAngleL));		// スティック角度情報
	memset(&m_nStickTriggerL[0],	0, sizeof(m_nStickTriggerL));	// スティックトリガー情報
	memset(&m_nStickTriggerR[0],	0, sizeof(m_nStickTriggerR));	// スティックトリガー情報
	memset(&m_bStickReleaseL[0],	0, sizeof(m_bStickReleaseL));	// スティックリリース情報
	memset(&m_bStickReleaseR[0],	0, sizeof(m_bStickReleaseR));	// スティックリリース情報
	memset(&m_bStickL[0],			0, sizeof(m_bStickL));			// スティック入力情報
	memset(&m_bStickR[0],			0, sizeof(m_bStickR));			// スティック入力情報
}

//============================================================
//	デストラクタ
//============================================================
CInputPad::~CInputPad()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CInputPad::Init(void)
{
	// メンバ変数を初期化
	memset(&m_aVibration[0], 0, sizeof(m_aVibration));				// バイブ情報
	memset(&m_aKeyStatePress[0], 0, sizeof(m_aKeyStatePress));		// プレス情報
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// トリガー情報
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// リリース情報
	memset(&m_nStickAngleL[0], 0, sizeof(m_nStickAngleL));			// スティック角度情報
	memset(&m_nStickAngleR[0], 0, sizeof(m_nStickAngleL));			// スティック角度情報
	memset(&m_nStickTriggerL[0], 0, sizeof(m_nStickTriggerL));		// スティックトリガー情報
	memset(&m_nStickTriggerR[0], 0, sizeof(m_nStickTriggerR));		// スティックトリガー情報
	memset(&m_bStickReleaseL[0], 0, sizeof(m_bStickReleaseL));		// スティックリリース情報
	memset(&m_bStickReleaseR[0], 0, sizeof(m_bStickReleaseR));		// スティックリリース情報
	memset(&m_bStickL[0], 0, sizeof(m_bStickL));					// スティック入力情報
	memset(&m_bStickR[0], 0, sizeof(m_bStickR));					// スティック入力情報

	// XInputのステートを有効化
	XInputEnable(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CInputPad::Uninit(void)
{
	// XInputのステートを無効化
	XInputEnable(false);
}

//============================================================
//	更新処理
//============================================================
void CInputPad::Update(void)
{
	// 変数配列を宣言
	XINPUT_STATE aKeyState[pad::MAX_NUM];	// パッドの入力情報

	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // プレイヤーの最大人数分ループ

		//スティックのトリガー情報をリセット
		m_nStickTriggerL[nCntJoyKey] = stick::INIT_VALUE;
		m_nStickTriggerR[nCntJoyKey] = stick::INIT_VALUE;

		if (XInputGetState(nCntJoyKey, &aKeyState[nCntJoyKey]) == ERROR_SUCCESS)
		{ // コントローラーが接続されている場合

			// パッドのリリース情報を保存 (ボタン)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.wButtons =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons | aKeyState[nCntJoyKey].Gamepad.wButtons) ^ aKeyState[nCntJoyKey].Gamepad.wButtons;

			// パッドのリリース情報を保存 (L2)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.bLeftTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bLeftTrigger | aKeyState[nCntJoyKey].Gamepad.bLeftTrigger) ^ aKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// パッドのリリース情報を保存 (R2)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.bRightTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bRightTrigger | aKeyState[nCntJoyKey].Gamepad.bRightTrigger) ^ aKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// パッドのトリガー情報を保存 (ボタン)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.wButtons =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons ^ aKeyState[nCntJoyKey].Gamepad.wButtons) & aKeyState[nCntJoyKey].Gamepad.wButtons;

			// パッドのトリガー情報を保存 (L2)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.bLeftTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bLeftTrigger ^ aKeyState[nCntJoyKey].Gamepad.bLeftTrigger) & aKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// パッドのトリガー情報を保存 (R2)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.bRightTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bRightTrigger ^ aKeyState[nCntJoyKey].Gamepad.bRightTrigger) & aKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			//スティックの角度を保存
			m_nStickAngleL[nCntJoyKey] = (int)D3DXToDegree(atan2f((float)aKeyState[nCntJoyKey].Gamepad.sThumbLX, (float)aKeyState[nCntJoyKey].Gamepad.sThumbLY));
			m_nStickAngleR[nCntJoyKey] = (int)D3DXToDegree(atan2f((float)aKeyState[nCntJoyKey].Gamepad.sThumbRX, (float)aKeyState[nCntJoyKey].Gamepad.sThumbRY));

			//スティックのトリガー情報を保存
			if (KnockLStickTrigger(nCntJoyKey, aKeyState[nCntJoyKey])) //左
			{
				m_nStickTriggerL[nCntJoyKey] = m_nStickAngleL[nCntJoyKey];
			}
			if (KnockRStickTrigger(nCntJoyKey, aKeyState[nCntJoyKey])) //右
			{
				m_nStickTriggerR[nCntJoyKey] = m_nStickAngleR[nCntJoyKey];
			}

			//スティックのリリース情報を保存
			m_bStickReleaseL[nCntJoyKey] = KnockLStickRelease(nCntJoyKey, aKeyState[nCntJoyKey]);
			m_bStickReleaseR[nCntJoyKey] = KnockRStickRelease(nCntJoyKey, aKeyState[nCntJoyKey]);

			// スティック入力情報
			if (fabsf(aKeyState[nCntJoyKey].Gamepad.sThumbLX) >= stick::TRIGGER_DEAD || fabsf(aKeyState[nCntJoyKey].Gamepad.sThumbLY) >= stick::TRIGGER_DEAD)
			{
				m_bStickL[nCntJoyKey] = true;
			}
			else { m_bStickL[nCntJoyKey] = false; }
			if (fabsf(aKeyState[nCntJoyKey].Gamepad.sThumbRX) >= stick::TRIGGER_DEAD || fabsf(aKeyState[nCntJoyKey].Gamepad.sThumbRY) >= stick::TRIGGER_DEAD)
			{
				m_bStickR[nCntJoyKey] = true;
			}
			else { m_bStickR[nCntJoyKey] = false; }

			// パッドのプレス情報を保存
			m_aKeyStatePress[nCntJoyKey] = aKeyState[nCntJoyKey];

			// バイブレーションの更新
			UpdateVibration(&m_aVibration[nCntJoyKey], nCntJoyKey);
		}
	}
}

//============================================================
//	バイブレーションの更新処理
//============================================================
void CInputPad::UpdateVibration(SVibration *pVibration, int nPadID)
{
	switch (pVibration->type)
	{ // 振動の更新
	case TYPE_NONE:		// 何もしていない

		// 処理を抜ける
		break;

	case TYPE_DEATH:	// 死亡

		if (pVibration->nTime > 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			pVibration->nTime--;

			// 振動レベルの設定
			pVibration->vibration.wLeftMotorSpeed  -= (WORD)(USHRT_MAX / vibration::TIME[TYPE_DEATH]);	// 左
			pVibration->vibration.wRightMotorSpeed -= (WORD)(USHRT_MAX / vibration::TIME[TYPE_DEATH]);	// 右
		}
		else
		{ // カウンターが 0以下になった場合

			// 何もしない状態に設定
			pVibration->type = TYPE_NONE;

			// メモリの初期化
			memset(&pVibration->vibration, 0, sizeof(pVibration->vibration));
		}

		// 処理を抜ける
		break;

	default:	// 上記以外

		if (pVibration->nTime >= 0)
		{ // カウンターが 0より大きい場合

			// カウンターを減算
			pVibration->nTime--;
		}
		else
		{ // カウンターが 0以下になった場合

			// 何もしない状態に設定
			pVibration->type = TYPE_NONE;

			// メモリの初期化
			memset(&pVibration->vibration, 0, sizeof(pVibration->vibration));
		}

		// 処理を抜ける
		break;
	}

	// モーターの補正
	if (pVibration->vibration.wLeftMotorSpeed < 0)
	{ // 左の振動が 0より小さい場合

		// 左のモーターの補正
		pVibration->vibration.wLeftMotorSpeed = 0;
	}
	if (pVibration->vibration.wRightMotorSpeed < 0)
	{ // 右の振動が 0より小さい場合

		// 右のモーターの補正
		pVibration->vibration.wRightMotorSpeed = 0;
	}

	// バイブ情報を設定
	XInputSetState(nPadID, &pVibration->vibration);
}

//============================================================
//	バイブ設定処理
//============================================================
void CInputPad::SetVibration(EType type, int nPadID)
{
	// バイブの状態を引数のものに設定
	m_aVibration[nPadID].type = type;

	// 振動レベルの設定
	m_aVibration[nPadID].vibration.wLeftMotorSpeed = vibration::LEVEL[type];	// 左
	m_aVibration[nPadID].vibration.wRightMotorSpeed = vibration::LEVEL[type];	// 右

	// 振動時間の設定
	m_aVibration[nPadID].nTime = vibration::TIME[type];

	// バイブ情報を設定
	XInputSetState(nPadID, &m_aVibration[nPadID].vibration);
}

//============================================================
//	プレス取得処理 (ボタン)
//============================================================
bool CInputPad::IsPress(EKey joyKey, int nPadID)
{
	// 入力情報を返す
	return (m_aKeyStatePress[nPadID].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	トリガー取得処理 (ボタン)
//============================================================
bool CInputPad::IsTrigger(EKey joyKey, int nPadID)
{
	// 入力情報を返す
	return (m_aKeyStateTrigger[nPadID].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	リリース取得処理 (ボタン)
//============================================================
bool CInputPad::IsRelease(EKey joyKey, int nPadID)
{
	// 入力情報を返す
	return (m_aKeyStateRelease[nPadID].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	全プレス取得処理 (ボタン)
//============================================================
bool CInputPad::IsAnyPress(int nPadID)
{
	for (int nCntPadKey = 0; nCntPadKey < KEY_MAX; nCntPadKey++)
	{ // パッドキーの最大数分繰り返す

		if (m_aKeyStatePress[nPadID].Gamepad.wButtons & (1 << nCntPadKey))
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	全トリガー取得処理 (ボタン)
//============================================================
bool CInputPad::IsAnyTrigger(int nPadID)
{
	for (int nCntPadKey = 0; nCntPadKey < KEY_MAX; nCntPadKey++)
	{ // パッドキーの最大数分繰り返す

		if (m_aKeyStateTrigger[nPadID].Gamepad.wButtons & (1 << nCntPadKey))
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	全リリース取得処理 (ボタン)
//============================================================
bool CInputPad::IsAnyRelease(int nPadID)
{
	for (int nCntPadKey = 0; nCntPadKey < KEY_MAX; nCntPadKey++)
	{ // パッドキーの最大数分繰り返す

		if (m_aKeyStateRelease[nPadID].Gamepad.wButtons & (1 << nCntPadKey))
		{ // 入力があった場合

			return true;
		}
	}

	return false;
}

//============================================================
//	プレス初期化処理 (ボタン)
//============================================================
void CInputPad::InitPress(int nPadID)
{
	// メモリをクリア
	memset(&m_aKeyStatePress[nPadID].Gamepad.wButtons, 0, sizeof(m_aKeyStatePress[nPadID].Gamepad.wButtons));
}

//============================================================
//	トリガー初期化処理 (ボタン)
//============================================================
void CInputPad::InitTrigger(int nPadID)
{
	// メモリをクリア
	memset(&m_aKeyStateTrigger[nPadID].Gamepad.wButtons, 0, sizeof(m_aKeyStateTrigger[nPadID].Gamepad.wButtons));
}

//============================================================
//	リリース初期化処理 (ボタン)
//============================================================
void CInputPad::InitRelease(int nPadID)
{
	// メモリをクリア
	memset(&m_aKeyStateRelease[nPadID].Gamepad.wButtons, 0, sizeof(m_aKeyStateRelease[nPadID].Gamepad.wButtons));
}

//============================================================
//	全プレス取得処理 (ボタン)
//============================================================
bool CInputPad::IsPressAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // プレイヤーの最大人数分ループ

		if (m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // 現在のパッドで入力が確認された場合

			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//============================================================
//	全トリガー取得処理 (ボタン)
//============================================================
bool CInputPad::IsTriggerAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // プレイヤーの最大人数分ループ

		if (m_aKeyStateTrigger[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // 現在のパッドで入力が確認された場合

			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//============================================================
//	全リリース取得処理 (ボタン)
//============================================================
bool CInputPad::IsReleaseAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // プレイヤーの最大人数分ループ

		if (m_aKeyStateRelease[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // 現在のパッドで入力が確認された場合

			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//============================================================
//	プレス取得処理 (LスティックX)
//============================================================
SHORT CInputPad::GetPressLStickX(int nPadID)
{
	// スティックの傾きの値を補正
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbLX, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// スティックの傾きの値を返す (左右)
	return m_aKeyStatePress[nPadID].Gamepad.sThumbLX;
}

//============================================================
//	プレス取得処理 (LスティックY)
//============================================================
SHORT CInputPad::GetPressLStickY(int nPadID)
{
	// スティックの傾きの値を補正
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbLY, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// スティックの傾きの値を返す (上下)
	return -m_aKeyStatePress[nPadID].Gamepad.sThumbLY;
}

//============================================================
//	プレス取得処理 (RスティックX)
//============================================================
SHORT CInputPad::GetPressRStickX(int nPadID)
{
	// スティックの傾きの値を補正
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbRX, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// スティックの傾きの値を返す (左右)
	return m_aKeyStatePress[nPadID].Gamepad.sThumbRX;
}

//============================================================
//	プレス取得処理 (RスティックY)
//============================================================
SHORT CInputPad::GetPressRStickY(int nPadID)
{
	// スティックの傾きの値を補正
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbRY, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// スティックの傾きの値を返す (上下)
	return -m_aKeyStatePress[nPadID].Gamepad.sThumbRY;
}

//============================================================
//	プレス取得処理 (Lスティック向き)
//============================================================
float CInputPad::GetPressLStickRot(int nPadID)
{
	// 変数を宣言
	float fRot = 0.0f;	// スティック傾き

	// ステックの傾きを計算
	fRot = atan2f((float)GetPressLStickY(nPadID), (float)GetPressLStickX(nPadID));

	// ステックの傾きを返す
	return fRot;
}

//============================================================
//	プレス取得処理 (Rスティック向き)
//============================================================
float CInputPad::GetPressRStickRot(int nPadID)
{
	// 変数を宣言
	float fRot = 0.0f;	// スティック傾き

	// ステックの傾きを計算
	fRot = atan2f((float)GetPressRStickY(nPadID), (float)GetPressRStickX(nPadID));

	// ステックの傾きを返す
	return fRot;
}

//============================================================
//	プレス取得処理 (Lスティック傾き)
//============================================================
float CInputPad::GetPressLStickTilt(int nPadID)
{
	// 変数を宣言
	D3DXVECTOR3	vecLTilt = D3DXVECTOR3((float)GetPressLStickX(nPadID), (float)GetPressLStickY(nPadID), 0.0f);	// Lスティック各軸の傾き量
	float fLTilt = sqrtf(vecLTilt.x * vecLTilt.x + vecLTilt.y * vecLTilt.y);	// Lスティックの傾き量

	// Lスティックの傾きを返す
	return fLTilt;
}

//============================================================
//	プレス取得処理 (Rスティック傾き)
//============================================================
float CInputPad::GetPressRStickTilt(int nPadID)
{
	// 変数を宣言
	D3DXVECTOR3	vecRTilt = D3DXVECTOR3((float)GetPressRStickX(nPadID), (float)GetPressRStickY(nPadID), 0.0f);	// Rスティック各軸の傾き量
	float fRTilt = sqrtf(vecRTilt.x * vecRTilt.x + vecRTilt.y * vecRTilt.y) * 0.5f;	// Rスティックの傾き量

	// Rスティックの傾きを返す
	return fRTilt;
}

//==========================================
//  入力判定(Lスティック)
//==========================================
bool CInputPad::KnockLStickTrigger(int nIdx, XINPUT_STATE JoyKey) //左
{
	//ローカル変数宣言
	bool bJudgment = false;

	//前回入力の判定
	if (fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbLX) <= stick::TRIGGER_DEAD && fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbLY) <= stick::TRIGGER_DEAD)
	{
		//今回入力の判定
		if (fabsf(JoyKey.Gamepad.sThumbLX) >= stick::TRIGGER_DEAD || fabsf(JoyKey.Gamepad.sThumbLY) >= stick::TRIGGER_DEAD)
		{
			bJudgment = true;
		}
	}

	//変数を返す
	return bJudgment;
}

//==========================================
//  入力判定(Rスティック)
//==========================================
bool CInputPad::KnockRStickTrigger(int nIdx, XINPUT_STATE JoyKey) //右
{
	//ローカル変数宣言
	bool bJudgment = false;

	//前回入力の判定
	if (fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbRX) <= stick::TRIGGER_DEAD && fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbRY) <= stick::TRIGGER_DEAD)
	{
		//今回入力の判定
		if (fabsf(JoyKey.Gamepad.sThumbRX) >= stick::TRIGGER_DEAD || fabsf(JoyKey.Gamepad.sThumbRY) >= stick::TRIGGER_DEAD)
		{
			bJudgment = true;
		}
	}

	//変数を返す
	return bJudgment;
}

//==========================================
//  入力判定(Lスティック)
//==========================================
bool CInputPad::KnockLStickRelease(int nIdx, XINPUT_STATE JoyKey)
{
	//ローカル変数宣言
	bool bJudgment = false;

	//前回入力の判定
	if (fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbLX) >= stick::TRIGGER_DEAD || fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbLY) >= stick::TRIGGER_DEAD)
	{
		//今回入力の判定
		if (fabsf(JoyKey.Gamepad.sThumbLX) <= stick::TRIGGER_DEAD && fabsf(JoyKey.Gamepad.sThumbLY) <= stick::TRIGGER_DEAD)
		{
			bJudgment = true;
		}
	}

	//変数を返す
	return bJudgment;
}

//==========================================
//  入力判定(Rスティック)
//==========================================
bool CInputPad::KnockRStickRelease(int nIdx, XINPUT_STATE JoyKey)
{
	//ローカル変数宣言
	bool bJudgment = false;

	//前回入力の判定
	if (fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbRX) >= stick::TRIGGER_DEAD || fabsf(m_aKeyStatePress[nIdx].Gamepad.sThumbRY) >= stick::TRIGGER_DEAD)
	{
		//今回入力の判定
		if (fabsf(JoyKey.Gamepad.sThumbRX) <= stick::TRIGGER_DEAD && fabsf(JoyKey.Gamepad.sThumbRY) <= stick::TRIGGER_DEAD)
		{
			bJudgment = true;
		}
	}

	//変数を返す
	return bJudgment;
}

//==========================================
//  トリガー取得(Lスティック)
//==========================================
bool CInputPad::GetTriggerLStick(int nDirection, int nPadID)
{
	// 全判定の場合
	if (nDirection == STICK_ALL)
	{
		// 初期化された値じゃない場合
		if (m_nStickTriggerL[nPadID] == stick::INIT_VALUE) { return false; }

		return true;
	}

	return abs(m_nStickTriggerL[nPadID] - nDirection) < 45 ? true : false;
}

//==========================================
//  トリガー取得(Rスティック)
//==========================================
bool CInputPad::GetTriggerRStick(int nDirection, int nPadID)
{
	// 全判定の場合
	if (nDirection == STICK_ALL)
	{
		// 初期化された値じゃない場合
		if (m_nStickTriggerR[nPadID] == stick::INIT_VALUE) { return false; }

		return true;
	}

	return abs(m_nStickTriggerR[nPadID] - nDirection) < 45 ? true : false;
}

//==========================================
//  リリース取得(Lスティック)
//==========================================
bool CInputPad::GetReleaseLStick(int nPadID)
{
	return m_bStickReleaseL[nPadID];
}

//==========================================
//  リリース取得(Rスティック)
//==========================================
bool CInputPad::GetReleaseRStick(int nPadID)
{
	return m_bStickReleaseR[nPadID];
}

//==========================================
//  入力フラグの取得(Lスティック)
//==========================================
bool CInputPad::GetLStick(int nPadID)
{
	return m_bStickL[nPadID];
}

//==========================================
//  入力フラグの取得(Rスティック)
//==========================================
bool CInputPad::GetRStick(int nPadID)
{
	return m_bStickR[nPadID];
}

//==========================================
//  スティックの割合(Lスティック)
//==========================================
D3DXVECTOR3 CInputPad::GetStickRateL(float fDead, int nPadID)
{
	//ローカル変数宣言
	D3DXVECTOR3 Stick = VEC3_ZERO;

	Stick.x = (float)m_aKeyStatePress[nPadID].Gamepad.sThumbLX / SHRT_MAX;
	if (fabsf(Stick.x) < fDead)
	{
		Stick.x = 0.0f;
	}

	Stick.z = (float)m_aKeyStatePress[nPadID].Gamepad.sThumbLY / SHRT_MAX;
	if (fabsf(Stick.z) < fDead)
	{
		Stick.z = 0.0f;
	}

	return Stick;
}

//==========================================
//  スティックの割合(Rスティック)
//==========================================
D3DXVECTOR3 CInputPad::GetStickRateR(float fDead, int nPadID)
{
	//ローカル変数宣言
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)m_aKeyStatePress[nPadID].Gamepad.sThumbRX / SHRT_MAX;
	if (fabsf(Stick.x) < fDead)
	{
		Stick.x = 0.0f;
	}

	Stick.z = (float)m_aKeyStatePress[nPadID].Gamepad.sThumbRY / SHRT_MAX;
	if (fabsf(Stick.z) < fDead)
	{
		Stick.z = 0.0f;
	}

	return Stick;
}

//============================================================
//	生成処理
//============================================================
CInputPad *CInputPad::Create(void)
{
	// パッドの生成
	CInputPad *pPad = new CInputPad;
	if (pPad == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// パッドの初期化
		if (FAILED(pPad->Init()))
		{ // 初期化に失敗した場合

			// パッドの破棄
			SAFE_DELETE(pPad);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pPad;
	}
}

//============================================================
//	破棄処理
//============================================================
void CInputPad::Release(CInputPad *&prPad)
{
	// パッドの終了
	assert(prPad != nullptr);
	prPad->Uninit();

	// メモリ開放
	SAFE_DELETE(prPad);
}
