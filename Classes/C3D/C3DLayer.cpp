#include "C3DLayer.h"
#include "Global.h"

C3DLayer::C3DLayer(void)
	:touched(false)
	, rotatex(0)
	, rotatey(0)
{
	m_pParticleSystem = NULL;
	shape = NULL;
	m_pCamera = NULL;
	m_mesh = NULL;
	m_conv = NULL;
	
}


C3DLayer::~C3DLayer(void)
{
}

bool C3DLayer::init()
{
	if (!Layer::init()){
		return false;
	}
	
	m_mesh = new C3DMesh;
	m_mesh->LoadMeshFromC3T("ashe@idle1.c3t");
	m_mesh->LoadAniFromC3T("ashe@idle1.c3t");
	m_pCamera = new C3DCamera;
	kmVec3 tEyeResult = kmVec3(0, 0, 2);
	kmVec3 tLookAtResult = kmVec3(0, 0, 0);
	kmVec3 tUpResult = kmVec3(0, 1, 0);
	m_pCamera->SetEyePosition(tEyeResult);
	m_pCamera->SetLookAtTarget(m_mesh);
	m_pCamera->SetUpDir(tUpResult);
	m_pCamera->SetActive();
	m_pCamera->SetTranslate_Parent(0, 0, -200);
	m_mesh->AddChild(m_pCamera);
	Director::getInstance()->setDepthTest(true);
	addTouchEvent();
	return true;
}

void C3DLayer::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPushMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLPushMatrix();

	UpdateViewProjMatrix();

	Render3DWorld();

	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPopMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLPopMatrix();
}

void C3DLayer::addTouchEvent()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(C3DLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(C3DLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(C3DLayer::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool C3DLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (!touched) {
		touched = true;
	}
	return true;
}

void C3DLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (touched) {
		auto loc1 = touch->getPreviousLocation();
		auto loc2 = touch->getLocation();
		auto offset = loc2 - loc1;

		float rotatefactor = 180.0f / 320;
		rotatex += offset.x*0.01f;
		rotatey -= offset.y*0.01f;
	}
}

void C3DLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (touched) {
		touched = false;
	}
}

void C3DLayer::UpdateViewProjMatrix()
{
	
	Vec3 tEyeResult = Vec3(0,0,1000);
	Vec3 tLookAtResult = Vec3(0, 0, 0);
	Vec3 tUpResult = Vec3(0, 1, 0);
	Mat4 tViewMatrix;
	Mat4 tRotateMatrixX;
	Mat4::createRotation(Vec3(1, 0, 0), -rotatey, &tRotateMatrixX);
	Mat4 tRotateMatrixY;
	Mat4::createRotation(Vec3(0, 1, 0), -rotatex, &tRotateMatrixY);
	tRotateMatrixX.transformPoint(tEyeResult, &tEyeResult);
	tRotateMatrixY.transformPoint(tEyeResult, &tEyeResult);
	tRotateMatrixX.transformPoint(tUpResult, &tUpResult);
	tRotateMatrixY.transformPoint(tUpResult, &tUpResult);
	kmMat4LookAt(&tViewMatrix, &tEyeResult, &tLookAtResult, &tUpResult);

	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();
	/*tViewMatrix.scale(0.1);
	static float rotation = 0;
	tViewMatrix.rotate(Vec3(0.5, 0.5, 0.5), CC_DEGREES_TO_RADIANS(rotation));
	rotation++;
	if (rotation >= 360){
	rotation = 0;
	}*/
	kmGLMultMatrix(&tViewMatrix);
	kmMat4 matrixPerspective;
	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLLoadIdentity();
	Size size = Director::getInstance()->getVisibleSize();
	kmMat4PerspectiveProjection(&matrixPerspective, 60, (GLfloat)size.width/size.height, 0.1f, 5000.0f);
	kmGLMultMatrix(&matrixPerspective);
	//if (m_pCamera)m_pCamera->UpdateViewMatrix();
}

void C3DLayer::Render3DWorld()
{
	if (m_pParticleSystem){
		m_pParticleSystem->Update();
		m_pParticleSystem->Render();
	}
	if (shape){
		shape->SetRotate(1, 1, 1, m_fRotateYAngle);
		shape->Render();
	}
	if (m_mesh){
		m_mesh->Update();
		m_mesh->Render();
	}
	m_fRotateYAngle += 0.01;
	if (m_conv){
		m_conv->Update();
	}
}