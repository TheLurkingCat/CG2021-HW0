#include "context_manager.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

GLFWwindow* OpenGLContext::window = nullptr;
int OpenGLContext::refresh_rate = 60;
int OpenGLContext::OpenGL_version = 41;
int OpenGLContext::OpenGL_profile = GLFW_OPENGL_COMPAT_PROFILE;

namespace {
const char* getErrorTypeString(GLenum error) {
  switch (error) {
    case GL_DEBUG_TYPE_ERROR:
      return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      return "Deprecated behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      return "Undefined behavior";
    case GL_DEBUG_TYPE_PORTABILITY:
      return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:
      return "Performance";
    case GL_DEBUG_TYPE_OTHER:
      [[fallthrough]];
    default:
      return "Other";
  }
}

const char* getSeverityString(GLenum severity) {
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      return "High";
    case GL_DEBUG_SEVERITY_MEDIUM:
      return "Medium";
    case GL_DEBUG_SEVERITY_LOW:
      return "Low";
    default:
      return "Info";
  }
}
void GLAPIENTRY errorCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar* message,
                              const void*) {
  const char* severityString = getSeverityString(severity);
  const char* errorTypeString = getErrorTypeString(type);
// Ignore Info level information when in release mode
#ifdef NDEBUG
  if (severityString[0] != 'I') {
#endif
    std::cerr << "[OpenGL][" << errorTypeString << "][" << severityString << "] " << message << std::endl;
#ifdef NDEBUG
  }
#endif
}
}  // namespace

OpenGLContext::OpenGLContext(int width, int height) {
  // Initialize GLFW
  if (glfwInit() == GLFW_FALSE) {
    THROW_EXCEPTION(std::runtime_error, "Failed to initialize GLFW!");
  }
  // Setup context property
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGL_version / 10);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGL_version % 10);
  if (OpenGLContext::OpenGL_profile == GLFW_OPENGL_CORE_PROFILE) {
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }
  glfwWindowHint(GLFW_OPENGL_PROFILE, OpenGLContext::OpenGL_profile);
  glfwWindowHint(GLFW_SAMPLES, 4);
  // Create OpenGL context
  window = glfwCreateWindow(width, height, "Hello", nullptr, nullptr);
  if (window == nullptr) {
    THROW_EXCEPTION(std::runtime_error, "Failed to create OpenGL context!");
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  // Load OpenGL function pointers
#ifdef GLAD_OPTION_GL_ON_DEMAND
  // Lazy loading
  gladSetGLOnDemandLoader(glfwGetProcAddress);
#else
  if (!gladLoadGL(glfwGetProcAddress)) {
    THROW_EXCEPTION(std::runtime_error, "Failed to load OpenGL!");
  }
#endif
  // For high dpi monitors like Retina display, we need to recalculate
  // framebuffer size
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  // OK, everything works fine
  // ----------------------------------------------------------
  // Enable some OpenGL feature
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glClearColor(0, 0, 0, 1);
}

OpenGLContext::~OpenGLContext() {
  if (window != nullptr) glfwDestroyWindow(window);
  glfwTerminate();
}

OpenGLContext& OpenGLContext::createContext(int GLversion, int profile, int width, int height) {
  // We should only initialize once
  if (window == nullptr) {
    OpenGLContext::OpenGL_version = GLversion;
    OpenGLContext::OpenGL_profile = profile;
  }
  static OpenGLContext context(width, height);
  return context;
}

void OpenGLContext::printSystemInfo() {
  GLFWmonitor* moniter = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidMode = glfwGetVideoMode(moniter);
  OpenGLContext::refresh_rate = vidMode->refreshRate;

  std::cout << std::left << std::setw(26) << "Current OpenGL renderer"
            << ": " << glGetString(GL_RENDERER) << std::endl;
  std::cout << std::left << std::setw(26) << "Current OpenGL context"
            << ": " << glGetString(GL_VERSION) << std::endl;
  std::cout << std::left << std::setw(26) << "Moniter refresh rate"
            << ": " << refresh_rate << " Hz" << std::endl;
}

void OpenGLContext::enableDebugLogging() {
  // Very useful when you are debugging, required OpenGL 4.3
  if (OpenGL_version < 43) {
    std::cout << "You need to change context to a version >= 43" << std::endl;
  } else {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorCallback, nullptr);
  }
}
