#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma warning(push)
#pragma warning(push)
#pragma warning(push)

#include <Ogre.h>
#include <Samples/Common/include/ExampleApplication.h>
#include <vector>
#include <fstream>
#include "Tools.h"

#pragma warning(pop)

class Example1FrameListener : public Ogre::FrameListener
{
public:
	Example1FrameListener(Ogre::SceneNode *sn)
		: _node(sn)
	{

	}

	virtual bool frameStarted(const Ogre::FrameEvent& evt)
	{
		_node->translate(Ogre::Vector3(1.0, 0.0, 0.0) * evt.timeSinceLastFrame);
		return true;
	}

private:
	Ogre::SceneNode *_node;
};

class Example1 : public ExampleApplication
{
public:

	~Example1()
	{
		delete _listener;
	}
	virtual void createScene()
	{
		Ogre::Entity *et = mSceneMgr->createEntity("Entity", "Sinbad.mesh");
		_node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		_node->attachObject(et);

		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -5.0);
		Ogre::MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane
			, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

		Ogre::SceneNode *ground = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		ground->attachObject(mSceneMgr->createEntity("ground", "plane"));
		mSceneMgr->getEntity("ground")->setMaterialName("Examples/BeachStones");


		Ogre::Light *light = mSceneMgr->createLight("Light");
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(1.0, -1.0, 0.0);
		light->setDiffuseColour(1.0, 1.0, 1.0);

		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	}

	virtual void createCamera(void)
	{
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");

		// Position it at 500 in Z direction
		mCamera->setPosition(Vector3(0,20,50));
		// Look back along -Z
		mCamera->lookAt(Vector3(0,0,0));
		mCamera->setNearClipDistance(1);

	}

	virtual void createViewports(void)
	{
		// Create one viewport, entire window
		Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(ColourValue(0.75,0.75,1.0));

		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(
			Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	}

	virtual void createFrameListener(void)
	{

		mFrameListener= new ExampleFrameListener(mWindow, mCamera);
		mFrameListener->showDebugOverlay(true);
		mRoot->addFrameListener(mFrameListener);

		_listener = new Example1FrameListener(_node);
		mRoot->addFrameListener(_listener);
	}

private:

	Example1FrameListener *_listener;
	Ogre::SceneNode *_node;
};

class Example2 : public ExampleApplication
{
public:
	Example2()
	{

	}
	~Example2(){}

	virtual void createScene()
	{	
		Ogre::Entity *entity = mSceneMgr->createEntity("Sinbad", "Sinbad.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entity);

		Ogre::CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "Mosaic/Compositor");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Mosaic/Compositor", true);

		//const Ogre::GpuProgramManager::SyntaxCodes &sc = Ogre::GpuProgramManager::getSingleton().getSupportedSyntax();
	}

	virtual void createCamera()
	{
		mCamera = mSceneMgr->createCamera("camera");
		mCamera->setPosition(0.0, 0.0, 10.0);
		mCamera->lookAt(0.0, 0.0, 0.0);
		mCamera->setNearClipDistance(1.0);
	}
};

class Example3 : public ExampleApplication
{
public:
	Example3()
	{

	}

	~Example3()
	{

	}

	virtual void createScene()
	{	

		Ogre::ManualObject *mo = mSceneMgr->createManualObject("Quad");
		mo->begin("VMaterialShader");
		mo->position(5.0f, 0.0f, 0.0f);
		mo->textureCoord(0, 1);
		mo->position(-5.0f, 10.0f, 0.0f);
		mo->textureCoord(1, 0);
		mo->position(-5.0f, 0.0f, 0.0f);
		mo->textureCoord(1, 1);
		mo->position(5.0f, 10.0f, 0.0f);
		mo->textureCoord(0, 0);

		mo->index(0);
		mo->index(1);
		mo->index(2);

		mo->index(0);
		mo->index(3);
		mo->index(1);
		mo->end();
		mo->convertToMesh("Quad");

		Ogre::Entity *ent = mSceneMgr->createEntity("Sinbad", "Sinbad.mesh");
		ent->setMaterialName("VMaterialShader");
		Ogre::SceneNode *sn1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("node1");
		sn1->attachObject(ent);

		//ent = mSceneMgr->createEntity("Quad");
		//ent->setMaterialName("VMaterial2");
		//Ogre::SceneNode *sn2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("node2", Ogre::Vector3(15.0, 0.0, 0.0));
		//sn2->attachObject(ent);

	}

	virtual void createCamera()
	{
		mCamera = mSceneMgr->createCamera("camera");
		mCamera->setPosition(0.0, 0.0, 10.0);
		mCamera->lookAt(0.0, 0.0, 0.0);
		mCamera->setNearClipDistance(1.0);
	}

};

float jitter()
{
	return ((float)rand() / RAND_MAX) -0.5f;
}

