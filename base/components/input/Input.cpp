#include "Input.h"
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

void Input::Initialize() {
	//DirectInputの初期化
	HRESULT result;
	result = DirectInput8Create(WinApp::GetInstance()->GetWc(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboardInput_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboardInput_->SetDataFormat(&c_dfDIKeyboard); //標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboardInput_->SetCooperativeLevel(
		WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	key_ = {};
	preKey_ = {};

	//マウスデバイスを作成
	result = directInput_->CreateDevice(GUID_SysMouse, &mouseInput_, NULL);

	//入力データ形式のセット
	result = mouseInput_->SetDataFormat(&c_dfDIMouse2);

	//排他制御レベルのセット
	result = mouseInput_->SetCooperativeLevel(
		WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void Input::Update() {
	preKey_ = key_;

	//キーボード情報の取得開始
	keyboardInput_->Acquire();

	//全キーの入力状態を取得
	keyboardInput_->GetDeviceState(sizeof(key_), &key_);

	mousePre_ = mouse_;
	//マウス情報の取得開始
	mouseInput_->Acquire();

	mouseInput_->GetDeviceState(sizeof(mouse_), &mouse_);

	m_Position_.Pos = MousePos();
	m_Position_.Velocity = MouseVelocity();
	m_Position_.Scroll += MouseScroll();
}

bool Input::TriggerKey(BYTE keyNumber) const {
	if (key_[keyNumber] != 0 && preKey_[keyNumber] == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::PressKey(BYTE keyNumber)const {
	if (key_[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::ReleaseKey(BYTE keyNumber)const {
	if (key_[keyNumber] == 0 && preKey_[keyNumber] != 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::pushMouse(uint32_t Mousebutton){
	if (mouse_.rgbButtons[Mousebutton] != 0) {
		return true;
	}
	return false;
}

bool Input::GetJoystickState(int32_t stickNo, XINPUT_STATE& out){
	DWORD dwResult = XInputGetState(stickNo, &out);
	if (out.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < out.Gamepad.sThumbLX) {
			out.Gamepad.sThumbLX = 0;
		}

	}
	if (out.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < out.Gamepad.sThumbLY) {
			out.Gamepad.sThumbLY = 0;
		}
	}
	if (out.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < out.Gamepad.sThumbRX) {
			out.Gamepad.sThumbRX = 0;
		}

	}
	if (out.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < out.Gamepad.sThumbRY) {
			out.Gamepad.sThumbRY = 0;
		}
	}
	return dwResult == ERROR_SUCCESS;
}

bool Input::PushLTrigger(XINPUT_STATE& out){
	if (out.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		return true;
	}
	return false;
}

bool Input::PushRTrigger(XINPUT_STATE& out){
	if (out.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		return true;
	}
	return false;
}

bool Input::PushLSHOULDER(XINPUT_STATE& out){
	if (out.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		return true;
	}
	return false;
}

bool Input::PushRSHOULDER(XINPUT_STATE& out){
	if (out.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		return true;
	}
	return false;
}

bool Input::PushAButton(XINPUT_STATE& out){
	if (out.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		return true;
	}
	return false;
}

bool Input::PushBButton(XINPUT_STATE& out){
	if (out.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		return true;
	}
	return false;
}

bool Input::PushXButton(XINPUT_STATE& out){
	if (out.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		return true;
	}
	return false;
}

bool Input::PushYButton(XINPUT_STATE& out){
	if (out.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		return true;
	}
	return false;
}

Vector2 Input::MousePos() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindowA(WinApp::GetInstance()->GetName(), nullptr), &p);
	return { (float)p.x,(float)p.y };
}

Vector2 Input::MouseVelocity() {
	return { (float)mouse_.lX,(float)mouse_.lY };
}

float Input::MouseScroll(){
	return (float)mouse_.lZ;
}