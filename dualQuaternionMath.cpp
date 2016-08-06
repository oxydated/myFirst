#include "dualQuaternionMath.h"

//dual_quaternion_product = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz)) : ((gs*qs - gx*qx - gy*qy - gz*qz, gs*qx + gx*qs - gy*qz + gz*qy, gs*qy + gx*qz + gy*qs - gz*qx, gs*qz - gx*qy + gy*qx + gz*qs), (dgs*qs - dgx*qx - dgy*qy - dgz*qz + dqs*gs - dqx*gx - dqy*gy - dqz*gz, dgs*qx + dgx*qs - dgy*qz + dgz*qy + dqs*gx + dqx*gs + dqy*gz - dqz*gy, dgs*qy + dgx*qz + dgy*qs - dgz*qx + dqs*gy - dqx*gz + dqy*gs + dqz*gx, dgs*qz - dgx*qy + dgy*qx + dgz*qs + dqs*gz + dqx*gy - dqy*gx + dqz*gs))
void dual_quaternion_product(DUALQUAARG(q),
	DUALQUAARG(g),
	DUALQUAARG(o)){
	os = gs*qs - gx*qx - gy*qy - gz*qz;
	ox = gs*qx + gx*qs - gy*qz + gz*qy;
	oy = gs*qy + gx*qz + gy*qs - gz*qx;
	oz = gs*qz - gx*qy + gy*qx + gz*qs;
	dos = dgs*qs - dgx*qx - dgy*qy - dgz*qz + dqs*gs - dqx*gx - dqy*gy - dqz*gz;
	dox = dgs*qx + dgx*qs - dgy*qz + dgz*qy + dqs*gx + dqx*gs + dqy*gz - dqz*gy;
	doy = dgs*qy + dgx*qz + dgy*qs - dgz*qx + dqs*gy - dqx*gz + dqy*gs + dqz*gx;
	doz = dgs*qz - dgx*qy + dgy*qx + dgz*qs + dqs*gz + dqx*gy - dqy*gx + dqz*gs;
}

//dual_quaternion_sum = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz)) : ((qs + gs, qx + gx, qy + gy, qz + gz), (dqs + dgs, dqx + dgx, dqy + dgy, dqz + dgz))
void dual_quaternion_sum(DUALQUAARG(q),
	DUALQUAARG(g),
	DUALQUAARG(o)) {
	os = qs + gs;
	ox = qx + gx;
	oy = qy + gy;
	oz = qz + gz;
	dos = dqs + dgs;
	dox = dqx + dgx;
	doy = dqy + dgy;
	doz = dqz + dgz;
}

//dual_quaternion_product_by_scalar = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), w : ((qs*w, qx*w, qy*w, qz*w), (dqs*w, dqx*w, dqy*w, dqz*w))
void dual_quaternion_product_by_scalar(DUALQUAARG(q), float w, DUALQUAARG(o)){
	os = qs*w; 
	ox = qx*w;
	oy = qy*w;
	oz = qz*w;
	dos = dqs*w;
	dox = dqx*w;
	doy = dqy*w;
	doz = dqz*w;
}

//dual_quaternion_conjugate = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)) : ((qs, -qx, -qy, -qz), (dqs, -dqx, -dqy, -dqz))
void dual_quaternion_conjugate(DUALQUAARG(q), DUALQUAARG(o)){
	os = qs;
	ox = -qx;
	oy = -qy;
	oz = -qz;
	dos = dqs;
	dox = -dqx;
	doy = -dqy;
	doz = -dqz;
}

//dual_quaternion_norm = lambda quat : np.sqrt(dual_quaternion_product(quat, dual_quaternion_conjugate(quat))[0][0])
float dual_quaternion_norm(DUALQUAARG(q)){
	////	X = dual_quaternion_conjugate(quat)
	DUALQUAVAR(conj);
	dual_quaternion_conjugate(DUALQUACOMP(q), DUALQUACOMP(conj));
	
	////	Y = dual_quaternion_product(quat, X)
	DUALQUAVAR(o);
	dual_quaternion_product(DUALQUACOMP(q), DUALQUACOMP(conj), DUALQUACOMP(o));

	////	np.sqrt(Y[0][0])
	//return sqrt(ox);
	return sqrt(qs*qs+qx*qx+qy*qy+qz*qz);
}

//dual_quaternion_epsilon = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)) : ((qs, qx, qy, qz), (-dqs, -dqx, -dqy, -dqz))
void dual_quaternion_epsilon(DUALQUAARG(q), DUALQUAARG(o)){
	os = qs;
	ox = qx;
	oy = qy;
	oz = qz;
	dos = -dqs;
	dox = -dqx;
	doy = -dqy;
	doz = -dqz;
}

