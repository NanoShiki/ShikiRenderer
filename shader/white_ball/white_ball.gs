#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoord;
	vec3 worldPos;
	vec3 normal;
} gs_in[];

out GS_OUT{
    vec2 texCoord;
	vec3 worldPos;
	vec3 normal;
} gs_out;

uniform float explosion;

vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * explosion; 
    return position + vec4(direction, 0.0);
} 
vec3 explode(vec3 position, vec3 normal)
{
    vec3 direction = normal * explosion; 
    return position + direction;
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

vec3 GetWorldNormal()
{
   vec3 a = gs_in[0].worldPos - gs_in[1].worldPos;
   vec3 b = gs_in[2].worldPos - gs_in[1].worldPos;
   return normalize(cross(a, b));
}  

void main() {  
    vec3 normal = GetNormal();
    vec3 worldNormal = GetWorldNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.texCoord = gs_in[0].texCoord;
    gs_out.normal = gs_in[0].normal;
    gs_out.worldPos = explode(gs_in[0].worldPos, worldNormal);
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.texCoord = gs_in[1].texCoord;
    gs_out.normal = gs_in[1].normal;
    gs_out.worldPos = explode(gs_in[1].worldPos, worldNormal);
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.texCoord = gs_in[2].texCoord;
    gs_out.normal = gs_in[2].normal;
    gs_out.worldPos = explode(gs_in[2].worldPos, worldNormal);
    EmitVertex();
    EndPrimitive();
}  