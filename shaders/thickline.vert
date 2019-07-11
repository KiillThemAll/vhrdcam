#version 150
#extension GL_EXT_gpu_shader4 : enable

in vec3 vertexPosition;
in vec3 vertexColor;
in float lineThickness;

out Attributes {
    vec3 color;
    float thickness;
} vs_out;

uniform mat4 modelViewProjection;
//uniform mat4 modelView;

void main(void)
{
    //gl_FrontColor = vertexColor;
        //gl_BackColor = gl_Color;
    vs_out.color = vertexColor;
    vs_out.thickness = lineThickness;
    gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
    //gl_Position = vertexPosition * viewMatrix;
}