Ogre::Image* CreateRandomPointsInSphereSurface(unsigned int tSize, unsigned int sampleU, unsigned int sampleV, SceneManager *sm)
{
	unsigned int tWidth = tSize;
	unsigned int tHeight = tSize;
	unsigned int samples = sampleU * sampleV;
	unsigned int tDepth = samples;

	Ogre::uchar *imgdata = new Ogre::uchar[Ogre::PixelUtil::getMemorySize(tWidth, tHeight, tDepth, Ogre::PF_FLOAT32_RGBA)];
	Ogre::Image *img = new Ogre::Image;
	img->loadDynamicImage(imgdata, tWidth, tHeight, tDepth, Ogre::PF_FLOAT32_RGBA);

	Ogre::ManualObject *mo = sm->createManualObject("randLines");
	mo->begin("", Ogre::RenderOperation::OT_LINE_LIST);

	unsigned int idx = 0;
	for (unsigned int w = 0; w < tWidth; ++w)
	{
		for (unsigned int h = 0; h < tHeight; ++h)
		{
			for (unsigned int d = 0; d < tDepth; ++d)
			{
				unsigned int x1,y1;
				x1 = d % sampleU;
				y1 = (samples - 1 - d) / sampleU;
				Ogre::Vector2 v = Ogre::Vector2(x1 + 0.5 + jitter()
					, y1 + 0.5 + jitter());

				v.x /= sampleU;
				v.y /= sampleV;

				float phi = 2 * jitter();
				Ogre::ColourValue cv;
				cv.r = v.y * cosf(Ogre::Math::HALF_PI * phi) * cosf(Ogre::Math::TWO_PI * v.x);
				cv.g = v.y * sinf(Ogre::Math::HALF_PI * phi);
				cv.b = v.y * cosf(Ogre::Math::HALF_PI * phi) * sinf(Ogre::Math::TWO_PI * v.x);

				mo->position(0.0, 0.0, 0.0);
				mo->index(idx++);
				mo->position(cv.r, cv.g, cv.b);
				mo->index(idx++);
				img->setColourAt(cv, w, h, d);
				//std::cout<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<std::endl;
			}
		}
	}

	mo->end();
	mo->convertToMesh("Lines");
	//Ogre::Entity *ent = sm->createEntity("Lines");
	//Ogre::SceneNode *sn = sm->getRootSceneNode()->createChildSceneNode("node1");
	//sn->attachObject(ent);

	return img;
};

Ogre::Image* CreateRandomPointsInCircleSurface(unsigned int tSize, unsigned int sampleU, unsigned int sampleV, SceneManager *sm)
{
	unsigned int tWidth = tSize;
	unsigned int tHeight = tSize;
	unsigned int samples = sampleU * sampleV;
	unsigned int tDepth = samples / 2;

	Ogre::uchar *imgdata = new Ogre::uchar[Ogre::PixelUtil::getMemorySize(tWidth, tHeight, tDepth, Ogre::PF_FLOAT32_RGBA)];
	Ogre::Image *img = new Ogre::Image;
	img->loadDynamicImage(imgdata, tWidth, tHeight, tDepth, Ogre::PF_FLOAT32_RGBA);

	Ogre::ManualObject *mo = sm->createManualObject("randLines");
	mo->begin("", Ogre::RenderOperation::OT_LINE_LIST);

	unsigned int idx = 0;
	for (unsigned int w = 0; w < tWidth; ++w)
	{
		for (unsigned int h = 0; h < tHeight; ++h)
		{
			for (unsigned int d = 0, k = 0; d < tDepth; ++d, k +=2)
			{
				unsigned int x1,y1,x2,y2;
				x1 = k % sampleU;
				y1 = (samples - 1 - k) / sampleU;
				x2 = (k + 1) % sampleU;
				y2 = (samples - 1 - k - 1) / sampleU;
				Ogre::Vector4 v = Ogre::Vector4(x1 + 0.5f + jitter()
					, y1 + 0.5f + jitter()
					, x2 + 0.5f + jitter()
					, y2 + 0.5f + jitter());

				v.x /= sampleU;
				v.y /= sampleV;
				v.z /= sampleU;
				v.w /= sampleV;

				Ogre::ColourValue cv;
				cv.r = v.y * cosf(Ogre::Math::TWO_PI * v.x);
				cv.g = v.y * sinf(Ogre::Math::TWO_PI * v.x);
				cv.b = v.w * cosf(Ogre::Math::TWO_PI * v.z);
				cv.a = v.w * sinf(Ogre::Math::TWO_PI * v.z);

				mo->position(0.0, 0.0, 0.0);
				mo->index(idx++);
				mo->position(cv.r, cv.g, 0.0);
				mo->index(idx++);
				mo->position(0.0, 0.0, 0.0);
				mo->index(idx++);
				mo->position(cv.b, cv.a, 0.0);
				mo->index(idx++);
				img->setColourAt(cv, w, h, d);
				//std::cout<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<std::endl;
			}
		}
	}

	mo->end();
	mo->convertToMesh("Lines");
	//Ogre::Entity *ent = sm->createEntity("Lines");
	//Ogre::SceneNode *sn = sm->getRootSceneNode()->createChildSceneNode("node1");
	//sn->attachObject(ent);

	return img;
};

