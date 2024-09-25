//============================================================
//
//	サウンド処理 [sound.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const CSound::SSoundInfo SOUND_FILE[] =	// サウンドファイル
	{
		{ "data\\BGM\\bgm_general.wav", -1 },		// BGM
		{ "data\\BGM\\bgm_game_forest.wav", -1 },	// BGM(森)
		{ "data\\BGM\\bgm_title.wav", -1 },			// BGM (タイトル)
		{ "data\\BGM\\bgm_select.wav", -1 },		// BGM (セレクト)
		{ "data\\BGM\\bgm_result.wav", -1 },		// BGM(リザルト)
		{ "data\\BGM\\bgm_gameover.wav", -0 },		// BGM(ゲームオーバー)

		{ "data\\SE\\select000.wav", 0 },		// 選択操作音00
		{ "data\\SE\\select001.wav", 0 },		// 選択操作音01
		{ "data\\SE\\decision000.wav", 0 },		// 決定音00
		{ "data\\SE\\decision001.wav", 0 },		// 決定音01
		{ "data\\SE\\decision002.wav", 0 },		// 決定音02
		{ "data\\SE\\pause000.wav", 0 },		// ポーズ開く音
		{ "data\\SE\\goal.wav", 0 },			// ゴール音
		{ "data\\SE\\land000.wav", 0 },			// 着地音 (小)
		{ "data\\SE\\land001.wav", 0 },			// 着地音 (大)
		{ "data\\SE\\stalkwalk000.wav", 0 },	// 追跡敵の歩く音
		{ "data\\SE\\stalkattack000.wav", 0 },	// 追跡敵が攻撃したときの音
		{ "data\\SE\\stalkattack001.wav", 0 },	// 追跡敵が分身を攻撃したときの音
		{ "data\\SE\\stalkfound000.wav", 0 },	// 追跡敵が発見したときの音
		{ "data\\SE\\stalkupset000.wav", 0 },	// 追跡敵が動揺したときの音
		{ "data\\SE\\stalkstance000.wav", 0 },	// 追跡敵が構えたときの音
		{ "data\\SE\\playerwalk000.wav", 0 },	// プレイヤーの歩行音
		{ "data\\SE\\playerwalk001.wav", 0 },	// プレイヤーの忍び歩き音
		{ "data\\SE\\playerjump000.wav", 0 },	// プレイヤーのジャンプ音
		{ "data\\SE\\playerstep000.wav", 0 },	// プレイヤーの回避音
		{ "data\\SE\\save000.wav", 0 },			// セーブ音
		{ "data\\SE\\clonepop000.wav", 0 },		// 分身生み出し音
		{ "data\\SE\\clonelost000.wav", 0 },	// 分身消失音
		{ "data\\SE\\getgoditem000.wav", 0 },	// 神器取得音
		{ "data\\SE\\gimmickset000.wav", 0 },	// ギミックに分身セット
		{ "data\\SE\\gimmickset001.wav", 0 },	// ギミック完成
		{ "data\\SE\\button000.wav", 0 },		// ボタン音
		{ "data\\SE\\wolfattack000.wav", 0 },	// 狼が攻撃したときの音
		{ "data\\SE\\wolfattack001.wav", 0 },	// 狼が分身を攻撃したときの音
		{ "data\\SE\\wolffound000.wav", 0 },	// 狼が発見したときの音
		{ "data\\SE\\wolfupset000.wav", 0 },	// 狼が動揺したときの音
		{ "data\\SE\\wolfstance000.wav", 0 },	// 狼が構えたときの音
		{ "data\\SE\\clonefail000.wav", 0 },	// 分身失敗音
		{ "data\\SE\\waterdeath000.wav", 0 },	// 着水音
		{ "data\\SE\\grass000.wav", 0 },		// 草踏んだ音
		{ "data\\SE\\gimmickjump.wav", 0 },		// ギミックジャンプ音
		{ "data\\SE\\gimmickbridge.wav", 0 },	// 橋完成音
		{ "data\\SE\\gimmickbutton.wav", 0 },	// ボタンを押した音
		{ "data\\SE\\gimmickbamboo.wav", 0 },	// 竹格子の動く音
		{ "data\\SE\\dooropen.wav", 0 },		// 重い扉が開く音
		{ "data\\SE\\doorclose.wav", 0 },		// 重い扉が閉じる音
		{ "data\\SE\\mash.wav", 0 },			// ふすまの音
		{ "data\\SE\\mashclose.wav", 0 },		// ふすまが閉じるときの音
		{ "data\\SE\\spinwall.wav", 0 },		// 回転扉の音
		{ "data\\SE\\wind000.wav", 0 },			// 風切り音00
		{ "data\\SE\\wind001.wav", 0 },			// 風切り音01
		{ "data\\SE\\wind002.wav", 0 },			// 風切り音02
		{ "data\\SE\\syakuhati.wav", 0 },		// 尺八音
		{ "data\\SE\\reflect.wav", 0 },			// カキン音
		{ "data\\SE\\in000.wav", 0 },			// イン結び
		{ "data\\SE\\resulttext.wav", 0 },		// リザルトテキスト
		{ "data\\SE\\resulttitle.wav", 0 },		// リザルトタイトル
		{ "data\\SE\\resultgoditem.wav", 0 },	// リザルトの神器
		{ "data\\SE\\goditemUI.wav", 0 },		// 神器UI
		{ "data\\SE\\shrubdelete.wav", 0 },		// 低木消滅音
		{ "data\\SE\\teleport.wav", 0 },		// テレポート音
		{ "data\\SE\\touchfly.wav", 0 },		// 石・枝が飛ぶ音
		{ "data\\SE\\bird.wav", 0 },			// 鳥の飛ぶ音
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(SOUND_FILE) == CSound::LABEL_MAX, "ERROR : Sound Count Mismatch");

