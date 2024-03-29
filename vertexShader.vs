attribute vec4 vPosition;
uniform vec3 vLightPos;
attribute vec2 vTexCoord;
attribute vec4 vNormal;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 World;
uniform mat4 invWorld;
uniform sampler2D theSampler;
varying float lightPower;
varying vec2 theCoord;

//varying vec4 showNormal;
// varying vec4 showLight;

////////////////////////////////////////////////////////////////// uniform array test

attribute float vIndices;
uniform float weight[200];

//////////////////////////////////////////////////////////////////

void main(){
	vec4 tempPos = vPosition;
	mat4 tempView = View;
	tempView[2].z = gl_DepthRange.diff/2.0;
	tempView[3].z = ( gl_DepthRange.near +gl_DepthRange.far) /2.0;
	vec4 worldPos = World * tempPos;
	vec4 pos = tempView * Proj * World * tempPos;
	pos = pos / pos.w;
	vec4 tNormal = invWorld * vNormal;
	//vec4 tNormal = World * vNormal;
	tNormal = tNormal / tNormal.w;
	vec4 tLight = tempView * Proj * World * vec4(vLightPos, 1.0);
	tLight = tLight / tLight.w;
	vec4 worldLight = World * vec4(vLightPos, 1.0);
	tLight = tLight / tLight.w;
	vec4 lightVec = vec4( worldLight.xyz - worldPos.xyz, 1.0);
	float lightAngle = dot(lightVec.xyz, tNormal.xyz)/ (length(lightVec.xyz) * length(tNormal.xyz) );
	theCoord = vTexCoord;
	lightPower = clamp(lightAngle + 0.75, -1.0, 1.0);
	
	//showNormal = tNormal;
	// showLight = lightVec;
	
	gl_Position = pos;
}
