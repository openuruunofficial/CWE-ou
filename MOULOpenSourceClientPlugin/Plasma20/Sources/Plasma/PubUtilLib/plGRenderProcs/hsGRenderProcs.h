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

#ifndef hsGRenderProcs_inc
#define hsGRenderProcs_inc

#include "hsRefCnt.h"
#include "hsScalar.h"
#include "hsMemory.h"
#include "hsBiExpander.h"
#include "../plPipeline/hsGColorizer.h"
#include "../plResMgr/plCreatable.h"

class plPipeline;
class plDrawable;
class hsTriangle3;
struct hsGTriVertex;
struct hsGVertex3;
struct hsGShadeVertex;
struct hsGSplat3;
class hsStream;
class plKey;
class hsBounds3Ext;
class hsResMgr;

class hsGRenderProcs : public plCreatable { 
public:
	enum {
		kMaxLabelLen			= 128
	};
	enum ProcType {
		kTypeAngleFade,
		kTypeDistFade,
		kTypeMotionBlur1,
		kTypeMotionBlur2,
		kTypeIntenseAlpha,
		kTypeGlobalShade,
		kTypeObjDistFade,
		kTypeDistShade,
		kTypeObjDistShade
	};
	enum {
		kNone					= 0x0,
		kInclusive				= 0x1,	// Affect children
		kNOP					= 0x2,	// Turned off (till EndObject)
		kOpaque					= 0x4,
		kCulled					= 0x8,
		kObjectRefs				= 0x10,
		kObjectRefsInit			= 0x20
	};
private:
	// Base class private stuff for managing the Queue of Procs on the device.

	UInt32						fLinkCount;
	hsGRenderProcs*				fNext;
	hsGRenderProcs**			fBack;
	
	hsGRenderProcs**			IOneBeforeMe(hsGRenderProcs** base);
	void						IInsert(hsGRenderProcs** beforeMe);
	void						IDetach();

protected:

	UInt32						fFlags;

	hsGColorizer				fColorizer;

	plPipeline*				fPipeline;

	hsDynamicArray<plKey*>	fObjectRefs;
public: 
	hsGRenderProcs();
	virtual ~hsGRenderProcs();

	// BeginTree returns false if entire subtree is don't bother to draw, 
	// else true. Mostly a culling tool. 
	virtual hsBool32 BeginTree(plPipeline* pipe, plDrawable* root);
	
	// BeginObject returns true if the object should be drawn, false if 
	// don't bother. Can also do any initialization it wants. Should this 
	// get something more innocuous like a bound instead of the SceneObject? 
	// Is there anything else it might need to know? 
	virtual hsBool32 BeginObject(plPipeline* pipe, plDrawable* obj);
	
	// ProcessVerts takes the list of TriVerts and does what it will. 
	// I'll outline the hsGTriVertex below. The difference between 
	// the BaseVertex list and the UsedVertex list is interpolation. 
	// Shade values and generated Texture Coords are computed for 
	// the Base Triangle only, and then interpolated for vertices 
	// generated by clipping. So Shade Values and Texture Coordinates 
	// should only be computed in ProcessBaseVerts(). On the other 
	// hand, only the vertices from actual drawn triangles are 
	// transformed, and hence have a screen position to mess with. 
	// So any wiggling of the screen position should happen in 
	// ProcessUsedVerts(). These functions might be better named 
	// ProcessShadeVerts() and ProcessXformVerts(), except that 
	// vertex illumination (shade) is interpolated, but then 
	// the interpolated shade is fed into the material to calculate 
	// color. So messing with final color would happen in ProcessUsedVerts(), 
	// whereas messing with illumination's in ProcessBaseVerts(). Messing 
	// with UV's is equally valid in either. In general though, the number 
	// of BaseVerts is less than or equal to the number of UsedVerts. Most 
	// shaders would have one or the other a no-op.

	// Process list of unique vertices (with unique hsGXformVerts) which will be drawn to screen
	virtual void ProcessScreenVerts(hsExpander<hsGVertex3*>& vList) {}

