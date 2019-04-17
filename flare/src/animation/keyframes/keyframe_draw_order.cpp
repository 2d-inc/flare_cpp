#include "flare/animation/keyframes/keyframe_draw_order.hpp"
#include "flare/actor.hpp"
#include "flare/actor_drawable.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

			
KeyFrameDrawOrder::KeyFrameDrawOrder() :
	m_OrderedNodes(NULL),
	m_OrderedNodesCount(0)
{

}

KeyFrameDrawOrder::~KeyFrameDrawOrder()
{
	delete m_OrderedNodes;
}


bool KeyFrameDrawOrder::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	m_OrderedNodesCount = (int)reader->readUint16();
	m_OrderedNodes = new DrawOrderIndex[m_OrderedNodesCount];
	for(int i = 0; i < m_OrderedNodesCount; i++)
	{
		DrawOrderIndex& drawOrder = m_OrderedNodes[i];
		drawOrder.nodeIdx = reader->readUint16();
		drawOrder.order = reader->readUint16();
	}
	return true;
}

void KeyFrameDrawOrder::setNext(KeyFrame* frame)
{
	// Do nothing, we don't interpolate.
}

void KeyFrameDrawOrder::apply(ActorComponent* component, float mix)
{
	ActorArtboard* artboard = component->artboard();

	for(int i = 0; i < m_OrderedNodesCount; i++)
	{
		DrawOrderIndex& doi = m_OrderedNodes[i];
		// Some pretty hard assumptions being made here. We're assuming we had good data to begin with.
		// Could validate it at load time by passing the actor into the read methods.
		ActorDrawable* renderNode = dynamic_cast<ActorDrawable*>(artboard->component(doi.nodeIdx));
		if(renderNode != nullptr)
		{
			renderNode->drawOrder(doi.order);
		}
	}
}

void KeyFrameDrawOrder::applyInterpolation(ActorComponent* node, float time, KeyFrame* toFrame, float mix)
{
	apply(node, mix);
}