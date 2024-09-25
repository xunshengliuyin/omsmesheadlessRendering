#define GLFW_EXPOSE_NATIVE_OSMESA
#define GLFW_NATIVE_INCLUDE_NONE
#include <glad/glad.h>
#include <GL/osmesa.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


const std::string kDefaultVertexShader = R"(
    attribute vec4 position; attribute vec4 inputTextureCoordinate;

    varying vec2 textureCoordinate;

    void main() {
      gl_Position = position;
      textureCoordinate = inputTextureCoordinate.xy;
    })";


const std::string kDefaultFragmentShader = R"(
    varying vec2 textureCoordinate; uniform sampler2D inputImageTexture;

    void main() {
      gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
    })";

#include <iostream>

#include "gpupixel_macros.h"

#include "math_toolbox.h"

//#include "gpupixel.h"
using namespace gpupixel;
// E :\work\vs\gpupixel\6\Project1\third_party\libyuv\include
GLuint texture1 = -1;
//std::shared_ptr<SourceImage> gpuSourceImage;
//std::shared_ptr<TargetView> target_view;

float beautyValue = 0;
float whithValue = 0;
float thinFaceValue = 0;
float bigeyeValue = 0;
float lipstickValue = 0;
float blusherValue = 0;

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);



int main() {
    // GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();

  /*  int ret = glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // must use legacy opengl profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    if (ret) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }
    else {
        // todo log error
        return 0;
    }
    */
   OSMesaContext ctx;
    GLubyte *buffer;
    int width_osm = 1280; // 渲染缓冲区的宽度
    int height_osm = 720; // 渲染缓冲区的高度
    int bitsPerPixel = 32; // 每个像素的位数

    ctx = OSMesaCreateContext(OSMESA_RGBA, NULL);
    if (!ctx) {
        std::cout <<"OSMesaCreateContext failed!\n"<< std::endl;
    }
    buffer = (GLubyte *)malloc(width_osm * height_osm * (bitsPerPixel / 8));
    if (!buffer) {
        std::cout <<"Alloc buffer failed!\n"<< std::endl;
    }
    if (!OSMesaMakeCurrent(ctx, buffer, GL_UNSIGNED_BYTE, width_osm, height_osm)) {
        std::cout <<"OSMesaMakeCurrent failed!\n"<< std::endl;
    }
    std::cout <<"GLFW_CONTEXT_CREATION_API!\n"<< std::endl;
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
    if (glfwInit()) {
        std::cout << "GLFW Init" << std::endl;
    }
    std::cout << glfwGetError(NULL)<< std::endl;
    std::cout <<"glfwInit!\n"<< std::endl;
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_OSMESA_CONTEXT_API);
    std::cout << glfwGetError(NULL)<< std::endl;
    std::cout <<"GLFW_CONTEXT_CREATION_API!\n"<< std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    std::cout <<"GLFW_CONTEXT_VERSION_MAJOR!"<< std::endl;
    std::cout << glfwGetError(NULL)<< std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //1.2 设置OpenGL启用核心模式（非立即渲染模式）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::cout << "glfwWindowHint!\n"<< std::endl;
    int platform = glfwGetPlatform();
    std::cout << platform<< std::endl;
    std::cout <<"platform!\n"<< std::endl;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    GLFWwindow* window =
        glfwCreateWindow(1280, 720,
            "gpupixel opengl context", NULL, NULL);
    if (!window) {
        // todo log error
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);













    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(
    //    window, framebuffer_size_callback);  // 窗口调整事件回调

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //gpuSourceImage = SourceImage::create("demo.png");
    int width, height, channel_count;
    std::vector<unsigned char> image_bytes;
    //std::vector<unsigned char> _colorBuffer;  // tlx
    unsigned char* pixels =
        stbi_load("demo.png", &width, &height, &channel_count, 0);


    CHECK_GL(glGenTextures(1, &texture1));             //_texture
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, texture1));  //_texture

    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // tlx设置纹理的采样方式， GL_TEXTURE_MIN_FILTER,
    // 当图片小于窗口口像素，GL_TEXTURE_MAG_FILTER，当图片大于窗口口像素
    //  _textureAttributes.magFilter是采样方式，_textureAttributes.wrapS是纹理包裹方式
    // TODO: Handle mipmaps
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));

    CHECK_GL(glBindTexture(GL_TEXTURE_2D, texture1));
    if (channel_count == 3) {
        CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, pixels));

        int rgba_size = width * height * 4;
        uint8_t* rgba = new uint8_t[rgba_size];

        for (int i = 0; i < width * height; i++) {
            rgba[i * 4 + 0] = pixels[i * 3 + 0];  // Red
            rgba[i * 4 + 1] = pixels[i * 3 + 1];  // Green
            rgba[i * 4 + 2] = pixels[i * 3 + 2];  // Blue
            rgba[i * 4 + 3] = 255;                // Alpha (fully opaque)
        }

        image_bytes.assign(rgba, rgba + width * height * 4);

        delete[] rgba;
    }
    else if (channel_count == 4) {
        CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, pixels));
        image_bytes.assign(pixels, pixels + width * height * 4);
    }
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));






















    // target_view = std::make_shared<TargetView>();

    // gpuSourceImage->addTarget(target_view);

    // target_view->onSizeChanged(1280, 720);

    std::vector<unsigned char> _colorBuffer(720 * 1280 *
        3);  // RGBA格式，每个像素4字节
    // render loop
    // -----------


    struct {
        float r;
        float g;
        float b;
        float a;
    } _backgroundColor;

    GLfloat _displayVertices[8];

    _backgroundColor.r = 0.0;
    _backgroundColor.g = 0.0;
    _backgroundColor.b = 0.0;
    _backgroundColor.a = 0.0;
    // -----
    // gpuSourceImage->Render();

    float scaledWidth = 0.375;
    float scaledHeight = 1.0;
    _displayVertices[0] = -scaledWidth;
    _displayVertices[1] = -scaledHeight;
    _displayVertices[2] = scaledWidth;
    _displayVertices[3] = -scaledHeight;
    _displayVertices[4] = -scaledWidth;
    _displayVertices[5] = scaledHeight;
    _displayVertices[6] = scaledWidth;
    _displayVertices[7] = scaledHeight;

    //GLProgram* _displayProgram;
    GLuint _program;
    GLuint _positionAttribLocation;
    GLuint _texCoordAttribLocation;
    GLuint _colorMapUniformLocation;
    //#####################################################################
  /*  _displayProgram = GLProgram::createByShaderString(kDefaultVertexShader,
        kDefaultFragmentShader);*/

    CHECK_GL(_program = glCreateProgram());

    CHECK_GL(GLuint vertShader = glCreateShader(GL_VERTEX_SHADER));
    const char* vertexShaderSourceStr = kDefaultVertexShader.c_str();
    CHECK_GL(glShaderSource(vertShader, 1, &vertexShaderSourceStr, NULL));
    CHECK_GL(glCompileShader(vertShader));

    //
    GLint compileSuccess;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(vertShader, sizeof(messages), 0, &messages[0]);

        std::cout << "GL ERROR GLProgram::_initWithShaderString frag shader "
            << messages << std::endl;
        return -1;
    }

    CHECK_GL(GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER));
    const char* fragmentShaderSourceStr = kDefaultFragmentShader.c_str();
    CHECK_GL(glShaderSource(fragShader, 1, &fragmentShaderSourceStr, NULL));
    CHECK_GL(glCompileShader(fragShader));

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(fragShader, sizeof(messages), 0, &messages[0]);
        std::cout << "GL ERROR GLProgram::_initWithShaderString frag shader " << messages << std::endl;

        return -1;
    }

    CHECK_GL(glAttachShader(_program, vertShader));
    CHECK_GL(glAttachShader(_program, fragShader));

    CHECK_GL(glLinkProgram(_program));

    CHECK_GL(glDeleteShader(vertShader));
    CHECK_GL(glDeleteShader(fragShader));


    //#####################################################################
    _positionAttribLocation = glGetAttribLocation(_program, "position");
    _texCoordAttribLocation = glGetAttribLocation(_program, "inputTextureCoordinate");
    _colorMapUniformLocation = glGetAttribLocation(_program, "textureCoordinate");
    CHECK_GL(glUseProgram(_program));
  /*  _positionAttribLocation = _displayProgram->getAttribLocation("position");

    _texCoordAttribLocation =
        _displayProgram->getAttribLocation("inputTextureCoordinate");
    _colorMapUniformLocation =
        _displayProgram->getUniformLocation("textureCoordinate");
   _displayProgram->use();*/







    CHECK_GL(glEnableVertexAttribArray(_positionAttribLocation));
    CHECK_GL(glEnableVertexAttribArray(_texCoordAttribLocation));




    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        //processInput(window);


        CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        CHECK_GL(glViewport(0, 0, 1280, 720));
        CHECK_GL(glClearColor(_backgroundColor.r,
            _backgroundColor.g,
            _backgroundColor.b, _backgroundColor.a));
        CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  
       // _displayProgram->use();
        CHECK_GL(glUseProgram(_program));
        //GPUPixelContext::getInstance()->setActiveShaderProgram(_displayProgram);
        CHECK_GL(glActiveTexture(GL_TEXTURE0));
        CHECK_GL(glBindTexture(
            GL_TEXTURE_2D,
            texture1));  //_inputFramebuffers[0].frameBuffer->getTexture()
        CHECK_GL(glUniform1i(_colorMapUniformLocation, 0));
        CHECK_GL(glVertexAttribPointer(_positionAttribLocation, 2,GL_FLOAT, 0, 0,_displayVertices));
        static const GLfloat horizontalFlipTextureCoordinates[] = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        };
        CHECK_GL(glVertexAttribPointer(_texCoordAttribLocation, 2,GL_FLOAT, 0, 0,horizontalFlipTextureCoordinates));
        // horizontalFlipTextureCoordinates
        // _getTexureCoordinate(_inputFramebuffers[0].rotationMode)

        CHECK_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));



        glReadPixels(0, 0, 1280, 720, GL_RGB, GL_UNSIGNED_BYTE,
            _colorBuffer.data());
        stbi_write_jpg("160.jpg", 1280, 720, 3, _colorBuffer.data(), 16);
          stbi_write_jpg("161.jpg", 1280, 720, 3, buffer, 16);
        // W,H,问题是窗口的黑边也会保存
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        break;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// 处理所有输入:查询GLFW相关的键是否被按下/释放，并做出相应的反应
// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window) {
//  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//    glfwSetWindowShouldClose(window, true);
//  }
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback
//// function executes
//// 每当窗口大小改变(由操作系统或用户调整大小)，这个回调函数执行
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//  // make sure the viewport matches the new window dimensions; note that width
//  // and height will be significantly larger than specified on retina displays.
//  // //确保视口匹配新的窗口尺寸;注意宽度和
//  // 高度将明显大于视网膜显示器上的指定。
//  glViewport(0, 0, width, height);
//}
