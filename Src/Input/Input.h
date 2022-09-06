#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "../Base/Window.h"
#include "../Base/GraphicsUtility.h"

// �L�[�ő吔
#define KEY_MAX 256

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>

class Input
{
public:
	// �R���X�g���N�^
	Input(Window* win);
	// �f�X�g���N�^
	~Input();

	// �L�[����
	bool CheckKey(UINT index);
	// �g���K�[�̓���
	bool TriggerKey(UINT index);

	// ���N���b�N����
	bool CheckMouseLeft();
	// ���N���b�N�g���K�[����
	bool TriggerMouseLeft();

	// �E�N���b�N����
	bool CheckMouseLight();
	// �E�N���b�N�g���K�[����
	bool TriggerMouseLight();

	// ���N���b�N����
	bool CheckMouseMiddle();
	// ���N���b�N�g���K�[����
	bool TriggerMouseMiddle();

	// �}�E�X�ړ��ʎ擾
	Vector2 GetMouseVel();

private:
	// �C���v�b�g�̐���
	HRESULT CreateInput(void);

	// �L�[�f�o�C�X�̐���
	HRESULT CreateKey(void);
	// �L�[�t�H�[�}�b�g�̃Z�b�g
	HRESULT SetKeyFormat(void);
	// �L�[�̋������x���̃Z�b�g
	HRESULT SetKeyCooperative(void);

	// �L�[�f�o�C�X�̐���
	HRESULT CreateMouse(void);
	// �}�E�X�t�H�[�}�b�g�̐ݒ�
	HRESULT SetMouseFormat(void);
	// �L�[�̋������x���̃Z�b�g
	HRESULT SetMouseCooperative(void);

	// �E�B���h�E
	Window* win;
	// �Q�ƌ���
	HRESULT result;
	// �C���v�b�g
	LPDIRECTINPUT8 input;
	// �L�[�{�[�h�C���v�b�g�f�o�C�X
	LPDIRECTINPUTDEVICE8 key;
	// �}�E�X�C���v�b�g�f�o�C�X
	LPDIRECTINPUTDEVICE8 mouse;
	// �L�[���
	BYTE keys[KEY_MAX];
	// �O�̃L�[���
	BYTE olds[KEY_MAX];

	DIMOUSESTATE mouseState;
	DIMOUSESTATE oldmouseState;
};
