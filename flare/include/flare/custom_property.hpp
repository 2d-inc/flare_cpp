#ifndef _FLARE_CUSTOMPROPERTY_HPP_
#define _FLARE_CUSTOMPROPERTY_HPP_

#include "actor_component.hpp"
#include <string>

namespace flare
{
	class CustomIntProperty : public ActorComponent
	{
		typedef ActorComponent Base;

	private:
		int m_Value;

	public:
		CustomIntProperty();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const CustomIntProperty* property, ActorArtboard* artboard);
		static CustomIntProperty* read(ActorArtboard* artboard, BlockReader* reader, CustomIntProperty* property = NULL);

		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		int value() const;
		void value(int v);
	};
	class CustomFloatProperty : public ActorComponent
	{
		typedef ActorComponent Base;

	private:
		float m_Value;

	public:
		CustomFloatProperty();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const CustomFloatProperty* property, ActorArtboard* artboard);
		static CustomFloatProperty* read(ActorArtboard* artboard, BlockReader* reader, CustomFloatProperty* property = NULL);

		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		float value() const;
		void value(float v);
	};
	class CustomStringProperty : public ActorComponent
	{
		typedef ActorComponent Base;

	private:
		std::string m_Value;

	public:
		CustomStringProperty();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const CustomStringProperty* property, ActorArtboard* artboard);
		static CustomStringProperty* read(ActorArtboard* artboard, BlockReader* reader, CustomStringProperty* property = NULL);

		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		const std::string& value() const;
		void value(const std::string& v);
	};
	class CustomBooleanProperty : public ActorComponent
	{
		typedef ActorComponent Base;

	private:
		bool m_Value;

	public:
		CustomBooleanProperty();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const CustomBooleanProperty* property, ActorArtboard* artboard);
		static CustomBooleanProperty* read(ActorArtboard* artboard, BlockReader* reader, CustomBooleanProperty* property = NULL);

		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		const bool value() const;
		void value(const bool v);
	};
} // namespace flare
#endif