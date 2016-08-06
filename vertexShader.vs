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

////////////////////////////////////////////////////////////////// uniform array test

attribute float vIndices;
uniform float weight[200];

//////////////////////////////////////////////////////////////////

void main(){
	vec4 tempPos = vPosition;
	theCoord = vTexCoord;
	mat4 tempView = View;
	tempView[2].z = gl_DepthRange.diff/2.0;
	tempView[3].z = ( gl_DepthRange.near +gl_DepthRange.far) /2.0;
	vec4 pos = tempView * Proj * World * tempPos;
	pos = pos / pos.w;
	vec4 tNormal = invWorld * vNormal;
	tNormal = tNormal / tNormal.w;
	vec4 tLight = tempView * Proj * World * vec4(vLightPos, 1.0);
	tLight = tLight / tLight.w;
	vec4 lightVec = vec4( pos.xyz - tLight.xyz, 1.0);
	float lightAngle = dot(lightVec.xyz, tNormal.xyz)/ (length(lightVec.xyz) * length(tNormal.xyz) );
	lightPower = clamp(lightAngle + 0.75, -1.0, 1.0);
	gl_Position = pos;
}