	// Take a list of verts and modulate shades for them. Care should be taken to only bother with verts that
	// are not (hsGVertex3::kCulled|hsGVertex3::kDisabled). Also, any verts that this RenderProc causes
	// to go completely transparent should be flagged hsGVertex3::kCulled (NOT DISABLED).
	// See hsSfxDistFade for example (not exemplary) code.

	// Process list of unique vertices (unique hsGShadeVerts) before interpolation
	virtual void ProcessPreInterpShadeVerts(hsExpander<hsGShadeVertex*>& vList) {}
	// Process list of unique vertices (unique hsGShadeVerts) after interpolation - these will be drawn to screen
	virtual void ProcessPostInterpShadeVerts(hsExpander<hsGShadeVertex*>& vList) {}

	// Process list of unique Pre or Post Interpolation TriVerts (with hsGShade and hsGXformVerts)
	// While the TriVerts are unique, there may be sharing among constituents, i.e. position and uv.
	// Care must be taken when accumulating effects.
	virtual void ProcessPreInterpVerts(hsExpander<hsGTriVertex*>& vList) {}
	virtual void ProcessPostInterpVerts(hsExpander<hsGTriVertex*>& vList) {}

	// Process list of triangles which are headed for the screen. vList is the full list of unique TriVerts
	// used by these triangles. If triangles are added, any generated verts MUST be added to vList. If
	// Triangles are removed, verts may be removed from vList (keeping in mind that vList verts may be
	// shared between triangles).
	virtual void ProcessPreClipTris(hsExpander<hsTriangle3*>& tList, hsExpander<hsGTriVertex*>& vList) {}
	virtual void ProcessPreInterpTris(hsExpander<hsTriangle3*>& tList, hsExpander<hsGTriVertex*>& vList) {}
	virtual void ProcessPostInterpTris(hsExpander<hsTriangle3*>& tList, hsExpander<hsGTriVertex*>& vList) {}
	
	// Any cleanup for this object 
	virtual void EndObject();
	
	// Any cleanup for this subtree 
	virtual void EndTree();
	
	// Shaders can set their priority to affect order in which they are called 
	// When pushed onto device, device uses this priority to sort into queue 
	virtual hsScalar GetPriority() { return 0; }
	
	// When a shader is pushed onto the device (by an object), the object 
	// will pop it back off either before or after drawing its children, 
	// depending on Inclusive(). Not meaningful for mate
	virtual hsBool32 Inclusive() { return fFlags & kInclusive; }

	virtual void Enqueue(hsGRenderProcs** list);
	virtual void Dequeue();
	hsGRenderProcs* GetNext() { return fNext; }

	// External object references. Individual RenderProc type responsible for what they're used for.
	void			SetNumObjectRefs(int n);
	UInt32			GetNumObjectRefs() { return fObjectRefs.GetCount(); }
	void			AddObjectRef(plKey* key) { fObjectRefs.Append(key); fFlags |= kObjectRefs; }
	void			SetObjectRef(plKey* key, int i=0);
	void			InsertObjectRef(int i, plKey* key) { fObjectRefs.InsertAtIndex(i, key); fFlags |= kObjectRefs; }
	plDrawable*	GetObjectRef(int i);
	plKey*	GetObjectRefKey(int i) { return fObjectRefs[i]; }
	void			ReadObjectRefs(hsStream* s, hsResMgr* mgr);
	void			WriteObjectRefs(hsStream* s, hsResMgr* mgr);


	virtual void Read(hsStream* s, hsResMgr* mgr);
	virtual void Write(hsStream* s, hsResMgr* mgr);

	virtual void Read(hsStream* s) = 0;
	virtual void Write(hsStream* s) = 0;

	virtual const char* GetLabel() const = 0;
	virtual ProcType GetType() const = 0;

	UInt32	GetFlags() { return fFlags; }
	void SetFlags(UInt32 f) { fFlags = f; }

	CLASSNAME_REGISTER( hsGRenderProcs );
	GETINTERFACE_ANY( hsGRenderProcs, plCreatable );
}; 

#endif // hsGRenderProcs_inc
