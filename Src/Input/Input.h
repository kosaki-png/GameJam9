#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "../Base/Window.h"
#include "../Base/GraphicsUtility.h"

// キー最大数
#define KEY_MAX 256

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

class Input
{
public:
	// コンストラクタ
	Input(Window* win);
	// デストラクタ
	~Input();

	// キー入力
	bool CheckKey(UINT index);
	// トリガーの入力
	bool TriggerKey(UINT index);

	// 左クリック入力
	bool CheckMouseLeft();
	// 左クリックトリガー入力
	bool TriggerMouseLeft();

	// 右クリック入力
	bool CheckMouseLight();
	// 右クリックトリガー入力
	bool TriggerMouseLight();

	// 中クリック入力
	bool CheckMouseMiddle();
	// 中クリックトリガー入力
	bool TriggerMouseMiddle();

	// マウス移動量取得
	Vector2 GetMouseVel();

private:
	// インプットの生成
	HRESULT CreateInput(void);

	// キーデバイスの生成
	HRESULT CreateKey(void);
	// キーフォーマットのセット
	HRESULT SetKeyFormat(void);
	// キーの協調レベルのセット
	HRESULT SetKeyCooperative(void);

	// キーデバイスの生成
	HRESULT CreateMouse(void);
	// マウスフォーマットの設定
	HRESULT SetMouseFormat(void);
	// キーの協調レベルのセット
	HRESULT SetMouseCooperative(void);

	// ウィンドウ
	Window* win;
	// 参照結果
	HRESULT result;
	// インプット
	LPDIRECTINPUT8 input;
	// キーボードインプットデバイス
	LPDIRECTINPUTDEVICE8 key;
	// マウスインプットデバイス
	LPDIRECTINPUTDEVICE8 mouse;
	// キー情報
	BYTE keys[KEY_MAX];
	// 前のキー情報
	BYTE olds[KEY_MAX];

	DIMOUSESTATE mouseState;
	DIMOUSESTATE oldmouseState;
};
