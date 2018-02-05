#version 430 core

in vec3 vPosition;
uniform vec3 vLightPos;
in vec2 vTexCoord;
in vec4 vNormal;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 World;
uniform mat4 invWorld;
uniform sampler2D theSampler;
//varying float lightPower;
out vec2 theCoord;

out vec4 varyNormal;
out vec4 varyLightVec;

//varying vec4 showNormal;
// varying vec4 showLight;

////////////////////////////////////////////////////////////////// uniform array test

//in float vIndices;
// float vIndices;
// uniform float weight[200];

// out vec4 vertexColor;

//////////////////////////////////////////////////////////////////

void main(){
	vec4 tempPos;
	tempPos.xyz = vPosition;
	tempPos.w = 1.0;
	mat4 tempView = View;
	tempView[2].z = gl_DepthRange.diff/2.0;
	tempView[3].z = ( gl_DepthRange.near +gl_DepthRange.far) /2.0;
	vec4 worldPos = World * tempPos;
	worldPos = worldPos / worldPos.w;
	vec4 pos = tempView * Proj * World * tempPos;
	vec4 posLatter = pos / pos.w;
	
	vec4 tempNormal = vec4(vNormal.xyz, 0.0);
	
	vec4 tNormal = invWorld * tempNormal;
	tNormal = vec4(normalize(tNormal.xyz), 0.0);
	//tNormal = tNormal / tNormal.w;
	
	vec4 worldLight = World * vec4(vLightPos, 1.0);	
	worldLight = worldLight/worldLight.w;
	
	//vec4 worldLight = vec4(vLightPos, 1.0);	
	//vec4 lightVec = vec4( normalize(worldLight.xyz - worldPos.xyz), 0.0);
	vec4 lightVec = vec4( normalize(vLightPos.xyz - tempPos.xyz), 0.0);
	
	theCoord = vTexCoord;
	//lightPower = clamp(lightAngle + 0.75, -1.0, 1.0);
	
	//showNormal = tNormal;
	// showLight = lightVec;
	varyNormal = tempNormal;
	varyLightVec = lightVec;
	
	gl_Position = posLatter;
}
