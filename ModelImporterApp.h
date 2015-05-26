#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.h"
#include "AssimpConverter.h"
#include "Renderer.h"
#include "ShapeRenderable.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include "SphereShape.h"
#include "BoxShape.h"
#include "Node.h"
#include "DebugDrawer.h"
#include <iostream>

class ModelImporterApp : public Application{
public:

	virtual void Render(double elapseTime) override {
		_renderer.render();
		_drawer->dirtyDraw();
		_drawer->drawBoundingBox(_node1->getBoundingBox(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		_drawer->drawBoundingBox(_node2->getBoundingBox(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		_drawer->drawBoundingBox(_root->getBoundingBox(), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		static float angle = 0.0f;
		_node1->setMatrix(glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)));
		angle += 0.1f;
	}

	virtual void Setup() override {
		//_model = new Model("datas/teapot.obj");

		auto root = new Node;
		{
			_node1 = AssimpConverter::Instance()->LoadModelFile("datas/teapot.obj");
			_node1->setMatrix(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)));
			auto bs = _node1->getBoundingBox();
			auto sr = new ShapeRenderable(new BoxShape((bs->getMax().x - bs->getMin().x) / 2.0f, (bs->getMax().y - bs->getMin().y) / 2.0f, (bs->getMax().z - bs->getMin().z) / 2.0f, glm::translate(glm::vec3((bs->getMax() + bs->getMin()) / 2.0f))), ShapeRenderable::ShapeRenderType::SR_LINE, 32);
			//_renderer.attachRenderableObject(sr);
			root->addChild(_node1);
		}

		{
			_node2 = AssimpConverter::Instance()->LoadModelFile("datas/teapot.obj");
			_node2->setMatrix(glm::translate(glm::vec3(100.0f, 50.0f, 50.0f)));
			auto bs = _node2->getBoundingBox();
			auto sr = new ShapeRenderable(new BoxShape((bs->getMax().x - bs->getMin().x) / 2.0f, (bs->getMax().y - bs->getMin().y) / 2.0f, (bs->getMax().z - bs->getMin().z) / 2.0f, glm::translate(glm::vec3((bs->getMax() + bs->getMin()) / 2.0f))), ShapeRenderable::ShapeRenderType::SR_LINE, 32);
			//_renderer.attachRenderableObject(sr);
			root->addChild(_node2);
		}

		auto bs = root->getBoundingBox();
		auto sr = new ShapeRenderable(new BoxShape((bs->getMax().x - bs->getMin().x) / 2.0f, (bs->getMax().y - bs->getMin().y) / 2.0f, (bs->getMax().z - bs->getMin().z) / 2.0f, glm::translate(glm::vec3((bs->getMax() + bs->getMin()) / 2.0f))), ShapeRenderable::ShapeRenderType::SR_LINE, 32);
		//_renderer.attachRenderableObject(sr);

		_renderer.attachRenderableObject(root);
		_renderer.depthTest(true);
		_renderer.setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		_root = root;
		_drawer = new DebugDrawer(&_renderer);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

private:

	Node *_node1;
	Node *_node2;
	Node *_root;
	DebugDrawer *_drawer;
};