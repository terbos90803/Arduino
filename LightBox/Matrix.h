#pragma once

#include "Pixel.h"

class Matrix
{
    typedef float mat[3][2];
    mat m;

  public:
    Matrix() {
      reset();
    }

    void reset() {
      mat identity = {{1, 0}, {0, 1}, {0, 0}};
      memcpy(m, identity, sizeof(m));
    }

    void rotate(float theta) {
      float ct = cosf(theta);
      float st = sinf(theta);
      mat r = {{ct, st}, { -st, ct}, {0, 0}};
      rmult(r);
    }

    void scale(float s) {
      mat r = {{s, 0}, {0, s}, {0, 0}};
      rmult(r);
    }

    void translate(float x, float y) {
      m[2][0] += x;
      m[2][1] += y;
    }

    Pixel transform(const Pixel &p) const {
      Pixel p1 = {.x = p.x * m[0][0] + p.y * m[1][0] + m[2][0],
                  .y = p.x * m[0][1] + p.y * m[1][1] + m[2][1]
                 };
      return p1;
    }

    void rmult(const Matrix &rval) {
      rmult(rval.m);
    }

  private:
    void rmult(const mat &rval) {
      mat result = {
        m[0][0] * rval[0][0] + m[0][1] * rval[1][0], // + 0 * rval[2][0]
        m[0][0] * rval[0][1] + m[0][1] * rval[1][1], // + 0 * rval[2][1]
        m[1][0] * rval[0][0] + m[1][1] * rval[1][0], // + 0 * rval[2][0]
        m[1][0] * rval[0][1] + m[1][1] * rval[1][1], // + 0 * rval[2][1]
        m[2][0] * rval[0][0] + m[2][1] * rval[1][0] + rval[2][0],
        m[2][0] * rval[0][1] + m[2][1] * rval[1][1] + rval[2][1]
      };
      memcpy(m, result, sizeof(m));
    }
};
