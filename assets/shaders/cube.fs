#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main(){
    //FragColor = vec4(0.14, 0.18, 0.18, 1.0);
    FragColor = texture(texture1, TexCoords);
}