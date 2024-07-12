#version 450

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

layout(set = 1, binding = 0) uniform GLTFMaterialData{   
	vec4 colorFactors;
	float metallicFactor;
} materialData;

layout(set = 1, binding = 1) uniform sampler2D colorTex;

void main()  {	
	outFragColor = texture(colorTex,inUV);
}
