#include "Sphere.h"

void Sphere::planeCol(GameObject * colObj)
{
	//Saker ej fullständinga: Ändrar ej ingående rotation. E och friktion ska sparas någonstans. Använder ej _radius.
	XMVECTOR ep, en;
	float vp, vn, up, un;
	if (colObj->getObjectType() == STATICOBJECT)
	{
		//ep = static_cast<BoundingPlane*>(colObj->getBoundingVolume())->getNormal(); //Sphere on plane, so line-of-action is the normal of the plane
		//
		//XMVECTOR test = XMVector3Normalize(XMVector3Cross(this->getVelocity(), ep));
		//en = XMVector3Cross(test, ep);

		//vp = XMVectorGetX(XMVector3Dot(this->getVelocity(), ep)); //Component along line-of-action
		//vn = XMVectorGetX(XMVector3Dot(this->getVelocity(), en));

		//up = -0.8f * vp;//Kolla upp krockkoeff i någon lista
		//un = (5.0f * vn) / 7.0f;

		//XMVECTOR newVelocity = up * ep + un * en; //New Velocity
		//this->setVelocity(XMVectorSet(XMVectorGetX(newVelocity), XMVectorGetY(newVelocity), XMVectorGetZ(newVelocity), 1.0f)); //Set w to 1.0f

		//if (XMVectorGetX(XMVector3Dot(newVelocity, ep)) < 10.0f)
		//{
		//	setVelocity(getVelocity() - XMVectorGetX(XMVector3Dot(getVelocity(), ep)) * ep); //Set velocity along plane
		//	float dot = XMVectorGetX(XMVector3Dot(getVelocity(), ep));
		//	if (getMeansofMovement() == GLIDING || getMeansofMovement() == ROLLING) //If two surfaces
		//	{
		//		XMVECTOR newSurfaceNormal = getSurfaceNormal() + ep;
		//		setSurfaceNormal(XMVector3Normalize(newSurfaceNormal));
		//	}
		//	else
		//		setSurfaceNormal(ep);//Surface normal

		//	setMeansOfMovement(GLIDING);
		//}

		//ep = static_cast<BoundingPlane*>(colObj->getBoundingVolume())->getNormal(); //Sphere on plane, so line-of-action is the normal of the plane
		//vp = XMVectorGetX(XMVector3Dot(this->getVelocity(), ep)); //Component along line-of-action
		//up = -0.8f * vp;//Kolla upp krockkoeff i någon lista

		//float collisionTime = 0.01f; //Based on surface?
		//float dt = 0.001f; //Affects number of iterations
		//float impulse = getMass() * (up - vp);
		//float Fp = impulse / collisionTime;
		//float Fy = 0.25f * Fp;

		////
		//XMVECTOR r = -ep;
		//XMVECTOR wPlane, vPlane, uPlane, Ffriction, torque, angularAcc;
		//float I = (2.0f / 5.0f) * getMass() * pow(0.0214f, 2.0f);
		//float I2 = (2.0f / 5.0f) * 0.0214f;

		//for (float f = 0.0f; f < collisionTime; f += dt)
		//{
		//	wPlane = XMVector3Cross(getAngularVelocity(), r); //Ska den vara negativ? (Positiv nu ty vänsterorienterat koords.)
		//	vPlane = getVelocity() - (vp * ep);
		//	uPlane = wPlane + vPlane;

		//	Ffriction = -Fy * uPlane;
		//	torque = XMVector3Cross(Ffriction, r);
		//	angularAcc = torque / I;

		//	setAngularVelocity(getAngularVelocity() + angularAcc * dt); //Updated angular velocity
		//}

		//setVelocity(uPlane + up * ep); // NEW VELOCITY

		//if (XMVectorGetX(XMVector3Dot(getVelocity(), ep)) < 10.0f) // ARE WE GLIDING?
		//{
		//	setVelocity(getVelocity() - XMVectorGetX(XMVector3Dot(getVelocity(), ep)) * ep); //Set velocity along plane
		//	float dot = XMVectorGetX(XMVector3Dot(getVelocity(), ep));
		//	if (getMeansofMovement() == GLIDING || getMeansofMovement() == ROLLING) //If two surfaces
		//	{
		//		XMVECTOR newSurfaceNormal = getSurfaceNormal() + ep;
		//		setSurfaceNormal(XMVector3Normalize(newSurfaceNormal));
		//	}
		//	else
		//		setSurfaceNormal(ep);//Surface normal

		//	setMeansOfMovement(GLIDING);
		//}

		//https://physics.stackexchange.com/questions/11686/finding-angular-velocity-and-regular-velocity-when-bouncing-off-a-surface-with-f

		ep = static_cast<BoundingPlane*>(colObj->getBoundingVolume())->getNormal(); //Sphere on plane, so line-of-action is the normal of the plane
		//		XMVECTOR test = XMVector3Normalize(XMVector3Cross(this->getVelocity(), ep));
		//en = XMVector3Cross(test, ep);
		vp = XMVectorGetX(XMVector3Dot(this->getVelocity(), ep)); //Initial velocity along line-of-action
		//vn = XMVectorGetX(XMVector3Dot(this->getVelocity(), en)); //Initial velocity along plane
		up = -0.3f * vp;//Velocity after collision along line-of-actioin  //Kolla upp krockkoeff i någon lista


		XMVECTOR wr = XMVector3Cross(getAngularVelocity() * 0.0214f, ep);
		float dot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(wr), XMVector3Normalize(getVelocity())));
		if (-0.95f > dot && dot >= -1.0f && XMVectorGetX(XMVector3Length(getVelocity())) <= XMVectorGetX(XMVector3Length(wr))) // v = w*r. Check start of roll-phase.
		{
			en = XMVector3Cross(XMVector3Normalize(XMVector3Cross(this->getVelocity(), ep)), ep);
			vn = XMVectorGetX(XMVector3Dot(this->getVelocity(), en));
			un = (5.0f * vn) / 7.0f;
			setVelocity(up * ep + un * en);
		}
		else //Sliding
		{
			float collisionTime = 0.01f; //Based on surface?
			float dt = 0.001f; //Affects number of iterations
			float impulse = getMass() * abs(up - vp);
			float Fp = impulse / collisionTime;
			float I = (2.0f / 5.0f) * getMass() * pow(0.0214f, 2.0f);

			XMVECTOR r = -ep * 0.0214f; //Center to poiint of collision
			XMVECTOR un = getVelocity() - (vp * ep);
			XMVECTOR uw = getAngularVelocity();
			XMVECTOR ef = XMVector3Normalize(XMVector3Cross(uw, r) - un); //direction of friction
			XMVECTOR directionBefore = XMVectorSet(XMVectorGetX(ef) / abs(XMVectorGetX(ef)), XMVectorGetY(ef) / abs(XMVectorGetY(ef)), XMVectorGetZ(ef) / abs(XMVectorGetZ(ef)), 0.0f);
			XMVECTOR directionAfter;
			bool notRolling = true;

			for (float f = 0.0f; f < collisionTime && notRolling; f += dt)
			{

				if (f == 0.001)
				{
					int a = 0;
				}
				ef = XMVector3Normalize(XMVector3Cross(uw, r) - un); //direction of friction
				XMVECTOR Fn = 0.35f * Fp * ef; //Force of friction along plane, based on velocity and spin
				XMVECTOR ew = XMVector3Normalize(XMVector3Cross(ef, r));

				directionAfter = XMVectorSet(XMVectorGetX(ef) / abs(XMVectorGetX(ef)), XMVectorGetY(ef) / abs(XMVectorGetY(ef)), XMVectorGetZ(ef) / abs(XMVectorGetZ(ef)), 0.0f);

				if (XMVectorGetX(directionBefore) + XMVectorGetX(directionAfter) != 0.0f && XMVectorGetY(directionBefore) + XMVectorGetY(directionAfter) != 0.0f && XMVectorGetZ(directionBefore) + XMVectorGetZ(directionAfter) != 0.0f)
				{
					un = un + ef * ((XMVectorGetX(XMVector3Length(Fn)) / getMass()) * dt); //Updated velocity
					uw = uw + ew * ((XMVectorGetX(XMVector3Length(Fn)) * 0.0214f / I) * dt); //Updated angular velocity
				}
				else
					notRolling = false;
			}
			setVelocity(up*ep + un); //New velocity
			setAngularVelocity(uw); //New angular velocity
		}
		
		if (XMVectorGetX(XMVector3Dot(getVelocity(), ep)) < 1.0f) // Stop bouncing?
		{
			setVelocity(getVelocity() - XMVectorGetX(XMVector3Dot(getVelocity(), ep)) * ep); //Set velocity along plane
			float dot = XMVectorGetX(XMVector3Dot(getVelocity(), ep));
			if (getMeansofMovement() == GLIDING || getMeansofMovement() == ROLLING) //If two surfaces
			{
				XMVECTOR newSurfaceNormal = getSurfaceNormal() + ep;
				setSurfaceNormal(XMVector3Normalize(newSurfaceNormal));
			}
			else
				setSurfaceNormal(ep);//Surface normal

			setMeansOfMovement(GLIDING);
		}
	}
	else
	{
		//Här ska colObj också få en ny vel
	}
	
}

