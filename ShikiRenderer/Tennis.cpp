#include "Tennis.h"

float Tennis::start_speed = 8.0f;

void Tennis::collision_and_update(Tennis& tennis) {
	glm::vec3 center_link = this->position - tennis.position;
	if (glm::length(center_link) <= 2 * this->radius && (this->v != glm::vec3(0.0f) || tennis.v != glm::vec3(0.0f))) {
		float offset = 2 * this->radius - glm::length(center_link);
		offset /= 2;
		center_link = glm::normalize(center_link);

		//防止球吸附情况出现.
		this->position += offset * center_link;
		tennis.position -= offset * center_link;
		
		//法向速度. 两小球的法向速度互换.
		glm::vec3 vn1 = glm::dot(this->v, -center_link) * (-center_link);
		glm::vec3 vn2 = glm::dot(tennis.v, center_link) * center_link;

		//切向速度.
		glm::vec3 vt1 = this->v - vn1;
		glm::vec3 vt2 = tennis.v - vn2;
		
		float ut = 0.001f; //小球之间的摩擦系数. 用于切向摩擦力冲量的计算.
		float Jn = m * glm::length(vn2 - vn1);	//法向冲量.
		float Jt = ut * Jn;	//切向摩擦力冲量
		float len1 = glm::length(vt1), len2 = glm::length(vt2);
		if (len1 < 0.001f && len2 < 0.001f) {
			//若切向速度过小, 不考虑切向速度的变化.
			this->v = vn2 + vt1;
			tennis.v = vn1 + vt2;
		}
		else {
			glm::vec3 vt1_, vt2_;
			if (len1 > len2) {
				glm::vec3 vt = glm::normalize(vt1);
				vt1_ = vt1 - Jt / m * vt;  
				vt2_ = vt2 + Jt / m * (-vt);
			}
			else {
				glm::vec3 vt = glm::normalize(vt2);
				vt1_ = vt1 - Jt / m * (-vt);
				vt2_ = vt2 + Jt / m * vt;
			}

			center_link = -center_link;
			glm::vec2 a = glm::vec2(this->v.x, this->v.z);
			glm::vec2 b = glm::vec2(center_link.x, center_link.z);
			if (a.x * b.y - a.y * b.x > 0) {
				//中心连线在母球速度逆时针方向
				this->w = this->w + Jt * radius / I;
				tennis.w = tennis.w - Jt * radius / I;
			}
			else {
				this->w = this->w - Jt * radius / I;
				tennis.w = tennis.w + Jt * radius / I;
			}
		
			this->v = vn2 + vt1_;
			tennis.v = vn1 + vt2_;
		}
	}
}