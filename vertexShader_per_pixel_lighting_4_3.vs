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

out vec2 theCoord;

out vec4 varyNormal;
out vec4 varyLightVec;

in float boneNumVertAttrib;
in float boneOffsetVertAttrib;

layout(std430, binding=1) buffer boneIndexes{
	int boneIndexesForSkinVertices [];
};

layout(std430, binding=2) buffer boneWeight{
	float boneWeightForSkinVertices [];
};

layout(std430, binding=3) buffer fromSkinPose{
	mat2x4 fromSkinPoseToCurrentTransf[];
};

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

void main(){

	mat2x4 theBlendedQuat = mat2x4(0,0,0,0,0,0,0,0) ;
	
	for (int j = 0; j < int(boneNumVertAttrib); j++) {	

		int boneIndex = int(boneIndexesForSkinVertices[j + int(boneOffsetVertAttrib)]);
		float  weight = boneWeightForSkinVertices[j + int(boneOffsetVertAttrib)];

		mat2x4 theVersor = mat2x4((fromSkinPoseToCurrentTransf[boneIndex])[0].yzwx, (fromSkinPoseToCurrentTransf[boneIndex])[1].yzwx);
		theBlendedQuat =  theBlendedQuat + weight * theVersor;	
	}
	float toNormalize = 1./length(theBlendedQuat[0]);
	mat2x4 normalizedBlend = toNormalize * theBlendedQuat;
	
	vec4 blendedVertex = dual_quat_transform_point( normalizedBlend, vec4(vPosition, 1.0) );	
 	vec4 blendedNormals = dual_quat_transform_vector( normalizedBlend, vNormal );
	//////////////////////////////////////////////
	vec4 tempPos;
	tempPos.xyz = blendedVertex.xyz;
	tempPos.w = 1.0;

	mat4 tempView = View;
	tempView[2].z = gl_DepthRange.diff/2.0;
	tempView[3].z = ( gl_DepthRange.near +gl_DepthRange.far) /2.0;
	vec4 worldPos = World * tempPos;
	worldPos = worldPos / worldPos.w;
	vec4 pos = tempView * Proj * World * tempPos;
	vec4 posLatter = pos / pos.w;
	
	vec4 tempNormal = vec4(blendedNormals.xyz, 0.0);
	
	vec4 tNormal = invWorld * tempNormal;
	tNormal = vec4(normalize(tNormal.xyz), 0.0);
	
	vec4 worldLight = World * vec4(vLightPos, 1.0);	
	worldLight = worldLight/worldLight.w;
	
	
	vec4 lightVec = vec4( normalize(vLightPos.xyz - tempPos.xyz), 0.0);
	
	theCoord = vTexCoord;

	varyNormal = tempNormal;
	varyLightVec = lightVec;
	
	gl_Position = posLatter;
}