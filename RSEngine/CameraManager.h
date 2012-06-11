#pragma once
#include <vector>
class CameraClass;
class CameraManager
{
public:
	static CameraManager* getInstance();
	CameraClass* getMainCamera();
	~CameraManager(void);
	void setMainCamera(CameraClass* cc);
	void insertCamera(CameraClass* cc);
	CameraClass* getCamera(std::string tag);

	void Shutdown();

private:
	static CameraManager* m_instance;
	CameraManager(void);
private:
	std::vector<CameraClass*> m_cameraList;
};

