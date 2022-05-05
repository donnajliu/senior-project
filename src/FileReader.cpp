#include "FileReader.h"
#include <QFile>
#include <QString>
#include <QTextStream>

static const float PI = 3.141592653589f;

FileReader::FileReader(string f)
    : fileName(f), nodes(), rope(nullptr)
{}

static void printVector(glm::vec3 v) {
	cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << endl;
}

bool FileReader::read() {
    QString text;
    QFile file(fileName.c_str());
    QString word;
    if (file.open(QFile::ReadOnly))
    {
        QTextStream myfile(&file);
        while (!myfile.atEnd()) {
            myfile >> word;

            if (word.compare("CAMERA") == 0) {
                myfile >> word; //RESO label
                myfile >> word; // RESO width
                RESOwidth = stof(word.toStdString());

                myfile >> word; // RESO height
                RESOheight = stof(word.toStdString());

                RESOwidth = 1280;
                RESOheight = 720;

                myfile >> word; // EYEP label
                myfile >> word;
                float eyeX = stof(word.toStdString());
                myfile >> word;
                float eyeY = stof(word.toStdString());
                myfile >> word;
                float eyeZ = stof(word.toStdString());

                EYEP = glm::vec3(eyeX, eyeY, eyeZ);

                myfile >> word; // VDIR label
                myfile >> word;
                float vX = stof(word.toStdString());
                myfile >> word;
                float vY = stof(word.toStdString());
                myfile >> word;
                float vZ = stof(word.toStdString());

                VDIR = glm::vec3(vX, vY, vZ);

                myfile >> word; // UVEC label
                myfile >> word;
                float uX = stof(word.toStdString());
                myfile >> word;
                float uY = stof(word.toStdString());
                myfile >> word;
                float uZ = stof(word.toStdString());

                UVEC = glm::vec3(uX, uY, uZ);

                myfile >> word; // FOVY label
                myfile >> word;

                FOVY = stof(word.toStdString());
            }

            if (word.compare("LIGHT") == 0) {

                myfile >> word; // LPOS label
                myfile >> word;
                float pX = stof(word.toStdString());
                myfile >> word;
                float pY = stof(word.toStdString());
                myfile >> word;
                float pZ = stof(word.toStdString());

                LPOS = glm::vec3(pX, pY, pZ);

                myfile >> word; // LCOL label
                myfile >> word;
                float cX = stof(word.toStdString());
                myfile >> word;
                float cY = stof(word.toStdString());
                myfile >> word;
                float cZ = stof(word.toStdString());

                LCOL = glm::vec3(cX, cY, cZ);

            } // end light

            if (word.compare("ROPE") == 0) {
                myfile >> word; // rope label
                myfile >> word; // DIM

                myfile >> word;
                buffX = stoi(word.toStdString());

                myfile >> word;
                buffY = stoi(word.toStdString());

                myfile >> word;
                buffZ = stoi(word.toStdString());

                // Scale label
                myfile >> word;

                myfile >> word;
                float scaleX = stof(word.toStdString());

                myfile >> word;
                float scaleY = stof(word.toStdString());

                myfile >> word;
                float scaleZ = stof(word.toStdString());

                // RGB label
                myfile >> word;

                myfile >> word;
                float ropeR = stof(word.toStdString());

                myfile >> word;
                float ropeG = stof(word.toStdString());

                myfile >> word;
                float ropeB = stof(word.toStdString());

                rope = mkU<Rope>();
                rope->buffX = buffX;
                rope->buffY = buffY;
                rope->buffZ = buffZ;
                rope->scaleX = scaleX;
                rope->scaleY = scaleY;
                rope->scaleZ = scaleZ;

                rope->sX = (float) scaleX / (float) buffX;
                rope->sY = (float) scaleY / (float) buffY;
                rope->sZ = (float) scaleZ / (float) buffZ;

                rope->color = glm::vec3(0,1,0);


                voxelBuffer = new Voxel***[buffX];
                for (int k = 0; k < buffX; k++) {
                    voxelBuffer[k] = new Voxel**[buffY];
                }

                for (int k = 0; k < buffX; k++) {
                    for (int w = 0; w < buffY; w++) {
                        voxelBuffer[k][w] = new Voxel*[buffZ];
                    }
                }

                // Grid label
                myfile >> word;

                // Fill in the voxels
                //proceduralRopeGenerator();

                for (int z = (buffZ - 1); z >= 0; z--) {
                    for (int y = 0; y < buffY; y++) {
                        for (int x = 0; x < buffX; x++) {

                            // The density
                            //myfile >> word;
                            //float density = stof(word.toStdString());

                            Voxel* voxel = new Voxel(0); //new Voxel(density);
                            voxel->x = x;
                            voxel->y = y;
                            voxel->z = z;

                            voxelBuffer[x][y][z] = voxel;
                        }

                    }
                }

                int minX = INFINITY;
                int minY = INFINITY;
                int minZ = INFINITY;
                int maxX = -INFINITY;
                int maxY = -INFINITY;
                int maxZ = -INFINITY;

                vector<vector<float>> values;

                // get the minimum and maximum values
                for (int i = 0; i < (104 - 19 + 1); i++) {
                    myfile >> word;
                    float x = stof(word.toStdString());
                    myfile >> word;
                    float y = stof(word.toStdString());
                    myfile >> word;
                    float z = stof(word.toStdString());

                    cout << "temp " << x << " " << y << " " << z << endl;

                    vector<float> value{x, y, z};
                    values.push_back(value);

                    if (x < minX) {
                        minX = x;
                    }
                    if (y < minY) {
                        minY = y;
                    }
                    if (z < minZ) {
                        minZ = z;
                    }

                    if (x > maxX) {
                        maxX = x;
                    }
                    if (y > maxY) {
                        maxY = y;
                    }
                    if (z > maxZ) {
                        maxZ = z;
                    }
                }

                int counter = 0;
                cout << values.size() << endl;

                for (int i = 0; i < values.size(); i++) {
                    int newX = (int) ((buffX * (values[i][0] - minX)) / (maxX - minX)) - 10;
                    int newY = (int) ((buffY * (values[i][1] - minY)) / (maxY - minY)) - 10;
                    int newZ = (int) ((buffZ * (values[i][2] - minZ)) / (maxZ - minZ)) - 10;

                    for (int x = -1; x < 1; x++) {
                        for (int y = -1; y < 2; y++) {
                            for (int z = 0; z < 1; z++) {
                                cout << "point " << newX + x << " " << newY + y << " " << newZ + z << endl;
                                Voxel* voxel = new Voxel(1);
                                voxel->x = newX + x;
                                voxel->y = newY + y;
                                voxel->z = newZ + z;

                                if ((((x >= 0) && (x < buffX)) && ((y >= 0) && (y < buffY))) && ((z >= 0) && (z < buffZ))) {
                                    voxelBuffer[newX + x][newY + y][newZ + z] = voxel;
                                }
                            }
                        }
                    }
                }

                rope->voxelBuffer = voxelBuffer;
                rope->setup();

            }

        } // end while myfile >> word
//        text = in.readAll();
//        text.append('\0');
    }
     else {
		cout << "Couldn't open the file!" << endl;
		getchar();
		return false;
	}

	return true;

}
