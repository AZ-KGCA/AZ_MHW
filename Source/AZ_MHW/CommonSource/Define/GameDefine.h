//============================================
// 매크로.
//============================================
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// Collision trace channels defines
#define ECC_PLAYER ECollisionChannel::ECC_GameTraceChannel1
#define ECC_MONSTER ECollisionChannel::ECC_GameTraceChannel2
#define ECC_PLAYERATTACK ECollisionChannel::ECC_GameTraceChannel1

// Physical material surface type defines
#define EPS_HEAD EPhysicalSurface::SurfaceType1
#define EPS_RIGHTWING EPhysicalSurface::SurfaceType2
#define EPS_LEFTWING EPhysicalSurface::SurfaceType3
#define EPS_TAIL EPhysicalSurface::SurfaceType4
#define EPS_NECK EPhysicalSurface::SurfaceType5
#define EPS_BODY EPhysicalSurface::SurfaceType6
#define EPS_BACK EPhysicalSurface::SurfaceType7
#define EPS_LEG EPhysicalSurface::SurfaceType8