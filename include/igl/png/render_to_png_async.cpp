#ifndef IGL_NO_BOOST
#include "render_to_png_async.h"
#include <YImage.hpp>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  ifdef _WIN32
#    define NOMINMAX
#    include <Windows.h>
#    undef NOMINMAX
#  endif
#  include <GL/gl.h>
#endif

  
static IGL_INLINE bool render_to_png_async_helper(
  YImage * img,
  const std::string png_file,
  const bool alpha,
  const bool fast)
{

  img->flip();
  const int width = img->width();
  const int height = img->height();
  if(!alpha)
  {
    for(int i = 0;i<width;i++)
    for(int j = 0;j<height;j++)
    {
      img->at(i,j).a = 255;
    }
  }

  return img->save(png_file.c_str(),fast);
}

IGL_INLINE boost::thread igl::render_to_png_async(
  const std::string png_file,
  const int width,
  const int height,
  const bool alpha,
  const bool fast)
{
  // Part that should serial
  YImage * img = new YImage();
  img->resize(width,height);
  glReadPixels(
    0,
    0,
    width,
    height,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    img->data());
  // Part that should be asynchronous  
  return boost::thread(render_to_png_async_helper,img,png_file,alpha,fast);
}
#endif