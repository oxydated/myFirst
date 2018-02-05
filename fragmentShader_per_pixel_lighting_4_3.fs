#version 430 core

uniform sampler2D theSampler;
in vec2 theCoord;
//varying float lightPower;

in vec4 varyNormal;
in vec4 varyLightVec;
 
//varying vec4 showNormal;
//varying vec4 showLight;

out vec4 color;

void main(){
	//vec4 theTexColor = vec4(lightPower, lightPower*0.1, lightPower*0.1, 1.0);
	vec4 backgroundLight = vec4(1.0, 1.0, 1.0, 1.0);
	
	//float lightAngle = dot(lightVec.xyz, tNormal.xyz)/ (length(lightVec.xyz) * length(tNormal.xyz) );
	float lightAngle = dot(varyLightVec.xyz, varyNormal.xyz)/ (length(varyLightVec.xyz) * length(varyNormal.xyz) );
	float lightPower = clamp(lightAngle, 0.0, 1.0);
	//float lightPower = lightAngle;
	
	//showNormal = tNormal;
	// showLight = lightVec;
	
	
	vec4 theTexColor = lightPower*texture2D( theSampler, theCoord );
	
	//vec4 theTexColor = lightPower*vec4(1.0, 1.0, 1.0, 1.0);
	
	vec4 lightColor = vec4( 1.0, 0.8, 0.95, 1.0);
	vec4 finalColor = vec4( clamp(theTexColor[0]*lightColor[0], 0.0, 1.0), clamp(theTexColor[1]*lightColor[1], 0.0, 1.0), clamp(theTexColor[2]*lightColor[2], 0.0, 1.0), 1.0);
	color = 0.90*finalColor+0.1*backgroundLight;
	//gl_FragColor = -varyNormal;	
}