void Sphere::obbCol(GameObject * colObj, XMVECTOR normal)
{
	if (colObj->getObjectType() == STATICOBJECT)
	{
		XMVECTOR ep = normal;
   		float vp = XMVectorGetX(XMVector3Dot(this->getVelocity(), ep)); //Initial velocity along line-of-action
		float up = -0.2f * vp;//Velocity after collision along line-of-actioin  //Kolla upp krockkoeff i någon lista

		float collisionTime = 0.01f; //Based on surface?
		float dt = 0.001f; //Affects number of iterations
		float impulse = getMass() * (up - vp);
		float Fp = impulse / collisionTime;
		float I = (2.0f / 5.0f) * getMass() * pow(0.0214f, 2.0f);

		XMVECTOR r = -ep * 0.0214f; //Center to poiint of collision
		XMVECTOR un = getVelocity() - (vp * ep);
		XMVECTOR uw = getAngularVelocity();

		for (float f = 0.0f; f < collisionTime; f += dt)
		{
			XMVECTOR ef = XMVector3Normalize(XMVector3Cross(uw, r) - un); //direction of friction
			XMVECTOR Fn = 0.2f * Fp * ef; //Force of friction along plane, based on velocity and spin
			XMVECTOR ew = XMVector3Normalize(XMVector3Cross(ef, r));

			un = un + ef * (XMVectorGetX(XMVector3Length(Fn)) / getMass()) * dt; //Updated velocity
			uw = uw + ew * (XMVectorGetX(XMVector3Length(Fn)) * 0.0214f / I) * dt; //Updated angular velocity
		}

		setVelocity(up*ep + un); //New velocity
		setAngularVelocity(uw); //New angular velocity

		if (XMVectorGetX(XMVector3Dot(getVelocity(), ep)) < 1.0f) // ARE WE GLIDING?
		{
			setVelocity(getVelocity() - XMVectorGetX(XMVector3Dot(getVelocity(), ep)) * ep); //Set velocity along plane
			float dot = XMVectorGetX(XMVector3Dot(getVelocity(), ep));
			if (getMeansofMovement() == GLIDING || getMeansofMovement() == ROLLING) //If two surfaces
			{
				XMVECTOR newSurfaceNormal = getSurfaceNormal() + ep;
				setSurfaceNormal(XMVector3Normalize(newSurfaceNormal));
			}
			else
				setSurfaceNormal(ep);//Surface normal

			setMeansOfMovement(GLIDING);
		}
	}
	else
	{
	//Här ska colObj också få en ny vel
	}
}

