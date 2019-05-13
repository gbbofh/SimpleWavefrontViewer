# version 330 core

in vec3 fragPosition;
in vec3 fragTexCoord;
in vec3 fragNormal;

out vec4 fragColor;

void main() {

        fragColor = vec4(cross(fragPosition, fragNormal), 1.0);
}

