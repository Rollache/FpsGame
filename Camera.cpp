#include"camera.h"


Camera::Camera()
{
	zoom = 45.0f;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 270.0f;//最初の向き（-Z方向）
	pitch = 0.0f;

	front = glm::vec3(0.0f, 0.0f, -1.0f);  //-Z向き
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::Reset()
{
	zoom = 45.0f;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 270.0f;//最初の向き（-Z）
	pitch = 0.0f;
	isShaking = false;
	shakeTime = SHAKING_TIME;

	front = glm::vec3(0.0f, 0.0f, -1.0f);  //-Z向き
	right = glm::normalize(cross(front, worldUp));
	up = glm::normalize(cross(right, front));
}
void Camera::RotateAroundZ()
{
	//角度をラジアンに変換
	float angleRadians = glm::radians(rotateDegrees);

	// Z軸回りの回転行列を作成
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

	// カメラの front、right、up ベクトルを更新
	front = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(front, 0.0f)));
	right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0.0f)));
	up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(up, 0.0f)));
}

void Camera::UpdateCamera(bool isShooting, float deltaTime)
{
	float pitchRadians = glm::radians(pitch);
	float yawRadians = glm::radians(yaw);

	float cosPitch = glm::cos(pitchRadians);
	float sinPitch = glm::sin(pitchRadians);
	float cosYaw = glm::cos(yawRadians);
	float sinYaw = glm::sin(yawRadians);

	//視線方向ベクトルを計算
	front = glm::normalize(glm::vec3(
		cosYaw * cosPitch,
		sinPitch,
		sinYaw * cosPitch
	));

	right = glm::normalize(cross(front, worldUp));
	up = glm::normalize(cross(right, front));

	RotateAroundZ();

	// 撃つのときの前オフセットを設定
	if (isShooting)
	{
		if (recoilFront > -0.5f)
		{
			recoilFront -= 0.01f;
		}

		glm::vec3 recoilOffset = glm::normalize(front) * recoilFront;
		front += recoilOffset;
	}
	else
	{
		recoilFront = 0.0f;
	}
	//ダメージ受けるときのカメラ振動
	if (isShaking)
	{
		front.x += (-50 + rand() % 100) / 50.0f * SHAKING_STRENGTH;
		front.y += (-50 + rand() % 100) / 50.0f * SHAKING_STRENGTH;
		front.z += (-50 + rand() % 100) / 50.0f * SHAKING_STRENGTH;

		shakeTime -= deltaTime;
		if (shakeTime <= 0)
		{
			shakeTime = SHAKING_TIME;
			isShaking = false;
		}
	}
}

