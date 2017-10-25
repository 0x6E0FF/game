#ifndef __SHADER_H__
#define __SHADER_H__

class Shader
{
public:
	Shader(const char *vertPath, const char *fragPath);
	void use();
	inline const unsigned int id() { return mId; }
private:
	unsigned int mId;
};

#endif // __SHADER_H__