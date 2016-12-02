#include "dualQuaternionFunctions.h"

//def dual_quat_transform_line(((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz))) :
void dual_quat_transform_line(DUALQUAARG(q),
	DUALQUAARG(g),
	DUALQUAARG(o)){
	
	//theDualVersor = ((qs, qx, qy, qz), (dqs, dqx, dqy, dqz))
	//theDualVersorConj = dual_quaternion_conjugate(theDualVersor)
	DUALQUAVAR(conj);
	dual_quaternion_conjugate(DUALQUACOMP(q), DUALQUACOMP(conj));
	//lineQuat = ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz))
	//X = dual_quaternion_product(lineQuat, theDualVersorConj)
	DUALQUAVAR(temp);
	dual_quaternion_product(DUALQUACOMP(g), DUALQUACOMP(conj), DUALQUACOMP(temp));
	//return dual_quaternion_product(theDualVersor, X)
	dual_quaternion_product(DUALQUACOMP(q), DUALQUACOMP(temp), DUALQUACOMP(o));
}

//def dual_quat_transform_point(((qs, qx, qy, qz), (dqs, dqx, dqy, dqz)), ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz))) :
void dual_quat_transform_point(DUALQUAARG(q),
	DUALQUAARG(g),
	DUALQUAARG(o)){

	//theDualVersor = ((qs, qx, qy, qz), (dqs, dqx, dqy, dqz))
	//theDualVersorConj = dual_quaternion_conjugate(theDualVersor)
	DUALQUAVAR(conj);
	dual_quaternion_conjugate(DUALQUACOMP(q), DUALQUACOMP(conj));
	//theDualVersorEpsilon = dual_quaternion_epsilon(theDualVersor)
	DUALQUAVAR(epsi);
	dual_quaternion_epsilon(DUALQUACOMP(q), DUALQUACOMP(epsi));
	//pointQuat = ((gs, gx, gy, gz), (dgs, dgx, dgy, dgz))
	//X = dual_quaternion_product(pointQuat, theDualVersorConj)
	DUALQUAVAR(temp);
	dual_quaternion_product(DUALQUACOMP(g), DUALQUACOMP(conj), DUALQUACOMP(temp));
	//return dual_quaternion_product(theDualVersorEpsilon, X)
	dual_quaternion_product(DUALQUACOMP(epsi), DUALQUACOMP(temp), DUALQUACOMP(o));
}

//def dual_quat_screw_line(theta, (ux, uy, uz), s, (mx, my, mz), (vx, vy, vz), (rx, ry, rz)) :

//theDualVersor = dual_Versor(theta, (ux, uy, uz), s, (mx, my, mz))
//theDualVersorConj = dual_quaternion_conjugate(theDualVersor)
//lineQuat = line_quaternion((vx, vy, vz), (rx, ry, rz))
//return dual_quaternion_product(theDualVersor, dual_quaternion_product(lineQuat, theDualVersorConj))

//def dual_quat_screw_point(theta, (ux, uy, uz), s, (mx, my, mz), (px, py, pz)) :

//theDualVersor = dual_Versor(theta, (ux, uy, uz), s, (mx, my, mz))
//theDualVersorConj = dual_quaternion_conjugate(theDualVersor)
//theDualVersorEpsilon = dual_quaternion_epsilon(theDualVersor)
//pointQuat = point_quaternion((px, py, pz))
//return dual_quaternion_product(theDualVersorEpsilon, dual_quaternion_product(pointQuat, theDualVersorConj))

//def make_dual_vector(vecU, vecX) :
void make_dual_vector(float Ux, float Uy, float Uz, float Vx, float Vy, float Vz, float& Nx, float& Ny, float& Nz, float& Mx, float& My, float& Mz){
	//nU = vecU / np.linalg.norm(vecU)
	float norm = sqrt((Ux*Ux) + (Uy*Uy) + (Uz*Uz));
	Nx = Ux / norm; Nx = Ux / norm; Nx = Ux / norm;
	//vecM = np.cross(nU, vecX)
	Mx = Ny*Vz - Nz*Vy;
	My = -Nx*Vz + Nz*Vx;
	Mz = Nx*Vy - Ny*Vx;
	//return nU, vecM
}

