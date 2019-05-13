# version 330 core

in vec3 position;
in vec3 normal;
in vec3 texCoord;

out vec3 fragPosition;
out vec3 fragTexCoord;
out vec3 fragNormal;

uniform mat4 mvp;

void main() {

        fragPosition = position;
        fragTexCoord = texCoord;
        fragNormal = normal;

        gl_Position = mvp * vec4(position, 1.0);
}

