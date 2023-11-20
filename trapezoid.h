#ifndef trapezoid_h
#define trapezoid_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Trapezoid {
public:
	//materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property,texture co-ordinate
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    //constructors
    Trapezoid()
    {
        setUpTrapVertexDataAndConfigureVertexAttribute();
    }
    Trapezoid(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpTrapVertexDataAndConfigureVertexAttribute();
    }
    Trapezoid(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpTrapVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Trapezoid()
    {
        glDeleteVertexArrays(1, &trapVAO);
        glDeleteVertexArrays(1, &lightTrapVAO);
        glDeleteVertexArrays(1, &lightTexTrapVAO);
        glDeleteBuffers(1, &trapVBO);
        glDeleteBuffers(1, &trapEBO);
    }
    void drawTrapezoidWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexTrapVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawTrapezoidWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightTrapVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawTrapezoid(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(trapVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int trapVAO;
    unsigned int lightTrapVAO;
    unsigned int lightTexTrapVAO;
    unsigned int trapVBO;
    unsigned int trapEBO;

    void  setUpTrapVertexDataAndConfigureVertexAttribute()
    {
        /*const float trapezoidVertices[] = {
        -0.5f, -0.1f, -0.5f,    0.0f, 0.0f, -1.0f, TXmax, TYmin, //0
        0.5f, -0.1f, -0.5f,     0.0f, 0.0f, -1.0f, TXmax, TYmin, //1
        0.5f,  0.1f, -0.5f,     0.0f, 0.0f, -1.0f, TXmax, TYmin, //2
        -0.5f,  0.1f, -0.5f,    0.0f, 0.0f, -1.0f, TXmax, TYmin, //3
        -0.2f, -0.1f,  0.5f,    0.0f, 0.0f,  1.0f, TXmax, TYmin, //4
        0.2f, -0.1f,  0.5f,     0.0f, 0.0f,  1.0f, TXmax, TYmin, //5
        0.2f,  0.1f,  0.5f,     0.0f, 0.0f,  1.0f, TXmax, TYmin, //6
        -0.2f,  0.1f,  0.5f,    0.0f, 0.0f,  1.0f, TXmax, TYmin  //7

        };

        unsigned int trapezoidIndices[] = {

            0, 1, 2, 2, 3, 0, // Front face
            1, 5, 6, 6, 2, 1, // Right face
            5, 4, 7, 7, 6, 5, // Back face
            4, 0, 3, 3, 7, 4, // Left face
            3, 2, 6, 6, 7, 3, // Top face
            4, 5, 1, 1, 0, 4  // Bottom face

        };*/

        float trapezoidVertices[] = {
            // positions            // normals         // texture
            0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f, TXmin, TYmin,
            1.0f, 1.0f, 0.0f,       0.0f, 0.0f, 1.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f,       0.0f, 0.0f, 1.0f, TXmax, TYmax,

            1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.2f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f,       1.0f, 0.0f, 0.2f, TXmax, TYmax,
            0.8f, 0.0f, 1.0f,       1.0f, 0.0f, 0.2f, TXmin, TYmin,
            0.8f, 1.0f, 1.0f,       1.0f, 0.0f, 0.2f, TXmin, TYmax,

            0.2f, 0.0f, 1.0f,       0.0f, 0.0f, 1.0f, TXmin, TYmin,
            0.8f, 0.0f, 1.0f,       0.0f, 0.0f, 1.0f, TXmax, TYmin,
            0.8f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f, TXmax, TYmax,
            0.2f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f, TXmin, TYmax,

            0.2f, 0.0f, 1.0f,       0.0f, 0.0f, 0.2f, TXmax, TYmin,
            0.2f, 1.0f, 1.0f,       0.0f, 0.0f, 0.2f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f,       0.0f, 0.0f, 0.2f, TXmin, TYmax,
            0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.2f, TXmin, TYmin,

            0.8f, 1.0f, 1.0f,       0.0f, -0.6f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f,       0.0f, -0.6f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f,       0.0f, -0.6f, 0.0f, TXmin, TYmax,
            0.2f, 1.0f, 1.0f,       0.0f, -0.6f, 0.0f, TXmin, TYmin,

            0.0f, 0.0f, 0.0f,       0.0f, -0.6f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f,       0.0f, -0.6f, 0.0f, TXmax, TYmin,
            0.8f, 0.0f, 1.0f,       0.0f, -0.6f, 0.0f, TXmax, TYmax,
            0.2f, 0.0f, 1.0f,       0.0f, -0.6f, 0.0f, TXmin, TYmax
        };
        unsigned int trapezoidIndices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };
        /*float trapezoidVertices[] = {
            
            0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,       0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,       0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.2f,
            1.0f, 1.0f, 0.0f,       1.0f, 0.0f, 0.2f,
            0.8f, 0.0f, 1.0f,       1.0f, 0.0f, 0.2f,
            0.8f, 1.0f, 1.0f,       1.0f, 0.0f, 0.2f,

            0.2f, 0.0f, 1.0f,       0.0f, 0.0f, 1.0f,
            0.8f, 0.0f, 1.0f,       0.0f, 0.0f, 1.0f,
            0.8f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f,
            0.2f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f,

            0.2f, 0.0f, 1.0f,       0.0f, 0.0f, 0.2f,
            0.2f, 1.0f, 1.0f,       0.0f, 0.0f, 0.2f,
            0.0f, 1.0f, 0.0f,       0.0f, 0.0f, 0.2f,
            0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.2f,

            0.8f, 1.0f, 1.0f,       0.0f, -0.6f, 0.0f,
            1.0f, 1.0f, 0.0f,       0.0f, -0.6f, 0.0f,
            0.0f, 1.0f, 0.0f,       0.0f, -0.6f, 0.0f,
            0.2f, 1.0f, 1.0f,       0.0f, -0.6f, 0.0f,

            0.0f, 0.0f, 0.0f,       0.0f, -0.6f, 0.0f,
            1.0f, 0.0f, 0.0f,       0.0f, -0.6f, 0.0f,
            0.8f, 0.0f, 1.0f,       0.0f, -0.6f, 0.0f,
            0.2f, 0.0f, 1.0f,       0.0f, -0.6f, 0.0f,
        };
        unsigned int trapezoidIndices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };*/

        glGenVertexArrays(1, &trapVAO);
        glGenVertexArrays(1, &lightTrapVAO);
        glGenVertexArrays(1, &lightTexTrapVAO);
        glGenBuffers(1, &trapVBO);
        glGenBuffers(1, &trapEBO);


        glBindVertexArray(lightTexTrapVAO);

        glBindBuffer(GL_ARRAY_BUFFER, trapVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trapezoidVertices), trapezoidVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trapEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trapezoidIndices), trapezoidIndices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightTrapVAO);

        glBindBuffer(GL_ARRAY_BUFFER, trapVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trapEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(trapVAO);

        glBindBuffer(GL_ARRAY_BUFFER, trapVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trapEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};

#endif

