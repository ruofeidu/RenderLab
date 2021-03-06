#ifndef _ENGINE_VIEWER_RASTER_H_
#define _ENGINE_VIEWER_RASTER_H_

#include <CppUtil/Engine/RasterBase.h>

namespace CppUtil {
	namespace Engine {
		class Raster : public RasterBase {
			ELEVISITOR_SETUP(Raster)
		public:
			Raster(Basic::Ptr<Scene> scene) : RasterBase(scene) { }

			void Draw();
			void Init();

		protected:
			virtual void Visit(Basic::Ptr<BSDF_Diffuse> bsdf);
			virtual void Visit(Basic::Ptr<BSDF_Glass> bsdf);
			virtual void Visit(Basic::Ptr<BSDF_Mirror> bsdf);
			virtual void Visit(Basic::Ptr<BSDF_Emission> bsdf);
			virtual void Visit(Basic::Ptr<BSDF_CookTorrance> bsdf);
			virtual void Visit(Basic::Ptr<BSDF_MetalWorkflow> bsdf);
			virtual void Visit(Basic::Ptr<BSDF_FrostedGlass> bsdf);

		private:
			void InitShaders();
			void InitShaderBasic();
			void InitShaderDiffuse();
			void InitShaderMetalWorkflow();
			void InitShaderFrostedGlass();

		private:
			OpenGL::Shader shader_basic;
			OpenGL::Shader shader_diffuse;
			OpenGL::Shader shader_metalWorkflow;
			OpenGL::Shader shader_frostedGlass;
		};
	}
}

#endif//!_ENGINE_VIEWER_RASTER_H_
