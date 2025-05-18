#include "Tennis.h"

float Tennis::start_speed = 4.0f;

void Tennis::collision_and_update(Tennis& tennis) {
	glm::vec3 center_link = this->position - tennis.position;
	if (glm::length(center_link) <= 2 * this->radius && (this->v != glm::vec3(0.0f) || tennis.v != glm::vec3(0.0f))) {
		float offset = 2 * this->radius - glm::length(center_link);
		offset /= 2;
		center_link = glm::normalize(center_link);

		//防止球吸附情况出现.
		this->position += offset * center_link;
		tennis.position -= offset * center_link;
		
		glm::vec3 vn1 = glm::dot(this->v, -center_link) * (-center_link);
		glm::vec3 vn2 = glm::dot(tennis.v, center_link) * center_link;
		glm::vec3 vt1 = this->v - vn1;
		glm::vec3 vt2 = tennis.v - vn2;
		
		this->v = vn2 + vt1;
		tennis.v = vn1 + vt2;
	}
}