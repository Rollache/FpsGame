#pragma once
#include"base.h"
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera() = default;
	//リセット
	void Reset();
	//z軸回転（予備用）
	void RotateAroundZ();
	//更新処理
	void UpdateCamera(bool isShooting, float deltaTime = 0.0f);
	//画面を揺れる
	void Shake()
	{
		isShaking = true;
	}

	glm::mat4 GetViewMatrix() const
	{
		return lookAt(position, position + front, up);
	}

	glm::vec3 GetPosition() const
	{
		return position;
	}

	glm::vec3 GetFront() const
	{
		return front;
	}

	glm::vec3 GetFrontWithoutRecoilZ() const
	{
		return normalize(front - normalize(front) * recoilFront);
	}

	glm::vec3 GetRight() const
	{
		return right;
	}

	glm::vec3 GetUp() const
	{
		return up;
	}

	glm::vec3 GetForward() const
	{
		glm::vec3 forward = normalize(cross(worldUp, right));
		return forward;
	}

	void SetWindow(GLFWwindow* window)
	{
		this->window = window;
	}

	void SetPosition(const glm::vec3& position)
	{
		this->position = position;
	}

	float GetZoom()const
	{
		return zoom;
	}

	void ChangeRotateDegrees(float angle)
	{
		rotateDegrees += angle;
	}

private:
	GLFWwindow* window;
	vec3 position;		//位置
	vec3 front;			//前ベクトル
	vec3 right;			//右ベクトル
	vec3 up;				//上ベクトル
	vec3 worldUp;		//世界の上ベクトル
	float zoom;

	float rotateDegrees = 0.0f;	//回転角度
	float recoilFront = 0.0f;	//弓を引くときにカメラが前に移動する

	bool isShaking;	//カメラが揺れているか
	float shakeTime;	//揺れる時間
	const float SHAKING_STRENGTH = 0.004f;	//揺れる範囲
	const float SHAKING_TIME = 0.8f;		//揺れる時間

public:
	float yaw;
	float pitch;

};

