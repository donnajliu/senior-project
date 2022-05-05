#include "Rope.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDir>

static const float PI = 3.141592653589f;

static void printVector(float x, float y, float z, bool endLine) {
	cout << x << ", " << y << ", " << z;
	if (endLine) {
		cout << endl;
	}
}

Rope::Rope() {

	vector<Voxel*> connected_component;
	vector<Voxel*> block;
	root = new Node();
	
	thinnedXY = false;
	thinnedXZ = false;
	thinnedYZ = false;
	allThinned = false;

	currentX = 0;
	currentY = 0;
	currentZ = 0;

    pushedXY = true;
    pushedXZ = true;
    pushedYZ = true;

    pushNumber = 1;

	changing = false;

    counter = 0;

    controlPoints = vector<glm::vec3>();
    interpPoints = vector<glm::vec4>();

    newRopeCreated = false;
}

// determine whether the voxel should be drawn
bool Rope::drawVoxel(Voxel**** VoxelBufferTemp, int x, int y, int z, int prevX, int prevY, int prevZ, int prevX2, int prevY2, int prevZ2, bool ending) {
    vector<int> signs;
    signs.push_back(-1);
    signs.push_back(1);

    // current spot already filled with density 1 (change 0)
    if (VoxelBufferTemp[x][y][z]->density == 1) {
        return true;
    }

    // face neighbor already filled with density 1 (change 1)
    // make sure we are not checking for previous drawn voxel density
    cout << "checking neighbors - count 1" << endl;
    for (int sign : signs) {
        cout << "hi" << endl;
        cout << "first if statement: " << x + sign << " " << buffX << endl;
        if ((x + sign < buffX) && (x + sign >= 0)) {
            cout << VoxelBufferTemp[x + sign][y][z]->density << endl;
        }
        if ((x + sign < buffX && x + sign >= 0) && (VoxelBufferTemp[x + sign][y][z]->density == 1)) {
            cout << "second if statement" << endl;
            if (!((x + sign == prevX) && (y == prevY) && (z == prevZ))) {
                cout << "third if statement" << endl;
                if (!((x + sign == prevX2) && (y == prevY2) && (z == prevZ2))) {
                    // if adjacent is the first vertex, draw it
                    cout << "fourth if statement" << endl;
                    if (ending && (x + sign == first->x && y == first->y && z == first->z)) {
                        cout << "fifth" << endl;
                        return false;
                    }
                    cout << "sixth" << endl;
                    return true;
                }
            }
        }

        cout << "passed 1st 1 check" << endl;

        if ((y + sign < buffY && y + sign >= 0) && (VoxelBufferTemp[x][y + sign][z]->density == 1)) {
            if (!((x == prevX) && (y + sign == prevY) && (z == prevZ))) {
                if (!((x == prevX2) && (y + sign == prevY2) && (z == prevZ2))) {
                    if (ending && (x == first->x && y + sign == first->y && z == first->z)) {
                        return false;
                    }
                    return true;
                }
            }
        }

        cout << "passed 2nd 1 check" << endl;

        if ((z + sign < buffZ && z + sign >= 0) && (VoxelBufferTemp[x][y][z + sign]->density == 1)) {
            if (!((x == prevX) && (y == prevY) && (z + sign == prevZ))) {
                if (!((x == prevX2) && (y == prevY2) && (z + sign == prevZ2))) {
                    if (ending && (x == first->x && y == first->y && z + sign == first->z)) {
                        return false;
                    }
                    return true;
                }
            }
        }
    }

    cout << "no face issues" << endl;

    // edge neighbor already filled with density 1 (change 2)
    cout << "checking neighbors - count 2" << endl;
    for (int sign : signs) {
        if ((x + sign < buffX && x + sign >= 0) && (y + sign < buffY && y + sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y + sign][z]->density == 1 && ((x + sign != prevX) && (y + sign != prevY) && (z != prevZ))) {
                if (!((x + sign == prevX) && (y + sign == prevY) && (z == prevZ))) {
                    if (!((x + sign == prevX2) && (y + sign == prevY2) && (z == prevZ2))) {
                        if (ending && (x + sign == first->x && y + sign == first->y && z == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((x + sign < buffX && x + sign >= 0) && (z + sign < buffZ && z + sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y][z + sign]->density == 1 && ((x + sign != prevX))) {
                if (!((x + sign == prevX) && (y == prevY) && (z + sign == prevZ))) {
                    if (!((x + sign == prevX2) && (y == prevY2) && (z + sign == prevZ2))) {
                        if (ending && (x + sign == first->x && y == first->y && z + sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((y + sign < buffY && y + sign >= 0) && (z + sign < buffZ && z + sign >= 0)) {
            if (VoxelBufferTemp[x][y + sign][z + sign]->density == 1) {
                if (!((x == prevX) && (y + sign == prevY) && (z + sign == prevZ))) {
                    if (!((x == prevX2) && (y + sign == prevY2) && (z + sign == prevZ2))) {
                        if (ending && (x == first->x && y + sign == first->y && z + sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }


        if ((x + sign < buffX && x + sign >= 0) && (y - sign < buffY && y - sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y - sign][z]->density == 1) {
                if (!((x + sign == prevX) && (y - sign == prevY) && (z == prevZ))) {
                    if (!((x + sign == prevX2) && (y - sign == prevY2) && (z == prevZ2))) {
                        if (ending && (x + sign == first->x && y - sign == first->y && z == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((x + sign < buffX && x + sign >= 0) && (z - sign < buffZ && z - sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y][z - sign]->density == 1) {
                if (!((x + sign == prevX) && (y == prevY) && (z - sign == prevZ))) {
                    if (!((x + sign == prevX2) && (y == prevY2) && (z - sign == prevZ2))) {
                        if (ending && (x + sign == first->x && y == first->y && z - sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((y + sign < buffY && y + sign >= 0) && (z - sign < buffZ && z - sign >= 0)) {
            if (VoxelBufferTemp[x][y + sign][z - sign]->density == 1) {
                if (!((x == prevX) && (y + sign == prevY) && (z - sign == prevZ))) {
                    if (!((x == prevX2) && (y + sign == prevY2) && (z - sign == prevZ2))) {
                        if (ending && (x == first->x && y + sign == first->y && z - sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
    }

    cout << "no face, edge issues" << endl;

    // vertex neighbor already filled with density 1 (change 3)
    cout << "checking neighbors - count 3" << endl;
    for (int sign : signs) {
        if ((x + sign < buffX && x + sign >= 0) && (y + sign < buffY && y + sign >= 0) && (z + sign < buffZ && z + sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y + sign][z + sign]->density == 1) {
                if (!((x + sign == prevX) && (y + sign == prevY) && (z + sign == prevZ))) {
                    if (!((x + sign == prevX2) && (y + sign == prevY2) && (z + sign == prevZ2))) {
                        if (ending && (x + sign == first->x && y + sign == first->y && z + sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }


        if ((x - sign < buffX && x - sign >= 0) && (y + sign < buffY && y + sign >= 0) && (z + sign < buffZ && z + sign >= 0)) {
            if (VoxelBufferTemp[x - sign][y + sign][z + sign]->density == 1) {
                if (!((x - sign == prevX) && (y + sign == prevY) && (z + sign == prevZ))) {
                    if (!((x - sign == prevX2) && (y + sign == prevY2) && (z + sign == prevZ2))) {
                        if (ending && (x - sign == first->x && y + sign == first->y && z + sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((x + sign < buffX && x + sign >= 0) && (y - sign < buffY && y - sign >= 0) && (z + sign < buffZ && z + sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y - sign][z + sign]->density == 1) {
                if (!((x + sign == prevX) && (y - sign == prevY) && (z + sign == prevZ))) {
                    if (!((x + sign == prevX2) && (y - sign == prevY2) && (z + sign == prevZ2))) {
                        if (ending && (x + sign == first->x && y - sign == first->y && z + sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((x + sign < buffX && x + sign >= 0) && (y + sign < buffY && y + sign >= 0) && (z - sign < buffZ && z - sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y + sign][z - sign]->density == 1) {
                if (!((x + sign == prevX) && (y + sign == prevY) && (z - sign == prevZ))) {
                    if (!((x + sign == prevX2) && (y + sign == prevY2) && (z - sign == prevZ2))) {
                        if (ending && (x + sign == first->x && y + sign == first->y && z - sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }


        if ((x - sign < buffX && x - sign >= 0) && (y - sign < buffY && y - sign >= 0) && (z + sign < buffZ && z + sign >= 0)) {
            if (VoxelBufferTemp[x - sign][y - sign][z + sign]->density == 1) {
                if (!((x - sign == prevX) && (y - sign == prevY) && (z + sign == prevZ))) {
                    if (!((x - sign == prevX2) && (y - sign == prevY2) && (z + sign == prevZ2))) {
                        if (ending && (x - sign == first->x && y - sign == first->y && z + sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((x - sign < buffX && x - sign >= 0) && (y + sign < buffY && y + sign >= 0) && (z - sign < buffZ && z - sign >= 0)) {
            if (VoxelBufferTemp[x - sign][y + sign][z - sign]->density == 1) {
                if (!((x - sign == prevX) && (y + sign == prevY) && (z - sign == prevZ))) {
                    if (!((x - sign == prevX2) && (y + sign == prevY2) && (z - sign == prevZ2))) {
                        if (ending && (x - sign == first->x && y + sign == first->y && z - sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
        if ((x + sign < buffX && x + sign >= 0) && (y - sign < buffY && y - sign >= 0) && (z - sign < buffZ && z - sign >= 0)) {
            if (VoxelBufferTemp[x + sign][y - sign][z - sign]->density == 1) {
                if (!((x + sign == prevX) && (y - sign == prevY) && (z - sign == prevZ))) {
                    if (!((x + sign == prevX2) && (y - sign == prevY2) && (z - sign == prevZ2))) {
                        if (ending && (x + sign == first->x && y - sign == first->y && z - sign == first->z)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }
    }

    cout << "no face, edge, vertex issues" << endl;

    // no issues
    return false;
}

vector<int> Rope::minDistance(int ***dist, bool ***visited) {
    // Initialize min value
    int min = INT_MAX;
    vector<int> min_index = vector<int>(3, -1);

    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                if (visited[i][j][k] == false  && dist[i][j][k] <= min) {
                    min = dist[i][j][k];
                    min_index[0] = i;
                    min_index[1] = j;
                    min_index[2] = k;
                }
            }
        }
    }
    return min_index;
}

vector<int> Rope::findSecNeighbors(Voxel**** VoxelBufferTemp, vector<int> currNeighbor, vector<int> u, vector<int> firstVec, vector<int> secVec, vector<int> thirdVec, vector<int> thirdToLastVec, vector<int> secToLastVec, vector<int> lastVec) {
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {
                vector<int> secNeighbor = currNeighbor;
                secNeighbor[0] += x;
                secNeighbor[1] += y;
                secNeighbor[2] += z;

                if (secNeighbor[0] >= 0 && secNeighbor[0] < buffX && secNeighbor[1] >= 0 && secNeighbor[1] < buffY && secNeighbor[2] >= 0 && secNeighbor[2] < buffZ) {
                    if((secNeighbor != lastVec && secNeighbor != firstVec) && (secNeighbor != secVec && secNeighbor != secToLastVec) && (secNeighbor != thirdVec && secNeighbor != thirdToLastVec)){
                        if (VoxelBufferTemp[secNeighbor[0]][secNeighbor[1]][secNeighbor[2]]->getDensity() != 0) {
                            if (secNeighbor != u) {
                                vector<int> notValid{-1, -1, -1};
                                return notValid;
                            }
                        }
                    }
                }
            }
        }
    }

    return currNeighbor;
}

vector<vector<int>> Rope::findNeighbors(Voxel**** VoxelBufferTemp, vector<int> u, vector<int> firstVec, vector<int> secVec, vector<int> thirdVec, vector<int> thirdToLastVec, vector<int> secToLastVec, vector<int> lastVec) {
    vector<vector<int>> uNeighbors{{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}};
    vector<vector<int>> validNeighbors;

    for (int i = 0; i < 6; i++) {
        vector<int> currNeighbor = uNeighbors[i];
        for (int j = 0; j < 3; j++) {
            currNeighbor[j] += u[j];
        }
        if(currNeighbor == lastVec){
            validNeighbors.push_back(currNeighbor);
        } else if (currNeighbor[0] >= 0 && currNeighbor[0] < buffX && currNeighbor[1] >= 0 && currNeighbor[1] < buffY && currNeighbor[2] >= 0 && currNeighbor[2] < buffZ) {
            currNeighbor = findSecNeighbors(VoxelBufferTemp, currNeighbor, u, firstVec, secVec, thirdVec, thirdToLastVec, secToLastVec, lastVec);
            if (currNeighbor[0] != -1 && currNeighbor[1] != -1 && currNeighbor[2] != -1) {
                validNeighbors.push_back(currNeighbor);
            }
        }
    }

    return validNeighbors;
}

void Rope::connectRope(Voxel**** VoxelBufferTemp, Voxel* first, Voxel* second, Voxel* third, Voxel* thirdToLast, Voxel* secondToLast, Voxel* last) {
    // The output array. dist[i] will hold the shortest distance from src to i
    int ***dist = new int**[buffX];
    bool ***visited = new bool**[buffX];
    // parent[i] will be true if vertex i is included in shortest
    vector<int> ***parent = new vector<int>**[buffX];

    for (int k = 0; k < buffX; k++) {
        dist[k] = new int*[buffY];
        visited[k] = new bool*[buffY];
        parent[k] = new vector<int>*[buffY];
    }

    for (int k = 0; k < buffX; k++) {
        for (int w = 0; w < buffY; w++) {
            dist[k][w] = new int[buffZ];
            visited[k][w] = new bool[buffZ];
            parent[k][w] = new vector<int>[buffZ];
        }
    }

    // cout << "@@@@ initialization @@@@" << endl;
    // path tree or shortest distance from src to i is finalized
    vector<int> notValid{-1, -1, -1};
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                dist[i][j][k] = INT_MAX;
                parent[i][j][k] = notValid;
                visited[i][j][k] = false;
            }
        }
    }
    vector<int> firstVec = {first->x,first->y,first->z};
    vector<int> lastVec = {last->x,last->y,last->z};
    vector<int> secVec = {second->x, second->y, second->z};
    vector<int> thirdVec = {third->x, third->y, third->z};
    vector<int> secToLastVec = {secondToLast->x, secondToLast->y, secondToLast->z};
    vector<int> thirdToLastVec = {thirdToLast->x, thirdToLast->y, thirdToLast->z};

    parent[first->x][first->y][first->z] = firstVec;
    cout << "@@@@ done initialization @@@@" << endl;
    // Distance of source vertex from itself is always 0
    dist[first->x][first->y][first->z] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < buffX * buffY * buffZ - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.

        vector<int> u = minDistance(dist, visited);
        if( u == notValid){
            break;
        }else{
            // Mark the picked vertex as processed
            visited[u[0]][u[1]][u[2]] = true;

            // Update dist value of the adjacent vertices of the picked vertex.
            // You are currently at u
            // Iterate through all nodes, check if they are a neighbor/adj
            // graph[u][v]
            // set of all candidate v's are x-1,y,z, x+1,y,z, x,y-1,z

            vector<vector<int>> validNeighbors = findNeighbors(VoxelBufferTemp, u, firstVec, secVec, thirdVec, thirdToLastVec, secToLastVec, lastVec);

            for (int i = 0; i < validNeighbors.size(); i++) {
                vector<int> currNeighbor = validNeighbors[i];
                if (visited[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] == false){
                    if( dist[u[0]][u[1]][u[2]] != INT_MAX
                    && dist[u[0]][u[1]][u[2]] + 1 < dist[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]]) {

                        dist[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] = dist[u[0]][u[1]][u[2]] + 1;
                        parent[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] = u;
                       // visited[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] = true;
                        if(currNeighbor == lastVec){
                            break;
                        }
                    }
                }
            }
        }
    }

    cout << "first " << firstVec[0] << " " << firstVec[1] << " "<< firstVec[2] << " " << endl;
    vector<int> cur = lastVec;
    cout << "last " << cur[0] << " " << cur[1] << " "<< cur[2] << " " << endl;

    if (dist[lastVec[0]][lastVec[1]][lastVec[2]] != INT_MAX) {
        while (cur != firstVec) {
            Voxel* voxel = new Voxel(1);
            voxel->x = cur[0];
            voxel->y = cur[1];
            voxel->z = cur[2];

            VoxelBufferTemp[cur[0]][cur[1]][cur[2]] = voxel;
            cur = parent[cur[0]][cur[1]][cur[2]];
        }
        /*Voxel* voxel = new Voxel(1);
        voxel->x = cur[0];
        voxel->y = cur[1];
        voxel->z = cur[2];

        voxelBuffer[cur[0]][cur[1]][cur[2]] = voxel;*/
    } else {
        cout << "did not find valid path! please restart me :)" << endl;
        for (int i = 0; i < buffX; i++) {
            for (int j = 0; j < buffY; j++) {
                for (int k = 0; k < buffZ; k++) {
                    Voxel* voxel = new Voxel(0);
                    voxel->x = i;
                    voxel->y = j;
                    voxel->z = k;

                    VoxelBufferTemp[i][j][k] = voxel;
                }
            }
        }

        proceduralRopeGenerator();
    }
}

void Rope::proceduralRopeGenerator() {
    buffX = 20;
    buffY = 20;
    buffZ = 20;

    sX = (float) scaleX / (float) buffX;
    sY = (float) scaleY / (float) buffY;
    sZ = (float) scaleZ / (float) buffZ;

    voxelBuffer = new Voxel***[buffX];
    for (int k = 0; k < buffX; k++) {
        voxelBuffer[k] = new Voxel**[buffY];
    }

    for (int k = 0; k < buffX; k++) {
        for (int w = 0; w < buffY; w++) {
            voxelBuffer[k][w] = new Voxel*[buffZ];
        }
    }

    // clear voxels before procedurally generating the rope
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* voxel = new Voxel(0);
                voxel->x = i;
                voxel->y = j;
                voxel->z = k;

                voxelBuffer[i][j][k] = voxel;
            }
        }
    }

    // allow the rope to start in a random spot within the buffer
//    int firstX = arc4random() % (buffX / 2) + buffX / 2;
//    int firstY = arc4random() % (buffY / 2) + buffY / 2;
//    int firstZ = arc4random() % (buffZ / 2) + buffZ / 2;

    int firstX = 0;
    int firstY = 0;
    int firstZ = 0;

    first = new Voxel(1);
    first->x = firstX;
    first->y = firstY;
    first->z = firstZ;

    cout << first->x << " " << first->y << " " << first->z << endl;

    // fill the buffer with this first voxel
    voxelBuffer[first->x][first->y][first->z] = first;

    // second voxel created
    Voxel* second = new Voxel(1);
    bool secondCreated = false;

    // third voxel created
    Voxel* third = new Voxel(1);
    int thirdCreated = 0;

    // keep track of the previous x, y and z values
    int prevX = first->x;
    int prevY = first->y;
    int prevZ = first->z;

    int prevX2 = first->x;
    int prevY2 = first->y;
    int prevZ2 = first->z;

    // fill the buffer with density 0
    /*for (int z = 0; z < buffZ; z++) {
        for (int y = 0; y < buffY; y++) {
            for (int x = 0; x < buffX; x++) {
                Voxel* voxel = new Voxel(0);
                voxel->x = x;
                voxel->y = y;
                voxel->z = z;

                voxelBuffer[x][y][z] = voxel;
            }
        }
    }*/

    /* number of twists/turns:
     * numTwists
     *
     * moving in direction:
     * 0 = up
     * 1 = down
     * 2 = left
     * 3 = right
     * 4 = forward
     * 5 = backward
     *
     * cases where we shouldn't draw rope:
     * density of this spot is already 1
     * within the buffer's bounds
     * does not have any neighbors
     * -- edge, vertex or face
     */
    int count = 0;
    for (int numTwists = 0; numTwists < 100; numTwists++) {
        cout << "Number of twists: " << count << endl;
        count++;

        int chooseDirection = arc4random() % 6;
        int chooseLength = 0;
        if (chooseDirection == 0 || chooseDirection == 1) {
            chooseLength = arc4random() % (buffY / 5) + 1;
        } else if (chooseDirection == 2 || chooseDirection == 3) {
            chooseLength = arc4random() % (buffX / 5) + 1;
        } else if (chooseDirection == 4 || chooseDirection == 5) {
            chooseLength = arc4random() % (buffZ / 5) + 1;
        }

        if (numTwists == 0) {
            chooseLength += 2;
        }

        cout << "chosen direction: " << chooseDirection << endl;
        cout << "chosen length: " << chooseLength << endl;

        if (chooseDirection == 0) {
            cout << "chosen direction entered 0 " << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY + 1;
                v->z = prevZ;

                cout << "about to enter 0 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->y >= buffY || drawVoxel(voxelBuffer, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 0: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    // no issues, fill the voxel
                    voxelBuffer[v->x][v->y][v->z] = v;
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    // keep track of the drawn voxel
                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    // second voxel has not been drawn yet
                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        } else if (chooseDirection == 1) {
            cout << "chosen direction entered 1" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY - 1;
                v->z = prevZ;

                cout << "about to enter 1 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->y < 0 || drawVoxel(voxelBuffer, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 1: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBuffer[v->x][v->y][v->z] = v;
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        } else if (chooseDirection == 2) {
            cout << "chosen direction entered 2" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX + 1;
                v->y = prevY;
                v->z = prevZ;

                cout << "about to enter 2 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->x >= buffX || drawVoxel(voxelBuffer, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 2: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBuffer[v->x][v->y][v->z] = v;
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        } else if (chooseDirection == 3) {
            cout << "chosen direction entered 3" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX - 1;
                v->y = prevY;
                v->z = prevZ;

                cout << "about to enter 3 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->x < 0 || drawVoxel(voxelBuffer, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 3: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBuffer[v->x][v->y][v->z] = v;
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        }  else if (chooseDirection == 4) {
            cout << "chosen direction entered 4" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY;
                v->z = prevZ + 1;

                cout << "about to enter 4 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->z >= buffZ || drawVoxel(voxelBuffer, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 4: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBuffer[v->x][v->y][v->z] = v;
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        }  else if (chooseDirection == 5) {
            cout << "chosen direction entered 5: " << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY;
                v->z = prevZ - 1;

                cout << "about to enter 5 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->z < 0 || drawVoxel(voxelBuffer, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 5: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBuffer[v->x][v->y][v->z] = v;
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        }
    }

    cout << "ready to close the rope" << endl;

    // get the last drawn voxel
    Voxel* last = new Voxel(1);
    last->x = prevX;
    last->y = prevY;
    last->z = prevZ;

    Voxel* secondToLast = new Voxel(1);
    secondToLast->x = prevX2;
    secondToLast->y = prevY2;
    secondToLast->z = prevZ2;

    Voxel* thirdToLast = new Voxel(1);
    vector<vector<int>> thirdNeighbors{{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}};

    // get the third to last voxel drawn
    for (int i = 0; i < 6; i++) {
        int xTTL = secondToLast->x + thirdNeighbors[i][0];
        int yTTL = secondToLast->y + thirdNeighbors[i][1];
        int zTTL = secondToLast->z + thirdNeighbors[i][2];

        if (xTTL >= 0 && xTTL < buffX) {
            if (yTTL >= 0 && yTTL < buffY) {
                if (zTTL >= 0 && zTTL < buffZ) {
                    if (!(thirdNeighbors[i][0] == 0 && thirdNeighbors[i][1] == 0 && thirdNeighbors[i][2] == 0)) {
                        if (!(xTTL == prevX && yTTL == prevY && zTTL == prevZ)) {
                            if (voxelBuffer[xTTL][yTTL][zTTL]->density == 1) {
                                thirdToLast->x = xTTL;
                                thirdToLast->y = yTTL;
                                thirdToLast->z = zTTL;
                            }
                        }
                    }
                }
            }
        }
    }
    // close the rope
    connectRope(voxelBuffer, first, second, third, thirdToLast, secondToLast, last);

    /* previous method
     * first voxel is called first, last voxel is called last
     *
     * check x, y, and z difference
     * if the difference is negative
     * -- start moving in neg direction
     * if the difference is positive
     * -- start moving in pos direction
     */

    cout << "procedural rope is complete :)" << endl;
    setup();
}

// dijkstra shortest path
Voxel**** Rope::connectRopeValues(Voxel**** voxelBufferTemp, Voxel* first, Voxel* second, Voxel* third, Voxel* thirdToLast, Voxel* secondToLast, Voxel* last) {
    // The output array. dist[i] will hold the shortest distance from src to i
    int ***dist = new int**[buffX];
    bool ***visited = new bool**[buffX];
    // parent[i] will be true if vertex i is included in shortest
    vector<int> ***parent = new vector<int>**[buffX];

    for (int k = 0; k < buffX; k++) {
        dist[k] = new int*[buffY];
        visited[k] = new bool*[buffY];
        parent[k] = new vector<int>*[buffY];
    }

    for (int k = 0; k < buffX; k++) {
        for (int w = 0; w < buffY; w++) {
            dist[k][w] = new int[buffZ];
            visited[k][w] = new bool[buffZ];
            parent[k][w] = new vector<int>[buffZ];
        }
    }

    // cout << "@@@@ initialization @@@@" << endl;
    // path tree or shortest distance from src to i is finalized
    vector<int> notValid{-1, -1, -1};
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                dist[i][j][k] = INT_MAX;
                parent[i][j][k] = notValid;
                visited[i][j][k] = false;
            }
        }
    }
    vector<int> firstVec = {first->x,first->y,first->z};
    vector<int> lastVec = {last->x,last->y,last->z};
    vector<int> secVec = {second->x, second->y, second->z};
    vector<int> thirdVec = {third->x, third->y, third->z};
    vector<int> secToLastVec = {secondToLast->x, secondToLast->y, secondToLast->z};
    vector<int> thirdToLastVec = {thirdToLast->x, thirdToLast->y, thirdToLast->z};

    parent[first->x][first->y][first->z] = firstVec;
    cout << "@@@@ done initialization @@@@" << endl;
    // Distance of source vertex from itself is always 0
    dist[first->x][first->y][first->z] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < buffX * buffY * buffZ - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.

        vector<int> u = minDistance(dist, visited);
        if( u == notValid){
            break;
        }else{
            // Mark the picked vertex as processed
            visited[u[0]][u[1]][u[2]] = true;

            // Update dist value of the adjacent vertices of the picked vertex.
            // You are currently at u
            // Iterate through all nodes, check if they are a neighbor/adj
            // graph[u][v]
            // set of all candidate v's are x-1,y,z, x+1,y,z, x,y-1,z

            vector<vector<int>> validNeighbors = findNeighbors(voxelBufferTemp, u, firstVec, secVec, thirdVec, thirdToLastVec, secToLastVec, lastVec);

            for (int i = 0; i < validNeighbors.size(); i++) {
                vector<int> currNeighbor = validNeighbors[i];
                if (visited[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] == false){
                    if( dist[u[0]][u[1]][u[2]] != INT_MAX
                    && dist[u[0]][u[1]][u[2]] + 1 < dist[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]]) {

                        dist[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] = dist[u[0]][u[1]][u[2]] + 1;
                        parent[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] = u;
                       // visited[currNeighbor[0]][currNeighbor[1]][currNeighbor[2]] = true;
                        if(currNeighbor == lastVec){
                            break;
                        }
                    }
                }
            }
        }
    }

    cout << "first " << firstVec[0] << " " << firstVec[1] << " "<< firstVec[2] << " " << endl;
    vector<int> cur = lastVec;
    cout << "last " << cur[0] << " " << cur[1] << " "<< cur[2] << " " << endl;

    if (dist[lastVec[0]][lastVec[1]][lastVec[2]] != INT_MAX) {
        while (cur != firstVec) {
            Voxel* voxel = new Voxel(1);
            voxel->x = cur[0];
            voxel->y = cur[1];
            voxel->z = cur[2];

            voxelBufferTemp[cur[0]][cur[1]][cur[2]] = voxel;
            vector<int> curValue{cur[0], cur[1], cur[2]};
            ropeValues.push_back(curValue);
            cur = parent[cur[0]][cur[1]][cur[2]];
        }
    } else {
        cout << "did not find valid path! please restart me :)" << endl;
        for (int i = 0; i < buffX; i++) {
            for (int j = 0; j < buffY; j++) {
                for (int k = 0; k < buffZ; k++) {
                    Voxel* voxel = new Voxel(0);
                    voxel->x = i;
                    voxel->y = j;
                    voxel->z = k;

                    voxelBufferTemp[i][j][k] = voxel;
                }
            }
        }
        newRopeCreated = false;
        ropeValues.clear();
        proceduralRopeValuesGenerator();
    }

    return voxelBufferTemp;
}

void Rope::proceduralRopeValuesGenerator() {
    Voxel**** voxelBufferTemp;
    // new rope has been created and hasn't connected
    if (newRopeCreated) {
        ropeValuesIdx++;
        return;
    } else {
        ropeValuesIdx = 0;
        ropeValues.clear();
        newRopeCreated = true;
    }

    buffX = 20;
    buffY = 20;
    buffZ = 20;

    sX = (float) scaleX / (float) buffX;
    sY = (float) scaleY / (float) buffY;
    sZ = (float) scaleZ / (float) buffZ;

    voxelBufferTemp = new Voxel***[buffX];
    for (int k = 0; k < buffX; k++) {
        voxelBufferTemp[k] = new Voxel**[buffY];
    }

    for (int k = 0; k < buffX; k++) {
        for (int w = 0; w < buffY; w++) {
            voxelBufferTemp[k][w] = new Voxel*[buffZ];
        }
    }

    // clear voxels before procedurally generating the rope
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* voxel = new Voxel(0);
                voxel->x = i;
                voxel->y = j;
                voxel->z = k;

                voxelBuffer[i][j][k] = voxel;
            }
        }
    }

    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* voxel = new Voxel(0);
                voxel->x = i;
                voxel->y = j;
                voxel->z = k;

                voxelBufferTemp[i][j][k] = voxel;
            }
        }
    }

    // allow the rope to start in a random spot within the buffer
    int firstX = arc4random() % (buffX / 2) + buffX / 2;
    int firstY = arc4random() % (buffY / 2) + buffY / 2;
    int firstZ = arc4random() % (buffZ / 2) + buffZ / 2;

    first = new Voxel(1);
    first->x = firstX;
    first->y = firstY;
    first->z = firstZ;

    cout << first->x << " " << first->y << " " << first->z << endl;

    // fill the buffer with this first voxel
    voxelBufferTemp[first->x][first->y][first->z] = first;
    vector<int> firstValue{first->x, first->y, first->z};
    ropeValues.push_back(firstValue);

    // second voxel created
    Voxel* second = new Voxel(1);
    bool secondCreated = false;

    // third voxel created
    Voxel* third = new Voxel(1);
    int thirdCreated = 0;

    // keep track of the previous x, y and z values
    int prevX = first->x;
    int prevY = first->y;
    int prevZ = first->z;

    int prevX2 = first->x;
    int prevY2 = first->y;
    int prevZ2 = first->z;

    /* number of twists/turns:
     * numTwists
     *
     * moving in direction:
     * 0 = up
     * 1 = down
     * 2 = left
     * 3 = right
     * 4 = forward
     * 5 = backward
     *
     * cases where we shouldn't draw rope:
     * density of this spot is already 1
     * within the buffer's bounds
     * does not have any neighbors
     * -- edge, vertex or face
     */
    int count = 0;
    for (int numTwists = 0; numTwists < 10; numTwists++) {
        cout << "Number of twists: " << count << endl;
        count++;

        int chooseDirection = arc4random() % 5;
        int chooseLength = 0;
        if (chooseDirection == 0 || chooseDirection == 1) {
            chooseLength = arc4random() % (buffY / 5) + 1;
        } else if (chooseDirection == 2 || chooseDirection == 3) {
            chooseLength = arc4random() % (buffX / 5) + 1;
        } else if (chooseDirection == 4 || chooseDirection == 5) {
            chooseLength = arc4random() % (buffZ / 5) + 1;
        }

        if (numTwists == 0) {
            chooseLength += 2;
        }

        cout << "chosen direction: " << chooseDirection << endl;
        cout << "chosen length: " << chooseLength << endl;

        if (chooseDirection == 0) {
            cout << "chosen direction entered 0 " << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY + 1;
                v->z = prevZ;

                cout << "about to enter 0 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->y >= buffY || drawVoxel(voxelBufferTemp, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 0: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    // no issues, fill the voxel
                    voxelBufferTemp[v->x][v->y][v->z] = v;
                    vector<int> vValue{v->x, v->y, v->z};
                    ropeValues.push_back(vValue);
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    // keep track of the drawn voxel
                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    // second voxel has not been drawn yet
                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        } else if (chooseDirection == 1) {
            cout << "chosen direction entered 1" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY - 1;
                v->z = prevZ;

                cout << "about to enter 1 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->y < 0 || drawVoxel(voxelBufferTemp, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 1: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBufferTemp[v->x][v->y][v->z] = v;
                    vector<int> vValue{v->x, v->y, v->z};
                    ropeValues.push_back(vValue);
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        } else if (chooseDirection == 2) {
            cout << "chosen direction entered 2" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX + 1;
                v->y = prevY;
                v->z = prevZ;

                cout << "about to enter 2 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->x >= buffX || drawVoxel(voxelBufferTemp, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 2: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBufferTemp[v->x][v->y][v->z] = v;
                    vector<int> vValue{v->x, v->y, v->z};
                    ropeValues.push_back(vValue);
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        } else if (chooseDirection == 3) {
            cout << "chosen direction entered 3" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX - 1;
                v->y = prevY;
                v->z = prevZ;

                cout << "about to enter 3 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->x < 0 || drawVoxel(voxelBufferTemp, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 3: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBufferTemp[v->x][v->y][v->z] = v;
                    vector<int> vValue{v->x, v->y, v->z};
                    ropeValues.push_back(vValue);
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        }  else if (chooseDirection == 4) {
            cout << "chosen direction entered 4" << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY;
                v->z = prevZ + 1;

                cout << "about to enter 4 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->z >= buffZ || drawVoxel(voxelBufferTemp, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 4: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBufferTemp[v->x][v->y][v->z] = v;
                    vector<int> vValue{v->x, v->y, v->z};
                    ropeValues.push_back(vValue);
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        }  else if (chooseDirection == 5) {
            cout << "chosen direction entered 5: " << endl;
            for (int length = 0; length < chooseLength; length++) {
                Voxel* v = new Voxel(1);
                v->x = prevX;
                v->y = prevY;
                v->z = prevZ - 1;

                cout << "about to enter 5 if statement: " << v->x << " " << v->y << " " << v->z << endl;
                if (v->z < 0 || drawVoxel(voxelBufferTemp, v->x, v->y, v->z, prevX, prevY, prevZ, prevX2, prevY2, prevZ2, false)) {
                    cout << "already filled 5: " << v->x << " " << v->y << " " << v->z << endl;
                    length = chooseLength;
                } else {
                    voxelBufferTemp[v->x][v->y][v->z] = v;
                    vector<int> vValue{v->x, v->y, v->z};
                    ropeValues.push_back(vValue);
                    cout << v->x << " " << v->y << " " << v->z << endl;

                    prevX2 = prevX;
                    prevY2 = prevY;
                    prevZ2 = prevZ;

                    prevX = v->x;
                    prevY = v->y;
                    prevZ = v->z;

                    if (secondCreated == false) {
                        secondCreated = true;
                        second->x = prevX;
                        second->y = prevY;
                        second->z = prevZ;
                        thirdCreated++;
                    } else if (thirdCreated == 1) {
                        thirdCreated++;
                        third->x = prevX;
                        third->y = prevY;
                        third->z = prevZ;
                    }
                }
            }
            cout << "left loop" << endl;
        }
    }

    cout << "ready to close the rope" << endl;

    // get the last drawn voxel
    Voxel* last = new Voxel(1);
    last->x = prevX;
    last->y = prevY;
    last->z = prevZ;

    Voxel* secondToLast = new Voxel(1);
    secondToLast->x = prevX2;
    secondToLast->y = prevY2;
    secondToLast->z = prevZ2;

    Voxel* thirdToLast = new Voxel(1);
    vector<vector<int>> thirdNeighbors{{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}};

    // get the third to last voxel drawn
    for (int i = 0; i < 6; i++) {
        int xTTL = secondToLast->x + thirdNeighbors[i][0];
        int yTTL = secondToLast->y + thirdNeighbors[i][1];
        int zTTL = secondToLast->z + thirdNeighbors[i][2];

        if (xTTL >= 0 && xTTL < buffX) {
            if (yTTL >= 0 && yTTL < buffY) {
                if (zTTL >= 0 && zTTL < buffZ) {
                    if (!(thirdNeighbors[i][0] == 0 && thirdNeighbors[i][1] == 0 && thirdNeighbors[i][2] == 0)) {
                        if (!(xTTL == prevX && yTTL == prevY && zTTL == prevZ)) {
                            if (voxelBufferTemp[xTTL][yTTL][zTTL]->density == 1) {
                                thirdToLast->x = xTTL;
                                thirdToLast->y = yTTL;
                                thirdToLast->z = zTTL;
                            }
                        }
                    }
                }
            }
        }
    }
    // close the rope
    voxelBufferTemp = connectRopeValues(voxelBufferTemp, first, second, third, thirdToLast, secondToLast, last);

    voxelBuffer = voxelBufferTemp;

    /* previous method
     * first voxel is called first, last voxel is called last
     *
     * check x, y, and z difference
     * if the difference is negative
     * -- start moving in neg direction
     * if the difference is positive
     * -- start moving in pos direction
     */

    int numVoxels = 0;

    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                if (voxelBuffer[i][j][k]->density == 1) {
                    numVoxels++;
                }
            }
        }
    }

    cout << "number of voxels in theory: " << numVoxels << ", num of voxels in reality: " << ropeValues.size() - 1 << endl;

    cout << "procedural rope values are complete :)" << endl;


}

void Rope::generateRopeValues() {
    // clear entire rope first
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* voxel = new Voxel(0);
                voxel->x = i;
                voxel->y = j;
                voxel->z = k;

                voxelBuffer[i][j][k] = voxel;
            }
        }
    }

    // draw rope up to current ropeValueIdx
    for (int i = 0; i < ropeValuesIdx; i++) {
        Voxel* voxel = new Voxel(1);
        voxel->x = ropeValues[i][0];
        voxel->y = ropeValues[i][1];
        voxel->z = ropeValues[i][2];

        voxelBuffer[voxel->x][voxel->y][voxel->z] = voxel;
    }

    setup();

    // turn the rope creation into a bspline
    if (ropeValuesIdx > 2) {
        vector<glm::vec3> newControlPoints = bSplineCurverRopeGen();
        if (newControlPoints.size() != this->controlPoints.size()) {
            writeOutRopeGen(newControlPoints);
        }
    }
}

void Rope::printVoxels() {
    int count = 0;
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* v = voxelBuffer[i][j][k];
                if (v->density == 1) {
                    //cout << "[" << i << "], " << "[" << j << "], " << "[" << k << "]" << endl;
                    //cout << "Position: " << "(" << v->position.x << ", " << v->position.y << ", " << v->position.z << ")" << endl;
                    count++;
                }
            }
        }
    }
    numVoxels = count;
    //cout << "Count: " << count << endl;
}

vector<glm::vec3> Rope::bSplineCurverRopeGen() {
    connectingEnds.clear();
    vector<glm::vec4> connectingEndsTemp;

    /*if (thinAll() == false || dontDraw == true) {
        vector<glm::vec3> empty;
        empty.push_back(glm::vec3(0, 0, 0));
        return empty;
    }*/

    vector<glm::vec4> controlPoints;
    Voxel* firstVoxel = nullptr;

    Voxel* currentVoxel = nullptr;
    Voxel* prevVoxel = nullptr;

    // get a filled voxel
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* v = voxelBuffer[i][j][k];
                if (v->density == 1) {
                    currentVoxel = v;
                    firstVoxel = v;
                }
            }
        }
    }

    // get the control points
    for (int i = 0; i < ropeValues.size() - ropeValuesIdx; i++) {
        currentVoxel->position[0] = ropeValues[i][0];
        currentVoxel->position[1] = ropeValues[i][1];
        currentVoxel->position[2] = ropeValues[i][2];
        controlPoints.push_back(currentVoxel->position);
    }

    // convert into vec3 for bspline
    vector<glm::vec3> inputPoints;
    for (int i = 0; i < (int) controlPoints.size(); i++) {
        std::cout << "cur " << controlPoints[i][0] << " " << controlPoints[i][1] << " " << controlPoints[i][2] << std::endl;
        inputPoints.push_back(glm::vec3(controlPoints[i]));
    }

    return inputPoints;
}

vector<glm::vec3> Rope::bSplineCurver() {
    connectingEnds.clear();
    vector<glm::vec4> connectingEndsTemp;

    if (thinAll() == false || dontDraw == true) {
        vector<glm::vec3> empty;
        empty.push_back(glm::vec3(0, 0, 0));
        return empty;
    }

    vector<glm::vec4> controlPoints;
    Voxel* firstVoxel = nullptr;

    Voxel* currentVoxel = nullptr;
    Voxel* prevVoxel = nullptr;

    // get a filled voxel
    for (int i = 0; i < buffX; i++) {
        for (int j = 0; j < buffY; j++) {
            for (int k = 0; k < buffZ; k++) {
                Voxel* v = voxelBuffer[i][j][k];
                if (v->density == 1) {
                    currentVoxel = v;
                    firstVoxel = v;
                }
            }
        }
    }

    bool firstTime = true;

    while (currentVoxel != firstVoxel || firstTime) {
        vector<Voxel*> neighbors = getNeighbors(currentVoxel);
        if (neighbors.size() > 2) {
            std::cout << "NEIGHBORS MORE THAN 2: #" << neighbors.size() << std::endl;
        }
        Voxel* vNext = nullptr;
        // what if there are more than 2 neigbors?
        for (Voxel* v : neighbors) {
            if (v != prevVoxel) {
                vNext = v;
            }
        }

        if (firstTime) {
            controlPoints.push_back(currentVoxel->position);
        } else {
            controlPoints.push_back(currentVoxel->position);
        }

        if (currentVoxel == firstVoxel) {
            firstTime = false;
        }
        //std::cout << "curr " << currentVoxel->position[0] << " " << currentVoxel->position[1] << " " << currentVoxel->position[2] << std::endl;
        prevVoxel = currentVoxel;
        currentVoxel = vNext;
    }
    // donna: add more control points in the end (to join)
    controlPoints.push_back(controlPoints[0]);
    controlPoints.push_back(controlPoints[1]);
    controlPoints.push_back(controlPoints[2]);

    // convert into vec3 for bspline
    vector<glm::vec3> inputPoints;
    for (int i = 0; i < (int) controlPoints.size(); i++) {
        std::cout << "cur " << controlPoints[i][0] << " " << controlPoints[i][1] << " " << controlPoints[i][2] << std::endl;
        inputPoints.push_back(glm::vec3(controlPoints[i]));
    }

    return inputPoints;
}

void Rope::writeOutRopeGen(vector<glm::vec3> inputPoints) {
    /* Writing to File */
    QString file = "/Users/donnaliu/Desktop/DMDSeniorProject-master/knots_txt_files/controlPoints.txt";
    QFile outputFile(file);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Append);

    if(!outputFile.isOpen())
    {
        //alert that file did not open
        std::cout << "FILE WRITING ERROR" << std::endl;
    }

    QTextStream outStream(&outputFile);
    QString str = "Knot Step \n";
    outStream << str;

    QString firstX;
    QString firstY;
    QString firstZ;

    for (glm::vec3 p : inputPoints) {
        QString x = QString::number(p.x);
        QString y = QString::number(p.y);
        QString z = QString::number(p.z);
        QString point = x + " " + y + " " + z + "\n";
        //std::cout << "point " << p.x << " " << p.y << " " << p.z << std::endl;
        outStream << point;
    }

    outputFile.close();

    this->controlPoints = inputPoints;
}

void Rope::writeOut(vector<glm::vec3> inputPoints) {
    /* Writing to File */
    QString file = "/Users/donnaliu/Desktop/DMDSeniorProject-master/knots_txt_files/controlPoints.txt";
    QFile outputFile(file);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Append);

    if(!outputFile.isOpen())
    {
        //alert that file did not open
        std::cout << "FILE WRITING ERROR" << std::endl;
    }

    QTextStream outStream(&outputFile);
    QString str = "Knot Step \n";
    outStream << str;
    int count = 0;

    QString firstX;
    QString firstY;
    QString firstZ;

    for (glm::vec3 p : inputPoints) {
        if (count == 3) {
            firstX = QString::number(p.x);
            firstY = QString::number(p.y);
            firstZ = QString::number(p.z);
        }
        if(count > 2) {
            QString x = QString::number(p.x);
            QString y = QString::number(p.y);
            QString z = QString::number(p.z);
            QString point = x + " " + y + " " + z + "\n";
            outStream << point;
        }
        count++;
    }

    QString point = firstX + " " + firstY + " " + firstZ + "\n";
    outStream << point;

    outputFile.close();

    this->controlPoints = inputPoints;
}


void Rope::getCurve(vector<glm::vec3> inputPoints) {

    if (inputPoints.size() == 0) {
        return;
    }

    /* De Boor's Methodology: */
    BSpline* curve = new BSpline();
    int degree = 3;
    std::vector<int> knots = {};
    vector<glm::vec4> interpNodes;

    for(double t = 0; t < 1; t += 0.001) {
        glm::vec3 tick = curve->deBoor(t, degree, inputPoints, knots);
        if (tick != glm::vec3(-1, -1, -1)) {
            interpNodes.push_back(glm::vec4(tick, 1.f));
        }
    }
    this->interpPoints = interpNodes;

    delete curve;

    /* this->interpPoints = controlPoints;
    vector<glm::vec4> interpNodes;

    for (int i = 0; i < (int) controlPoints.size(); i = i + 3) {
        Curve* curve = new BSpline();
        curve->set_steps(100); // generate 100 interpolate points between the last 4 way points
        int p1 = i;
        int p2 = (i + 1) % controlPoints.size();
        int p3 = (i + 2) % controlPoints.size();
        int p4 = (i + 3) % controlPoints.size();

        if (i == (int) controlPoints.size() - 2) {

        }

        curve->add_way_point(Vector(controlPoints[p1].x, controlPoints[p1].y, controlPoints[p1].z));
        curve->add_way_point(Vector(controlPoints[p2].x, controlPoints[p2].y, controlPoints[p2].z));
        curve->add_way_point(Vector(controlPoints[p3].x, controlPoints[p3].y, controlPoints[p3].z));
        curve->add_way_point(Vector(controlPoints[p4].x, controlPoints[p4].y, controlPoints[p4].z));


        for (int i = 0; i < curve->node_count(); ++i) {
            // std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
            glm::vec4 node = glm::vec4(curve->node(i).x, curve->node(i).y, curve->node(i).z, 1.f);
            interpNodes.push_back(node);
        }
    }
    this->controlPoints = interpNodes;*/
}

bool Rope::pushedCheck() {
    return !(!pushedXY && !pushedXZ && !pushedYZ);
}

// Simulate a timestep of the rope
void Rope::simulate() {
    // donna: stops when reaches 9 voxels
    cout << "simulate again" << endl;
    printVoxels();
    if (numVoxels == 8) {
        return;
    }

    dontDraw = false;

    bool doubledResolution = false;
    counter++;

    //// SKELETONIZE STEP
    // Keep thinning until we can't anymore
    // donna: continuously thin
    allThinned = false;
    while (!allThinned) {
        allThinned = thinAll();
    }
    cout << "thinned " << counter << endl;

    //// PUSH STEP
    // donna: pushed variable is positive if something changes
    if (pushedCheck() && pushNumber == 1) { // pushedXY &&
        pushedXY = pushXY();
        pushNumber = 2;
        cout << "pushedXY" << endl;
        if (!pushedXY) {
            simulate();
        }
    } else if (pushedCheck() && pushNumber == 2) { // pushedXZ &&
        pushedXZ = pushXZ();
        pushNumber = 3;
        cout << "pushedXZ" << endl;
        if (!pushedXZ) {
            simulate();
        }
        return;
    } else if (pushedCheck() && pushNumber == 3) { // pushedYZ &&
        cout << "going to push YZ" << endl;
        pushedYZ = pushYZ();
        pushNumber = 1;
        cout << "pushedYZ" << endl;
        if (!pushedYZ) {
            simulate();
        }
    } else if (pushNumber == 1) { // keep changing pushNumber if nothing happened
        pushNumber = 2;
        cout << "pushed1" << endl;
        if (pushedCheck()) {
            simulate();
        }
    } else if (pushNumber == 2) {
        pushNumber = 3;
        cout << "pushed2" << endl;
        if (pushedCheck()) {
            simulate();
        }
    } else if (pushNumber == 3) {
        pushNumber =  1;
        cout << "pushed3" << endl;
        if (pushedCheck()) {
            simulate();
        }
    }

    //// DOUBLE RESOLUTION STEP
    if (!pushedXY && !pushedXZ && !pushedYZ) {
        cout << "doubling res now" << endl;
        changing = false;
        doubleResolution();
        changing = true;
        pushedXY = true;
        pushedXZ = true;
        pushedYZ = true;
        doubledResolution = true;
        simulate();
    } else {
        doubledResolution = false;
    }

    // generation of bspline curve
    // donna: don't show b-spline when doubled resolution
    if (doubledResolution == false) {
        vector<glm::vec3> newControlPoints = bSplineCurver();

        //std::cout << "Number of Points: " << newControlPoints.size() << std::endl;

        vector<glm::vec3> differentPoints;
        for (glm::vec3 x : this->controlPoints) {
            if(std::find(newControlPoints.begin(), newControlPoints.end(), x) != newControlPoints.end()) {
                // contains x
            } else {
                // does not contain x
                differentPoints.push_back(x);
            }
        }

        //std::cout << "Different Points: " << differentPoints.size() << std::endl;
        for (glm::vec3 p : differentPoints) {
            glm::vec3 closest = newControlPoints[0];
            glm::vec3 secondClosest = newControlPoints[1];
            // get the 2 points in newControlPoints that is closest to each old point that has disappeared
            for (int i = 2; i < (int) newControlPoints.size(); i++) {
                glm::vec3 newP = newControlPoints[i];
                if (glm::distance(p, newP) < glm::distance(p, closest)) {
                    secondClosest = closest;
                    closest = newP;
                }
                else if (glm::distance(p, newP) < glm::distance(p, secondClosest)) {
                    secondClosest = newP;
                }
            }
        }

        getCurve(newControlPoints);
        this->controlPoints = newControlPoints;
    }

    cout << "leaving loop" << endl;
}

// ******************* XY Plane ********************** //

bool Rope::thinXY() {

	return thin(2);

}

bool Rope::pushXY() {
	return push(2);
}

// ******************* XZ Plane ********************** //

bool Rope::thinXZ() {

	return thin(1);
}

bool Rope::pushXZ() {
	return push(1);
}

// ******************* YZ Plane ********************** //
bool Rope::thinYZ() {

	return thin(0);

}

bool Rope::pushYZ() {
	return push(0);
}

// ******************* Generalized operations ********************** //

bool Rope::checkExternal() {

	for (int x = 0; x < buffX; x++) {
		for (int y = 0; y < buffY; y++) {
			for (int z = 0; z < buffZ; z++) {

				Voxel* thisVoxel = voxelBuffer[x][y][z];
				
				if (thisVoxel->density == 0) {
					continue;
                }

				vector<Voxel*> neighbors = getNeighbors(thisVoxel);

				if (neighbors.size() < 2) {
                    //thisVoxel->clearVoxel();
					return true;
				}
			
			}
		}
    }

	return false;

}

bool Rope::thin(int third) {

	int blockCounter = 0;

	int axis1, axis2;
	int dim1, dim2, dim3;
	
	// Set the axes
	if (third == 2) {
		axis1 = 0;
		axis2 = 1;
		dim1 = buffX;
		dim2 = buffY;
		dim3 = buffZ;
	} else if (third == 1) {
		axis1 = 0;
		axis2 = 2;
		dim1 = buffX;
		dim2 = buffZ;
		dim3 = buffY;
	} else {
		axis1 = 1;
		axis2 = 2;
		dim1 = buffY;
		dim2 = buffZ;
		dim3 = buffX;
	}

	Voxel* v1;
	Voxel* v2;
	Voxel* v3;
	Voxel* v4;

	for (int j = 0; j < dim2 - 1; j++) {
		for (int i = 0; i < dim1 - 1; i++) {
			for (int thirdValue = 0; thirdValue < dim3; thirdValue++) {

				//  [ ][  ][  ][ ][ ][ ][ ]
				//  [ ][v1][v2][ ][ ][ ][ ] 
				//  [ ][v3][v4][ ][ ][ ][ ]
				//  [ ][  ][  ][ ][ ][ ][ ]

				if (third == 2) {
				
					v1 = voxelBuffer[i][j][thirdValue];
					v2 = voxelBuffer[i + 1][j][thirdValue];
					v3 = voxelBuffer[i][j + 1][thirdValue];
					v4 = voxelBuffer[i + 1][j + 1][thirdValue];

				} else if (third == 1) {
				
					v1 = voxelBuffer[i][thirdValue][j];
					v2 = voxelBuffer[i + 1][thirdValue][j];
					v3 = voxelBuffer[i][thirdValue][j + 1];
					v4 = voxelBuffer[i + 1][thirdValue][j + 1];

			
				} else {

					v1 = voxelBuffer[thirdValue][i][j];
					v2 = voxelBuffer[thirdValue][i + 1][j];
					v3 = voxelBuffer[thirdValue][i][j + 1];
					v4 = voxelBuffer[thirdValue][i + 1][j + 1];

				}

				// Initialize everything in the block to be false
				v1->block = false;
				v2->block = false;
				v3->block = false;
				v4->block = false;

				// Continue if the block is not a 2x2 block
				if (v1->density != 1 || v2->density != 1 || v3->density != 1 || v4->density != 1) {
					continue;
				}

				v1->block = true;
				v2->block = true;
				v3->block = true;
				v4->block = true;

				// Potential block
				vector<Voxel*> component; 

				component.push_back(v1);
				component.push_back(v2);
				component.push_back(v3);
				component.push_back(v4);
					
				Voxel* neighbor;

				for (int i = 0; i < 4; i++) {

					Voxel* thisVoxel = component[i];

					int vX = thisVoxel->x;
					int vY = thisVoxel->y;
					int vZ = thisVoxel->z;
					
					// Neighbors not in the block
					int neighbors = 0;

					//  [  ][n2][  ][ ][ ][ ][ ]
					//  [n1][x ][n3][ ][ ][ ][ ] 
					//  [  ][n4][  ][ ][ ][ ][ ]
					//  [  ][  ][  ][ ][ ][ ][ ]

					Voxel* neighbor;

					// Left neighbor
					if (vX > 0) {
						neighbor = voxelBuffer[vX - 1][vY][vZ];
						if (neighbor->density == 1 && neighbor->block == false) {
							neighbors++;
						}
					}

					// Top neighbor
					if (vY > 0) {
						neighbor = voxelBuffer[vX][vY - 1][vZ];
						if (neighbor->density == 1 && neighbor->block == false) {
							neighbors++;
						}
					}

					// Right neighbor
					if (vX < buffX - 1) {
						neighbor = voxelBuffer[vX + 1][vY][vZ];
						if (neighbor->density == 1 && neighbor->block == false) {
							neighbors++;
						}
					}

					// Bottom neighbor
					if (vY < buffY - 1) {
						neighbor = voxelBuffer[vX][vY + 1][vZ];
						if (neighbor->density == 1 && neighbor->block == false) {
							neighbors++;
						}
					}

					// Back neighbor
					if (vZ > 0) {
						neighbor = voxelBuffer[vX][vY][vZ - 1];
						if (neighbor->density == 1 && neighbor->block == false) {
							neighbors++;
						}
					}

					// Forward neighbor
					if (vZ < buffZ - 1) {
						neighbor = voxelBuffer[vX][vY][vZ + 1];
						if (neighbor->density == 1 && neighbor->block == false) {
							neighbors++;
						}
					}

					// If the voxel has no neighbors outside of its block
					if (neighbors == 0) {
						thisVoxel->clearVoxel();
						return true;
					}
					

				} // end for the block

			}// end for thirdValue

		} // end for j

	} // end for i

	// We make it down here if there are no 2x2 blocks left to break up
	return false;

}

bool Rope::pushDir(int sign, int third) {

    bool externalsExist = checkExternal();
	while (externalsExist == true) {
		externalsExist = checkExternal();
    }

	int dim1; // axis 1 of the plane we're looking at
	int dim2; // axis 2 of the plane we're looking at
	int dim3; // axis along which we are moving
	Voxel* thisVoxel;

	// Plane is the XY plane
	if (third == 2) {
		dim1 = buffX;
		dim2 = buffY;
		dim3 = buffZ;
	// In the XZ plane
	} else if (third == 1) {
		dim1 = buffX;
		dim2 = buffZ;
		dim3 = buffY;
	// In the YZ plane
	} else {
		dim1 = buffY;
		dim2 = buffZ;
		dim3 = buffX;
	}

    if (sign > 0) {
        for (int i = 0; i < dim3; i++) { // i < dim3 - 2
			for (int j = 0; j < dim1; j++) {
				for (int k = 0; k < dim2; k++) {

					if (third == 2) {
						thisVoxel = voxelBuffer[j][k][i];
					} else if (third == 1) {
						thisVoxel = voxelBuffer[j][i][k];
					} else {
						thisVoxel = voxelBuffer[i][j][k];
					}

					connected_component.clear();

					if (thisVoxel->density == 1) {
						
                        endpoints = 0;
						fillCC(thisVoxel, third);

                        int componentSize = connected_component.size();

                        bool change = checkComponent(sign, third);
						if (change) {
                            // generation of curve TODO:: do this when not changing = false or something?
                            vector<glm::vec3> newControlPoints = bSplineCurver();
                            if (newControlPoints.size() != this->controlPoints.size()) {
                                std::cout << "2Number of Points: " << newControlPoints.size() << std::endl;
                                writeOut(newControlPoints);
                            }
							return true;
						}

					}
				}
			}
		}

    } else {
        for (int i = dim3 - 1; i > 0; i--) { // i > 1
			for (int j = 0; j < dim1; j++) {
				for (int k = 0; k < dim2; k++) {
				
					if (third == 2) {
						thisVoxel = voxelBuffer[j][k][i];
					} else if (third == 1) {
						thisVoxel = voxelBuffer[j][i][k];
					} else {
						thisVoxel = voxelBuffer[i][j][k];
					}

					connected_component.clear();

					if (thisVoxel->density == 1) {
						
						endpoints = 0;
                        fillCC(thisVoxel, third);

						bool change = checkComponent(sign, third);
						if (change) {

                            // generation of curve TODO:: do this when not changing = false or something?
                            vector<glm::vec3> newControlPoints = bSplineCurver();
                            if (newControlPoints.size() != this->controlPoints.size()) {
                                //std::cout << "1 Number of Points: " << newControlPoints.size() << std::endl;
                                writeOut(newControlPoints);
                            }
							return true;
						}

					}
				}
			}

        }

    } // end if sign is negative

	return false;

}

void Rope::fillCC(Voxel* v, int plane) {

	if (endpoints == 2) {
		return;
	}

	vector<Voxel*> neighbors = getPlanarNeighbors(v, plane);

	// If v is an endpoint
	if (neighbors.size() == 1) {
		if (endpoints == 0) {
			connected_component.insert(connected_component.begin(), v);
			//cout << "Adding an endpoint: (" << v->x << ", " << v->y << ", " << v->z << ")" << endl;
		} else {
			if (!isMember(v)) {
				connected_component.push_back(v);
				//cout << "Adding an endpoint: (" << v->x << ", " << v->y << ", " << v->z << ")" << endl;
			}
		}
		endpoints++;
		//cout << "	And now we've found " << endpoints << " endpoints of the thing" << endl;
	}

	for (int i = 0; i < neighbors.size(); i++) {
			
		Voxel* thisNeighbor = neighbors[i];

		if (!isMember(thisNeighbor)) {
            connected_component.push_back(thisNeighbor);
			//cout << "Just added (" << thisNeighbor->x << ", " << thisNeighbor->y << ", " << thisNeighbor->z << ")" << endl;
			fillCC(thisNeighbor, plane);
		}

	}
}

// Is the voxel already a member of the cc?
bool Rope::isMember(Voxel *v) {

	for (int i = 0; i < connected_component.size(); i++) {
		Voxel* thisVoxel = connected_component[i];
		if (v->my_equals(thisVoxel)) {
			return true;
		}
	}
	return false;

}

bool Rope::checkComponent(int sign, int axis) {

	if (connected_component.size() < 3) {
        return false;
	}

	Voxel* first = connected_component.at(0);
	Voxel* last  = connected_component.at(connected_component.size() - 1);

    first->setColor(1, 1, 1);
    last->setColor(1, 1, 1);

	// If we're going in the x direction
    if (axis == 0) {

        // Check the edges
        if (voxelBuffer[first->x + (sign)][first->y][first->z]->density == 0) {
            return false;
        }
        if (voxelBuffer[last->x + (sign)][last->y][last->z]->density == 0) {
            return false;
        }

        // donna: check for corner edge adjacencies

        Voxel* edgeVoxelStart = voxelBuffer[first->x][first->y][first->z];
        Voxel* edgeVoxelEnd = voxelBuffer[last->x][last->y][last->z];

        vector<Voxel*> edgeVoxels;
        edgeVoxels.push_back(edgeVoxelStart);
        edgeVoxels.push_back(edgeVoxelEnd);

        bool pushingEdgeWithDiagonal = false;

        /*
        for (int v = 0; v < 2; v++) {
            // if the rope is in the y direction
            if ((edgeVoxels[v]->y + 1 < buffY && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y + 1][edgeVoxels[v]->z]->density == 1) &&
                (edgeVoxels[v]->y - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y - 1][edgeVoxels[v]->z]->density == 1)) {
                //cout << " entered x 1" << endl;
                if (edgeVoxels[v]->z + 1 < buffZ && voxelBuffer[edgeVoxels[v]->x + 2 * (sign)][edgeVoxels[v]->y][edgeVoxels[v]->z + 1]->density == 1) {
                    //cout << "x 1" << endl;
                    return false;
                }
                if (edgeVoxels[v]->z - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x + 2 * (sign)][edgeVoxels[v]->y][edgeVoxels[v]->z - 1]->density == 1) {
                    //cout << "x 2" << endl;
                    return false;
                }
            }

            // if the rope is in the z direction
            if ((edgeVoxels[v]->z + 1 < buffZ && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y][edgeVoxels[v]->z + 1]->density == 1) &&
                (edgeVoxels[v]->z - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y][edgeVoxels[v]->z - 1]->density == 1)) {
                //cout << " entered x 2" << endl;
                if (edgeVoxels[v]->y + 1 < buffX && voxelBuffer[edgeVoxels[v]->x + 2 * (sign)][edgeVoxels[v]->y + 1][edgeVoxels[v]->z]->density == 1) {
                    //cout << "x 3" << endl;
                    return false;
                }
                if (edgeVoxels[v]->y - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x + 2 * (sign)][edgeVoxels[v]->y - 1][edgeVoxels[v]->z]->density == 1) {
                    //cout << "x 4" << endl;
                    return false;
                }
            }
        }
        */

		// Check the inside ones
        for (int i = 1; i < connected_component.size() - 1; i++) {
			
            Voxel* thisVoxel = connected_component.at(i);

            if (voxelBuffer[thisVoxel->x + (sign)][thisVoxel->y][thisVoxel->z]->density == 1) {
				return false;
			}
            if (voxelBuffer[thisVoxel->x + (2 * sign)][thisVoxel->y][thisVoxel->z]->density == 1) {
                return false;
            }

            // donna: check for the diagonal ones too

            // if the rope is in the y direction
            if ((thisVoxel->y + 1 < buffY && voxelBuffer[thisVoxel->x][thisVoxel->y + 1][thisVoxel->z]->density == 1) &&
                (thisVoxel->y - 1 >= 0 && voxelBuffer[thisVoxel->x][thisVoxel->y - 1][thisVoxel->z]->density == 1)) {
                //cout << " entered x 1" << endl;
                if (thisVoxel->z + 1 < buffZ && voxelBuffer[thisVoxel->x + 2 * (sign)][thisVoxel->y][thisVoxel->z + 1]->density == 1) {
                    //cout << "x 1" << endl;
                    return false;
                }
                if (thisVoxel->z - 1 >= 0 && voxelBuffer[thisVoxel->x + 2 * (sign)][thisVoxel->y][thisVoxel->z - 1]->density == 1) {
                    //cout << "x 2" << endl;
                    return false;
                }
            }

            // if the rope is in the z direction
            if ((thisVoxel->z + 1 < buffZ && voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z + 1]->density == 1) &&
                (thisVoxel->z - 1 >= 0 && voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z - 1]->density == 1)) {
                //cout << " entered x 2" << endl;
                if (thisVoxel->y + 1 < buffX && voxelBuffer[thisVoxel->x + 2 * (sign)][thisVoxel->y + 1][thisVoxel->z]->density == 1) {
                    //cout << "x 3" << endl;
                    return false;
                }
                if (thisVoxel->y - 1 >= 0 && voxelBuffer[thisVoxel->x + 2 * (sign)][thisVoxel->y - 1][thisVoxel->z]->density == 1) {
                    //cout << "x 4" << endl;
                    return false;
                }
            }
        }

        // check the middle values y and z both change
        if (connected_component.size() > 4) {
            cout << "entering 0 connected_component size > 4" << endl;
            for (int i = 2; i < connected_component.size() - 2; i++) {
                Voxel* thisVoxel = connected_component.at(i);

                if ((thisVoxel->y + 1 < buffY) && (thisVoxel->z + 1 < buffZ)) {
                    if (voxelBuffer[thisVoxel->x + (2 * sign)][thisVoxel->y + 1][thisVoxel->z + 1]->density == 1) {
                        return false;
                    }
                }

                if ((thisVoxel->y + 1 < buffY) && (thisVoxel->z - 1 >= 0)) {
                    if (voxelBuffer[thisVoxel->x + (2 * sign)][thisVoxel->y + 1][thisVoxel->z - 1]->density == 1) {
                        return false;
                    }
                }

                if ((thisVoxel->y - 1 >= 0) && (thisVoxel->z + 1 < buffZ)) {
                    if (voxelBuffer[thisVoxel->x + (2 * sign)][thisVoxel->y - 1][thisVoxel->z + 1]->density == 1) {
                        return false;
                    }
                }

//                if ((thisVoxel->y - 1 >= 0) && (thisVoxel->z - 1 >= 0)) {
//                    if (voxelBuffer[thisVoxel->x + (2 * sign)][thisVoxel->y - 1][thisVoxel->z - 1]->density == 1) {
//                        thisVoxel->setColor(0.1, 0.1, 0.1);
//                        return false;
//                    }
//                 }
            }
        }

		// If we've made it this far, we can push it
		for (int i = 0; i < connected_component.size(); i++) {

            Voxel* thisVoxel = connected_component.at(i);

            voxelBuffer[thisVoxel->x + sign][thisVoxel->y][thisVoxel->z]->density = 1;
            voxelBuffer[thisVoxel->x + sign][thisVoxel->y][thisVoxel->z]->node->hasGeometry = true;

			thisVoxel->clearVoxel();

			changing = true;

		}

		return true;

	// If we're going in the y direction
    } else if (axis == 1) {

		// Check the edges
        if (voxelBuffer[first->x][first->y + sign][first->z]->density == 0) {
            return false;
        }
        if (voxelBuffer[last->x][last->y + sign][last->z]->density == 0) {
            return false;
        }

        // donna: check for corner edge adjacencies
        Voxel* edgeVoxelStart = voxelBuffer[first->x][first->y][first->z];
        Voxel* edgeVoxelEnd = voxelBuffer[last->x][last->y][last->z];

        vector<Voxel*> edgeVoxels;
        edgeVoxels.push_back(edgeVoxelStart);
        edgeVoxels.push_back(edgeVoxelEnd);

        /*
        for (int v = 0; v < 2; v++) {
            // if the rope is in the x direction
            if ((edgeVoxels[v]->x + 1 < buffX && voxelBuffer[edgeVoxels[v]->x + 1][edgeVoxels[v]->y][edgeVoxels[v]->z]->density == 1) ||
                (edgeVoxels[v]->x - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x - 1][edgeVoxels[v]->y][edgeVoxels[v]->z]->density == 1)) {
                //cout << " AXIS 1 - ENTERED 1.0 " << endl;
                if (edgeVoxels[v]->z + 1 < buffZ && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y + 2 * (sign)][edgeVoxels[v]->z + 1]->density == 1) {
                    //cout << "y 1" << endl;
                    return false;
                }
                if (edgeVoxels[v]->z - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y + 2 * (sign)][edgeVoxels[v]->z - 1]->density == 1) {
                    //cout << "y 2" << endl;
                    return false;
                }
            }

            // if the rope is in the z direction
            if ((edgeVoxels[v]->z + 1 < buffZ && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y][edgeVoxels[v]->z + 1]->density == 1) ||
                (edgeVoxels[v]->z - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y][edgeVoxels[v]->z - 1]->density == 1)) {
                //cout << " AXIS 1 - ENTERED 2.0 " << edgeVoxels[v]->x << " " << buffX << endl;
                if (edgeVoxels[v]->x + 1 < buffX && voxelBuffer[edgeVoxels[v]->x + 1][edgeVoxels[v]->y + 2 * (sign)][edgeVoxels[v]->z]->density == 1) {
                    //cout << "y 3" << endl;
                    return false;
                }
                if (edgeVoxels[v]->x - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x - 1][edgeVoxels[v]->y + 2 * (sign)][edgeVoxels[v]->z]->density == 1) {
                    //cout << "y 4" << endl;
                    return false;
                }
            }
        }
        */

		// Check the inside ones
        for (int i = 1; i < connected_component.size() - 1; i++) {
			
			Voxel* thisVoxel = connected_component.at(i);

            if (voxelBuffer[thisVoxel->x][thisVoxel->y + sign][thisVoxel->z]->density == 1) {
                return false;
            }
            if (voxelBuffer[thisVoxel->x][thisVoxel->y + (2 * sign)][thisVoxel->z]->density == 1) {
                return false;
            }

            // donna: check for edge adjacencies on the components

            // if the rope is in the x direction
            if ((thisVoxel->x + 1 < buffX && voxelBuffer[thisVoxel->x + 1][thisVoxel->y][thisVoxel->z]->density == 1) ||
                (thisVoxel->x - 1 >= 0 && voxelBuffer[thisVoxel->x - 1][thisVoxel->y][thisVoxel->z]->density == 1)) {
                //cout << " AXIS 1 - ENTERED 1.0 " << endl;
                if (thisVoxel->z + 1 < buffZ && voxelBuffer[thisVoxel->x][thisVoxel->y + 2 * (sign)][thisVoxel->z + 1]->density == 1) {
                    //cout << "y 1" << endl;
                    return false;
                }
                if (thisVoxel->z - 1 >= 0 && voxelBuffer[thisVoxel->x][thisVoxel->y + 2 * (sign)][thisVoxel->z - 1]->density == 1) {
                    //cout << "y 2" << endl;
                    return false;
                }
            }

            // if the rope is in the z direction
            if ((thisVoxel->z + 1 < buffZ && voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z + 1]->density == 1) ||
                (thisVoxel->z - 1 >= 0 && voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z - 1]->density == 1)) {
                //cout << " AXIS 1 - ENTERED 2.0 " << thisVoxel->x << " " << buffX << endl;
                if (thisVoxel->x + 1 < buffX && voxelBuffer[thisVoxel->x + 1][thisVoxel->y + 2 * (sign)][thisVoxel->z]->density == 1) {
                    //cout << "y 3" << endl;
                    return false;
                }
                if (thisVoxel->x - 1 >= 0 && voxelBuffer[thisVoxel->x - 1][thisVoxel->y + 2 * (sign)][thisVoxel->z]->density == 1) {
                    //cout << "y 4" << endl;
                    return false;
                }
            }
        }

        // check the middle values y and z both change
        if (connected_component.size() > 4) {
            cout << "entering 1 connected_component size > 4" << endl;
            for (int i = 2; i < connected_component.size() - 2; i++) {
                Voxel* thisVoxel = connected_component.at(i);

                if (thisVoxel->x + 1 < buffX && thisVoxel->z + 1 < buffZ) {
                    if (voxelBuffer[thisVoxel->x + 1][thisVoxel->y + (2 * sign)][thisVoxel->z + 1]->density == 1) {
                        return false;
                    }
                }

                if (thisVoxel->x + 1 < buffX && thisVoxel->z - 1 >= 0) {
                    if (voxelBuffer[thisVoxel->x + 1][thisVoxel->y + (2 * sign)][thisVoxel->z - 1]->density == 1) {
                        return false;
                    }
                }

                if (thisVoxel->x - 1 >= 0 && thisVoxel->z + 1 < buffZ) {
                    if (voxelBuffer[thisVoxel->x - 1][thisVoxel->y + (2 * sign)][thisVoxel->z + 1]->density == 1) {
                        return false;
                    }
                }

                if (thisVoxel->x - 1 >= 0 && thisVoxel->z - 1 >= 0) {
                    if (voxelBuffer[thisVoxel->x - 1][thisVoxel->y + (2 * sign)][thisVoxel->z - 1]->density == 1) {
                        return false;
                    }
                }
            }
        }

		// If we've made it this far, we can push it
		for (int i = 0; i < connected_component.size(); i++) {

            Voxel* thisVoxel = connected_component.at(i);

            voxelBuffer[thisVoxel->x][thisVoxel->y + sign][thisVoxel->z]->density = 1;
            voxelBuffer[thisVoxel->x][thisVoxel->y + sign][thisVoxel->z]->node->hasGeometry = true;

            thisVoxel->clearVoxel();

            changing = true;

        }

		return true;

    // If we're going in the z direction
    } else if (axis == 2) {

		// Check the edges
        if (voxelBuffer[first->x][first->y][first->z + sign]->density == 0) {
            return false;
        }
        if (voxelBuffer[last->x][last->y][last->z + sign]->density == 0) {
            return false;
        }

        // donna: check for corner edge adjacencies

        Voxel* edgeVoxelStart = voxelBuffer[first->x][first->y][first->z];
        Voxel* edgeVoxelEnd = voxelBuffer[last->x][last->y][last->z];

        vector<Voxel*> edgeVoxels;
        edgeVoxels.push_back(edgeVoxelStart);
        edgeVoxels.push_back(edgeVoxelEnd);

        /*
        for (int v = 0; v < 2; v++) {
            // if the rope is in the x direction
            if ((edgeVoxels[v]->x + 1 < buffX && voxelBuffer[edgeVoxels[v]->x + 1][edgeVoxels[v]->y][edgeVoxels[v]->z]->density == 1) ||
                (edgeVoxels[v]->x - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x - 1][edgeVoxels[v]->y][edgeVoxels[v]->z]->density == 1)) {
                //cout << " enteredz 1 " << edgeVoxels[v]->z << " " << buffZ << " y: "<< edgeVoxels[v]->y - 1 << " " << buffY << endl;
                if (edgeVoxels[v]->y + 1 < buffY && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y + 1][edgeVoxels[v]->z + 2 * (sign)]->density == 1) {
                    //cout << "z 1" << endl;
                    return false;
                }
                if (edgeVoxels[v]->y - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y - 1][edgeVoxels[v]->z + 2 * (sign)]->density == 1) {
                    //cout << "z 2" << endl;
                    return false;
                }
            }

            // if the rope is in the y direction
            if ((edgeVoxels[v]->y + 1 < buffY && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y + 1][edgeVoxels[v]->z]->density == 1) ||
                (edgeVoxels[v]->y - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x][edgeVoxels[v]->y - 1][edgeVoxels[v]->z]->density == 1)) {
                //cout << " enteredz 2" << endl;
                if (edgeVoxels[v]->x + 1 < buffX && voxelBuffer[edgeVoxels[v]->x + 1][edgeVoxels[v]->y][edgeVoxels[v]->z + 2 * (sign)]->density == 1) {
                    //cout << "z 3" << endl;
                    return false;
                }
                if (edgeVoxels[v]->x - 1 >= 0 && voxelBuffer[edgeVoxels[v]->x - 1][edgeVoxels[v]->y][edgeVoxels[v]->z + 2 * (sign)]->density == 1) {
                    //cout << "z 4" << endl;
                    return false;
                }
            }
        }
        */

		// Check the inside ones
        for (int i = 1; i < connected_component.size() - 1; i++) {
			
			Voxel* thisVoxel = connected_component.at(i);

			if (voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z + sign]->density == 1) {
				return false;
			}
			if (voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z + (sign * 2)]->density == 1) {
				return false;
            }

            // donna: check for the diagonal ones too

            // if the rope is in the x direction
            if ((thisVoxel->x + 1 < buffX && voxelBuffer[thisVoxel->x + 1][thisVoxel->y][thisVoxel->z]->density == 1) ||
                (thisVoxel->x - 1 >= 0 && voxelBuffer[thisVoxel->x - 1][thisVoxel->y][thisVoxel->z]->density == 1)) {
                //cout << " enteredz 1 " << thisVoxel->z << " " << buffZ << " y: "<< thisVoxel->y - 1 << " " << buffY << endl;
                if (thisVoxel->y + 1 < buffY && voxelBuffer[thisVoxel->x][thisVoxel->y + 1][thisVoxel->z + 2 * (sign)]->density == 1) {
                    //cout << "z 1" << endl;
                    return false;
                }
                if (thisVoxel->y - 1 >= 0 && voxelBuffer[thisVoxel->x][thisVoxel->y - 1][thisVoxel->z + 2 * (sign)]->density == 1) {
                    //cout << "z 2" << endl;
                    return false;
                }
            }

            // if the rope is in the y direction
            if ((thisVoxel->y + 1 < buffY && voxelBuffer[thisVoxel->x][thisVoxel->y + 1][thisVoxel->z]->density == 1) ||
                (thisVoxel->y - 1 >= 0 && voxelBuffer[thisVoxel->x][thisVoxel->y - 1][thisVoxel->z]->density == 1)) {
                //cout << " enteredz 2" << endl;
                if (thisVoxel->x + 1 < buffX && voxelBuffer[thisVoxel->x + 1][thisVoxel->y][thisVoxel->z + 2 * (sign)]->density == 1) {
                    //cout << "z 3" << endl;
                    return false;
                }
                if (thisVoxel->x - 1 >= 0 && voxelBuffer[thisVoxel->x - 1][thisVoxel->y][thisVoxel->z + 2 * (sign)]->density == 1) {
                    //cout << "z 4" << endl;
                    return false;
                }
            }
		}

        // check the middle values y and z both change
        if (connected_component.size() > 4) {
            cout << "entering 2 connected_component size > 4" << endl;
            for (int i = 2; i < connected_component.size() - 2; i++) {
                Voxel* thisVoxel = connected_component.at(i);

                if (thisVoxel->x + 1 < buffX && thisVoxel->y + 1 < buffY) {
                    if (voxelBuffer[thisVoxel->x + 1][thisVoxel->y + 1][thisVoxel->z + (2 * sign)]->density == 1) {
                        return false;
                    }
                }

                if (thisVoxel->x + 1 < buffX && thisVoxel->y - 1 >=0) {
                    if (voxelBuffer[thisVoxel->x + 1][thisVoxel->y - 1][thisVoxel->z + (2 * sign)]->density == 1) {
                        return false;
                    }
                }

                if (thisVoxel->x - 1 >= 0 && thisVoxel->y + 1 < buffY) {
                    if (voxelBuffer[thisVoxel->x - 1][thisVoxel->y + 1][thisVoxel->z + (2 * sign)]->density == 1) {
                        return false;
                    }
                }

                if (thisVoxel->x - 1 >= 0 && thisVoxel->y - 1 >= 0) {
                    if (voxelBuffer[thisVoxel->x - 1][thisVoxel->y - 1][thisVoxel->z + (2 * sign)]->density == 1) {
                        return false;
                    }
                }
            }
        }


		// If we've made it this far, we can push it
        //cout << "did i make it?" << endl;
		for (int i = 0; i < connected_component.size(); i++) {
			Voxel* thisVoxel = connected_component.at(i);

			voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z + sign]->density = 1;
            voxelBuffer[thisVoxel->x][thisVoxel->y][thisVoxel->z + sign]->node->hasGeometry = true;

			thisVoxel->clearVoxel();

			changing = true;

        }

		return true;

	}

    return false;
}

bool Rope::push(int third) {
    // donna: fix pushing order + push all the way in on both sides
    /*if (third == 0) {
        if (!pushDirectionX) {
            pushDirectionX = true;
            if (pushDir(1, third)) {
                return true;
            } else {
                if (push(third)) {
                    return true;
                }
            }
        } else {
            pushDirectionX = false;
            if (pushDir(-1, third)) {
                return true;
            } else {
                if (push(third)) {
                    return true;
                }
            }
        }
    } else if (third == 1) {
        if (!pushDirectionY) {
            pushDirectionY = true;
            if (pushDir(1, third)) {
                return true;
            } else {
                if (push(third)) {
                    return true;
                }
            }
        } else {
            pushDirectionY = false;
            if (pushDir(-1, third)) {
                return true;
            } else {
                if (push(third)) {
                    return true;
                }
            }
        }
    } else if (third == 1) {
        if (!pushDirectionZ) {
            pushDirectionZ = true;
            if (pushDir(1, third)) {
                return true;
            } else {
                if (push(third)) {
                    return true;
                }
            }
        } else {
            pushDirectionZ = false;
            if (pushDir(-1, third)) {
                return true;
            } else {
                if (push(third)) {
                    return true;
                }
            }
        }
    }*/

    if (pushDir(1, third)) {
        pushDir(-1, third);
        return true;
    }

    if (pushDir(-1, third)) {
        pushDir(1, third);
        return true;
    }

	return false;
}

void Rope::setup() {

	// Assume it isn't thinned
	thinnedXY = false;
	thinnedXZ = false;
	thinnedYZ = false;

    root->children.clear();
    root->hasGeometry = false;

	root->setXTrans(-1.5);
	root->setYTrans(2);
	root->transform();

    int count = 0;

	// Set up the spheres in the voxel buffer
    for (int z = buffZ - 1; z >= 0; z--) {
		for (int y = 0; y < buffY; y++) {
            for (int x = buffX - 1; x >= 0; x--) {

				Voxel* voxel = voxelBuffer[x][y][z];
				voxel->block = false;

				float vX = sX * (x);
				float vY = sY * -(y);
				float vZ = sZ * (z);

				voxel->position = glm::vec4(vX, vY, vZ, 1.0f);

				Node* cubeNode = new Node();

                //Sphere* cube = new Sphere;
                Cube* cube = new Cube();

                cube->geoColor = glm::vec3((double) rand() / RAND_MAX,(double) rand() / RAND_MAX,(double) rand() / RAND_MAX);
                //cube->geoColor = glm::vec3(1,1,1);

				cube->refreshColor();
                cubeNode->geometry = cube;

                // show b-spline only
                //if (count == 0) {
                    cubeNode->hasGeometry = voxel->density;
                    count++;
                //}

				cubeNode->setXScale(sX);
				cubeNode->setYScale(sY);
				cubeNode->setZScale(sZ);

				cubeNode->setXTrans(vX);
				cubeNode->setYTrans(vY);
				cubeNode->setZTrans(vZ);

				cubeNode->transform();

				root->addChild(cubeNode);
				cubeNode->parent = root;

                voxel->node = cubeNode;

			}
		}
	}
}

vector<Voxel*> Rope::getNeighbors(Voxel* v) {

	int vX = v->x;
	int vY = v->y;
	int vZ = v->z;

	vector<Voxel*> neighbors;
	Voxel* neighbor;

	// Left neighbor
	if (vX > 0) {
		neighbor = voxelBuffer[vX - 1][vY][vZ];
		if (neighbor->density == 1) {
			neighbors.push_back(neighbor);
		}
	}

	// Top neighbor
	if (vY > 0) {
		neighbor = voxelBuffer[vX][vY - 1][vZ];
		if (neighbor->density == 1) {
			neighbors.push_back(neighbor);
		}
	}

	// Right neighbor
	if (vX < buffX - 1) {
		neighbor = voxelBuffer[vX + 1][vY][vZ];
		if (neighbor->density == 1) {
			neighbors.push_back(neighbor);
		}
	}

	// Bottom neighbor
	if (vY < buffY - 1) {
		neighbor = voxelBuffer[vX][vY + 1][vZ];
		if (neighbor->density == 1) {
			neighbors.push_back(neighbor);
		}
	}

	// Back neighbor
	if (vZ > 0) {
		neighbor = voxelBuffer[vX][vY][vZ - 1];
		if (neighbor->density == 1) {
			neighbors.push_back(neighbor);
		}
	}

	// Forward neighbor
	if (vZ < buffZ - 1) {
		neighbor = voxelBuffer[vX][vY][vZ + 1];
		if (neighbor->density == 1) {
			neighbors.push_back(neighbor);
		}
	}

	return neighbors;

}

// donna: edge & vertex neighbors
vector<Voxel*> Rope::getEdgeAndVertexNeighbors(Voxel *v) {

    int vX = v->x;
    int vY = v->y;
    int vZ = v->z;

    vector<Voxel*> neighbors;
    Voxel* neighbor;

    // both are positive
    if (vX + 1 < buffX && vY + 1 < buffY) {
        neighbor = voxelBuffer[vX + 1][vY + 1][vZ];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vX + 1 < buffX && vZ + 1 < buffZ) {
        neighbor = voxelBuffer[vX + 1][vY][vZ + 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vY + 1 < buffY && vZ + 1 < buffZ) {
        neighbor = voxelBuffer[vX][vY + 1][vZ + 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    // both are negative
    if (vX - 1 >= 0 && vY - 1 >= 0) {
        neighbor = voxelBuffer[vX - 1][vY - 1][vZ];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vX - 1 >= 0 && vZ - 1 >= 0) {
        neighbor = voxelBuffer[vX - 1][vY][vZ - 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vY - 1 >= 0 && vZ - 1 >= 0) {
        neighbor = voxelBuffer[vX][vY - 1][vZ - 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    // only first is positive
    if (vX + 1 < buffX && vY - 1 >= 0) {
        neighbor = voxelBuffer[vX + 1][vY - 1][vZ];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vX + 1 < buffX && vZ - 1 >= 0) {
        neighbor = voxelBuffer[vX + 1][vY][vZ - 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vY + 1 < buffY && vZ - 1 >= 0) {
        neighbor = voxelBuffer[vX][vY + 1][vZ - 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    // only first is negative
    if (vX - 1 >= 0 && vY + 1 < buffY) {
        neighbor = voxelBuffer[vX - 1][vY + 1][vZ];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vX - 1 >= 0 && vZ + 1 < buffZ) {
        neighbor = voxelBuffer[vX - 1][vY][vZ + 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    if (vY - 1 >= 0 && vZ + 1 < buffZ) {
        neighbor = voxelBuffer[vX][vY - 1][vZ + 1];
        if (neighbor->density == 1) {
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
}

vector<Voxel*> Rope::getPlanarNeighbors(Voxel* v, int plane) {
	
	// plane = 0 --> yz plane
	// plane = 1 --> xz plane
	// plane = 2 --> xy plane

	int vX = v->x;
	int vY = v->y;
	int vZ = v->z;

	vector<Voxel*> neighbors;
	Voxel* neighbor;

	if (plane != 0) {

		// Left neighbor
		if (vX > 0) {
			neighbor = voxelBuffer[vX - 1][vY][vZ];
			if (neighbor->density == 1) {
				neighbors.push_back(neighbor);
			}
		}

		// Right neighbor
		if (vX < buffX - 1) {
			neighbor = voxelBuffer[vX + 1][vY][vZ];
			if (neighbor->density == 1) {
				neighbors.push_back(neighbor);
			}
		}

	}

	if (plane != 1) {

		// Top neighbor
		if (vY > 0) {
			neighbor = voxelBuffer[vX][vY - 1][vZ];
			if (neighbor->density == 1) {
				neighbors.push_back(neighbor);
			}
		}

		// Bottom neighbor
		if (vY < buffY - 1) {
			neighbor = voxelBuffer[vX][vY + 1][vZ];
			if (neighbor->density == 1) {
				neighbors.push_back(neighbor);
			}
		}

	}

	if (plane != 2) {

		// Back neighbor
		if (vZ > 0) {
			neighbor = voxelBuffer[vX][vY][vZ - 1];
			if (neighbor->density == 1) {
				neighbors.push_back(neighbor);
			}
		}

		// Forward neighbor
		if (vZ < buffZ - 1) {
			neighbor = voxelBuffer[vX][vY][vZ + 1];
			if (neighbor->density == 1) {
				neighbors.push_back(neighbor);
			}
		}

	}

	return neighbors;
}

bool Rope::thinAll() {

    // Thin in xy until we can't anymore
    if (!thinnedXY) {
        thinnedXY = !(thinXY());
    }
	
    // If we can thin in XZ, we need to set thinnedXY = false again
    if (!thinnedXZ) {
        if (thinXZ()) {
            thinnedXY = false;
        } else {
            thinnedXZ = true;
        }
    }

    // If we can thin in YZ, we need to set thinnedXY and thinnedXZ = false again
    if (!thinnedYZ) {
        if (thinYZ()) {
            thinnedXY = false;
            thinnedXZ = false;
        } else {
            thinnedYZ = true;
        }
    }

	// If we're all thinned, pick off the extra ones
    if (thinnedXY && thinnedXZ && thinnedYZ) {

		// Look for solo voxels *first*
		for (int x = 0; x < buffX; x++) {
			for (int y = 0; y < buffY; y++) {
				for (int z = 0; z < buffZ; z++) {
					Voxel* thisVoxel = voxelBuffer[x][y][z];

                    if (thisVoxel->density == 1) {
                        if (getNeighbors(thisVoxel).size() <= 1) {
                            thisVoxel->clearVoxel();
                            return false;
                        }
                    }
				}
			}
		}

        //cout << "Finished one pass of pinching all the solos off!" << endl;

		// Make sure everything only has 2 neighbors
		for (int x = 0; x < buffX; x++) {
			for (int y = 0; y < buffY; y++) {
				for (int z = 0; z < buffZ; z++) {

                    Voxel* thisVoxel = voxelBuffer[x][y][z];

                    if (thisVoxel->density == 1) {
                        vector<Voxel*> neighbors = getNeighbors(thisVoxel);
                        int neighborsCount = neighbors.size();

                        if (neighborsCount > 2) {

                            for (int i = 0; i < neighbors.size(); i++) {

                                Voxel* thisNeighbor = neighbors[i];

                                vector<Voxel*> neighborNeighbors = getNeighbors(thisNeighbor);

                                for (int j = 0; j < neighborNeighbors.size(); j++) {

                                    Voxel* thisNeighborNeighbor = neighborNeighbors[j];

                                    // If we haven't seen this voxel before, look at its neighbors
                                    if (!thisNeighborNeighbor->my_equals(thisVoxel)) {

                                        // If we haven't seen this neighbor before
                                        vector<Voxel*> thirdNeighbors = getNeighbors(thisNeighborNeighbor);

                                        if (thirdNeighbors.size() > 2) {
                                            cout << "2nd neighbor has 3 neighbors, so we're deleting 1st neighbor" << endl;
                                            thisNeighbor->clearVoxel();
                                            cout << "returning false because more than 2 neighbors" << endl;
                                            return false;
                                        }

                                        for (int k = 0; k < thirdNeighbors.size(); k++) {

                                            Voxel* third = thirdNeighbors[k];

                                            if (!third->my_equals(thisNeighbor) && !third->my_equals(thisNeighborNeighbor)) {

                                                vector<Voxel*> fourthNeighbors = getNeighbors(third);

                                                if (fourthNeighbors.size() > 2) {
                                                    cout << "3rd neighbor has 3 neighbors, so we're deleting 1st and 2nd neighbor" << endl;
                                                    thisNeighbor->clearVoxel();
                                                    thisNeighborNeighbor->clearVoxel();
                                                    return false;

                                                } // end if fourthNeighbors.size > 2

                                                for (int m = 0; m < fourthNeighbors.size(); m++) {

                                                    Voxel* fourth = fourthNeighbors[m];

                                                    if (!fourth->my_equals(thisVoxel) && !fourth->my_equals(thisNeighbor) && !fourth->my_equals(thisNeighborNeighbor)) {

                                                        vector<Voxel*> fifthNeighbors = getNeighbors(fourth);

                                                        if (fifthNeighbors.size() > 2) {
                                                            cout << "4th neighbor has 3 neighbors, so we're deleting 1st, 2nd and third neighbors" << endl;
                                                            thisNeighbor->clearVoxel();
                                                            thisNeighborNeighbor->clearVoxel();
                                                            third->clearVoxel();
                                                            return false;

                                                        } // end if fourthNeighbors.size > 2

                                                    }
                                                }

                                            } // end if third != thisNeighbor and third != thisNeighborNeighbor
                                        } // end for neighbors of thisNeighborNeighbor
                                    } // end if thisNeighborNeighbor != thisVoxel
                                } // end for neighbors of thisNeighbor
                            } // end for neighbors of thisVoxel

                            //return true;

                        } else {
                            //thisVoxel->setColor(0.2, 0.6, 1);
                        }
                    } //*/

				} // end for z
			} // end for y
		} // end for x
    }  else {
		// There's still some thinning to do
        return false;
	}
	
	// end if all 3 thinned bools are true
	// Make sure everything is 2-connected
    for (int x = 0; x < buffX; x++) {
        for (int y = 0; y < buffY; y++) {
            for (int z = 0; z < buffZ; z++) {

                Voxel* thisVoxel = voxelBuffer[x][y][z];

                if (thisVoxel->density == 1) {
                    vector<Voxel*> neighbors = getNeighbors(thisVoxel);

                    int neighborsCount = neighbors.size();

                    if (neighborsCount != 2) {
                        cout << "### We made it all the way here and there's still something wrong!! ###" << endl;
                        cout << "	Dis dude has " << neighborsCount << " neighbors fam" << endl;

                        thisVoxel->setColor(0, 0, 0);

                        // donna: clear any diagonal neighbors at this point
                        vector<Voxel*> edgeAndVertexNeighbors = getEdgeAndVertexNeighbors(thisVoxel);
                        /*for (int ev = 0; ev < edgeAndVertexNeighbors.size(); ev++) {
                            Voxel* thisEVVoxel = edgeAndVertexNeighbors[ev];
                            cout << "iterating through neighbor voxel" << endl;
                            thisEVVoxel->setColor(glm::vec3((double) rand() / RAND_MAX,(double) rand() / RAND_MAX,(double) rand() / RAND_MAX));
                            thisEVVoxel->clearVoxel();
                        }*/

                        int numEVs = edgeAndVertexNeighbors.size() - 1;

                        if (edgeAndVertexNeighbors.size() >= 1) {
                            edgeAndVertexNeighbors[numEVs]->clearVoxel();
                        }

                        dontDraw = true;
                        return false;
                    }
                }
            }
        }
    }

    // donna: remove edge & vertex neighbors
    for (int x = 0; x < buffX; x++) {
        for (int y = 0; y < buffY; y++) {
            for (int z = 0; z < buffZ; z++) {

                Voxel* thisVoxel = voxelBuffer[x][y][z];

                vector<Voxel*> edgeAndVertexNeighbors = getEdgeAndVertexNeighbors(thisVoxel);
                if (edgeAndVertexNeighbors.size() == 1) {
                    //thisVoxel->clearVoxel();
                }
            }
        }
    }

    //cout << "All thinned!" << endl;
	return true;
}

// Double the resolution of the buffer
void Rope::doubleResolution() {
    cout << "doubling res actually " << buffZ << " " << buffY << " " << buffX << endl;
	// First we find the min and max extent of the rope in every direction, so we only expand the buffer to we have to
    int minX_extent = INT_MAX;
    int minY_extent = INT_MAX;
    int minZ_extent = INT_MAX;

	int maxX_extent = 0;
	int maxY_extent = 0;
	int maxZ_extent = 0;

	for (int z = 0; z < buffZ; z++) {
		for (int y = 0; y < buffY; y++) {
			for (int x = 0; x < buffX; x++) {
				
                Voxel* thisVoxel = voxelBuffer[x][y][z];
                //cout << x << " " << y << " " << z << endl;

                if (thisVoxel->density == 1) {
                    thisVoxel->setColor(1, 1, 0);

                    if (x < minX_extent) {
                        minX_extent = x;
                        cout << "minXExtent " << minX_extent << endl;
                    }
                    if (y < minY_extent) {
                        minY_extent = y;
                    }
                    if (z < minZ_extent) {
                        minZ_extent = z;
                    }

					maxX_extent = max(maxX_extent, x);
					maxY_extent = max(maxY_extent, y);
					maxZ_extent = max(maxZ_extent, z);
				}
					
			}
		}
	}

    cout << "dr check1 " << minX_extent << " " << maxX_extent << endl;

    int new_buffX = (maxX_extent - minX_extent + 1) * 2;
    int new_buffY = (maxY_extent - minY_extent + 1) * 2;
    int new_buffZ = (maxZ_extent - minZ_extent + 1) * 2;

    cout << "dr check2" << " " << (maxZ_extent - minZ_extent + 1) * 2 << " " << new_buffY << " " << (maxX_extent - minX_extent + 1) * 2 << endl;

    Voxel ****newBuffer = new Voxel***[new_buffX];
	for (int k = 0; k < new_buffX; k++) {
		newBuffer[k] = new Voxel**[new_buffY];
	}

	for (int k = 0; k < new_buffX; k++) {
		for (int w = 0; w < new_buffY; w++) {
			newBuffer[k][w] = new Voxel*[new_buffZ];
		}
	}

    cout << "dr check2.5" << endl;

	// Fill in the voxels
	for (int z = 0; z < new_buffZ; z++) {
		for (int y = 0; y < new_buffY; y++) {
			for (int x = 0; x < new_buffX; x++) {
			
				// The corresponding voxel in the original buffer
				int xCoord = minX_extent + (x/2);
				int yCoord = minY_extent + (y/2);
				int zCoord = minZ_extent + (z/2);
				
				Voxel* corresp = voxelBuffer[xCoord][yCoord][zCoord];
                Voxel* voxel = new Voxel(corresp->density);
				voxel->x = x;
				voxel->y = y;
				voxel->z = z;

				newBuffer[x][y][z] = voxel;
			}

		}
	}

    cout << "dr check3" << endl;

	for (int z = 0; z < buffZ; z++) {
		for (int y = 0; y < buffY; y++) {
			for (int x = 0; x < buffX; x++) {
				delete voxelBuffer[x][y][z];	
			}
		}
	}

	// Swap the buffers & run setup
    cout << "got to new buffer" << endl;
	voxelBuffer = newBuffer;

	buffX = new_buffX;
    buffY = new_buffY;
	buffZ = new_buffZ;

	setup();
	thinnedXY = false;
	thinnedXZ = false;
	thinnedYZ = false;
	allThinned = false;
}
