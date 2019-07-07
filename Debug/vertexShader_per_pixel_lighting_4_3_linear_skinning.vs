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

	vec4 unormalizedBlendedVertex = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 unormalizedBlendedNormal = vec4(0.0, 0.0, 0.0, 0.0);
	float sumWeight = 0.0;
	
	for (int j = 0; j < boneNumVertAttrib; j++) {	

		int boneIndex = boneIndexesForSkinVertices[j + boneOffsetVertAttrib];
		float  weight = boneWeightForSkinVertices[j + boneOffsetVertAttrib];		

		mat2x4 theVersor = mat2x4((fromSkinPoseToCurrentTransf[boneIndex])[0].yzwx, (fromSkinPoseToCurrentTransf[boneIndex])[1].yzwx);
		
		mat2x4 theVersorForNormals =mat2x4(theVersor[0][0],theVersor[0][1],theVersor[0][2],theVersor[0][3], 0, 0, 0, 0);	
		
		unormalizedBlendedVertex += vec4(weight * (dual_quat_transform_point( theVersor, vec4(vPosition, 1.0) )).xyz, 1.0);
		unormalizedBlendedNormal += vec4(weight * (dual_quat_transform_normal( theVersorForNormals, vNormal )).xyz, 1.0);

		sumWeight += weight;

	}
	
	vec4 blendedVertex = vec4(unormalizedBlendedVertex.xyz / sumWeight, 1.0);
	vec4 blendedNormals = vec4( normalize( (unormalizedBlendedNormal.xyz / sumWeight) ), 1.0);

	//////////////////////////////////////////////

    vec4 tempPos = vec4(blendedVertex.xyz, 1.0);

    vec4 pos = View * Proj * World * tempPos;
	
	theCoord = vTexCoord;

    varyNormal = vec4(blendedNormals.xyz, 1.0);

	varyLightVec = vec4(vLightPos, 1.);
	
    gl_Position = pos;
}