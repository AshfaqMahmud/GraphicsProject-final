#ifndef pentagon_h
#define pentagon_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std; 

class Pentagon {

public:
    // materialistic property
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

    Pentagon()
    {
        setUpPentVertexDataAndConfigureVertexAttribute();
    }

    Pentagon(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpPentVertexDataAndConfigureVertexAttribute();
    }

    Pentagon(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpPentVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Pentagon()
    {
        glDeleteVertexArrays(1, &pentVAO);
        glDeleteVertexArrays(1, &lightPentVAO);
        glDeleteVertexArrays(1, &lightTexPentVAO);
        glDeleteBuffers(1, &pentVBO);
        glDeleteBuffers(1, &pentEBO);
    }

    void drawPentagonWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
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

        glBindVertexArray(lightTexPentVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
    }
    void drawPentWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightPentVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 15);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawPent(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(pentVAO);
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
        unsigned int pentVAO;
        unsigned int lightPentVAO;
        unsigned int lightTexPentVAO;
        unsigned int pentVBO;
        unsigned int pentEBO;

        void setUpPentVertexDataAndConfigureVertexAttribute()
        {
            float pentaVertices[] = {
                // Top pentagon
                

                1.236f, 0.0f,   0.0f,   0.0f,0.0f,1.0f,    TXmax, TYmin,    //0
                5.236f, 0.0f,   0.0f,   0.0f,0.0f,1.0f,    TXmax, TYmin,    //1
                6.472f, 3.873f, 0.0f,   0.0f,0.0f,1.0f,    TXmax, TYmin,    //2
                3.236f, 6.155f, 0.0f,   0.0f,0.0f,1.0f,    TXmax, TYmin,    //3
                0.0f,   3.873f, 0.0f,   0.0f,0.0f,1.0f,    TXmax, TYmin,    //4

                // Bottom pentagon

                1.236f, 0.0f,   -0.5f,   0.0f,0.0f,-1.0f,    TXmax, TYmin,  //5
                5.236f, 0.0f,   -0.5f,   0.0f,0.0f,-1.0f,    TXmax, TYmin,  //6
                6.472f, 3.873f, -0.5f,   0.0f,0.0f,-1.0f,    TXmax, TYmin,  //7
                3.236f, 6.155f, -0.5f,   0.0f,0.0f,-1.0f,    TXmax, TYmin,  //8
                0.0f,   3.873f, -0.5f,   0.0f,0.0f,-1.0f,    TXmax, TYmin,  //9

                //side faces
                1.236f, 0.0f,   0.0f,   0.0f,1.0f,0.0f,    TXmax, TYmin,    //0 10
                5.236f, 0.0f,   0.0f,   0.0f,1.0f,0.0f,    TXmax, TYmin,    //1 11
                5.236f, 0.0f,   -0.5f,   0.0f,1.0f,0.0f,    TXmax, TYmin,  //6 12
                1.236f, 0.0f,   -0.5f,   0.0f,1.0f,0.0f,    TXmax, TYmin,  //5 13

                5.236f, 0.0f,   0.0f,   -0.96f,0.304f,0.0f,    TXmax, TYmin,    //1 14
                6.472f, 3.873f, 0.0f,   -0.96f,0.304f,0.0f,    TXmax, TYmin,    //2 15
                6.472f, 3.873f, -0.5f,  -0.96f,0.304f,0.0f,    TXmax, TYmin,  //7 16
                5.236f, 0.0f,   -0.5f,  -0.96f,0.304f,0.0f,    TXmax, TYmin,  //6 17

                6.472f, 3.873f, 0.0f,   -1.141f,-1.618f,0.0f,    TXmax, TYmin,    //2 18
                3.236f, 6.155f, 0.0f,   -1.141f,-1.618f,0.0f,    TXmax, TYmin,    //3 19
                3.236f, 6.155f, -0.5f,  -1.141f,-1.618f,0.0f,    TXmax, TYmin,  //8 20
                6.472f, 3.873f, -0.5f,  -1.141f,-1.618f,0.0f,    TXmax, TYmin,  //7 21

                0.0f,   3.873f, 0.0f,  1.141f,-1.618f,0.0f,    TXmax, TYmin,    //4 22
                3.236f, 6.155f, 0.0f,  1.141f,-1.618f,0.0f,    TXmax, TYmin,    //3 23
                3.236f, 6.155f, -0.5f, 1.141f,-1.618f,0.0f,    TXmax, TYmin,  //8 24
                0.0f,   3.873f, -0.5f, 1.141f,-1.618f,0.0f,    TXmax, TYmin,  //9 25

                0.0f,   3.873f, 0.0f,   -1.9365f,-0.618f,0.0f,    TXmax, TYmin,    //4 26
                0.0f,   3.873f, -0.5f,  -1.9365f,-0.618f,0.0f,    TXmax, TYmin,  //9 27
                1.236f, 0.0f,   -0.5f,  -1.9365f,-0.618f,0.0f,    TXmax, TYmin,  //5 28
                1.236f, 0.0f,   0.0f,   -1.9365f,-0.618f,0.0f,    TXmax, TYmin,    //0 29

            };

            // Define triangles connecting the top and bottom pentagons
            unsigned int pentaIndices[] = {
                // Top face
                0, 1, 2,
                0, 2, 3,
                0, 3, 4,

                // Bottom face
                5, 6, 7,
                5, 7, 8,
                5, 8, 9,

                // Side faces
                10,11,12, 12,13,10,
                14,15,16, 16,17,14,
                18,19,20, 20,21,18,
                22,23,24, 24,25,22,
                26,27,28, 28,29,26

             

                /*0,1,6,
                6,5,0,
                1,2,7,
                7,6,1,
                2,3,8,
                8,7,2,
                4,3,8,
                8,9,4,
                4,9,5,
                5,0,4,*/
            };

            //float pentaVertices[] = {
            //    // Vertex positions (x, y, z)
            //    0.0f, 0.0f, 0.0f,
            //    -1.0f, 0.0f, 0.0f,
            //    -0.5f, 0.0f, -0.866f,
            //    0.5f, 0.0f, -0.866f,
            //    1.0f, 0.0f, 0.0f,

            //    0.0f, 1.0f, 0.0f,
            //    -0.5f, 1.0f, 0.0f,
            //    -0.25f, 1.0f, -0.433f,
            //    0.25f, 1.0f, -0.433f,
            //    0.5f, 1.0f, 0.0f,
            //};

            //// Normals for the pentagon
            //float pentaNormals[] = {
            //    // Normal vectors (x, y, z)
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,

            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //    0.0f, 1.0f, 0.0f,
            //};

            //unsigned int pentaIndices[] = {
            //    // Bottom face
            //    0, 1, 2,
            //    0, 2, 3,
            //    0, 3, 4,

            //    // Top face
            //    5, 6, 7,
            //    5, 7, 8,

            //    // Side faces
            //    0, 1, 6,
            //    1, 6, 7,
            //    1, 2, 7,
            //    2, 7, 8,
            //    2, 3, 8,
            //    3, 8, 9,
            //    3, 4, 9,
            //    4, 9, 5,
            //    4, 5, 0,
            //};

            glGenVertexArrays(1, &pentVAO);
            glGenVertexArrays(1, &lightPentVAO);
            glGenVertexArrays(1, &lightTexPentVAO);
            glGenBuffers(1, &pentVBO);
            glGenBuffers(1, &pentEBO);


            glBindVertexArray(lightTexPentVAO);

            glBindBuffer(GL_ARRAY_BUFFER, pentVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(pentaVertices), pentaVertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pentEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pentaIndices), pentaIndices, GL_STATIC_DRAW);


            // Link vertex attributes
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);


            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // vertex normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
            glEnableVertexAttribArray(1);

            // texture coordinate attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
            glEnableVertexAttribArray(2);


            glBindVertexArray(lightPentVAO);

            glBindBuffer(GL_ARRAY_BUFFER, pentVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pentEBO);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
            glEnableVertexAttribArray(1);


            glBindVertexArray(pentVAO);

            glBindBuffer(GL_ARRAY_BUFFER, pentVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pentEBO);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }
};


#endif

