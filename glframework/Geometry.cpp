#include "Geometry.h"
#include "../GLHead.h"
#include <vector>
Geometry::Geometry()
{
}

Geometry::Geometry(const std::vector<float>& positions,
        const std::vector<float>& normals,
        const std::vector<float>& uvs,
        const std::vector<unsigned int>& indices)
{
	mIndicesCount = indices.size();

    GLCall(glGenBuffers(1, &mPosVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mPosVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW));


    GLCall(glGenBuffers(1, &mUvVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mUvVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW));


    GLCall(glGenBuffers(1, &mNormalVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW));


    GLCall(glGenBuffers(1, &mEbo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    GLCall(glGenVertexArrays(1, &mVao));
    GLCall(glBindVertexArray(mVao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mPosVbo));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mUvVbo));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    // 将ebo加入到vao中
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo));

    GLCall(glBindVertexArray(0));

}

Geometry::Geometry(const std::vector<float>& positions,
        const std::vector<float>& normals,
        const std::vector<float>& uvs,
        const std::vector<float>& colors,
        const std::vector<unsigned int>& indices)
{
	mIndicesCount = indices.size();

    GLCall(glGenBuffers(1, &mPosVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mPosVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW));


    GLCall(glGenBuffers(1, &mUvVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mUvVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW));


    GLCall(glGenBuffers(1, &mNormalVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW));

    GLCall(glGenBuffers(1, &mColorVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mColorVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW));


    GLCall(glGenBuffers(1, &mEbo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    GLCall(glGenVertexArrays(1, &mVao));
    GLCall(glBindVertexArray(mVao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mPosVbo));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mUvVbo));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mColorVbo));
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    // 将ebo加入到vao中
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo));

    GLCall(glBindVertexArray(0));

}

Geometry::~Geometry()
{
    if (mVao != 0) 
    {
        GLCall(glDeleteVertexArrays(1, &mVao));
    }
    if (mPosVbo != 0) 
    {
        GLCall(glDeleteVertexArrays(1, &mPosVbo));
    }
    if (mUvVbo != 0) 
    {
        GLCall(glDeleteVertexArrays(1, &mUvVbo));
    }
    if (mNormalVbo != 0) 
    {
        GLCall(glDeleteVertexArrays(1, &mNormalVbo));
    }
    if (mColorVbo != 0) 
    {
        GLCall(glDeleteVertexArrays(1, &mColorVbo));
    }
    if (mEbo != 0) 
    {
        GLCall(glDeleteVertexArrays(1, &mEbo));
    }
}

