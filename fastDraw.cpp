/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.1.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "fastDraw.h"

using namespace std;
using namespace qglviewer;

void Viewer::init() {
  // Increase the material shininess, so that the difference between
  // the two versions of the spiral is more visible.
    //!材质与光源相似，也需要设置众多的属性。
    //!不同的是，光源是通过glLight*函数来设置的，而材质则是通过glMaterial*函数来设置的。
    //!GL_FRONT_AND_BACK。分别表示设置“正面”“背面”的材质，或者两面同时设置。
    //GL_SHININESS属性。该属性只有一个值，称为“镜面指数”，取值范围是0到128。
    //该值越小，表示材质越粗糙，点光源发射的光线照射到上面，也可以产生较大的亮点。
    //该值越大，表示材质越类似于镜面，光源照射到上面后，产生较小的亮点。
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
  GLfloat specular_color[4] = {0.8f, 0.8f, 0.8f, 1.0};//镜面颜色RGBA
  //!GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR属性。
  //这三个属性与光源的三个对应属性类似，每一属性都由四个值组成。
  //GL_AMBIENT表示各种光线照射到该材质上，经过很多次反射后最终遗留在环境中的光线强度（颜色）。
  //GL_DIFFUSE表示光线照射到该材质上，经过漫反射后形成的光线强度（颜色）。
  //GL_SPECULAR表示光线照射到该材质上，经过镜面反射后形成的光线强度（颜色）。
  //通常，GL_AMBIENT和GL_DIFFUSE都取相同的值，可以达到比较真实的效果。
  //使用GL_AMBIENT_AND_DIFFUSE可以同时设置GL_AMBIENT和GL_DIFFUSE属性。
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);

  //更新窗口的显示
  restoreStateFromFile();
  help();
}

static void drawSpiral(bool simplified = false) {//绘制螺旋模型
  int nbSteps = 600;
  int nbSub = 50;
  if (simplified) {
    nbSteps = 60;
    nbSub = 2;
  }

  for (float n = 0; n < nbSub; ++n) {
    glBegin(GL_QUAD_STRIP);//开始绘制
    for (float i = 0.0; i < nbSteps; ++i) {
      float ratio = i / nbSteps;
      float angle = 21.0 * ratio;
      float radius = 1.0 - 0.5 * ratio;

      Vec center(radius * cos(angle), ratio - 0.5, radius * sin(angle));

      for (unsigned short j = 0; j < 2; ++j) {
        float delta = 3.0 * (n + j) / nbSub;
        Vec shift(cos(angle) * cos(delta), sin(delta), sin(angle) * cos(delta));

        glColor3f(1 - ratio, (n + j) / nbSub, ratio);
        glNormal3fv(shift);
        glVertex3fv(center + 0.2f * shift);
      }
    }
    glEnd();
  }
}

void Viewer::draw() { drawSpiral(); }

void Viewer::fastDraw() { drawSpiral(true); }

QString Viewer::helpString() const {
  QString text("<h2>F a s t D r a w</h2>");
  text += "The <code>fastDraw()</code> function is called instead of "
          "<code>draw()</code> when the camera ";
  text += "is manipulated. Providing such a simplified version of "
          "<code>draw()</code> allows for interactive ";
  text += "frame rates when the camera is moved, even for very complex scenes.";
  return text;
}