//Ogre::Entity* CreateOffsetSphere(Ogre::SceneManager *sm, const std::string &eName)
//{
//	Ogre::ManualObject *mo = sm->createManualObject(eName);
//	mo->begin("Lines", Ogre::RenderOperation::OT_LINE_LIST);
//}

class CompositorExample : public ExampleApplication
{
public:
	CompositorExample(){}
	~CompositorExample(){}

	virtual void createScene()
	{	
		Ogre::Entity *entity = mSceneMgr->createEntity("sibenik", "sibenik.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entity);
		entity->setMaterialName("SSAOMat/DeferredRender");

		Ogre::uint sz = 4;
		Ogre::uint sampU = 4;
		Ogre::uint sampV = 4;

		// use ssao
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().createManual("offsetTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_3D
			, sz, sz, sampU * sampV, 0, Ogre::PF_FLOAT32_RGBA, Ogre::TU_DEFAULT);
		Ogre::Image *sampleOffset = CreateRandomPointsInSphereSurface(sz, sampU, sampV, mSceneMgr);

		//use nsao
		//Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().createManual("offsetTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_3D
		//	, sz, sz, (sampU * sampV) / 2, 0, Ogre::PF_FLOAT32_RGBA, Ogre::TU_DEFAULT);
		//Ogre::Image *sampleOffset = CreateRandomPointsInCircleSurface(sz, sampU, sampV, mSceneMgr);

		Ogre::ConstImagePtrList imgList;
		imgList.push_back(sampleOffset);
		tex->_loadImages(imgList);
		Ogre::Material* mat = dynamic_cast<Ogre::Material*>(Ogre::MaterialManager::getSingleton().getByName("SSAOMat/SSAORender").get());
		mat->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTexture(tex);


		Ogre::CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "SSAO/SSAOCompositor");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "SSAO/SSAOCompositor", true);
		Ogre::CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "SSAO/SamplingCompositor");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "SSAO/SamplingCompositor", true);

	}

	virtual void createCamera()
	{
		mCamera = mSceneMgr->createCamera("camera");
		mCamera->setPosition(20.0, 2.0, 0.0);
		mCamera->lookAt(0.0, 2.0, 0.0);
		mCamera->setNearClipDistance(1.0);
	}
};

class ShadowExample : public ExampleApplication
{
public:
	ShadowExample(){}
	~ShadowExample(){}

	virtual void createScene()
	{	
		Ogre::Entity *et = mSceneMgr->createEntity("Entity", "Sinbad.mesh");
		Ogre::SceneNode *sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->attachObject(et);

		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -5.0);
		Ogre::MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane
			, 100, 100, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

		Ogre::SceneNode *ground = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		ground->attachObject(mSceneMgr->createEntity("ground", "plane"));
		mSceneMgr->getEntity("ground")->setMaterialName("Examples/BeachStones");
	}

	virtual void createCamera()
	{
		mCamera = mSceneMgr->createCamera("camera");
		mCamera->setPosition(0.0, 0.0, 50.0);
		mCamera->lookAt(0.0, 0.0, 0.0);
		mCamera->setNearClipDistance(1.0);
	}
};

class AdvanceLightingExample : public ExampleApplication
{
public:
	AdvanceLightingExample(){}
	~AdvanceLightingExample(){}

	virtual void createScene()
	{	
		Ogre::Entity *entity = mSceneMgr->createEntity("sphere", "teapot.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entity);
		//entity->setMaterialName("WarnLightingMaterial");
		//entity->setMaterialName("OrenNayarMaterial");
		//entity->setMaterialName("MinnaertMaterial");
		//entity->setMaterialName("WardReflectionMaterial");
		//entity->setMaterialName("SchlickReflectionMaterial");
		entity->setMaterialName("CookTorranceMaterial");
	}

	virtual void createCamera()
	{
		mCamera = mSceneMgr->createCamera("camera");
		mCamera->setPosition(0.0, 4.0, 5.0);
		mCamera->lookAt(0.0, 0.0, 0.0);
		mCamera->setNearClipDistance(0.1);
	}

	virtual void createViewports(void)
	{
		// Create one viewport, entire window
		Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(ColourValue(0.2,0.2,0.2));

		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(
			Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	}
};

int main()
{
	//Example1 exp1;
	//exp1.go();

	//Example2 ep2;
	//ep2.go();

	//Example3 ep3;
	//ep3.go();

	//ShadowExample se;
	//se.go();

	//CompositorExample ce;
	//ce.go();

	//ClothsExample ce;
	//ce.go();

	AdvanceLightingExample ale;
	ale.go();

	return 0;
}