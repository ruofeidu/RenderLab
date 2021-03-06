#ifndef _ENGINE_MATERIAL_BSDF_DIFFUSE_H_
#define _ENGINE_MATERIAL_BSDF_DIFFUSE_H_

#include <CppUtil/Engine/BSDF.h>

#include <CppUtil/Basic/CosineWeightedHemisphereSampler3D.h>

namespace CppUtil {
	namespace Basic {
		class Image;
	}

	namespace Engine {
		class BSDF_Diffuse : public BSDF {
			ELE_SETUP(BSDF_Diffuse)
		public:
			BSDF_Diffuse(const glm::vec3 & albedoColor = glm::vec3(1)) :albedoColor(albedoColor), albedoTexture(nullptr) { }

			virtual glm::vec3 F(const glm::vec3 & wo, const glm::vec3 & wi, const glm::vec2 & texcoord);

			// probability density function
			virtual float PDF(const glm::vec3 & wo, const glm::vec3 & wi, const glm::vec2 & texcoord);

			// PD is probability density
			// return albedo
			// @arg0 in
			// @arg1 out
			// @arg2 out
			virtual glm::vec3 Sample_f(const glm::vec3 & wo, const glm::vec2 & texcoord, glm::vec3 & wi, float & pd);

			virtual bool IsDelta() const { return false; }

			//glm::vec3 GetAlbedo() const { return albedo; }
			//void SetAlbedo(const glm::vec3 & albedo) { this->albedo = albedo; }

		public:
			Basic::CPtr<Basic::Image> GetAlbedoTexture() const { return albedoTexture; }

			void SetAlbedoTexture(Basic::Ptr<Basic::Image> albedoTexture) {
				this->albedoTexture = albedoTexture;
			}

		private:
			const glm::vec3 GetAlbedo(const glm::vec2 & texcoord) const;

		public:
			glm::vec3 albedoColor;
			Basic::Ptr<Basic::Image> albedoTexture;

		private:
			Basic::CosineWeightedHemisphereSampler3D sampler;
		};
	}
}

#endif//!_ENGINE_MATERIAL_BSDF_DIFFUSE_H_