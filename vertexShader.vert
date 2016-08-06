 attribute vec4 vPosition;                     
 uniform vec4 vLightPos;                       
 attribute vec2 vTexCoord;                     
 attribute vec4 vNormal;                       
 uniform mat4 Proj;                            
 uniform mat4 View;                            
 uniform mat4 World;                            
 uniform mat4 invWorld;                          
 uniform sampler2D theSampler;                   
 varying float lightPower;                            
 varying vec2 theCoord;                         
 void main(){                                  
	vec4 tempPos = vPosition;                    
	theCoord = vTexCoord; 
	
	tempPos = World * tempPos;                     
	tempPos = tempPos / tempPos.w;                  
	vec4 pos =  Proj * tempPos; 
	pos = pos / pos.w; 
	
	vec4 lightVec = vec4(vLightPos.xyz - vPosition.xyz, 1.0);
	
	float lightAngle = dot(lightVec.xyz, vNormal.xyz)/ (length(lightVec.xyz) * length(vNormal.xyz) );
	
	// vec4 theLightPos = Proj*vLightPos;     
	// theLightPos = theLightPos/ theLightPos.w;    
	// vec4 theLight = normalize(  -(Proj*World*vPosition) + (Proj*vLightPos) );  
	// vec4 theNormal = normalize(  invWorld*(-vNormal) );             
	// lightPower = clamp(dot( theLight, theNormal  ), 0.0, 1.0 );
	
	lightPower = lightAngle;
	gl_Position = pos;                    
 }                                            