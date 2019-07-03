#version 430 core

uniform sampler2D theSampler;

smooth in vec2 theCoord;
in vec4 varyNormal;
smooth in vec4 varyLightVec;

out vec4 color;

void main(){
	vec4 backgroundLight = vec4(0.25, 0.25, 0.25, 0.25);

	vec3 lightNorm = normalize(varyLightVec.xyz - gl_FragCoord.xyz );
	
	float lightAngle = dot(lightNorm, varyNormal.xyz);
	float lightPower = clamp(lightAngle, 0.0, 1.0);	
	
	vec4 theTexColor = (0.75*lightPower + backgroundLight) * texture2D( theSampler, theCoord );

	color = theTexColor;
}
