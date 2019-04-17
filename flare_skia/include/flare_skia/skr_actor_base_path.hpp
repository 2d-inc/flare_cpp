#ifndef _SKR_ACTOR_BASE_PATH_HPP_
#define _SKR_ACTOR_BASE_PATH_HPP_

#include "SkPath.h"

class SkPath;

namespace flare
{
	class ActorBasePath;

	class SkrActorBasePath
	{
		SkPath m_Path;
		ActorBasePath* m_BasePath;

	protected:
		bool m_IsPathValid;

	private:
		void updatePath();

	protected:
		SkrActorBasePath(ActorBasePath* path);

	public:
		const SkPath& path();
		ActorBasePath* basePath() const { return m_BasePath; }
	};
} // namespace flare

#endif