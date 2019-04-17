#include "GLFW/glfw3.h"
#include "GrBackendSurface.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkColorSpace.h"
#include "SkSurface.h"
#include "SkTypes.h"
#include "flare/animation/actor_animation.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include "flare/exceptions/unsupported_version_exception.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_skia/skr_actor.hpp"
#include "flare_skia/skr_actor_artboard.hpp"
#include "flare_skia/to_skia.hpp"
#include "gl/GrGLInterface.h"
#include "layout.hpp"
#include <cmath>
#include <stdio.h>


const char* fileName = "assets/Filip.flr";
const char* animationName = "idle";
// const char* fileName = "assets/Teddy.flr";
// const char* animationName = "test";

bool isRightMouseDown = false;
double mouseX = 0.0, mouseY = 0.0;
double lastMouseX = 0.0, lastMouseY = 0.0;
double viewX = 0.0, viewY = 0.0;
double viewTargetX = 0.0, viewTargetY = 0.0;
double viewZoom = 1.0;
double viewZoomTarget = 1.0;
int perfBarWidth = 10;

void zoomTo(double x, double y, double scale)
{
	scale = std::max(0.05, scale);
	double zoomDelta = scale / viewZoomTarget;
	viewZoomTarget = scale;

	double ox = x - viewTargetX;
	double oy = y - viewTargetY;

	double ox2 = ox * zoomDelta;
	double oy2 = oy * zoomDelta;

	viewTargetX += ox - ox2;
	viewTargetY += oy - oy2;
}

void glfwErrorCallback(int error, const char* description) { puts(description); }

static void glfwCursorPositionCallback(GLFWwindow* window, double x, double y)
{
	mouseX = x;
	mouseY = y;
	if (isRightMouseDown)
	{
		// dragging
		double dx = (x - lastMouseX);
		double dy = (y - lastMouseY);
		viewTargetX += dx;
		viewTargetY += dy;
	}
	lastMouseX = mouseX;
	lastMouseY = mouseY;
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
        viewTargetX = viewTargetY = 0.0f;
		viewZoomTarget = 1.0f;
	}
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		switch (action)
		{
			case GLFW_PRESS:
				lastMouseX = mouseX;
				lastMouseY = mouseY;
				isRightMouseDown = true;
				break;
			case GLFW_RELEASE:
				isRightMouseDown = false;
				break;
		}
	}
}

std::vector<float> performanceBars;
int performanceIndex = 0;

void glfwScrollCallback(GLFWwindow* window, double x, double y) { zoomTo(mouseX, mouseY, viewZoomTarget + y / 22.0); }

int main()
{
	if (!glfwInit())
	{
		// Initialization failed
		printf("Failed to initialize glfw.\n");
		return 1;
	}
	glfwSetErrorCallback(glfwErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Flare", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(1920, 1080, "Flare", NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		printf("Failed to make window or GL.\n");
		glfwTerminate();
		return 1;
	}
	glfwSetCursorPosCallback(window, glfwCursorPositionCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetScrollCallback(window, glfwScrollCallback);

	glfwMakeContextCurrent(window);
	// Enable VSYNC.
	glfwSwapInterval(1);

	// Setup Skia
	GrContextOptions options;
	sk_sp<GrContext> context = GrContext::MakeGL(nullptr, options);
	GrGLFramebufferInfo framebufferInfo;
	// glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferInfo.fFBOID);
	framebufferInfo.fFBOID = 0;
	framebufferInfo.fFormat = GL_RGBA8;

	SkSurface* surface = nullptr;
	SkCanvas* canvas = nullptr;

	flare::SkrActor* actor = new flare::SkrActor();

	try
	{
		actor->load(fileName);
	}
	catch (flare::OverflowException ex)
	{
		printf("Bad data, got an overflow. %s\n", ex.message().c_str());
	}
	catch (flare::UnsupportedVersionException ex)
	{
		printf("Unsupported version. %d %d\n", ex.versionFound(), ex.versionRequired());
	}
	printf("Loaded ok.\n");

	flare::SkrActorArtboard* artboard = actor->artboard<flare::SkrActorArtboard>();
	artboard->initializeGraphics();

	flare::ActorAnimation* animation = artboard->animation(animationName);
	float animationTime = 0.0f;

	int width = 0, height = 0;
	int lastScreenWidth = 0, lastScreenHeight = 0;
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &width, &height);
		if (surface == nullptr || width != lastScreenWidth || height != lastScreenHeight)
		{
			performanceBars.resize(width / perfBarWidth);
			performanceIndex = 0;

			lastScreenWidth = width;
			lastScreenHeight = height;

			SkColorType colorType;
			if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig)
			{
				colorType = kRGBA_8888_SkColorType;
			}
			else
			{
				colorType = kBGRA_8888_SkColorType;
			}

			GrBackendRenderTarget backendRenderTarget(width, height,
			                                          0, // sample count
			                                          0, // stencil bits
			                                          framebufferInfo);

			delete surface;
			surface = SkSurface::MakeFromBackendRenderTarget(context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin,
			                                                            colorType, nullptr, nullptr)
			                         .release();
			if (surface == nullptr)
			{
				printf("Failed to create Skia surface\n");
				return 1;
			}
			canvas = surface->getCanvas();
		}

		double time = glfwGetTime();
		float elapsed = (float)(time - lastTime);
		lastTime = time;

		// --- update camera
		double ds = viewZoomTarget - viewZoom;
		double dx = viewTargetX - viewX;
		double dy = viewTargetY - viewY;

		double factor = std::min(1.0, elapsed * 16.0);

		if (std::abs(ds) > 0.00001)
		{
			ds *= factor;
		}
		if (std::abs(dx) > 0.01)
		{
			dx *= factor;
		}
		if (std::abs(dy) > 0.01)
		{
			dy *= factor;
		}

		viewZoom += ds;
		viewX += dx;
		viewY += dy;

		flare::Mat2D view;
		view[0] = viewZoom;
		view[3] = viewZoom;
		view[4] = viewX;
		view[5] = viewY;
		// ---

		if (animation != nullptr)
		{
			animationTime += elapsed;
			animation->apply(std::fmod(animationTime, animation->duration()), artboard, 1.0);
		}
		artboard->advance(elapsed);

		SkPaint paint;
		paint.setColor(SK_ColorDKGRAY);
		canvas->drawPaint(paint);
		flare::AABB screenBounds(0.0f, 0.0f, width, height);
		canvas->save();
		canvas->concat(flare::ToSkia::convert(view));
		flare::Alignment::center().transform(canvas, screenBounds, artboard->aabb(), flare::Fit::contain);

		artboard->draw(canvas);
		canvas->restore();

		// Draw some perf metrics.
		float fps = 1.0 / elapsed;
		performanceBars[performanceIndex % performanceBars.size()] = fps;
		performanceIndex = (performanceIndex + 1) % performanceBars.size();

		float x = 0.0;
		paint.setColor(SkColorSetARGB(0x88, 0xAA, 0xAA, 0xAA));
		for (float fps : performanceBars)
		{
			float h = fps / 60.0 * height/3.0;

			canvas->drawRect({x, height - h, x+static_cast<SkScalar>(perfBarWidth)-1.0f, static_cast<SkScalar>(height)}, paint);

			x += perfBarWidth;
		}
		paint.setColor(SkColorSetARGB(0xFF, 0xFF, 0x56, 0x78));
		canvas->drawRect({0.0f, height*0.66f, static_cast<SkScalar>(width), height*0.66f+2.0f}, paint);

		context->flush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup.
	delete actor;

	// Cleanup Skia.
	delete surface;
	context = nullptr;

	// Cleanup GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}