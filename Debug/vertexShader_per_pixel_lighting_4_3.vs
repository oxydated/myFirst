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

smooth out vec2 theCoord;

out vec4 varyNormal;
smooth out vec4 varyLightVec;

//out vec4 inWorldPos;

in int boneNumVertAttrib;
in int boneOffsetVertAttrib;

layout(std430, binding=1) buffer boneIndexes{
	int boneIndexesForSkinVertices [];
};

layout(std430, binding=2) buffer boneWeight{
	float boneWeightForSkinVertices [];
};

layout(std430, binding=3) buffer fromSkinPose{
	mat2x4 fromSkinPoseToCurrentTransf[];
};

vec4 dual_quat_transform_normal(mat2x4 q, vec4 v){
	float qs = q[0][3];
	float qx = q[0][0];
	float qy = q[0][1];
	float qz = q[0][2];

	float vx = v.x;
	float vy = v.y;
	float vz = v.z;

	return vec4(
				(1 - 2*pow(qy,2) - 2*pow(qz,2))*vx + 2*(qx*qy - qs*qz)*vy + 2*(qs*qy + qx*qz)*vz,
				2*(qx*qy + qs*qz)*vx + (1 - 2*pow(qx,2) - 2*pow(qz,2))*vy + (-2*qs*qx + 2*qy*qz)*vz,
				(-2*qs*qy + 2*qx*qz)*vx + 2*(qs*qx + qy*qz)*vy + (1 - 2*pow(qx,2) - 2*pow(qy,2))*vz,
				1.
	);
}

vec4 dual_quat_transform_point( mat2x4 q, vec4 p ){
	return vec4(
			-2*q[1].x*q[0].w + 2*q[1].w*q[0].x - 2*q[1].z*q[0].y + 2*q[1].y*q[0].z + p.y*(2*q[0].x*q[0].y - 2*q[0].w*q[0].z) + p.z*(2*q[0].w*q[0].y + 2*q[0].x*q[0].z) + p.x*(q[0].w*q[0].w + q[0].x*q[0].x - q[0].y*q[0].y - q[0].z*q[0].z),
			-2*q[1].y*q[0].w + 2*q[1].z*q[0].x + 2*q[1].w*q[0].y - 2*q[1].x*q[0].z + p.x*(2*q[0].x*q[0].y + 2*q[0].w*q[0].z) + p.z*(-2*q[0].w*q[0].x + 2*q[0].y*q[0].z) + p.y*(q[0].w*q[0].w - q[0].x*q[0].x + q[0].y*q[0].y - q[0].z*q[0].z),
			-2*q[1].z*q[0].w - 2*q[1].y*q[0].x + 2*q[1].x*q[0].y + 2*q[1].w*q[0].z + p.x*(-2*q[0].w*q[0].y + 2*q[0].x*q[0].z) + p.y*(2*q[0].w*q[0].x + 2*q[0].y*q[0].z) + p.z*(q[0].w*q[0].w - q[0].x*q[0].x - q[0].y*q[0].y + q[0].z*q[0].z),
			1.);
}

vec4 dual_quat_transform_vector( mat2x4 q, vec4 v ){
	return vec4(
			(2*q[0].x*q[0].y - 2*q[0].w*q[0].z)*v.y + (2*q[0].w*q[0].y + 2*q[0].x*q[0].z)*v.z + v.x*(q[0].w*q[0].w + q[0].x*q[0].x - q[0].y*q[0].y - q[0].z*q[0].z),
			(2*q[0].x*q[0].y + 2*q[0].w*q[0].z)*v.x + (-2*q[0].w*q[0].x + 2*q[0].y*q[0].z)*v.z + v.y*(q[0].w*q[0].w - q[0].x*q[0].x + q[0].y*q[0].y - q[0].z*q[0].z),
			(-2*q[0].w*q[0].y + 2*q[0].x*q[0].z)*v.x + (2*q[0].w*q[0].x + 2*q[0].y*q[0].z)*v.y + v.z*(q[0].w*q[0].w - q[0].x*q[0].x - q[0].y*q[0].y + q[0].z*q[0].z),
			1.);
}

mat2x4 conjugate_quat(mat2x4 q){
	return mat2x4(q[0][0],-q[0][1],-q[0][2],-q[0][3],q[1][0],-q[1][1],-q[1][2],-q[1][3]);
}

