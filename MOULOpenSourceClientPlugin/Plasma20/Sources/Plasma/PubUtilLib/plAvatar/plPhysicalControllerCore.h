/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011 Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/
#ifndef PLPHYSICALCONTROLLERCORE_H
#define PLPHYSICALCONTROLLERCORE_H

#include "hsGeometry3.h"
#include "hsMatrix44.h"
#include "hsQuat.h"
#include "hsTemplates.h"
#include "../pnKeyedObject/plKey.h"
#include "../plPhysical/plSimDefs.h"
#include <vector>

class plCoordinateInterface;
class plPhysical;
class plMovementStrategy;
class plAGApplicator;
class plSwimRegionInterface;

#define kSlopeLimit (cosf(hsScalarDegToRad(55.f)))

enum plControllerCollisionFlags
{
	kSides = 1,
	kTop = (1 << 1),
	kBottom = (1 << 2)
};

struct plControllerSweepRecord
{
	plPhysical *ObjHit;
	hsPoint3 Point;
	hsVector3 Normal;
};

class plPhysicalControllerCore
{
public:
	plPhysicalControllerCore(plKey ownerSceneObject, hsScalar height, hsScalar radius);
	virtual ~plPhysicalControllerCore() { }

	// An ArmatureMod has its own idea about when physics should be enabled/disabled. 
	// Use plArmatureModBase::EnablePhysics() instead.
	virtual void Enable(bool enable) = 0;
	virtual bool IsEnabled() { return fEnabled; }

	// Subworld
	virtual plKey GetSubworld() { return fWorldKey; }
	virtual void SetSubworld(plKey world) = 0;
	virtual const plCoordinateInterface* GetSubworldCI();

	// For the avatar SDL only
	virtual void GetState(hsPoint3& pos, float& zRot) = 0;
	virtual void SetState(const hsPoint3& pos, float zRot) = 0;

	// The LOS DB this avatar is in (only one)
	virtual plSimDefs::plLOSDB GetLOSDB() { return fLOSDB; }
	virtual void SetLOSDB(plSimDefs::plLOSDB losDB) { fLOSDB = losDB; }

	// Movement strategy
	virtual void SetMovementStrategy(plMovementStrategy* strategy) = 0;

	// Global location
	virtual const hsMatrix44& GetLastGlobalLoc() { return fLastGlobalLoc; }
	virtual void SetGlobalLoc(const hsMatrix44& l2w) = 0;

	// Local sim position
	virtual void GetPositionSim(hsPoint3& pos) = 0;

	// Move kinematic controller
	virtual void Move(hsVector3 displacement, unsigned int collideWith, unsigned int &collisionResults) = 0;

	// Set linear velocity on dynamic controller
	virtual void SetLinearVelocitySim(const hsVector3& linearVel) = 0;

	// Sweep the controller path from startPos through endPos
	virtual int SweepControllerPath(const hsPoint3& startPos,const hsPoint3& endPos, hsBool vsDynamics, hsBool vsStatics, 
		UInt32& vsSimGroups, std::vector<plControllerSweepRecord>& hits) = 0;

	// any clean up for the controller should go here
	virtual void LeaveAge() = 0;

	// Local rotation
	const hsQuat& GetLocalRotation() const { return fLocalRotation; }
	void IncrementAngle(hsScalar deltaAngle);
	
	// Linear velocity
	void SetLinearVelocity(const hsVector3& linearVel) { fLinearVelocity = linearVel; }
	const hsVector3& GetLinearVelocity() const { return fLinearVelocity; }

	// Acheived linear velocity
	const hsVector3& GetAchievedLinearVelocity() const { return fAchievedLinearVelocity; }
	void OverrideAchievedLinearVelocity(const hsVector3& linearVel) { fAchievedLinearVelocity = linearVel; }
	void ResetAchievedLinearVelocity() { fAchievedLinearVelocity.Set(0.f, 0.f, 0.f); }

	// SceneObject
	plKey GetOwner() { return fOwner; }

	// When seeking no longer want to interact with exclude regions
	void SetSeek(bool seek) { fSeeking = seek; }
	bool IsSeeking() const { return fSeeking; }

	// Pushing physical
	plPhysical* GetPushingPhysical() const { return fPushingPhysical; }
	void SetPushingPhysical(plPhysical* phys) { fPushingPhysical = phys; }
	bool GetFacingPushingPhysical() const { return fFacingPushingPhysical; }
	void SetFacingPushingPhysical(bool facing) { fFacingPushingPhysical = facing; }
	
	// Controller dimensions
	hsScalar GetRadius() const { return fRadius; }
	hsScalar GetHeight() const { return fHeight; }

	// Create a new controller instance - Implemented in the physics system
	static plPhysicalControllerCore* Create(plKey ownerSO, hsScalar height, hsScalar radius);

protected:
	virtual void IHandleEnableChanged() = 0;

	void IApply(hsScalar delSecs);
	void IUpdate(int numSubSteps, hsScalar alpha);
	void IUpdateNonPhysical(hsScalar alpha);

	void ISendCorrectionMessages(bool dirtySynch = false);

	plKey fOwner;
	plKey fWorldKey;

