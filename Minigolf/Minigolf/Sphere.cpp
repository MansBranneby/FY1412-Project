#include "Sphere.h"

void Sphere::planeCol(GameObject * colObj)
{
	//Saker ej fullst�ndinga: �ndrar ej ing�ende rotation. E och friktion ska sparas n�gonstans. Anv�nder ej _radius.
	XMVECTOR ep, en, w;
	float vp, vn, up, un;
	if (colObj->getObjectType() == STATICOBJECT)
	{
		ep = static_cast<BoundingPlane*>(colObj->getBoundingVolume())->getNormal(); //Sphere on plane, so line-of-action is the normal of the plane
		
		XMVECTOR test = XMVector3Normalize(XMVector3Cross(this->getVelocity(), ep));
		en = XMVector3Cross(test, ep);

		vp = XMVectorGetX(XMVector3Dot(this->getVelocity(), ep)); //Component along line-of-action
		vn = XMVectorGetX(XMVector3Dot(this->getVelocity(), en));

		up = -0.8 * vp;//Kolla upp krockkoeff i n�gon lista
		//float MOI = (2 * this->getMass() * pow(0.0214, 2)) / 5;
		//un = (vn * this->getMass() * pow(0.0214, 2)) / (this->getMass() * pow(0.0214, 2) * MOI);
		un = (5.0f * vn) / 7.0f;

		XMVECTOR newVelocity = up * ep + un * en; //New Velocity
		this->setVelocity(XMVectorSet(XMVectorGetX(newVelocity), XMVectorGetY(newVelocity), XMVectorGetZ(newVelocity), 1.0f)); //Set w to 1.0f
		//this->setVelocity(this->getVelocity() + (up - vp) * (ep));// + 0.2*en)); //New velocity
	}
	else
	{
		//H�r ska colObj ocks� f� en ny vel
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
		
		float ballArea = XM_PI * pow(0.0214, 2); //Kom ih�g att modell f�r en boll ska ha radien 0.0214 (21,4mm)

		float absVel = sqrt(pow(XMVectorGetX(velocity), 2) + pow(XMVectorGetY(velocity), 2) + pow(XMVectorGetZ(velocity), 2)); //The length of vector velocity
		float dragCoeff = 0.53f - ((5.1f * absVel) / 1000); //Tillf�lligt s�tt kanske! Eftersom alla sf�rer inte kommer vara golfbollar kan denna info inte finnas h�r. Ska Cd vara n�got i dynamicobject?

		drag = -(0.5f * environment->airDensity * ballArea * dragCoeff * absRelVel) * relVel; //The air drag on the object
		break;
	}
	case GLIDING:
		break;
	case ROLLING:
		break;
	default:
		break;
	}

	return drag;
}

void Sphere::calculateAfterColVel(GameObject * colObj)
{
	switch (colObj->getBoundingType())
	{
	case BOUNDING_SPHERE:
		break;
	case BOUNDING_PLANE:
		planeCol(colObj);
		break;
	case ORIENTED_BOUNDING_BOX:
		break;
	default:
		break;
	}
}