void main(){

	mat2x4 theBlendedQuat = mat2x4(0,0,0,0,0,0,0,0) ;
	
	for (int j = 0; j < boneNumVertAttrib; j++) {	

		int boneIndex = boneIndexesForSkinVertices[j + boneOffsetVertAttrib];
		float  weight = boneWeightForSkinVertices[j + boneOffsetVertAttrib];

		mat2x4 theVersor = mat2x4((fromSkinPoseToCurrentTransf[boneIndex])[0].yzwx, (fromSkinPoseToCurrentTransf[boneIndex])[1].yzwx);
		theBlendedQuat =  theBlendedQuat + weight * theVersor;	
	}
	float toNormalize = 1./length(theBlendedQuat[0]);
	mat2x4 normalizedBlend = toNormalize * theBlendedQuat;
	
	vec4 blendedVertex = dual_quat_transform_point( normalizedBlend, vec4(vPosition, 1.0) );

	//mat2x4 normalizedBlendForNormals = conjugate_quat(normalizedBlend);
	mat2x4 normalizedBlendForNormals =mat2x4(normalizedBlend[0][0],normalizedBlend[0][1],normalizedBlend[0][2],normalizedBlend[0][3], 0, 0, 0, 0);	

 	//vec4 blendedNormals = dual_quat_transform_vector( normalizedBlendForNormals, vNormal );
	// vec4 blendedNormals = dual_quat_transform_vector( normalizedBlend, vNormal );

	vec4 blendedNormals = dual_quat_transform_normal( normalizedBlendForNormals, vNormal );
	//////////////////////////////////////////////
	vec4 tempPos;
	tempPos.xyz = blendedVertex.xyz;
	tempPos.w = 1.0;

	mat4 tempView = View;
	//tempView[2].z = gl_DepthRange.diff/2.0;
	//tempView[3].z = ( gl_DepthRange.near +gl_DepthRange.far) /2.0;
	// vec4 worldPos = World * tempPos;
	// worldPos = worldPos / worldPos.w;
	mat4 transformPosition = tempView * Proj * World;

	// mat3 forNormalsMat = mat3( transformPosition[0].xyz, transformPosition[1].xyz, transformPosition[2].xyz );
	// mat3 transformNormal = transpose(inverse(forNormalsMat));

	mat4 transformNormal4 = transpose(inverse(transformPosition));

	vec4 pos = transformPosition * tempPos;
	vec4 posLatter = pos;
	// vec4 posLatter = pos / pos.w;
	
	vec4 tempNormal = vec4(blendedNormals.xyz, 1.0);

	// vec4 winSpaceNormal = vec4(transformNormal*tempNormal.xyz, 0.);
	
	// vec4 tempWinNormal = transformNormal4*tempNormal;
	// vec4 winSpaceNormal = tempWinNormal/tempWinNormal.w;

	vec4 winSpaceNormal = transformNormal4*tempNormal;
	
	vec4 tNormal = invWorld * tempNormal;
	tNormal = vec4(normalize(tNormal.xyz), 0.0);
	
	vec4 worldLight = World * vec4(vLightPos, 1.0);	
	worldLight = worldLight/worldLight.w;
	
	
	vec4 lightVec = vec4( normalize(vLightPos.xyz - tempPos.xyz), 0.0);
	//vec4 lightVec = vec4( normalize(tempPos.xyz - vLightPos.xyz ), 0.0);
	
	theCoord = vTexCoord;

	 varyNormal = tempNormal;
	//varyNormal = winSpaceNormal;
	//varyNormal = vec4( normalize(winSpaceNormal.xyz),0.);

	//varyNormal = vNormal;
	//varyLightVec = lightVec;

	//varyLightVec = vec4( 0., 400., -400., 1.);
	//varyLightVec = World * vec4(vLightPos, 1.);

	// // vec4 tempLightvec = inverse(transformPosition) * vec4(vLightPos, 1.);
	// vec4 tempLightvec = transformPosition * vec4(vLightPos, 1.);
	// varyLightVec = tempLightvec / tempLightvec.w;

	varyLightVec = vec4(vLightPos, 1.);
	
	//inWorldPos = tempPos;
	
	gl_Position = posLatter;
}