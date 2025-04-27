#include"main.h"

int main()
{
	double currentFrame;
	double deltaTime;
	double endFrame;
	double frameTime = 0.0;
	double renderAccum = 0.0;
	double smoothFrameTime = TARGET_FRAME;

	srand(time(0));
	//アプリケーション初期化
	OpenWindow();
	PrepareOpenGL();

	Sound sound;
	sound.Init();
	CharacterTexture textures;
	//シーンの準備
	TitleScene* titleScene = new TitleScene(window, windowSize, textures, sound);
	GameScene* gameScene = new GameScene(window, windowSize, textures, sound);
	GameClearScene* gameclearScene = new GameClearScene(window, windowSize, textures, sound);
	SceneManager sceneManager(*gameScene, *titleScene, *gameclearScene);
	titleScene->SetManager(sceneManager);
	gameScene->SetManager(sceneManager);
	gameclearScene->SetManager(sceneManager);

	currentFrame = glfwGetTime();
	endFrame = currentFrame;

	while (!glfwWindowShouldClose(window) && !sceneManager.isQuit)
	{
		// 現在のフレームの時間を取得
		currentFrame = glfwGetTime();
		// 前のフレームとの時間差を計算
		deltaTime = currentFrame - endFrame;
		// 現在のフレーム時間を保存
		endFrame = currentFrame;
		//蓄積時間を更新
		renderAccum += deltaTime;

		// ターゲットフレーム時間に達した場合、処理を行う
		if (renderAccum >= TARGET_FRAME)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderAccum -= TARGET_FRAME;

			sceneManager.Update(deltaTime);
			sceneManager.Render();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//すべてのseを停止させる
	for (int i = 0; i < SOUND_LABEL_MAX; i++)
	{
		sound.Stop(static_cast<SOUND_LABEL>(i));
	}
	sound.Uninit();

	glfwTerminate();

	return 0;
}