Sphere::Sphere(ID3D11Device * device, ID3D11DeviceContext * deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:DynamicObject(device, deviceContext, boundingType, startingPosition, modelFile)
{
	_radius = static_cast<BoundingSphere*>(getBoundingVolume())->getRadius();
}

XMVECTOR Sphere::calculateDrag(Environment * environment)
{
	XMVECTOR drag;
	XMVECTOR velocity = getVelocity();
	switch (getMeansofMovement())
	{
	case PROJECTILE:
	{
		XMVECTOR relVel = velocity - environment->windVelocity; //Velocity relative to wind
		float absRelVel = sqrt(pow(XMVectorGetX(relVel), 2) + pow(XMVectorGetY(relVel), 2) + pow(XMVectorGetZ(relVel), 2)); //The length of vector relVel
		
		float ballArea = XM_PI * pow(0.0214f, 2); //Kom ihåg att modell för en boll ska ha radien 0.0214 (21,4mm)

		float absVel = sqrt(pow(XMVectorGetX(velocity), 2) + pow(XMVectorGetY(velocity), 2) + pow(XMVectorGetZ(velocity), 2)); //The length of vector velocity
		float dragCoeff = 0.53f - ((5.1f * absVel) / 1000.0f); //Tillfälligt sätt kanske! Eftersom alla sfärer inte kommer vara golfbollar kan denna info inte finnas här. Ska Cd vara något i dynamicobject?

		drag = -(0.5f * environment->airDensity * ballArea * dragCoeff * absRelVel) * relVel; //The air drag on the object
		break;
	}
	case GLIDING:
	{
		XMVECTOR relVel = velocity - environment->windVelocity; //Velocity relative to wind
		float absRelVel = sqrt(pow(XMVectorGetX(relVel), 2) + pow(XMVectorGetY(relVel), 2) + pow(XMVectorGetZ(relVel), 2)); //The length of vector relVel

		float ballArea = XM_PI * pow(0.0214f, 2); //Kom ihåg att modell för en boll ska ha radien 0.0214 (21,4mm)

		float absVel = sqrt(pow(XMVectorGetX(velocity), 2) + pow(XMVectorGetY(velocity), 2) + pow(XMVectorGetZ(velocity), 2)); //The length of vector velocity
		float dragCoeff = 0.53f - ((5.1f * absVel) / 1000.0f); //Tillfälligt sätt kanske! Eftersom alla sfärer inte kommer vara golfbollar kan denna info inte finnas här. Ska Cd vara något i dynamicobject?

		drag = -(0.5f * environment->airDensity * ballArea * dragCoeff * absRelVel) * relVel; //The air drag on the object
		break;
	}
	case ROLLING:
	{
		XMVECTOR relVel = velocity - environment->windVelocity; //Velocity relative to wind
		float absRelVel = sqrt(pow(XMVectorGetX(relVel), 2) + pow(XMVectorGetY(relVel), 2) + pow(XMVectorGetZ(relVel), 2)); //The length of vector relVel

		float ballArea = XM_PI * pow(0.0214f, 2); //Kom ihåg att modell för en boll ska ha radien 0.0214 (21,4mm)

		float absVel = sqrt(pow(XMVectorGetX(velocity), 2) + pow(XMVectorGetY(velocity), 2) + pow(XMVectorGetZ(velocity), 2)); //The length of vector velocity
		float dragCoeff = 0.53f - ((5.1f * absVel) / 1000.0f); //Tillfälligt sätt kanske! Eftersom alla sfärer inte kommer vara golfbollar kan denna info inte finnas här. Ska Cd vara något i dynamicobject?

		drag = -(0.5f * environment->airDensity * ballArea * dragCoeff * absRelVel) * relVel; //The air drag on the object
		break;
	}
		break;
	default:
		break;
	}

	return drag;
}

XMVECTOR Sphere::calcGliding(float deltaSeconds, Environment *environment)
{
	XMVECTOR acceleration;

	//Forces
	XMVECTOR dragForce = calculateDrag(environment); //Drag
	XMVECTOR gForce = environment->gravAcc * getMass(); //Gravity
	XMVECTOR normalForce = getSurfaceNormal() * abs(XMVectorGetX(XMVector3Dot(getSurfaceNormal(), gForce)));//Normal force

	XMVECTOR resForce = gForce + normalForce; //Resultant

	//Acceleration
	acceleration = resForce / getMass(); // a = F/m.

	XMVECTOR ef = XMVector3Normalize(XMVector3Cross(getAngularVelocity(), -getSurfaceNormal() * 0.0214f) - getVelocity());

	XMVECTOR newPosition = getPositionVector() + (getVelocity() * deltaSeconds);

	float yg = 0.9f, yr = 0.025f; //Ska ej finnas här
	//_angularVelocity = XMVectorSetY(_angularVelocity, (5.0f * yg * 9.82f *deltaSeconds) / (2.0f * 0.0214f) + XMVectorGetY(_angularVelocity)); //Update angularVelocity //Ersätt radius, getY så länge det bara är backspinn. Annars måste vi räkna beloppet.
	//float lenghtFactor = (XMVectorGetX(XMVector3Length(_velocity)) - yg * 9.82f * deltaSeconds) / XMVectorGetX(XMVector3Length(_velocity));
	//_velocity = _velocity * lenghtFactor + acceleration * deltaSeconds; //Update velocity
	setVelocity(getVelocity() + deltaSeconds * yg * 9.82f * ef + acceleration * deltaSeconds);
	setAngularVelocity(getAngularVelocity() + (5.0f * yg * 9.82f * deltaSeconds / (2.0f * 0.0214f)) * XMVector3Cross(getSurfaceNormal(), ef));

	setVelocity(getVelocity() - XMVectorGetX(XMVector3Dot(getVelocity(), getSurfaceNormal())) * getSurfaceNormal()); //Set velocity along plane

	XMVECTOR wr = XMVector3Cross(getAngularVelocity() * 0.0214f, getSurfaceNormal());
	float dot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(wr), XMVector3Normalize(getVelocity())));
	if (-0.95f > dot && dot >= -1.0f && XMVectorGetX(XMVector3Length(wr)) - XMVectorGetX(XMVector3Length(getVelocity())) <= 0.001) // v = w*r. Check start of roll-phase. //Ändra radius!
		setMeansOfMovement(ROLLING);
	
	if (dot == 0.0f)
		setMeansOfMovement(REST);

	return newPosition;
}

