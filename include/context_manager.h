#pragma once
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "utils.h"

class OpenGLContext final {
 public:
  // Not copyable
  DELETE_COPY(OpenGLContext)
  // Not movables
  DELETE_MOVE(OpenGLContext)
  /// @brief Release resources
  ~OpenGLContext();
  /**
   * @brief Create OpenGL context.
   *
   * @param window_title title of the created window.
   * @param width window width.
   * @param height window height.
   *
   * @return reference of context.
   */
  static OpenGLContext& createContext(int GLversion, int profile, int width = 1280, int height = 720);
  /// @return Current window handle.
  static GLFWwindow* getWindow() { return window; }
  /// @return Refresh rate of the primary monitor.
  static int getRefreshRate() { return refresh_rate; }
  /// @brief Enable OpenGL's debug callback
  static void printSystemInfo();
  /// @brief Useful to report OpenGL errors.
  static void enableDebugLogging();

 private:
  /**
   * @brief Create OpenGL context, call by createContext method
   *
   * @param window_title title of the created window.
   * @param width window width.
   * @param height window height.
   *
   * @return reference of context.
   */
  OpenGLContext(int width, int height);
  // You can change OpenGL version here
  static int OpenGL_version;
  static int OpenGL_profile;
  // Cached data
  static GLFWwindow* window;
  static int refresh_rate;
};
