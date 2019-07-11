#version 150 core
out vec4 fragColor;
in Color {
    vec3  color;
} fs_in;

void main(void)
{
        fragColor = vec4(fs_in.color, 1.0);
//    if(min(min(triangle.x, triangle.y), triangle.z) < 1/10.0) {
//        fragColor  = vec4(1, 0, 0, 1.0);
//    } else {
//        fragColor  = vec4(0, 0, 0, 1.0);
//    }
}
