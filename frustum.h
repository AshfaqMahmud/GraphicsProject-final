//
//  frustum.h
//  test
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef frustum_h
#define frustum_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Frustum {
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

    // constructors
    Frustum()
    {
        setUpFrustumVertexDataAndConfigureVertexAttribute();
    }

    Frustum(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpFrustumVertexDataAndConfigureVertexAttribute();
    }

    Frustum(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpFrustumVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Frustum()
    {
        glDeleteVertexArrays(1, &frustumVAO);
        glDeleteVertexArrays(1, &lightFrustumVAO);
        glDeleteVertexArrays(1, &lightTexFrustumVAO);
        glDeleteBuffers(1, &frustumVBO);
        glDeleteBuffers(1, &frustumEBO);
    }

    void drawFrustumWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
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

        glBindVertexArray(lightTexFrustumVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawFrustumWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightFrustumVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawFrustum(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(frustumVAO);
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
    unsigned int frustumVAO;
    unsigned int lightFrustumVAO;
    unsigned int lightTexFrustumVAO;
    unsigned int frustumVBO;
    unsigned int frustumEBO;

    void setUpFrustumVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float frustum_vertices[] = {
            // positions      // normals         // texture
           -1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmax,
           -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,

           -2.0f, 1.0f, -2.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
            2.0f, 1.0f, -2.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            2.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
           -2.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,

           -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            2.0f, 1.0f, -2.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
           -2.0f, 1.0f, -2.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,

           -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            2.0f, 1.0f, 2.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
           -2.0f, 1.0f, 2.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,
            2.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,
            2.0f, 1.0f, -2.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,

           -1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
           -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
           -2.0f, 1.0f, 2.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
           -2.0f, 1.0f, -2.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
        };
        unsigned int frustum_indices[] = {
            0,1,2, 2,3,0,
            4,5,6, 6,7,4,
            8,9,10, 10,11,8,
            12,13,14, 14,15,12,
            16,17,18, 18,19,16,
            20,21,22, 22,23,20,
        };

        glGenVertexArrays(1, &frustumVAO);
        glGenVertexArrays(1, &lightFrustumVAO);
        glGenVertexArrays(1, &lightTexFrustumVAO);
        glGenBuffers(1, &frustumVBO);
        glGenBuffers(1, &frustumEBO);


        glBindVertexArray(lightTexFrustumVAO);

        glBindBuffer(GL_ARRAY_BUFFER, frustumVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(frustum_vertices), frustum_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frustumEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frustum_indices), frustum_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightFrustumVAO);

        glBindBuffer(GL_ARRAY_BUFFER, frustumVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frustumEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(frustumVAO);

        glBindBuffer(GL_ARRAY_BUFFER, frustumVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frustumEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


#endif /* frustum_h */
