struct M2Header
{
	char id[4];
	unsigned char version[4];
	uint32 nameLength;
	uint32 nameOfs;
	uint32 type;
	uint32 nGlobalSequences;
	uint32 ofsGlobalSequences;
	uint32 nAnimations;
	uint32 ofsAnimations;
	uint32 nAnimationLookup;
	uint32 osdAnimationLookup;
	uint32 nBones;
	uint32 ofsBones;
	uint32 nKeyBoneLookup;
	uint32 ofsKeyBoneLookup;

	uint32 nVertices;
	uint32 ofsVertices;
	uint32 nViews;

	uint32 nColors;
	uint32 ofsColors;

	uint32 nTextures;
	uint32 ofsTextures;

	uint32 nTransparency;
	uint32 ofsTransparency;
	uint32 nUVAnimation;
	uint32 ofsUVAnimation;
	uint32 nTexReplace;
	uint32 ofsTexReplace;

	uint32 nRenderFlags;
	uint32 ofsRenderFlags;
	uint32 nBoneLookupTable;
	uint32 ofsBoneLookupTable;

	uint32 nTexLookup;
	uint32 ofsTexLookup;

	uint32 nTexUnitLookup;
	uint32 ofsTexUnitLookup;
	uint32 nTransparencyLookup;
	uint32 ofsTransparencyLookup;
	uint32 nUVAnimLookup;
	uint32 ofsUVAnimLookup;

	float vertexbox1[3];
	float vertexbox2[3];
	float vertexradius;
	float boundingbox1[3];
	float boundingbox2[3];
	float boundingradius;

	uint32 nBoundingTriangles;
	uint32 ofsBoundingTriangles;
	uint32 nBoundingVertices;
	uint32 ofsBoundingVertices;
	uint32 nBoundingNormals;
	uint32 ofsBoundingNormals;

	uint32 nAttachments;
	uint32 ofsAttachments;
	uint32 nAttachmentLookup;
	uint32 ofsAttachmentLookup;
	uint32 nEvents;
	uint32 ofsEvents;
	uint32 nLights;
	uint32 ofsLights;
	uint32 nCameras;
	uint32 ofsCameras;
	uint32 nCameraLookup;
	uint32 ofsCameraLookup;
	uint32 nRibbonEmitters;
	uint32 ofsRibbonEmitters;
	uint32 nParticleEmitters;
	uint32 ofsParticleEmitters;
};

struct AnimationBlock
{
	uint16 interpolation;
	uint16 globalsequenceid;
	uint32 list1offset;
	uint32 timestampdataoffset;
	uint32 list2offset;
	uint32 keysoffset;
};

struct M2Attachment
{
	uint32 id;
	uint32 bone;
	float pos[3];
	AnimationBlock unk;
};

struct M2Vertex
{
	float position[3];
	unsigned char boneweight[4];
	unsigned char boneindex[4];
	float normal[3];
	float textcoord[2];
	float unk[2];
};

struct M2Bone
{
	int keyboneid;
	uint32 flags;
	short parentbone;
	uint16 unk[3];
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	float pivotpoint[3];
};
