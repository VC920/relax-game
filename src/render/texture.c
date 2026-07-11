#include "texture.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Texture texture_load(const char *path)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, color_bit;
    unsigned char *data = stbi_load(path, &width, &height, &color_bit, 0);
    if (data == NULL) {
        fprintf(stderr, "stbi_load Error: %s\n", path);
    }
    

    GLenum format;
    if (color_bit == 1) format = GL_RED;
    else if (color_bit == 3) format = GL_RGB;
    else if (color_bit == 4) format = GL_RGBA; 

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return (Texture) {
        .texture_id = texture
    };
}