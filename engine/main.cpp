// Debugging
#include <core/tpnt_log.h>
#include <iostream>

// Core
#include "core/time.h"
#include "core/window.h"

// Assets
#include "managers/asset_manager.h"
#include "stb_image.h"

// Maths
#include "math/transform.h"
#include <ce_math.h>

// Rendering
#include "ce_render_fundementals.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "rendering/rendering_engine.h"

/*
 * Vertices
 */
// clang-format off
ce::Vertex vertices[] = {
	//<POS>							<COLOR>								<TEX COORD>
	glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),// 0
	glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f),// 1
	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f),// 2
	glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f),// 3
	
	glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),// 4
	glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f),// 5
	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f),// 6
	glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f),// 7
};
ce::Vertex planeVertices[] = {
	glm::vec3( 1.0f,  0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),// 0
	glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f),// 1
	glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f),// 2
	glm::vec3(-1.0f,  0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)// 3
};


/*
 * this is a cube
	  /7-4
	 / 6-5
	3-0 /
	2-1/
*/
// clang-format on
unsigned vertexCount = sizeof(vertices) / sizeof(ce::Vertex);
unsigned planeVertexCount = sizeof(planeVertices) / sizeof(ce::Vertex);
// 7<=>5
// clang-format off
GLuint indices [] = {
	// F
	0, 1, 3,
	1, 2, 3,
	// U
	4, 0, 7,
	0, 3, 7,
	// L
	3, 2, 7,
	2, 6, 7,
	// R
	4, 5, 0,
	5, 1, 0,
	// D
	1, 5, 2,
	5, 6, 2,
	// B
	7, 6, 4,
	6, 5, 4,
};

GLuint planeIndices[] = {
	0, 1, 3,
	1, 2, 3
};
// clang-format on
unsigned indexCount = sizeof(indices) / sizeof(GLuint);
unsigned planeIndexCount = sizeof(planeIndices) / sizeof(GLuint);

int main(int argc, char* argv[]) {
	LOG_INFO("Hello World");

	ce::Time* time = new ce::Time();

	ce::Window* window = new ce::Window("Cinnabar");
	ce::RenderingEngine* renderingEngine = new ce::RenderingEngine();
	renderingEngine->setFOV(75.0f);
	renderingEngine->setSize(window->getWindowSize());
	renderingEngine->setClipRange(0.1f, 100.0f);

	ce::Transform* transform = new ce::Transform();
	ce::Mesh* mesh = new ce::Mesh(vertices, vertexCount, indices, indexCount);
	ce::Material* material = new ce::Material(new ce::Shader("basic"));
	material->setTexture(new ce::Texture("uv-map.png"));

	ce::Material* planeMaterial = new ce::Material(new ce::Shader("color"));
	ce::Mesh* plane = new ce::Mesh(planeVertices, planeIndexCount, planeIndices, planeVertexCount);
	ce::Transform* planeTransform = new ce::Transform();

	float mouseSensitivity = 0.1f;
	ce::Camera* camera = new ce::Camera();
	// Seperate so i can put in a player class later
	glm::vec3 cameraVelocity(0.0f);
	camera->getTransform()->setPosition(0.0f, 0.0f, 1.5f);
	camera->getTransform()->setYaw(-90.0f);
	renderingEngine->setCamera(camera);
	/*
	 * Game Loop
	 */
	SDL_Event event;
	int running = 1;
	while (running) {
		time->update();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_MOUSEMOTION: {
					if (window->mouseVisible())
						break;
					glm::vec2 mouseDelta(event.motion.xrel, event.motion.yrel);
					mouseDelta *= mouseSensitivity;
					camera->getTransform()->yaw(mouseDelta.x);
					camera->getTransform()->pitch(-mouseDelta.y);
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					if (window->mouseVisible())
						window->setMouseVisibility(false);
					break;
				}
				case SDL_KEYDOWN: {
					float cameraSpeed = 2.5f * time->getDeltaTime();
					if (event.key.keysym.sym == SDLK_w)
						cameraVelocity.z = cameraSpeed;
					else if (event.key.keysym.sym == SDLK_s)
						cameraVelocity.z = -cameraSpeed;
					if (event.key.keysym.sym == SDLK_a)
						cameraVelocity.x = -cameraSpeed;
					else if (event.key.keysym.sym == SDLK_d)
						cameraVelocity.x = cameraSpeed;
					if (event.key.keysym.sym == SDLK_SPACE)
						cameraVelocity.y = cameraSpeed;
					else if (event.key.keysym.sym == SDLK_LSHIFT)
						cameraVelocity.y = -cameraSpeed;
					if (event.key.keysym.sym == SDLK_ESCAPE)
						window->setMouseVisibility(true);
					break;
				}
				case SDL_KEYUP: {
					if ((event.key.keysym.sym == SDLK_w && cameraVelocity.z > 0) ||
						 (event.key.keysym.sym == SDLK_s && cameraVelocity.z < 0))
						cameraVelocity.z = 0;
					if ((event.key.keysym.sym == SDLK_a && cameraVelocity.x < 0) ||
						 (event.key.keysym.sym == SDLK_d && cameraVelocity.x > 0))
						cameraVelocity.x = 0;
					if ((event.key.keysym.sym == SDLK_SPACE && cameraVelocity.y > 0) ||
						 (event.key.keysym.sym == SDLK_LSHIFT && cameraVelocity.y < 0))
						cameraVelocity.y = 0;
					break;
				}
				case SDL_QUIT: {
					running = 0;
					break;
				}
				case SDL_WINDOWEVENT: {
					renderingEngine->setSize(window->getWindowSize());
					break;
				}
			}
		}
		// Transform
		transform->roll(25.0f * time->getDeltaTime());
		transform->yaw(50.0f * time->getDeltaTime());
		transform->pitch(100.0f * time->getDeltaTime());

		// Camera
		glm::vec3
			cameraFront = camera->getTransform()->getForward(),
			cameraRight = camera->getRight(),
			cameraUp = ce::Transform::GetGlobalUp();
		camera->getTransform()->translate(
			(cameraFront * cameraVelocity.z) +
			(cameraRight * cameraVelocity.x) +
			(cameraUp * cameraVelocity.y));

		/* Render */
		renderingEngine->registerCommand({transform, material, mesh, mesh->GetIndexCount()});
		renderingEngine->registerCommand({planeTransform, planeMaterial, plane, plane->GetIndexCount()});
		renderingEngine->render();

		window->swapBuffers();
	}
	delete mesh;
	delete material;
	delete transform;

	delete renderingEngine;
	delete window;
	return 0;
}
