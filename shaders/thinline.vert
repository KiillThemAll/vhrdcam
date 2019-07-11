#version 150 core

in vec4 vertexPosition;
//in vec2 vertexTexCoord;

//out vec2 texCoord;

uniform mat4 modelViewProjection;

void main()
{
    //texCoord = vertexTexCoord;
    gl_Position = modelViewProjection * vertexPosition;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