	hsScalar fHeight;
	hsScalar fRadius;
	
	plSimDefs::plLOSDB fLOSDB;

	plMovementStrategy* fMovementStrategy;

	hsScalar fSimLength;

	hsQuat fLocalRotation;
	hsPoint3 fLocalPosition;
	hsPoint3 fLastLocalPosition;

	hsMatrix44 fLastGlobalLoc;
	hsMatrix44 fPrevSubworldW2L;
	
	hsVector3 fLinearVelocity;
	hsVector3 fAchievedLinearVelocity;

	plPhysical* fPushingPhysical;
	bool fFacingPushingPhysical;

	bool fSeeking;
	bool fEnabled;
	bool fEnableChanged;
};

class plMovementStrategy
{
public:
	plMovementStrategy(plPhysicalControllerCore* controller);
	virtual ~plMovementStrategy() { }

	virtual void Apply(hsScalar delSecs) = 0;
	virtual void Update(hsScalar delSecs) { }

	virtual void AddContactNormals(hsVector3& vec) { }
	virtual void Reset(bool newAge);
	virtual bool IsKinematic() { return true; }

protected:
	plPhysicalControllerCore* fController;
};

class plAnimatedMovementStrategy : public plMovementStrategy
{
public:
	plAnimatedMovementStrategy(plAGApplicator* rootApp, plPhysicalControllerCore* controller);
	virtual ~plAnimatedMovementStrategy() { }

	virtual void RecalcVelocity(double timeNow, hsScalar elapsed, hsBool useAnim = true);
	void SetTurnStrength(hsScalar val) { fTurnStr = val; }
	hsScalar GetTurnStrength() const { return fTurnStr; }

private:
	void IRecalcLinearVelocity(float elapsed, hsMatrix44 &prevMat, hsMatrix44 &curMat);
	void IRecalcAngularVelocity(float elapsed, hsMatrix44 &prevMat, hsMatrix44 &curMat);

	plAGApplicator* fRootApp;
	hsVector3	fAnimLinearVel;
	hsScalar	fAnimAngularVel;
	hsScalar	fTurnStr;
};

class plWalkingStrategy : public plAnimatedMovementStrategy
{
public:
	plWalkingStrategy(plAGApplicator* rootApp, plPhysicalControllerCore* controller);
	virtual ~plWalkingStrategy() { }

	virtual void Apply(hsScalar delSecs);
	virtual void Update(hsScalar delSecs);

	virtual void AddContactNormals(hsVector3& vec);
	virtual void Reset(bool newAge);

	virtual void RecalcVelocity(double timeNow, hsScalar elapsed, hsBool useAnim = true);

	bool HitGroundInThisAge() const { return fHitGroundInThisAge; }
	bool IsOnGround() const { return fTimeInAir < kAirTimeThreshold || fFalseGround; }

	hsScalar GetAirTime() const { return fTimeInAir; }
	void ResetAirTime() { fTimeInAir = 0.0f; }

	hsScalar GetImpactTime() const { return fImpactTime; }
	const hsVector3& GetImpactVelocity() const { return fImpactVelocity; }

	bool EnableControlledFlight(bool status);
 	bool IsControlledFlight() const { return fControlledFlight != 0; }	

	plPhysical* GetPushingPhysical() const;
 	bool GetFacingPushingPhysical() const;

protected:
	static const hsScalar kAirTimeThreshold;
	static const hsScalar kControlledFlightThreshold;

	hsTArray<hsVector3> fSlidingNormals;

	hsVector3 fImpactVelocity;
	hsScalar fImpactTime;

	hsScalar fTimeInAir;
	
	hsScalar fControlledFlightTime;
	int fControlledFlight;

	bool fGroundHit;
	bool fFalseGround;
	bool fHeadHit;
	bool fSliding;

	bool fClearImpact;
	bool fHitGroundInThisAge;
};

class plSwimStrategy : public plAnimatedMovementStrategy
{
public:
	plSwimStrategy(plAGApplicator* rootApp, plPhysicalControllerCore* controller);
	virtual ~plSwimStrategy() { }

	virtual void Apply(hsScalar delSecs);

	virtual void AddContactNormals(hsVector3& vec);

	void SetSurface(plSwimRegionInterface* region, hsScalar surfaceHeight);

	hsScalar GetBuoyancy() const { return fBuoyancy; }
	bool IsOnGround() const { return fOnGround; }
	bool HadContacts() const { return fHadContacts; }

protected:
	void IAdjustBuoyancy();

	hsScalar fBuoyancy;
	hsScalar fSurfaceHeight;

	plSwimRegionInterface *fCurrentRegion;

	bool fOnGround;
	bool fHadContacts;
};

class plDynamicWalkingStrategy : public plWalkingStrategy
{
public:
	plDynamicWalkingStrategy(plAGApplicator* rootApp, plPhysicalControllerCore* controller);
	virtual ~plDynamicWalkingStrategy() { }

	virtual void Apply(hsScalar delSecs);

	virtual bool IsKinematic() { return false; }

protected:
	bool ICheckForGround(hsScalar& zVelocity);
};

#endif// PLPHYSICALCONTROLLERCORE_H