Geometry *Geometry::createBox(float size)
{
    Geometry *geomerty = new Geometry();

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f,  -0.5f, 0.0f,
    //     0.5f,   0.5f, 0.0f,
    //     -0.5f,  0.5f, 0.0f,
    // };

    // unsigned int indices[] = {
    //     0, 1, 2,
    //     0, 2, 3,
    // };

    // geomerty->mIndicesCount = sizeof(indices) / sizeof(unsigned int);

    // float uvs[] = {
    //     0.0f,0.0f,
    //     1.0f,0.0f,
    //     1.0f,1.0f,
    //     0.0f,1.0f,
    // };

    // GLuint& vbo = geomerty->mPosVbo;
    // GLCall(glGenBuffers(1, &vbo));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    // GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    // GLuint& uvVbo = geomerty->mUvVbo;
    // GLCall(glGenBuffers(1, &uvVbo));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    // GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));

    // GLuint& ebo = geomerty->mEbo;
    // GLCall(glGenBuffers(1, &ebo));
    // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    // GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    // GLuint& vao = geomerty->mVao;
    // GLCall(glGenVertexArrays(1, &vao));
    // GLCall(glBindVertexArray(vao));

    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    // GLCall(glEnableVertexAttribArray(0));
    // GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    // GLCall(glEnableVertexAttribArray(1));
    // GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0));

    // // 将ebo加入到vao中
    // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    // GLCall(glBindVertexArray(0));
    

    // float halfSize = size / 2.0f;
    // float positions[] = {       
    //     -halfSize, -halfSize, -halfSize,   // 左下后 
    //      halfSize, -halfSize, -halfSize,   // 右下后 
    //      halfSize,  halfSize, -halfSize,   // 右上后 
    //     -halfSize,  halfSize, -halfSize,   // 左上后 
    //     -halfSize, -halfSize,  halfSize,   // 左下前 
    //      halfSize, -halfSize,  halfSize,   // 右下前 
    //      halfSize,  halfSize,  halfSize,   // 右上前 
    //     -halfSize,  halfSize,  halfSize,   // 左上前 
    // };

    // float uvs[] = {       
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,

    //     0.0f, 1.0f,
    //     1.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    // };

    // // 立方体索引数据
    // unsigned int indices[] = {
    //     // 后面
    //     0, 1, 2,
    //     2, 3, 0,
    //     // 前面
    //     4, 5, 6,
    //     6, 7, 4,
    //     // 左面
    //     0, 4, 7,
    //     7, 3, 0,
    //     // 右面
    //     1, 5, 6,
    //     6, 2, 1,
    //     // 底面
    //     0, 1, 5,
    //     5, 4, 0,
    //     // 顶面
    //     3, 2, 6,
    //     6, 7, 3
    // };

    Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;

	float halfSize = size / 2.0f;

	float positions[] = {
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};

	float uvs[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};

	float normals[] = {
		//Front face
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		//Back face
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		//Top face
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		//Bottom face
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		//Right face
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		//Left face
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,   // Front face
		4, 5, 6, 6, 7, 4,   // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19,  16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};

    geomerty->mIndicesCount = sizeof(indices) / sizeof(unsigned int);

    GLuint& posVbo = geomerty->mPosVbo;
    GLCall(glGenBuffers(1, &posVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLuint& uvVbo = geomerty->mUvVbo;
    GLCall(glGenBuffers(1, &uvVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));

    GLuint& normalVbo = geomerty->mNormalVbo;
    GLCall(glGenBuffers(1, &normalVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW));


    GLuint& ebo = geomerty->mEbo;
    GLCall(glGenBuffers(1, &ebo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GLuint& vao = geomerty->mVao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    // 将ebo加入到vao中
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    GLCall(glBindVertexArray(0));

    return geomerty;
}

Geometry *Geometry::createSphere(float radius)
{
    Geometry *geomerty = new Geometry();

    std::vector<float> positions{};
    std::vector<float> uvs{};
    std::vector<float> normals{};
    std::vector<unsigned int> indices{};

    int numLatLines = 60;
    int numLongLines = 60;

    // 两层循环（纬线在外，经线在内） -> 位置 uv坐标

    for (int i = 0; i <= numLatLines; i++) {
        for (int j = 0; j <= numLongLines; j++) {
            float phi = i * glm::pi<float>() / numLatLines;
            float theta = j * 2 * glm::pi<float>() / numLongLines;

            float y = radius * cos(phi);
            float x = radius * sin(phi) * cos(theta);
            float z = radius * sin(phi) * sin(theta);

            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);

            float u = 1.0f - (float) j / (float)numLongLines;
            float v = 1.0f - (float) i / (float)numLatLines;

            uvs.push_back(u);
            uvs.push_back(v);
            
            //法线 原点指向坐标点，长度不为1，需要归一化
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
    }


    //  两层循环（没有=号） -> 顶点索引
    for (int i = 0; i < numLatLines; i++) {
        for (int j = 0; j < numLongLines; j++) {
            int p1 = i * (numLongLines + 1) + j;
            int p2 = p1 + numLongLines + 1;
            int p3 = p1 + 1;
            int p4 = p2 + 1;

            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);

        }
    }

    // 生成vbo与vao

    geomerty->mIndicesCount = indices.size();

    GLuint& posVbo = geomerty->mPosVbo;
    GLCall(glGenBuffers(1, &posVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW));

    GLuint& uvVbo = geomerty->mUvVbo;
    GLCall(glGenBuffers(1, &uvVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW));

    GLuint& normalVbo = geomerty->mNormalVbo;
    GLCall(glGenBuffers(1, &normalVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW));

    GLuint& ebo = geomerty->mEbo;
    GLCall(glGenBuffers(1, &ebo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    GLuint& vao = geomerty->mVao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    // 将ebo加入到vao中
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    GLCall(glBindVertexArray(0));


    return geomerty;
}

Geometry* Geometry::createPlane(float width, float height) {
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;

	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	float positions[] = {
		-halfW, -halfH, 0.0f,
		halfW, -halfH, 0.0f,
		halfW, halfH, 0.0f,
		-halfW, halfH, 0.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	GLuint& posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo, &normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::createScreenPlane() {
    Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;


	float positions[] = {
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	float uvs[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};


	GLuint& posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}