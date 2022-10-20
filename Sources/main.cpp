#include <iostream>
#include <SDL_syswm.h>

#include "OpenGL/Core/Scene.h"
#include "OpenGL/Core/Texture2d.h"
#include "WindowSystem/OpenglWindow.h"

using namespace std;


std::string vShaderCode = R"(#version 330

layout(location = 0)in vec3 iVert;
layout(location = 1)in vec4 iVertColor;

out vec4 vertColor;

void main()
{
    gl_Position = vec4(iVert, 1.0);
    vertColor = iVertColor;
}
)";
std::string fShaderCode = R"(#version 330

in vec4 vertColor;
out vec4 fragColor;

void main()
{
    fragColor = vertColor;
}
)";
std::string vTextureShaderCode = R"(#version 330

layout(location = 0)in vec3 iVert;
layout(location = 1)in vec2 iUV;

out vec2 uvCoord;

void main()
{
    gl_Position = vec4(iVert, 1.0);
    uvCoord = iUV;
}
)";
std::string fTextureShaderCode = R"(#version 330

in vec2 uvCoord;

out vec4 fragColor;

uniform sampler2D uTexture = 0;

void main()
{
    fragColor = texture( uTexture, uvCoord );
}
)";

ShaderPart vShader(ShaderPart::Type::Vertex, vShaderCode);
ShaderPart fShader(ShaderPart::Type::Fragment, fShaderCode);
Shader shader(&vShader, &fShader);

ShaderPart vTextureShader(ShaderPart::Type::Vertex, vTextureShaderCode);
ShaderPart fTextureShader(ShaderPart::Type::Fragment, fTextureShaderCode);
Shader textureShader(&vTextureShader, &fTextureShader);

void BaseInput(ISdlWindow& window)
{
    InputSystem::Get().Add(SDL_SCANCODE_ESCAPE,
        [&window](KeyState state)
        {
            if (state == KeyState::Pressed)
                window.Close();
        });
}

static unsigned pboId;

void BaseObjects(Scene& scene, OpenglWindow& window)
{
    if (!shader.Compile(true))
    {
        std::cout<<"Shader compilation error\n";
        return;
    }

    const Texture2d texture({window.GetParams().width, window.GetParams().height});

    auto pbo = Buffer()
        .Data(DataPtr(nullptr, texture.GetBufferSize(), sizeof(uint8_t)))
        .Type(GL_PIXEL_UNPACK_BUFFER)
        .Mode(GL_STREAM_DRAW);
    
    pboId = pbo.Create();
    
    
    struct {
        float x, y, z;
        float r, g, b, a;
    } triangle[] {
        {-0.1f, -0.1f, 0.f, 1.f, 0.f, 0.f, 1.f},
        {-0.1f,  0.1f, 0.f, 1.f, 1.f, 0.f, 1.f},
        { 0.0f,  0.0f, 0.f, 1.f, 0.f, 0.f, 1.f},

    };
    const auto buffer = Buffer()
                        .Data(DataPtr(triangle, sizeof(triangle) / sizeof(triangle[0]), sizeof(triangle[0])))
                        .Layout(BufferLayout().Float(3).Float(4));
    
    auto& Obj = scene.AddObject(new IRenderable(buffer));
    Obj.SetShader(&shader);
}

int main(int argc, char** argv)
{
    ISdlWindowParams params;
    params.vsync = true;

    OpenglWindow window(params);
    {
        Scene scene;
    
        window.SetBackgroundColor(glm::vec4(0.6, 0.6, 0.6, 1.0));
    
        window.SetScene(&scene);
        BaseInput(window);
        BaseObjects(scene, window);

        window.Show();
    }
    glDeleteBuffers(1, &pboId);
    shader.Destroy();
    vShader.Destroy();
    fShader.Destroy();
    
    return 0;
}