XMVECTOR Sphere::calcRolling(float deltaSeconds, Environment *environment)
{
	XMVECTOR acceleration;

	//Forces
	XMVECTOR dragForce = calculateDrag(environment); //Drag
	XMVECTOR gForce = environment->gravAcc * getMass(); //Gravity
	XMVECTOR normalForce = getSurfaceNormal() * abs(XMVectorGetX(XMVector3Dot(getSurfaceNormal(), gForce)));//Normal force

	XMVECTOR resForce = gForce + normalForce; //Resultant

	//Acceleration
	acceleration = resForce / getMass(); // a = F/m.

	XMVECTOR newPosition = getPositionVector() + (getVelocity() * deltaSeconds);

	float yr = 0.16f;
	float lenghtFactor = (XMVectorGetX(XMVector3Length(getVelocity())) - yr * 9.82f * deltaSeconds) / XMVectorGetX(XMVector3Length(getVelocity()));
	setVelocity(getVelocity() * lenghtFactor + acceleration * deltaSeconds);
	setVelocity(getVelocity() - XMVectorGetX(XMVector3Dot(getVelocity(), getSurfaceNormal())) * getSurfaceNormal()); //Set velocity along plane
	//getAngularVelocity() = getVelocity() / _radius;
	if (XMVectorGetX(XMVector3Length(getVelocity())) < 0.001) //Check if at rest
		setMeansOfMovement(REST);

	return newPosition;
}

void Sphere::calculateAfterColVel(GameObject * colObj, XMVECTOR normal)
{
	switch (colObj->getBoundingType())
	{
	case BOUNDING_SPHERE:
		break;
	case BOUNDING_PLANE:
		planeCol(colObj);
		break;
	case ORIENTED_BOUNDING_BOX:
		obbCol(colObj, normal);
		break;
	default:
		break;
	}
}