//************************************************************
//	親クラス [CSound] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSound::CSound() :
	m_pXAudio2			(nullptr),	// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice	(nullptr)	// マスターボイス
{
	// メンバ変数をクリア
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// ソースボイス
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// オーディオデータ
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// オーディオデータサイズ
}

//============================================================
//	デストラクタ
//============================================================
CSound::~CSound()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSound::Init(HWND hWnd)
{
	// 変数を宣言
	HRESULT hr;	// 終了確認用

	// メンバ変数を初期化
	m_pXAudio2			= nullptr;	// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice	= nullptr;	// マスターボイス
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// ソースボイス
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// オーディオデータ
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// オーディオデータサイズ

	// COMライブラリの初期化
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2 != nullptr)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSound::Uninit(void)
{
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
		}

		// オーディオデータの開放
		SAFE_FREE(m_apDataAudio[nCntSound]);
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;

	// XAudio2オブジェクトの開放
	SAFE_RELEASE(m_pXAudio2);

	// COMライブラリの終了処理
	CoUninitialize();
}

//============================================================
//	全読込処理
//============================================================
HRESULT CSound::LoadAll(HWND hWnd)
{
	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HRESULT hr;
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(SOUND_FILE[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{ // ファイルポインタを先頭に移動

			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = SOUND_FILE[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	セグメント再生 (再生中なら停止)
//============================================================
HRESULT CSound::Play(ELabel label)
{
	// 変数を宣言
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes	= m_aSizeAudio[label];
	buffer.pAudioData	= m_apDataAudio[label];
	buffer.Flags		= XAUDIO2_END_OF_STREAM;
	buffer.LoopCount	= SOUND_FILE[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中

		if (label == LABEL_SE_GRASS_000)
		{ // 草の音の場合

			// 重ねて再生
			if (FAILED(m_apSourceVoice[label]->Start(0))) { assert(false); }

			return E_FAIL;
		}

		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//============================================================
//	セグメント停止 (ラベル指定)
//============================================================
void CSound::Stop(ELabel label)
{
	// 変数を宣言
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中

		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// 音量を初期化
		m_apSourceVoice[label]->SetVolume(1.0f);

		// 周波数を初期化
		m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//============================================================
//	セグメント停止 (全て)
//============================================================
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// 音量を初期化
			m_apSourceVoice[nCntSound]->SetVolume(1.0f);

			// 周波数を初期化
			m_apSourceVoice[nCntSound]->SetFrequencyRatio(1.0f);
		}
	}
}

//============================================================
//	音量の設定処理
//============================================================
void CSound::SetVolume(const ELabel label, float fVolume)
{
	// 変数を宣言
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中の場合

		// 音量を設定
		useful::LimitNum(fVolume, 0.0f, 1.0f);
		m_apSourceVoice[label]->SetVolume(fVolume);
	}
}

//============================================================
//	音量の取得処理
//============================================================
float CSound::GetVolume(const ELabel label) const
{
	// 変数を宣言
	float fVolume = 0.0f;	// 音量

	// 音量を取得
	m_apSourceVoice[label]->GetVolume(&fVolume);

	// 音量を返す
	return fVolume;
}

//============================================================
//	周波数の設定処理
//============================================================
void CSound::SetFrequency(const ELabel label, float fFreq)
{
	// 変数を宣言
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中の場合

		// 周波数を設定
		m_apSourceVoice[label]->SetFrequencyRatio(fFreq);
	}
}

//============================================================
//	周波数の取得処理
//============================================================
float CSound::GetFrequency(const ELabel label) const
{
	// 変数を宣言
	float fFreq = 0.0f;	// 周波数

	// 周波数を取得
	m_apSourceVoice[label]->GetFrequencyRatio(&fFreq);

	// 周波数を返す
	return fFreq;
}

//============================================================
//	生成処理
//============================================================
CSound *CSound::Create(HWND hWnd)
{
	// サウンドの生成
	CSound *pSound = new CSound;
	if (pSound == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// サウンドの初期化
		if (FAILED(pSound->Init(hWnd)))
		{ // 初期化に失敗した場合

			// サウンドの破棄
			SAFE_DELETE(pSound);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pSound;
	}
}

//============================================================
//	破棄処理
//============================================================
void CSound::Release(CSound *&prSound)
{
	// サウンドの終了
	assert(prSound != nullptr);
	prSound->Uninit();

	// メモリ開放
	SAFE_DELETE(prSound);
}

//============================================================
//	チャンクのチェック
//============================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	// 変数を宣言
	HRESULT hr = S_OK;	// 終了確認用
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // ファイルポインタを先頭に移動

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{ // チャンクの読み込み

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{ // チャンクデータの読み込み

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':

			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{ // ファイルタイプの読み込み

				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			break;

		default:

			if (SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{ // ファイルポインタをチャンクデータ分移動

				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//============================================================
//	チャンクデータの読み込み
//============================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	// 変数を宣言
	DWORD dwRead;
	
	if (SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // ファイルポインタを指定位置まで移動

		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{ // データの読み込み

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
