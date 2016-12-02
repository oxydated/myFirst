attribute vec3 vPosition;
uniform vec3 vLightPos;
attribute vec2 vTexCoord;
attribute vec4 vNormal;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 World;
uniform mat4 invWorld;
uniform sampler2D theSampler;
//varying float lightPower;
varying vec2 theCoord;

varying vec4 varyNormal;
varying vec4 varyLightVec;

//varying vec4 showNormal;
// varying vec4 showLight;

////////////////////////////////////////////////////////////////// uniform array test

attribute float vIndices;
uniform float weight[200];

//////////////////////////////////////////////////////////////////

void main(){
	vec4 tempPos;
	tempPos.xyz = vPosition;
	tempPos.w = 1.0;
	mat4 tempView = View;
	tempView[2].z = gl_DepthRange.diff/2.0;
	tempView[3].z = ( gl_DepthRange.near +gl_DepthRange.far) /2.0;
	vec4 worldPos = World * tempPos;
	vec4 pos = tempView * Proj * World * tempPos;
	//vec4 pos = View * Proj * World * tempPos;
	vec4 posLatter = pos / pos.w;
	vec4 tNormal = invWorld * vNormal;
	tNormal = tNormal / tNormal.w;
	
	vec4 worldLight = World * vec4(vLightPos, 1.0);	
	vec4 lightVec = vec4( worldLight.xyz - worldPos.xyz, 1.0);
	
	theCoord = vTexCoord;
	//lightPower = clamp(lightAngle + 0.75, -1.0, 1.0);
	
	//showNormal = tNormal;
	// showLight = lightVec;
	varyNormal = tNormal;
	varyLightVec = lightVec;
	
	gl_Position = posLatter;
}