//dual_quaternion_complement = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)) : ((-qs, -qx, -qy, -qz), (-dqs, -dqx, -dqy, -dqz))
void dual_quaternion_complement(DUALQUAARG(q), DUALQUAARG(o)){
	os = -qs;
	ox = -qx;
	oy = -qy;
	oz = -qz;
	dos = -dqs;
	dox = -dqx;
	doy = -dqy;
	doz = -dqz;
}

//dual_Versor = lambda theta, (ux, uy, uz), s, (mx, my, mz) : ((np.cos(theta / 2.0), ux*np.sin(theta / 2.0), uy*np.sin(theta / 2.0), uz*np.sin(theta / 2.0)), (-s*np.sin(theta / 2.0) / 4.0, mx*np.sin(theta / 2.0) + s*ux*np.cos(theta / 2.0) / 4.0, my*np.sin(theta / 2.0) + s*uy*np.cos(theta / 2.0) / 4.0, mz*np.sin(theta / 2.0) + s*uz*np.cos(theta / 2.0) / 4.0))
void dual_Versor(float theta, float ux, float uy, float uz, float s, float mx, float my, float mz, DUALQUAARG(o)){
	os = cos(theta / 2.0);
	ox = ux*sin(theta / 2.0);
	oy = uy*sin(theta / 2.0);
	oz = uz*sin(theta / 2.0);
	dos = -s*sin(theta / 2.0) / 4.0;
	dox = mx*sin(theta / 2.0) + (s*ux*cos(theta / 2.0) / 4.0);
	doy = my*sin(theta / 2.0) + (s*uy*cos(theta / 2.0) / 4.0);
	doz = mz*sin(theta / 2.0) + (s*uz*cos(theta / 2.0) / 4.0);
}

//pure_Real_quaternion = lambda(qs, qx, qy, qz) : ((qs, qx, qy, qz), (0, 0, 0, 0))
void pure_Real_quaternion(float qs, float qx, float qy, float qz, DUALQUAARG(o)){
	os = qs;
	ox = qx;
	oy = qy;
	oz = qz;
	dos = 0;
	dox = 0;
	doy = 0;
	doz = 0;
}

//point_quaternion = lambda(px, py, pz) : ((1, 0, 0, 0), (0, px, py, pz))
void point_quaternion(float px, float py, float pz, DUALQUAARG(o)){
	os = 1; 
	ox = 0;
	oy = 0;
	oz = 0;
	dos = 0;
	dox = px;
	doy = py;
	doz = pz;
}

//translation_quaternion = lambda(dx, dy, dz) : ((1, 0, 0, 0), (0, -dx / 2, -dy / 2, -dz / 2))
void translation_quaternion(float dx, float dy, float dz, DUALQUAARG(o)){
	os = 1;
	ox = 0;
	oy = 0;
	oz = 0;
	dos = 0;
	dox = -dx / 2.0;
	doy = -dy / 2.0;
	doz = -dz / 2.0;
}

//line_quaternion = lambda(vx, vy, vz), (rx, ry, rz) : ((0, vx, vy, vz), (0, rx, ry, rz))
void line_quaternion(float vx, float vy, float vz, float rx, float ry, float rz, DUALQUAARG(o)){
	os = 0;
	ox = vx;
	oy = vy;
	oz = vz;
	dos = 0;
	dox = rx;
	doy = ry;
	doz = rz;
}

//dual_quaternion_dot_product = lambda((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz)) : (gs*qs + gx*qx + gy*qy + gz*qz, dgs*qs + dgx*qx + dgy*qy + dgz*qz + dqs*gs + dqx*gx + dqy*gy + dqz*gz)
void dual_quaternion_dot_product(DUALQUAARG(q),
	DUALQUAARG(g),
	float &r, float &d){
	r = gs*qs + gx*qx + gy*qy + gz*qz;
	d = dgs*qs + dgx*qx + dgy*qy + dgz*qz + dqs*gs + dqx*gx + dqy*gy + dqz*gz;
}

//dual_quaternion_unity = lambda : ((1.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0))
void dual_quaternion_unity(DUALQUAARG(o)){
	os = 1.0;
	ox = 0.0;
	oy = 0.0;
	oz = 0.0;
	dos = 0.0;
	dox = 0.0;
	doy = 0.0;
	doz = 0.0;
}