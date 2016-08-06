precision mediump float;
uniform sampler2D theSampler;
varying vec2 theCoord;
 varying float lightPower;
void main(){
	vec4 theTexColor = vec4(lightPower, lightPower*0.1, lightPower*0.1, 1.0);
	vec4 backgroundLight = vec4(1.0, 1.0, 0.8, 1.0);
	theTexColor = lightPower*texture2D( theSampler, theCoord );
	vec4 lightColor = vec4( 1.0, 0.8, 0.95, 1.0);
	vec4 finalColor = vec4( theTexColor[0]*lightColor[0], theTexColor[1]*lightColor[1], theTexColor[2]*lightColor[2], 1.0);
	gl_FragColor = 0.90*finalColor+0.1*backgroundLight;
}
