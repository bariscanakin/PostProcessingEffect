#ifndef PostProcessingEffect_h
#define PostProcessingEffect_h

#include "Effect.h"
#include "SOIL\SOIL.h"

class PostProcessingEffect : public Effect
{
private:
	GLuint image, ramp_map;

	GLint LoadGLTexture(const char *filename)
	{
		GLuint _texture;

		_texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture 
			(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_MIPMAPS
			);

		// check for an error during the load process 
		if(_texture == 0)
		{
			printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		}

		glBindTexture(GL_TEXTURE_2D, _texture); // select our current texture
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // texture should tile
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return _texture;
	}

public:
	PostProcessingEffect(int* screen_width, int* screen_height) : Effect("effect.frag", screen_width, screen_height)
	{
		image = glGetUniformLocation(programID, "image");
		ramp_map = glGetUniformLocation(programID, "ramp_map");
		glActiveTexture(GL_TEXTURE1);
		LoadGLTexture("ironman.jpg");
		glActiveTexture(GL_TEXTURE2);
		LoadGLTexture("ramp.png");
		glActiveTexture(GL_TEXTURE0);
	}

	virtual void draw()
	{
		glUseProgram(programID);
		glUniform1i(image, /*GL_TEXTURE*/1);
		glUniform1i(ramp_map, /*GL_TEXTURE*/2);
		Effect::draw();
	}
};

#endif
