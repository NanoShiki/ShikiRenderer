#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoord;
	vec3 worldPos;
	vec3 worldNormal;
} gs_in[];

out vec2 texCoord;
out vec3 worldPos;
out vec3 worldNormal; 

uniform float explosion;

vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * explosion; 
    return position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].worldPos) - vec3(gl_in[1].worldPos);
   vec3 b = vec3(gl_in[2].worldPos) - vec3(gl_in[1].worldPos);
   return normalize(cross(a, b));
}  

void main() {  
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    texCoord = gs_in[0].texCoord;
    worldNormal = gs_in[0].worldNormal;
    worldPos = explode(gs_in[0].worldPos, worldNormal);
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    texCoord = gs_in[1].texCoord;
    worldNormal = gs_in[1].worldNormal;
    worldPos = explode(gs_in[1].worldPos, worldNormal);
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    texCoord = gs_in[2].texCoord;
    worldNormal = gs_in[2].worldNormal;
    worldPos = explode(gs_in[2].worldPos, worldNormal);
    EmitVertex();
    EndPrimitive();
}  