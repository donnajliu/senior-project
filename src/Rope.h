#ifndef ROPE_H
#define ROPE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "scene/Voxel.h"
#include "Node.h"
#include <BSpline.h>

class Rope {
public:
    int currentX, currentY, currentZ;
    Rope();

    // immediately create a procedural rope
    Voxel* first;

    vector<int> minDistance(int ***dist, bool ***visited);
    vector<int> findSecNeighbors(Voxel**** VoxelBufferTemp, vector<int> currNeighbor, vector<int> u,vector<int> firstVec, vector<int> secVec, vector<int> thirdVec, vector<int> thirdToLastVec, vector<int> secToLastVec, vector<int> lastVec);
    vector<vector<int>> findNeighbors(Voxel**** VoxelBufferTemp, vector<int> u, vector<int> firstVec, vector<int> secVec, vector<int> thirdVec, vector<int> thirdToLastVec, vector<int> secToLastVec, vector<int> lastVec);
    void connectRope(Voxel**** VoxelBufferTemp, Voxel* first, Voxel* second, Voxel* third, Voxel* thirdToLast, Voxel* secondToLast, Voxel* last);

    bool drawVoxel(Voxel**** VoxelBufferTemp, int x, int y, int z, int prevX, int prevY, int prevZ, int prevX2, int prevY2, int prevZ2, bool ending);
    void proceduralRopeGenerator();

    // create a procedural rope and visually animate the creation of the rope
    // list of coordinates to draw (in order)
    bool newRopeCreated;
    vector<vector<int>> ropeValues;
    int ropeValuesIdx;

    Voxel**** connectRopeValues(Voxel**** VoxelBufferTemp, Voxel* first, Voxel* second, Voxel* third, Voxel* thirdToLast, Voxel* secondToLast, Voxel* last);
    void proceduralRopeValuesGenerator();

    void generateRopeValues();

	int buffX, buffY, buffZ;			// Number of voxels in the x, y, z directions
	float scaleX, scaleY, scaleZ;		// Size in the x, y, z directions
	float sX, sY, sZ;					// Size of the individual spheres/cubes
	
	Node* root;							// The root node which we will use for transformations and 
	
	glm::mat4 transformation;
	glm::vec3 color;

    void setup(); //
    bool pushedCheck();
    void simulate(); // ok
	
	// Voxel buffer
	Voxel ****voxelBuffer;

	// Stuff for thinning
	vector<Voxel*> connected_component;
	vector<Voxel*> block;
    vector<Voxel*> getNeighbors(Voxel* v); // ok

	// XY plane operations
    bool thinXY(); //
    bool pushXY(); // ok

	// XZ plane operations
	bool thinXZ();
	bool pushXZ();

	// YZ plane operations
	bool thinYZ();
	bool pushYZ();
	
	// Thin, push and check component in one plane
    bool thin(int third); //
    bool push(int third); // ok
    bool pushDirectionX;
    bool pushDirectionY;
    bool pushDirectionZ;
    int pushingOrder;
    bool pushDir(int, int); // ok

    //
    void fillCC(Voxel*, int); // ok
    bool isMember(Voxel*); // ok
	int endpoints;

    //
    bool checkComponent(int, int); // ok

    //
    vector<Voxel*> getEdgeAndVertexNeighbors(Voxel* v);
    vector<Voxel*> getPlanarNeighbors(Voxel* v, int plane); // ok

    //// Has the rope been thinned?
	bool thinnedXY;
	bool thinnedXZ;
	bool thinnedYZ;
	bool allThinned;

    //// Have we pushed in a plane?
	bool pushedXY;
	bool pushedXZ;
	bool pushedYZ;
    int pushNumber;

    //// Thin in all planes
    bool thinAll(); //
	bool changedThinning;
	bool changing;

    //// donna: heckc if the
    bool cantMove;

	// Double the resolution (in every direction)
    void doubleResolution(); //

	int counter;

	// Are there any voxels with 0 or 1 neighbor?
    bool checkExternal(); // ok

    //// hold on this for a moment
    void printVoxels(); //
    int numVoxels;
    bool dontDraw;
    vector<glm::vec3> connectingEnds;
    vector<glm::vec3> bSplineCurverRopeGen();
    vector<glm::vec3> bSplineCurver(); //
    void getCurve(vector<glm::vec3> inputPoints); //
    vector<glm::vec3> controlPoints;
    vector<glm::vec4> interpPoints;

    void writeOutRopeGen(vector<glm::vec3> inputPoints);
    void writeOut(vector<glm::vec3> inputPoints); //

};

#endif
