#pragma once

//シーン一覧、シーン追加時はここに追加する
enum SCENE {
	TITLE_SCENE,
	SELECT_SCENE,
	GAME_SCENE,
	GAME_SCENE2,
	GAME_SCENE3,
	GAME_SCENE4,
	GAME_SCENE5,
	GAME_SCENE6,
	CLEAR_SCENE,
	DEMO_SCENE,
	SCENE_MAX
};

class Iscene{
public:
	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

	static int sceneNo;

	virtual ~Iscene() {};
};
