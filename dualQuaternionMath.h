#ifndef __DUALQUATERIONMATH_H__
#define __DUALQUATERIONMATH_H__

#include <math.h>

#define DUALQUAARRAY( q ) q##[0], q##[1], q##[2], q##[3], q##[4], q##[5], q##[6], q##[7] 

#define DUALQUAARG( q ) float& q##s, float& q##x, float& q##y, float& q##z, float& d##q##s, float& d##q##x, float& d##q##y, float& d##q##z 

#define DUALQUACOMP( q ) q##s, q##x, q##y, q##z, d##q##s, d##q##x, d##q##y, d##q##z 

#define DUALQUAVAR( q ) float q##s, q##x, q##y, q##z, d##q##s, d##q##x, d##q##y, d##q##z 

////////////////////////////////////////////////////////////////////////////////////////////

//dual_quaternion_product = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz)) : ((gs*qs - gx*qx - gy*qy - gz*qz, gs*qx + gx*qs - gy*qz + gz*qy, gs*qy + gx*qz + gy*qs - gz*qx, gs*qz - gx*qy + gy*qx + gz*qs), (dgs*qs - dgx*qx - dgy*qy - dgz*qz + dqs*gs - dqx*gx - dqy*gy - dqz*gz, dgs*qx + dgx*qs - dgy*qz + dgz*qy + dqs*gx + dqx*gs + dqy*gz - dqz*gy, dgs*qy + dgx*qz + dgy*qs - dgz*qx + dqs*gy - dqx*gz + dqy*gs + dqz*gx, dgs*qz - dgx*qy + dgy*qx + dgz*qs + dqs*gz + dqx*gy - dqy*gx + dqz*gs))
void dual_quaternion_product(DUALQUAARG(q),
	DUALQUAARG(g),
	DUALQUAARG(o));

//dual_quaternion_sum = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz)) : ((qs + gs, qx + gx, qy + gy, qz + gz), (dqs + dgs, dqx + dgx, dqy + dgy, dqz + dgz))
void dual_quaternion_sum(DUALQUAARG(q),
	DUALQUAARG(g),
	DUALQUAARG(o));

//dual_quaternion_product_by_scalar = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), w : ((qs*w, qx*w, qy*w, qz*w), (dqs*w, dqx*w, dqy*w, dqz*w))
void dual_quaternion_product_by_scalar(DUALQUAARG(q), float w, DUALQUAARG(o));

//dual_quaternion_conjugate = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)) : ((qs, -qx, -qy, -qz), (dqs, -dqx, -dqy, -dqz))
void dual_quaternion_conjugate(DUALQUAARG(q), DUALQUAARG(o));

//dual_quaternion_norm = lambda quat : np.sqrt(dual_quaternion_product(quat, dual_quaternion_conjugate(quat))[0][0])
float dual_quaternion_norm(DUALQUAARG(q));

//dual_quaternion_epsilon = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)) : ((qs, qx, qy, qz), (-dqs, -dqx, -dqy, -dqz))
void dual_quaternion_epsilon(DUALQUAARG(q), DUALQUAARG(o));

//dual_quaternion_complement = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)) : ((-qs, -qx, -qy, -qz), (-dqs, -dqx, -dqy, -dqz))
void dual_quaternion_complement(DUALQUAARG(q), DUALQUAARG(o));

//dual_Versor = lambda theta, (ux, uy, uz), s, (mx, my, mz) : ((np.cos(theta / 2.0), ux*np.sin(theta / 2.0), uy*np.sin(theta / 2.0), uz*np.sin(theta / 2.0)), (-s*np.sin(theta / 2.0) / 4.0, mx*np.sin(theta / 2.0) + s*ux*np.cos(theta / 2.0) / 4.0, my*np.sin(theta / 2.0) + s*uy*np.cos(theta / 2.0) / 4.0, mz*np.sin(theta / 2.0) + s*uz*np.cos(theta / 2.0) / 4.0))
void dual_Versor(float theta, float ux, float uy, float uz, float s, float mx, float my, float mz, DUALQUAARG(o));

//pure_Real_quaternion = lambda(qs, qx, qy, qz) : ((qs, qx, qy, qz), (0, 0, 0, 0))
void pure_Real_quaternion(float qs, float qx, float qy, float qz, DUALQUAARG(o));

//point_quaternion = lambda(px, py, pz) : ((1, 0, 0, 0), (0, px, py, pz))
void point_quaternion(float px, float py, float pz, DUALQUAARG(o));

//vector_quaternion = lambda(vx, vy, vz) : ((0, 0, 0, 0), (0, vx, vy, vz))
void vector_quaternion(float vx, float vy, float vz, DUALQUAARG(o));

//translation_quaternion = lambda(dx, dy, dz) : ((1, 0, 0, 0), (0, -dx / 2, -dy / 2, -dz / 2))
void translation_quaternion(float dx, float dy, float dz, DUALQUAARG(o));

//line_quaternion = lambda(vx, vy, vz), (rx, ry, rz) : ((0, vx, vy, vz), (0, rx, ry, rz))
void line_quaternion(float vx, float vy, float vz, float rx, float ry, float rz, DUALQUAARG(o));

//dual_quaternion_dot_product = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz)) : (gs*qs + gx*qx + gy*qy + gz*qz, dgs*qs + dgx*qx + dgy*qy + dgz*qz + dqs*gs + dqx*gx + dqy*gy + dqz*gz)
void dual_quaternion_dot_product(DUALQUAARG(q),
	DUALQUAARG(g),
	float &r, float &d);

//dual_quaternion_unity = lambda : ((1.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0))
void dual_quaternion_unity(DUALQUAARG(o));

#endif