//def extractDualVersorParameters(someDualVersor) :
void extractDualVersorParameters(DUALQUAARG(q),
	//return (qS, theSin), theUvector, theSfactor, theMvector
	//float& qS, float& theSin, float& Ux, float& Uy, float& Uz, float& theSfactor, float& Mx, float& My, float& Mz){
	dualQuatParameters& theParameters){
	//((qS, qx, qy, qz), (dqS, dqx, dqy, dqz)) = someDualVersor
	theParameters.qS = qs;

	//qV = np.array((qx, qy, qz))
	//dqV = np.array((dqx, dqy, dqz))
	//theSin = np.linalg.norm(qV)
	theParameters.theSin = sqrt((qx*qx) + (qy*qy) + (qz*qz));

	//if (np.abs(qS)> 1.0) :
		//print "qS:", qS
		//if qS < 0.0 :
			//	qS = -1.0
		//else :
			//qS = 1.0
	theParameters.qS = (theParameters.qS < -1.0) ? -1.0 :
		((theParameters.qS > 1.0) ? 1.0 : theParameters.qS);
	
	//if (np.abs(theSin)> 1.0) :
	//	print "theSin:", theSin
	//	if theSin < 0.0 :
	//		theSin = -1.0
	//	else :
	//	theSin = 1.0
	theParameters.theSin = (theParameters.theSin < -1.0) ? -1.0 :
		((theParameters.theSin > 1.0) ? 1.0 : theParameters.theSin);

	//if np.abs(theSin) < 1.0e-7 :
	//	theUvector = np.array((1.0, 0.0, 0.0))
	//	theMvector = np.array((0.0, 0.0, 0.0))
	//	theSfactor = 0.0
	if (fabs(theParameters.theSin) < 1.0e-7){
		theParameters.Ux = 1.0; theParameters.Uy = 0.0; theParameters.Uz = 0.0;
		theParameters.Mx = .0; theParameters.My = 0.0; theParameters.Mz = 0.0;
		theParameters.theSfactor = 0.0;
	}
	//else:
	else{
		//theUvector = qV / theSin
		theParameters.Ux = qx / theParameters.theSin; theParameters.Uy = qy / theParameters.theSin; theParameters.Uz = qz / theParameters.theSin;

		//theMvector = (dqV + (qV*(1.0 / theSin))*qS*(dqS / theSin)) * (1.0 / theSin)
		theParameters.Mx = (dqx + (qx*(1.0 / theParameters.theSin))*theParameters.qS*(dqs / theParameters.theSin)) * (1.0 / theParameters.theSin);
		theParameters.My = (dqy + (qy*(1.0 / theParameters.theSin))*theParameters.qS*(dqs / theParameters.theSin)) * (1.0 / theParameters.theSin);
		theParameters.Mz = (dqz + (qz*(1.0 / theParameters.theSin))*theParameters.qS*(dqs / theParameters.theSin)) * (1.0 / theParameters.theSin);

		//theSfactor = dqS / (theSin / 2.0)
		theParameters.theSfactor = dqs / (theParameters.theSin / 2.0);

		//return (qS, theSin), theUvector, theSfactor, theMvector
	}
}

//def transformFromSourceToDestinationAxis(sourceTransform, destTransform) :
void transformFromSourceToDestinationAxis(DUALQUAARG(source), DUALQUAARG(dest), DUALQUAARG(o)){

	//															dual_quaternion_conjugate(sourceTransform)

	DUALQUAVAR(conjSource);
	dual_quaternion_conjugate(DUALQUACOMP(source), DUALQUACOMP(conjSource));

	//	# transforming from Source to Destination
	//
	//	fromDestToSourceTransform = dual_quaternion_product(
	//															destTransform,
	//															dual_quaternion_conjugate(sourceTransform)
	//														)

	dual_quaternion_product(DUALQUACOMP(dest), DUALQUACOMP(conjSource), DUALQUACOMP(o));
	//	return fromDestToSourceTransform
}

void extractRotationQuatFromDualQuat(DUALQUAARG(q), DUALQUAARG(o)){
	os = qs;
	ox = qx;
	oy = qy;
	oz = qz;
	dos = 0;
	dox = 0;
	doy = 0;
	doz = 0;
}