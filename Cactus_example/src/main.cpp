#include "GLFW/glfw3.h"
#include "GrBackendSurface.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkColorSpace.h"
#include "SkSurface.h"
#include "SkTypes.h"
#include "flare/custom_property.hpp"
#include "flare/actor_bone.hpp"
#include "flare/actor_node_solo.hpp"
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
#include <iostream>



const char* fileName = "assets/Cactus_No_Joy.flr";
const char* animationName = "Idle";

float happyAnimTime = 0.0f;
bool playHappy = false;

bool isMouseDown = false;
double mouseX = 0.0, mouseY = 0.0;
double lastMouseX = 0.0, lastMouseY = 0.0;
double viewX = 0.0, viewY = 0.0;
double viewZoom = 1.0;
int perfBarWidth = 10;
int currSoloNode = 1;

void glfwErrorCallback(int error, const char* description) {
    puts(description);
    
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
        printf("here");
       
	}
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		switch (action)
		{
			case GLFW_PRESS:
				lastMouseX = mouseX;
				lastMouseY = mouseY;
				isMouseDown = true;
           
				break;
			case GLFW_RELEASE:
				isMouseDown = false;
				break;
		}
	}
}

std::vector<float> performanceBars;
int performanceIndex = 0;



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

	if (!window)
	{
		// Window or OpenGL context creation failed
		printf("Failed to make window or GL.\n");
		glfwTerminate();
		return 1;
	}
    
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	
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

	flare::ActorNode* myNode = static_cast<flare::ActorNode*>(artboard->component("Face"));
	/*printf("node's x %f\n", myNode->x());
	printf("node's y %f\n", myNode->y());
	printf("node's scale x %f\n", myNode->scaleX());
	printf("node's scale y %f\n", myNode->scaleY());
	printf("node's rotation %f\n", myNode->rotation());
	printf("node's opactiy %f\n", myNode->opacity());*/
    

    flare::ActorNode* cpNode = static_cast<flare::ActorNode*>(artboard->component("Scale Node_Special Property"));
            
    flare::CustomBooleanProperty* customProp = cpNode->getCustomBooleanProperty("huh_sound");
   // printf(customProp->value() ? "t" : "f");

	flare::ActorBone* bone = static_cast<flare::ActorBone*>(artboard->component("Bone"));
	//printf("node's length %f\n", bone->length());

	flare::ActorAnimation* animation = artboard->animation(animationName);
    
    flare::ActorAnimation* happyAnim = artboard->animation("Mustache_New");
	float animationTime = 0.0f;
    
    flare::ActorAnimation* eventAnimation = artboard->animation("Mustache_New");

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
			surface = SkSurface::MakeFromBackendRenderTarget(context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr).release();
            
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


		flare::Mat2D view;
		view[0] = viewZoom;
		view[3] = viewZoom;
		view[4] = viewX;
		view[5] = viewY;
		// ---
        std::vector<flare::ActorAnimationEvent> events;
        
        double currLayerAnim = animationTime;

        animationTime += elapsed;
		if (animation != nullptr && playHappy == false)
		{
			animation->apply(std::fmod(animationTime, animation->duration()), artboard, 1.0);
		}
        
        eventAnimation->triggerEvents(artboard,currLayerAnim, animationTime, events);
        
        happyAnimTime += elapsed;
        if(happyAnimTime > happyAnim->duration() && playHappy == true){
            playHappy = false;
            
        }
        
        if(playHappy == true)
        {
            happyAnim->apply(std::fmod(happyAnimTime, happyAnim->duration()), artboard, 1.0);
        }
        
       artboard->advance(elapsed);

        
        if (isMouseDown == true)
        {
            currSoloNode ++;
            if(currSoloNode > 5)
            {
                currSoloNode = 1;
            }
          
            flare::ActorNodeSolo* soloNode = static_cast<flare::ActorNodeSolo*>(artboard->component("Mustache_Solo"));
            soloNode->setActiveChildIndex(currSoloNode);

            //play new animation
            playHappy = true;
            isMouseDown = false;
            happyAnimTime = 0;
        }
        
       
        
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
