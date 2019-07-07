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

	bvec3 testVec = lessThan(texture2D( theSampler, theCoord ).xyz, vec3(0.2, 0.2, 0.2));
	bool testIfWire = all(testVec);

	vec4 theTexColorA = vec4(0.75, 1.0, 1.0, 1.0);
	vec4 theTexColorB = (0.75*lightPower + backgroundLight) * vec4(0.5, 0.5, 0.5, 1.0);

	float par = clamp(pow(texture2D( theSampler, theCoord ).w, 2), 0.0, 1.0);	

	vec4 theTexColor  = par * theTexColorA + (1.0 -par)*theTexColorB;

	color = theTexColor;
}
